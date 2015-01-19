/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_adc.h"
//
u16 B_ADC_Tab[2];
extern unsigned int b_bat_value;	
//
void B_ADC_Configuration(void)	
{
	B_ADC_IO_Configuration();
	B_ADC_DEVICE_Configuration();
	//
	B_COM1_Send_Str_Head();
	B_COM1_Send_Str_Body("finish to init adc device.");		//Բ�㲩ʿ:��ʼ��ADC
	B_COM1_Send_Str_Tail();	
	//
}
//
void B_ADC_IO_Configuration(void)	
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//
  GPIO_InitStructure.GPIO_Pin = B_VOLTAGE_CHA | B_VOLTAGE_CHB;					//Բ�㲩ʿ:����ʹ�õ�ADC��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //Բ�㲩ʿ:����IO�������������ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	      //Բ�㲩ʿ:����IO��ģ������
  GPIO_Init(B_VOLTAGE_PORT, &GPIO_InitStructure); 
}

void B_ADC_DEVICE_Configuration(void)	
{
		DMA_InitTypeDef	 DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;  
		//
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = B_ADC_Address;
		DMA_InitStructure.DMA_MemoryBaseAddr =(u32)B_ADC_Tab;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 2;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1, ENABLE);											//Բ�㲩ʿ:ʹ��DMA

		RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 2;
		ADC_Init(ADC1,&ADC_InitStructure);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_9,2,ADC_SampleTime_239Cycles5);

		ADC_DMACmd(ADC1,ENABLE);
		ADC_Cmd(ADC1,ENABLE);														//Բ�㲩ʿ:ʹ��ADC

		ADC_ResetCalibration(ADC1);											//Բ�㲩ʿ:ADCУ��
		while(ADC_GetResetCalibrationStatus(ADC1));
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1));
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);					//Բ�㲩ʿ:��ʼADC��ѹת��
}

void B_ADC_Get_ADC_Value(void)	
{
	float bat_volatge=0;
	//
	if(B_ADC_Tab[0]>B_ADC_Tab[1]) bat_volatge=(float)B_ADC_Tab[1];
	else bat_volatge=(float)B_ADC_Tab[0];
	//
	bat_volatge=(bat_volatge/2048)*3300;
	b_bat_value=(unsigned int) bat_volatge;
}
