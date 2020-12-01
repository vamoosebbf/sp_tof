#include "vl53l0x.h"
#include "syslog.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//VL53L0X-���ܲ��� ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

VL53L0X_Dev_t vl53l0x_dev;             //�豸I2C���ݲ���
VL53L0X_DeviceInfo_t vl53l0x_dev_info; //�豸ID�汾��Ϣ
uint8_t AjustOK = 0;                   //У׼��־λ

//VL53L0X������ģʽ����
//0��Ĭ��;1:�߾���;2:������;3:����
mode_data Mode_data[] =
    {
        {(FixPoint1616_t)(0.25 * 65536),
         (FixPoint1616_t)(18 * 65536),
         33000,
         14,
         10}, //Ĭ��

        {(FixPoint1616_t)(0.25 * 65536),
         (FixPoint1616_t)(18 * 65536),
         200000,
         14,
         10}, //�߾���

        {(FixPoint1616_t)(0.1 * 65536),
         (FixPoint1616_t)(60 * 65536),
         33000,
         18,
         14}, //������

        {(FixPoint1616_t)(0.25 * 65536),
         (FixPoint1616_t)(32 * 65536),
         20000,
         14,
         10}, //����

};

//API������Ϣ��ӡ
//Status�����鿴VL53L0X_Error�����Ķ���
void print_pal_error(VL53L0X_Error Status)
{

    char buf[VL53L0X_MAX_STRING_LENGTH];

    VL53L0X_GetPalErrorString(Status, buf); //����Status״̬��ȡ������Ϣ�ַ���

    printf("API Status: %i : %s\r\n", Status, buf); //��ӡ״̬�ʹ�����Ϣ
}

//ģʽ�ַ�����ʾ
//mode:0-Ĭ��;1-�߾���;2-������;3-����
void mode_string(uint8_t mode, char *buf)
{
    switch (mode)
    {
    case Default_Mode:
        strcpy(buf, "Default");
        break;
    case HIGH_ACCURACY:
        strcpy(buf, "High Accuracy");
        break;
    case LONG_RANGE:
        strcpy(buf, "Long Range");
        break;
    case HIGH_SPEED:
        strcpy(buf, "High Speed");
        break;
    }
}

//����VL53L0X�豸I2C��ַ
//dev:�豸I2C�����ṹ��
//newaddr:�豸��I2C��ַ
VL53L0X_Error vl53l0x_Addr_set(VL53L0X_Dev_t *dev, uint8_t newaddr)
{
    uint16_t Id;
    uint8_t FinalAddress;
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    uint8_t sta = 0x00;

    FinalAddress = newaddr;

    if (FinalAddress == dev->I2cDevAddr) //���豸I2C��ַ��ɵ�ַһ��,ֱ���˳�
        return VL53L0X_ERROR_NONE;
    //�ڽ��е�һ���Ĵ�������֮ǰ����I2C��׼ģʽ(400Khz)
    Status = VL53L0X_WrByte(dev, 0x88, 0x00);
    if (Status != VL53L0X_ERROR_NONE)
    {
        sta = 0x01; //����I2C��׼ģʽ����
        goto set_error;
    }
    //����ʹ��Ĭ�ϵ�0x52��ַ��ȡһ���Ĵ���
    Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
    if (Status != VL53L0X_ERROR_NONE)
    {
        sta = 0x02; //��ȡ�Ĵ�������
        goto set_error;
    }
    if (Id == 0xEEAA)
    {
        //�����豸�µ�I2C��ַ
        Status = VL53L0X_SetDeviceAddress(dev, FinalAddress);
        if (Status != VL53L0X_ERROR_NONE)
        {
            sta = 0x03; //����I2C��ַ����
            goto set_error;
        }
        //�޸Ĳ����ṹ���I2C��ַ
        dev->I2cDevAddr = FinalAddress;
        //����µ�I2C��ַ��д�Ƿ�����
        Status = VL53L0X_RdWord(dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id);
        if (Status != VL53L0X_ERROR_NONE)
        {
            sta = 0x04; //��I2C��ַ��д����
            goto set_error;
        }
    }
set_error:
    if (Status != VL53L0X_ERROR_NONE)
    {
        print_pal_error(Status); //��ӡ������Ϣ
    }
    if (sta != 0)
        printf("sta:0x%x\r\n", sta);
    return Status;
}

