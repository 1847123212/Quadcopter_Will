/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include <math.h>
#include <stdlib.h>	 
#include "stm32f10x_it.h"
#include "b_bluetooth.h"

#define B_MOTOR_POWER_MA				GPIO_Pin_9
#define B_MOTOR_POWER_MA_PORT		GPIOB
#define B_MOTOR_POWER_MB				GPIO_Pin_15
#define B_MOTOR_POWER_MB_PORT		GPIOC
//
#define B_MOTOR_PWM_M1			GPIO_Pin_9		  //PA9(M1)
#define B_MOTOR_PWM_M2			GPIO_Pin_7		//PB7(M2)
#define B_MOTOR_PWM_M3			GPIO_Pin_6		//PB6(M3)
#define B_MOTOR_PWM_M4			GPIO_Pin_8		//PB8(M4)
#define B_MOTOR_PWM_PORT		GPIOC
//
#define B_MOTOR_PWM_SYSCLK_DIV	0
#define B_MOTOR_PWM_CLK_72MHZ  	1
//
#define B_FLY_MAX_OUT 999
#define B_FLY_MIN_OUT 15
//
void B_Motor_GPIO_Configuration(void);
void B_Motor_PWM_Configuration(void);
void B_Motor_NVIC_Configuration(void);

void B_Motor_Power_On(void);
void B_Motor_Power_Off(void);
//
void B_Motor_Interrupt(void);
//
void B_Motor_Reset(void);
void B_Motor_Control(void);
signed short B_Motor_Speed_Scale(float motor_speed_input);



