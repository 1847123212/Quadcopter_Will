/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_sys.h"
//
extern unsigned int system_led_timer_counter,system_timer_1ms_event,system_timer_counter;
unsigned int b_sys_timer_period=999;
//

void B_RCC_Configuration(void)		//Բ�㲩ʿ:����ϵͳʱ��
{
  ErrorStatus HSEStartUpStatus;
	//
	NVIC_DeInit();
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH_SetLatency(FLASH_Latency_2);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK2Config(RCC_HCLK_Div2);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
}

void B_NVIC_Configuration(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,(u32)0x8000);	//Բ�㲩ʿ:����  NVIC �� Vector Table 
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		//Բ�㲩ʿ:ʹ�ܴ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);			//Բ�㲩ʿ:ʹ�ܵ�ѹ���
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);				//Բ�㲩ʿ:ʹ�ܵ�ѹDMA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//Բ�㲩ʿ:ʹ��ϵͳʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);			//Բ�㲩ʿ:ʹ�ܵ������
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//Բ�㲩ʿ:ʹ��SWD������
	//
	NVIC_RESETPRIMASK();
}

void B_SYS_LED_Configuration(void)			
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//

	GPIO_InitStructure.GPIO_Pin = B_Bootloader_USB_CON;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(B_Bootloader_USB_CON_PORT, &GPIO_InitStructure);
	GPIO_SetBits(B_Bootloader_USB_CON_PORT, B_Bootloader_USB_CON); 	
	//
	GPIO_InitStructure.GPIO_Pin = B_MCU_LED;					//Բ�㲩ʿ:����ʹ�õ�LED��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   		//Բ�㲩ʿ:����LED�������������ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  	//Բ�㲩ʿ:����LED��Ϊ���
  GPIO_Init(B_MCU_LED_PORT, &GPIO_InitStructure); 
	B_MCU_LED_ON();
	//
  GPIO_InitStructure.GPIO_Pin = B_LED_EXT;					//Բ�㲩ʿ:����ʹ�õ�LED��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;   		//Բ�㲩ʿ:����LED�������������ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  	//Բ�㲩ʿ:����LED��Ϊ���
  GPIO_Init(B_LED_EXT_PORT, &GPIO_InitStructure); 
	B_LED_EXT_ON();
}

void B_SYS_Timer_Configuration(void)		
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//
	TIM_TimeBaseStructure.TIM_ClockDivision = B_SYS_TIMER_SYSCLK_DIV;
	TIM_TimeBaseStructure.TIM_Prescaler = B_SYS_TIMER_CLK_1MHZ;
	TIM_TimeBaseStructure.TIM_Period = b_sys_timer_period;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);								//Բ�㲩ʿ:����PWM���ں�Ƶ��:72*1K/72M=1mS			
	//
	B_SYS_NVIC_Configuration();
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE);	//Բ�㲩ʿ:���ж�
	TIM_Cmd(TIM1, ENABLE);										//Բ�㲩ʿ:����PWM
}

void B_SYS_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;			//Բ�㲩ʿ:����PWM�ж����ȼ�	
	//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void B_SYS_Timer_Interrupt(void)					//Բ�㲩ʿ:PWM�жϺ���
{	
	system_timer_1ms_event=1;
}

void B_LED_EXT_OFF(void)		
{	
	GPIO_ResetBits(B_LED_EXT_PORT, B_LED_EXT);	 				//Բ�㲩ʿ:����LED
}
void B_LED_EXT_ON(void)		
{
	GPIO_SetBits(B_LED_EXT_PORT, B_LED_EXT);		//Բ�㲩ʿ:Ϩ��LED
}
//
void B_MCU_LED_OFF(void)		
{
	GPIO_SetBits(B_MCU_LED_PORT, B_MCU_LED);		//Բ�㲩ʿ:Ϩ��LED
}
void B_MCU_LED_ON(void)		
{
	GPIO_ResetBits(B_MCU_LED_PORT, B_MCU_LED);	 				//Բ�㲩ʿ:����LED
}
//
void B_SYS_LED_TWINKLE(void)										//Բ�㲩ʿ:��˸LED
{
	if(system_led_timer_counter>1000) system_led_timer_counter=0;
	if(system_led_timer_counter==500) 
	{
		B_MCU_LED_OFF();
		B_LED_EXT_ON();
	}
	else if(system_led_timer_counter==1000) 
	{
		B_MCU_LED_ON();
		B_LED_EXT_OFF();
	}
}

void B_Long_Delay(unsigned int nCount) 		  //Բ�㲩ʿ:��ʱ����
{
	for(; nCount != 0; nCount--);
}



