#include "vl53l0x_cali.h"
#include "sleep.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK MiniV3 STM32开发板
//VL53L0X-校准模式 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/7/1
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

_vl53l0x_adjust Vl53l0x_adjust; //校准数据24c02写缓存区(用于在校准模式校准数据写入24c02)
_vl53l0x_adjust Vl53l0x_data;   //校准数据24c02读缓存区（用于系统初始化时向24C02读取数据）

#define adjust_num 5 //校准错误次数

//VL53L0X校准函数
//dev:设备I2C参数结构体
VL53L0X_Error vl53l0x_adjust(VL53L0X_Dev_t *dev)
{

    VL53L0X_DeviceError Status = VL53L0X_ERROR_NONE;
    uint32_t refSpadCount = 7;
    uint8_t isApertureSpads = 0;
    uint32_t XTalkCalDistance = 100;
    uint32_t XTalkCompensationRateMegaCps;
    uint32_t CalDistanceMilliMeter = 100 << 16;
    int32_t OffsetMicroMeter = 30000;
    uint8_t VhvSettings = 23;
    uint8_t PhaseCal = 1;
    uint8_t i = 0;

    VL53L0X_StaticInit(dev); //数值恢复默认,传感器处于空闲状态
    // LED1 = 0;
//SPADS校准----------------------------
spads:
    msleep(10);
    printf("The SPADS Calibration Start...\r\n");
    Status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads); //执行参考Spad管理
    if (Status == VL53L0X_ERROR_NONE)
    {
        printf("refSpadCount = %d\r\n", refSpadCount);
        Vl53l0x_adjust.refSpadCount = refSpadCount;
        printf("isApertureSpads = %d\r\n", isApertureSpads);
        Vl53l0x_adjust.isApertureSpads = isApertureSpads;
        printf("The SPADS Calibration Finish...\r\n\r\n");
        i = 0;
    }
    else
    {
        i++;
        if (i == adjust_num)
            return Status;
        printf("SPADS Calibration Error,Restart this step\r\n");
        goto spads;
    }
//设备参考校准---------------------------------------------------
ref:
    msleep(10);
    printf("The Ref Calibration Start...\r\n");
    Status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal); //Ref参考校准
    if (Status == VL53L0X_ERROR_NONE)
    {
        printf("VhvSettings = %d\r\n", VhvSettings);
        Vl53l0x_adjust.VhvSettings = VhvSettings;
        printf("PhaseCal = %d\r\n", PhaseCal);
        Vl53l0x_adjust.PhaseCal = PhaseCal;
        printf("The Ref Calibration Finish...\r\n\r\n");
        i = 0;
    }
    else
    {
        i++;
        if (i == adjust_num)
            return Status;
        printf("Ref Calibration Error,Restart this step\r\n");
        goto ref;
    }
//偏移校准------------------------------------------------
offset:
    msleep(10);
    printf("Offset Calibration:need a white target,in black space,and the distance keep 100mm!\r\n");
    printf("The Offset Calibration Start...\r\n");
    Status = VL53L0X_PerformOffsetCalibration(dev, CalDistanceMilliMeter, &OffsetMicroMeter); //偏移校准
    if (Status == VL53L0X_ERROR_NONE)
    {
        printf("CalDistanceMilliMeter = %d mm\r\n", CalDistanceMilliMeter);
        Vl53l0x_adjust.CalDistanceMilliMeter = CalDistanceMilliMeter;
        printf("OffsetMicroMeter = %d mm\r\n", OffsetMicroMeter);
        Vl53l0x_adjust.OffsetMicroMeter = OffsetMicroMeter;
        printf("The Offset Calibration Finish...\r\n\r\n");
        i = 0;
    }
    else
    {
        i++;
        if (i == adjust_num)
            return Status;
        printf("Offset Calibration Error,Restart this step\r\n");
        goto offset;
    }
//串扰校准-----------------------------------------------------
xtalk:
    msleep(20);
    printf("Cross Talk Calibration:need a grey target\r\n");
    printf("The Cross Talk Calibration Start...\r\n");
    Status = VL53L0X_PerformXTalkCalibration(dev, XTalkCalDistance, &XTalkCompensationRateMegaCps); //串扰校准
    if (Status == VL53L0X_ERROR_NONE)
    {
        printf("XTalkCalDistance = %d mm\r\n", XTalkCalDistance);
        Vl53l0x_adjust.XTalkCalDistance = XTalkCalDistance;
        printf("XTalkCompensationRateMegaCps = %d\r\n", XTalkCompensationRateMegaCps);
        Vl53l0x_adjust.XTalkCompensationRateMegaCps = XTalkCompensationRateMegaCps;
        printf("The Cross Talk Calibration Finish...\r\n\r\n");
        i = 0;
    }
    else
    {
        i++;
        if (i == adjust_num)
            return Status;
        printf("Cross Talk Calibration Error,Restart this step\r\n");
        goto xtalk;
    }
    // LED1 = 1;
    printf("All the Calibration has Finished!\r\n");
    printf("Calibration is successful!!\r\n");

    memcpy(&Vl53l0x_data, &Vl53l0x_adjust, sizeof(_vl53l0x_adjust)); //将校准数据复制到Vl53l0x_data结构体
    return Status;
}

//vl53l0x校准测试
//dev:设备I2C参数结构体
void vl53l0x_calibration_test(VL53L0X_Dev_t *dev)
{
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t key = 0;
    uint8_t i = 0;

    printf("need a white target,and the distance keep 100mm.\r\n");
    while (1)
    {
        printf("Start calibration...\r\n");
        status = vl53l0x_adjust(dev);     //进入校准
        if (status != VL53L0X_ERROR_NONE) //校准失败
        {
            printf("Calibration is error!!\r\n");
            i = 3;
            while (i--)
            {
                printf("Calibration is error\r\n");
            }
        }
        else
            printf("Calibration is complete!\r\n");
        msleep(500);
        break;
    }
}