//vl53l0x��λ����
//dev:�豸I2C�����ṹ��
void vl53l0x_reset(VL53L0X_Dev_t *dev)
{
    uint8_t addr;
    addr = dev->I2cDevAddr; //�����豸ԭI2C��ַ
    gpiohs_set_pin(VL53L0X_SHT, 0);
    msleep(30);
    gpiohs_set_pin(VL53L0X_SHT, 1);
    msleep(30);
    dev->I2cDevAddr = VL53L0X_Addr;
    vl53l0x_Addr_set(dev, addr); //����VL53L0X������ԭ���ϵ�ǰԭI2C��ַ
    VL53L0X_DataInit(dev);
}

//��ʼ��vl53l0x
//dev:�豸I2C�����ṹ��
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    VL53L0X_Dev_t *pMyDevice = dev;

    pMyDevice->I2cDevAddr = VL53L0X_Addr; //device addr

    VL53L0X_i2c_init(); //VL53L0X init

    gpiohs_set_pin(VL53L0X_SHT, 0);
    msleep(50);
    gpiohs_set_pin(VL53L0X_SHT, 1);
    msleep(50);

    Status = VL53L0X_DataInit(pMyDevice); //�豸��ʼ��
    printf("VL53L0X_DataInit status: %d\r\n", Status);

    if (Status != VL53L0X_ERROR_NONE)
        goto error;
    msleep(2);
    Status = VL53L0X_GetDeviceInfo(pMyDevice, &vl53l0x_dev_info); //��ȡ�豸ID��Ϣ
    printf("VL53L0X_GetDeviceInfo status: %d\r\n", Status);

    if (Status != VL53L0X_ERROR_NONE)
        goto error;
error:
    if (Status != VL53L0X_ERROR_NONE)
    {
        print_pal_error(Status); //��ӡ������Ϣ
        return Status;
    }

    return Status;
}

void vl53l0x_test(void)
{
    uint8_t i = 0;
    while (vl53l0x_init(&vl53l0x_dev))
    {
        printf("VL53L0X init error!!!\r\n");
        msleep(500);
    }

    printf("VL53L0X init success!\r\n");

    // adjusting
    printf("VL53L0X adjusting\r\n");
    vl53l0x_calibration_test(&vl53l0x_dev);

    // get distance
    printf("VL53L0X start work\r\n");
    vl53l0x_general_test(&vl53l0x_dev);
}

void vl53l0x_info(void)
{
    printf("\r\n-------vl53l0x Device information-------\r\n\r\n");
    printf("  Name:%s\r\n", vl53l0x_dev_info.Name);
    printf("  Addr:0x%x\r\n", vl53l0x_dev.I2cDevAddr);
    printf("  ProductId:%s\r\n", vl53l0x_dev_info.ProductId);
    printf("  RevisionMajor:0x%x\r\n", vl53l0x_dev_info.ProductRevisionMajor);
    printf("  RevisionMinor:0x%x\r\n", vl53l0x_dev_info.ProductRevisionMinor);
    printf("\r\n-----------------------------------\r\n");
}

//��ȡһ�β�����������
//modeģʽ���� 0:Ĭ��;1:�߾���;2:������;3:����
void One_measurement(uint8_t mode)
{
    vl53l0x_set_mode(&vl53l0x_dev, mode);
    VL53L0X_PerformSingleRangingMeasurement(&vl53l0x_dev, &vl53l0x_data);
    printf("\r\n d: %4d mm.\r\n", vl53l0x_data.RangeMilliMeter);
}
