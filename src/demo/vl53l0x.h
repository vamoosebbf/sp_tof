#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "vl53l0x_cali.h"
#include "sleep.h"
#include "gpiohs.h"

#define _VLX53L0X_DEBUG_
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

//VL53L0X�������ϵ�Ĭ��IIC��ַΪ0X52(���������λ)
#define VL53L0X_Addr 0x29

//����Xshut��ƽ,�Ӷ�ʹ��VL53L0X���� 1:ʹ�� 0:�ر�
// #define VL53L0X_Xshut PAout(5)	

//ʹ��2.8V IO��ƽģʽ
#define USE_I2C_2V8  0

//����ģʽ
#define Default_Mode   0// Ĭ��
#define HIGH_ACCURACY  1//�߾���
#define LONG_RANGE     2//������
#define HIGH_SPEED     3//����

//vl53l0xģʽ���ò�����
typedef __packed struct
{
	FixPoint1616_t signalLimit;    //Signal������ֵ 
	FixPoint1616_t sigmaLimit;     //Sigmal������ֵ
	uint32_t timingBudget;         //����ʱ������
	uint8_t preRangeVcselPeriod ;  //VCSEL��������
	uint8_t finalRangeVcselPeriod ;//VCSEL�������ڷ�Χ

}mode_data;


extern mode_data Mode_data[];
extern uint8_t AjustOK;

VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev);//��ʼ��vl53l0x
void print_pal_error(VL53L0X_Error Status);//������Ϣ��ӡ
void mode_string(uint8_t mode,char *buf);//ģʽ�ַ�����ʾ
void vl53l0x_test(void);//vl53l0x����
void vl53l0x_reset(VL53L0X_Dev_t *dev);//vl53l0x��λ

void vl53l0x_info(void);//��ȡvl53l0x�豸ID��Ϣ
void One_measurement(uint8_t mode);//��ȡһ�β�����������
#endif


