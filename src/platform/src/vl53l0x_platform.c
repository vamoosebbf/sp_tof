#include "vl53l0x_platform.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//VL53L0X �ײ����� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

VL53L0X_Error VL53L0X_WriteMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    int32_t status_int = 0;

    uint8_t deviceAddress;

    if (count >= VL53L0X_MAX_I2C_XFER_SIZE)
    {
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_write_multi(deviceAddress, index, pdata, count);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_ReadMulti(VL53L0X_DEV Dev, uint8_t index, uint8_t *pdata, uint32_t count)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    int32_t status_int;

    uint8_t deviceAddress;

    if (count >= VL53L0X_MAX_I2C_XFER_SIZE)
    {
        Status = VL53L0X_ERROR_INVALID_PARAMS;
    }

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_read_multi(deviceAddress, index, pdata, count);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_write_byte(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_write_word(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_write_dword(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

VL53L0X_Error VL53L0X_UpdateByte(VL53L0X_DEV Dev, uint8_t index, uint8_t AndData, uint8_t OrData)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;
    uint8_t data;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_read_byte(deviceAddress, index, &data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    if (Status == VL53L0X_ERROR_NONE)
    {
        data = (data & AndData) | OrData;
        status_int = VL53L0X_write_byte(deviceAddress, index, data);

        if (status_int != 0)
            Status = VL53L0X_ERROR_CONTROL_INTERFACE;
    }

    return Status;
}

//VL53L0X �����ֽڼĴ���
//Dev:�豸I2C�����ṹ��
//index:ƫ�Ƶ�ַ
//pdata:����ָ��
//count:����
//����ֵ: 0:�ɹ�
//       ����:����
VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_read_byte(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

//VL53L0X ���֣�2�ֽڣ��Ĵ���
//Dev:�豸I2C�����ṹ��
//index:ƫ�Ƶ�ַ
//pdata:����ָ��
//count:����
//����ֵ: 0:�ɹ�
//       ����:����
VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_read_word(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

//VL53L0X ��˫�֣�4�ֽڣ��Ĵ���
//Dev:�豸I2C�����ṹ��
//index:ƫ�Ƶ�ַ
//pdata:����ָ��
//count:����
//����ֵ: 0:�ɹ�
//       ����:����
VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *data)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    int32_t status_int;
    uint8_t deviceAddress;

    deviceAddress = Dev->I2cDevAddr;

    status_int = VL53L0X_read_dword(deviceAddress, index, data);

    if (status_int != 0)
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;

    return Status;
}

//VL53L0X �ײ���ʱ����
//Dev:�豸I2C�����ṹ��
//����ֵ: 0:�ɹ�
//       ����:����
#define VL53L0X_POLLINGDELAY_LOOPNB 250
VL53L0X_Error VL53L0X_PollingDelay(VL53L0X_DEV Dev)
{

    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    volatile uint32_t i;

    for (i = 0; i < VL53L0X_POLLINGDELAY_LOOPNB; i++)
    {
        //Do nothing
    }

    return status;
}
