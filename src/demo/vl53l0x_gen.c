#include "vl53l0x_gen.h"

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

VL53L0X_RangingMeasurementData_t vl53l0x_data;
uint16_t Distance_data = 0; //distance

VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev, uint8_t mode)
{

    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;

    vl53l0x_reset(dev); //��λvl53l0x(Ƶ���л�����ģʽ���׵��²ɼ��������ݲ�׼���������һ����)
    status = VL53L0X_StaticInit(dev);

    if (AjustOK != 0) //��У׼����,д��У׼ֵ
    {
        status = VL53L0X_SetReferenceSpads(dev, Vl53l0x_data.refSpadCount, Vl53l0x_data.isApertureSpads); //�趨SpadsУ׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
        status = VL53L0X_SetRefCalibration(dev, Vl53l0x_data.VhvSettings, Vl53l0x_data.PhaseCal); //�趨RefУ׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
        status = VL53L0X_SetOffsetCalibrationDataMicroMeter(dev, Vl53l0x_data.OffsetMicroMeter); //�趨ƫ��У׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
        status = VL53L0X_SetXTalkCompensationRateMegaCps(dev, Vl53l0x_data.XTalkCompensationRateMegaCps); //�趨����У׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
    }
    else
    {
        status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal); //Ref�ο�У׼
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
        status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads); //ִ�вο�SPAD����
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        msleep(2);
    }
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING); //ʹ�ܵ��β���ģʽ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); //ʹ��SIGMA��Χ���
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); //ʹ���ź����ʷ�Χ���
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, Mode_data[mode].sigmaLimit); //�趨SIGMA��Χ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, Mode_data[mode].signalLimit); //�趨�ź����ʷ�Χ��Χ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, Mode_data[mode].timingBudget); //�趨��������ʱ��
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod); //�趨VCSEL��������
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod); //�趨VCSEL�������ڷ�Χ

error:
    if (status != VL53L0X_ERROR_NONE)
    {
        print_pal_error(status);
        return status;
    }
    return status;
}

VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev, VL53L0X_RangingMeasurementData_t *pdata, char *buf)
{
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t RangeStatus;

    status = VL53L0X_PerformSingleRangingMeasurement(dev, pdata); //ִ�е��β�ಢ��ȡ����������
    if (status != VL53L0X_ERROR_NONE)
        return status;

    RangeStatus = pdata->RangeStatus; //��ȡ��ǰ����״̬
    memset(buf, 0x00, VL53L0X_MAX_STRING_LENGTH);
    VL53L0X_GetRangeStatusString(RangeStatus, buf); //���ݲ���״̬��ȡ״̬�ַ���

    Distance_data = pdata->RangeMilliMeter; //�������һ�β���������

    return status;
}

void vl53l0x_general_start(VL53L0X_Dev_t *dev, uint8_t mode)
{
    static char buf[VL53L0X_MAX_STRING_LENGTH];
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t i = 0;

    mode_string(mode, buf);
    while (vl53l0x_set_mode(dev, mode))
    {
        printf("mode set error !!\r\n");
        msleep(500);
        i++;
        if (i == 2)
            return; // timeout
    }
    printf("mode : %s \r\n", buf);
    while (1)
    {
        if (Status == VL53L0X_ERROR_NONE)
        {
            Status = vl53l0x_start_single_test(dev, &vl53l0x_data, buf); //ִ single read distance
            printf("distance: %4imm\r\n", Distance_data);
        }
        msleep(50);
    }
}

//vl53l0x��ͨ����ģʽ����
//dev:�豸I2C�����ṹ��
void vl53l0x_general_test(VL53L0X_Dev_t *dev)
{
    uint8_t mode = 0;

    vl53l0x_general_start(dev, mode);
}
