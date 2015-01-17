/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_main.h"
//
unsigned int system_idle_counter=0,system_timer_1ms_event=0,system_timer_state=0,system_status=0;
unsigned int system_timer_counter=0,system_led_timer_counter=0;
extern unsigned char b_com_command_ops;
extern unsigned char com_status_is_idle;		
extern unsigned char B_Ctrl_Gas,B_Ctrl_Valid;
extern signed char   B_Ctrl_Pitch,B_Ctrl_Roll,B_Ctrl_Yaw;
extern unsigned int Motor_B_M4,Motor_B_M2,Motor_B_M3,Motor_B_M1,B_Motor_Scale;
//
int main()
{
	B_RCC_Configuration();
	B_NVIC_Configuration();
	//
	B_SYS_LED_Configuration();
	B_SYS_Timer_Configuration();
	//
	B_LED_GPIO_Configuration();
	//
	B_COM1_GPIO_Configuration();
	B_COM1_Port_Configuration();
	//
	B_Load_Fly_Parameter();			//Բ�㲩ʿ:װ�ز���
	//
	B_ADC_Configuration();
	//
	B_Motor_GPIO_Configuration();
	B_Motor_PWM_Configuration(); 	
	//	
	B_MPU6050_Init();
	//
	B_Long_Delay(10000);	//Բ�㲩ʿ:������ʱ
	//	
	while(1)
	{
		if(system_timer_1ms_event)			//Բ�㲩ʿ:1MS�¼�����
		{	
			system_timer_1ms_event=0;			//Բ�㲩ʿ:����¼�
			system_timer_counter++;       //Բ�㲩ʿ:ϵͳ������
			system_led_timer_counter++;
			//
			switch (system_timer_state)
			{
				case IDLE:
					system_idle_counter++;
					B_Get_MPU6050_Data();
					//
					if(system_idle_counter>5) system_timer_state=UPDATE_COMMAND;
					break;

				case UPDATE_COMMAND:
					B_Get_MPU6050_Data();
					system_status=B_COM1_Task_Process();
					//
					if(system_status) system_timer_state=UPDATE_MOTOR;
					else system_timer_state=RESET_MOTOR;
					break;
				
				case UPDATE_MOTOR:
					B_Get_MPU6050_Data();
					B_Quad_Calculation();
				    B_PID_Control();
					B_Motor_Control();
					//
					system_timer_state=UPDATE_COMMAND;
					break;

				case RESET_MOTOR:
					B_Get_MPU6050_Data();
				  B_Quad_Calculation();
					B_Motor_Reset();
					//
					system_timer_state=UPDATE_COMMAND;
					break;
				
				default:
					break;
			}
		}
		else
		{			
			B_SYS_LED_TWINKLE();
		}
	}	
}


