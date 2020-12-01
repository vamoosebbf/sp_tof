#include "vl53l0x_i2c.h"
#include "i2c.h"
#include "fpioa.h"
#include "gpiohs.h"
#include "syslog.h"
#include "stdio.h"
#include "sipeed_i2c.h"
#include <stdlib.h>
#include <string.h>

//VL53L0X I2C初始化
void VL53L0X_i2c_init(void)
{
    //set io mux
    fpioa_set_function(VL53L0X_SCL, FUNC_I2C0_SCLK + VL53L0X_I2C_DEVICE * 2);
    fpioa_set_function(VL53L0X_SDA, FUNC_I2C0_SDA + VL53L0X_I2C_DEVICE * 2);
    fpioa_set_function(VL53L0X_SHT, FUNC_GPIOHS0 + VL53L0X_SHT);

    gpiohs_set_drive_mode(VL53L0X_SHT, GPIO_DM_OUTPUT);

    //i2c init
    maix_i2c_init(VL53L0X_I2C_DEVICE, 7, VL53L0X_I2C_FREQ_KHZ * 1000);
}

//IIC写n字节数据
uint8_t VL_IIC_Write_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, uint8_t *buf)
{
    uint8_t *send_buf = (uint8_t *)malloc(len + 1);
    uint8_t ret;
    *send_buf = REG_Address;
    memcpy(send_buf + 1, buf, len);
    ret = maix_i2c_send_data(VL53L0X_I2C_DEVICE, SlaveAddress, send_buf, len + 1, 50);
    free(send_buf);
    return ret;
}

//IIC读n字节数据
uint8_t VL_IIC_Read_nByte(uint8_t SlaveAddress, uint8_t REG_Address, uint16_t len, uint8_t *buf)
{
    return maix_i2c_recv_data(VL53L0X_I2C_DEVICE, SlaveAddress, &REG_Address, 1, buf, len, 50);
}

//VL53L0X 写多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
uint8_t VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t *pdata, uint16_t count)
{
    uint8_t status = STATUS_OK;

    if (VL_IIC_Write_nByte(address, index, count, pdata))
    {
        status = STATUS_FAIL;
    }

    return status;
}

//VL53L0X 读多个数据
//address:地址
//index:偏移地址
//pdata:数据指针
//count:长度 最大65535
uint8_t VL53L0X_read_multi(uint8_t address, uint8_t index, uint8_t *pdata, uint16_t count)
{
    uint8_t status = STATUS_OK;

    if (VL_IIC_Read_nByte(address, index, count, pdata))
    {
        status = STATUS_FAIL;
    }

    return status;
}

//VL53L0X 写1个数据(单字节)
//address:地址
//index:偏移地址
//data:数据(8位)
uint8_t VL53L0X_write_byte(uint8_t address, uint8_t index, uint8_t data)
{
    uint8_t status = STATUS_OK;

    status = VL53L0X_write_multi(address, index, &data, 1);

    return status;
}

//VL53L0X 写1个数据(双字节)
//address:地址
//index:偏移地址
//data:数据(16位)
uint8_t VL53L0X_write_word(uint8_t address, uint8_t index, uint16_t data)
{
    uint8_t status = STATUS_OK;

    uint8_t buffer[2];

    //将16位数据拆分成8位
    buffer[0] = (uint8_t)(data >> 8);   //高八位
    buffer[1] = (uint8_t)(data & 0xff); //低八位

    if (index % 2 == 1)
    {
        //串行通信不能处理对非2字节对齐寄存器的字节
        status = VL53L0X_write_multi(address, index, &buffer[0], 1);
        status = VL53L0X_write_multi(address, index, &buffer[0], 1);
    }
    else
    {
        status = VL53L0X_write_multi(address, index, buffer, 2);
    }

    return status;
}

//VL53L0X 写1个数据(四字节)
//address:地址
//index:偏移地址
//data:数据(32位)
uint8_t VL53L0X_write_dword(uint8_t address, uint8_t index, uint32_t data)
{

    uint8_t status = STATUS_OK;

    uint8_t buffer[4];

    //将32位数据拆分成8位
    buffer[0] = (uint8_t)(data >> 24);
    buffer[1] = (uint8_t)((data & 0xff0000) >> 16);
    buffer[2] = (uint8_t)((data & 0xff00) >> 8);
    buffer[3] = (uint8_t)(data & 0xff);

    status = VL53L0X_write_multi(address, index, buffer, 4);

    return status;
}

//VL53L0X 读1个数据(单字节)
//address:地址
//index:偏移地址
//data:数据(8位)
uint8_t VL53L0X_read_byte(uint8_t address, uint8_t index, uint8_t *pdata)
{
    uint8_t status = STATUS_OK;

    status = VL53L0X_read_multi(address, index, pdata, 1);

    return status;
}

//VL53L0X 读个数据(双字节)
//address:地址
//index:偏移地址
//data:数据(16位)
uint8_t VL53L0X_read_word(uint8_t address, uint8_t index, uint16_t *pdata)
{
    uint8_t status = STATUS_OK;

    uint8_t buffer[2];

    status = VL53L0X_read_multi(address, index, buffer, 2);

    *pdata = ((uint16_t)buffer[0] << 8) + (uint16_t)buffer[1];

    return status;
}

//VL53L0X 读1个数据(四字节)
//address:地址
//index:偏移地址
//data:数据(32位)
uint8_t VL53L0X_read_dword(uint8_t address, uint8_t index, uint32_t *pdata)
{
    uint8_t status = STATUS_OK;

    uint8_t buffer[4];

    status = VL53L0X_read_multi(address, index, buffer, 4);

    *pdata = ((uint32_t)buffer[0] << 24) + ((uint32_t)buffer[1] << 16) + ((uint32_t)buffer[2] << 8) + ((uint32_t)buffer[3]);

    return status;
}
