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
#define B_MPU6050_INT					GPIO_Pin_3		//PB3
#define B_MPU6050_INT_PORT		GPIOB
#define B_MPU6050_INT_STATE   GPIO_ReadInputDataBit(B_MPU6050_INT_PORT, B_MPU6050_INT)
//
#define B_I2C_SDA 			GPIO_Pin_11	 
#define B_I2C_SCL 			GPIO_Pin_10	
#define B_I2C_PORT   		GPIOB
//
#define B_I2C_SCL_0 		GPIO_ResetBits(B_I2C_PORT, B_I2C_SCL)
#define B_I2C_SCL_1 		GPIO_SetBits(B_I2C_PORT, B_I2C_SCL)
#define B_I2C_SDA_0 		GPIO_ResetBits(B_I2C_PORT, B_I2C_SDA)
#define B_I2C_SDA_1   	GPIO_SetBits(B_I2C_PORT, B_I2C_SDA)
//
#define B_I2C_SDA_STATE   	GPIO_ReadInputDataBit(B_I2C_PORT, B_I2C_SDA)
#define B_I2C_DELAY 				B_I2C_Delay(100000)
#define B_I2C_NOP						B_I2C_Delay(10) 
//
#define B_I2C_READY					0x00
#define B_I2C_BUS_BUSY			0x01	
#define B_I2C_BUS_ERROR			0x02
//
#define B_I2C_NACK	  0x00 
#define B_I2C_ACK			0x01
//
#define B_MPU6050_GYRO_ADDR 				0xD0
#define B_MPU6050_GYRO_WHOAMI_ADDR 0x75
//
#define B_MPU6050_USER_CTRL_ADDR		0x6A
#define B_MPU6050_USER_CTRL_VALUE	0x00
//
#define B_MPU6050_PWR_MGMT_1_ADDR		0x6B
#define B_MPU6050_PWR_MGMT_1_VALUE 	0x01
#define B_MPU6050_EXIT_SLEEP_VALUE 	0x01
//
#define B_MPU6050_GYRO_CFG_ADDR 		0x1B
#define B_MPU6050_GYRO_CFG_VALUE 		0x18
//
#define B_MPU6050_ACCEL_CFG_ADDR 		0x1C
#define B_MPU6050_ACCEL_CFG_VALUE 	0x18
//
#define B_MPU6050_I2CBYPASS_CFG_ADDR 		0x37
#define B_MPU6050_I2CBYPASS_CFG_VALUE 	0xB2		//bit7=1, bit6=0, bit5=1, bit4=1, bit3=0, bit2=0, bit1=1, bit0=0
//
#define B_MPU6050_INT_CFG_ADDR 		0x38
#define B_MPU6050_INT_CFG_VALUE 		0x01
//
#define B_MPU6050_ACCEL_DATA_ADDR 		0x3B
//
unsigned char B_MPU6050_GYRO_WHOAMI_FUN(void);
unsigned char B_MPU6050_READ_REG_FUN(unsigned char b_mpu6050_dev_addr,unsigned char b_mpu6050_reg_addr);
void B_MPU6050_PWM_CFG_FUN(void);
void B_MPU6050_GYRO_CFG_FUN(void);
void B_MPU6050_ACCEL_CFG_FUN(void);
void B_MPU6050_EXIT_SLEEP_FUN(void);
void B_MPU6050_I2CBYPASS_CFG_FUN(void);
void B_MPU6050_USER_CTRL_FUN(void); 
void B_MPU6050_INT_CFG_FUN(void);
void B_MPU6050_Init(void);
void B_Get_MPU6050_Data(void);
//
void B_I2C_Configuration(void);
void B_I2C_Delay(unsigned int dly);
unsigned char  B_I2C_START(void);
void B_I2C_STOP(void);
void B_I2C_SendACK(void);
void B_I2C_SendNACK(void);
unsigned char  B_I2C_SendByte(unsigned char  b_i2c_data);
unsigned char  B_I2C_ReceiveByte_WithACK(void);
unsigned char  B_I2C_ReceiveByte_NoACK(void);
void B_I2C_Receive14Bytes(u8 *b_i2c_data_buffer);



