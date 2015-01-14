/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_lib.h"
#include "b_motor.h"
#include "b_mpu6050.h"
#include "b_imu.h"
#include "b_pid.h"
//
#define B_MCU_LED				GPIO_Pin_8		//PA8
#define B_MCU_LED_PORT	GPIOA
#define B_LED_EXT				GPIO_Pin_13		//PC13
#define B_LED_EXT_PORT	GPIOC
//
#define B_Bootloader_USB_CON_PORT		GPIOB					
#define B_Bootloader_USB_CON   			GPIO_Pin_8			
//
#define B_SYS_TIMER_SYSCLK_DIV	0
#define B_SYS_TIMER_CLK_1MHZ  	71
//
void B_RCC_Configuration(void);
void B_NVIC_Configuration(void);
void B_Long_Delay(unsigned int nCount);
//
void B_SYS_LED_Configuration(void);
void B_LED_EXT_OFF(void);
void B_LED_EXT_ON(void);
void B_MCU_LED_OFF(void);
void B_MCU_LED_ON(void);
void B_SYS_LED_TWINKLE(void);
void B_SYS_EULER_Update(void);
//
void B_SYS_Timer_Configuration(void);
void B_SYS_NVIC_Configuration(void);
void B_SYS_Timer_Interrupt(void);

