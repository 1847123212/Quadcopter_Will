/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_it.h"
#include <math.h>

#define B_COM1_TX	GPIO_Pin_9
#define B_COM1_RX	GPIO_Pin_10

void B_COM1_Communication(void);
void B_COM1_Interrupt(void);  
//
void B_COM1_Send_Char(unsigned char ascii_code);
void B_COM1_Send_Num(unsigned char number);
void B_COM1_Send_Str_Head(void);
void B_COM1_Send_Str_Body(unsigned char* str_buf);
void B_COM1_Send_Str_Tail(void);
void B_COM1_Send_4bits_BCD_Num(int number);
//
void B_COM1_GPIO_Configuration(void);
void B_COM1_Port_Configuration(void);
void B_COM1_NVIC_Configuration(void);
unsigned char B_COM1_Task_Process(void);
//
