/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_lib.h"
#include "b_bluetooth.h"
//
#define B_VOLTAGE_CHA 		GPIO_Pin_0	 	//Բ�㲩ʿ:PB0
#define B_VOLTAGE_CHB 		GPIO_Pin_1	  //Բ�㲩ʿ:PB1
#define B_VOLTAGE_PORT   GPIOB
//
#define	B_ADC_Address	((u32)0x4001244C)
//
void B_ADC_IO_Configuration(void);
void B_ADC_DEVICE_Configuration(void);
void B_ADC_Configuration(void);
void B_ADC_Get_ADC_Value(void);

