#ifndef __VL53L0_I2C_H
#define __VL53L0_I2C_H

#include "syslog.h"
#include "board_config.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniV3 STM32������
//VL53L0X IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/7/1
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////


//IO��������
#define VL_SDA_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=8<<8;}
#define VL_SDA_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=3<<8;}


//IO��������	 
#define VL_IIC_SCL    PAout(3)      //SCL
#define VL_IIC_SDA    PAout(2) 		//SDA	 
#define VL_READ_SDA   PAin(2) 		//����SDA 

//״̬
#define STATUS_OK       0x00
#define STATUS_FAIL     0x01

//IIC��������
void VL53L0X_i2c_init(void);//��ʼ��IIC��IO��

uint8_t VL53L0X_write_byte(uint8_t address,uint8_t index,uint8_t data);              //IICдһ��8λ����
uint8_t VL53L0X_write_word(uint8_t address,uint8_t index,uint16_t data);             //IICдһ��16λ����
uint8_t VL53L0X_write_dword(uint8_t address,uint8_t index,uint32_t data);            //IICдһ��32λ����
uint8_t VL53L0X_write_multi(uint8_t address, uint8_t index,uint8_t *pdata,uint16_t count);//IIC����д

uint8_t VL53L0X_read_byte(uint8_t address,uint8_t index,uint8_t *pdata);             //IIC��һ��8λ����
uint8_t VL53L0X_read_word(uint8_t address,uint8_t index,uint16_t *pdata);            //IIC��һ��16λ����
uint8_t VL53L0X_read_dword(uint8_t address,uint8_t index,uint32_t *pdata);           //IIC��һ��32λ����
uint8_t VL53L0X_read_multi(uint8_t address,uint8_t index,uint8_t *pdata,uint16_t count);  //IIC������


#endif 


