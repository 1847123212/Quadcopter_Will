/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_it.h"
#include "b_bluetooth.h"
#include "b_sys.h"
//
extern float b_filter_high;
extern float b_filter_low;
extern float b_filter_time;
extern unsigned int b_sys_timer_period;
extern float b_mpu6050_gyro_scale;
extern float b_mpu6050_acc_scale;
extern float b_hmc5883l_mag_scale;
extern unsigned int b_motor_pwm_period;
extern unsigned int B_Motor_Scale;
extern float b_quad_halfT;
extern float b_quad_Kp;
extern float b_quad_Ki;
//
extern unsigned int pid_setting_P_value[3],pid_setting_I_value[3],pid_setting_D_value[3];
extern float b_fly_gas_scale,b_fly_pitch_scale,b_fly_roll_scale,b_fly_yaw_scale;
//
extern signed short int b_mpu6050_gyro_pitch_cal,b_mpu6050_gyro_roll_cal,b_mpu6050_gyro_yaw_cal;
extern signed short int b_mpu6050_acc_pitch_cal,b_mpu6050_acc_roll_cal,b_mpu6050_acc_yaw_cal;
extern signed short int b_hmc5883l_mag_pitch_cal,b_hmc5883l_mag_roll_cal,b_hmc5883l_mag_yaw_cal;
//
void B_Load_Filter_Parameter(void);
void B_Load_Pid_Parameter(void);
void B_Load_Calibration_Parameter(void);
void B_Load_Fly_Parameter(void);

