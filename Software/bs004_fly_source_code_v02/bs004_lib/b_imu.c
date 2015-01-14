/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "b_imu.h" 
//
float b_mpu6050_gyro_scale=0,b_mpu6050_acc_scale=0,b_gyro_to_acc_scale=0,b_hmc5883l_mag_scale=0;
extern float b_mpu6050_gyro_angel_pitch_ave,b_mpu6050_gyro_angel_roll_ave,b_mpu6050_gyro_angel_yaw_ave;
extern float b_mpu6050_acc_angel_pitch_ave,b_mpu6050_acc_angel_roll_ave,b_mpu6050_acc_angel_yaw_ave;
//
float q0=1,q1=0,q2=0,q3=0;	
float exInt=0,eyInt=0,ezInt=0;	
float b_imu_pitch=0,b_imu_roll=0,b_imu_yaw=0;
float b_quad_Kp=0,b_quad_Ki=0,b_quad_halfT=0;			
//
void B_Quad_Calculation(void)
{
	float ax=0,ay=0,az=0,gx=0,gy=0,gz=0;
	//
	b_gyro_to_acc_scale=b_mpu6050_acc_scale*b_mpu6050_gyro_scale;
	//	
	gx=b_mpu6050_gyro_angel_pitch_ave*b_gyro_to_acc_scale;
	gy=b_mpu6050_gyro_angel_roll_ave*b_gyro_to_acc_scale;
	gz=b_mpu6050_gyro_angel_yaw_ave*b_gyro_to_acc_scale;		
	//
	ax=b_mpu6050_acc_angel_roll_ave;
	ay=b_mpu6050_acc_angel_pitch_ave;	
	az=b_mpu6050_acc_angel_yaw_ave;		
	//
	B_IMU_Update(ax,ay,az,gx,gy,gz);
}
//
unsigned char B_IMU_Update(float ax,float ay,float az,float gx,float gy,float gz) 
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;  
  //	
	//Բ�㲩ʿ:��Ԫ���˷�����
	float q0q0 = q0 * q0;							
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q1q1 = q1 * q1;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;
	//	
	//Բ�㲩ʿ:��һ������
	norm = sqrt(ax*ax + ay*ay + az*az);     
	if(norm==0) return 0;	
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;   
  //	
	//Բ�㲩ʿ:����С��������ϵ	
	vx = 2*(q1q3 - q0q2);								
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	//
	//Բ�㲩ʿ:����ϵ�������������
	ex = (ay*vz - az*vy);								
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	//
	//Բ�㲩ʿ:��������
	exInt = exInt + ex*b_quad_Ki;
	eyInt = eyInt + ey*b_quad_Ki;
	ezInt = ezInt + ez*b_quad_Ki;
	//
	//Բ�㲩ʿ:�������ں�
	gx = gx + b_quad_Kp*ex + exInt;
	gy = gy + b_quad_Kp*ey + eyInt;
	gz = gz + b_quad_Kp*ez + ezInt;
	//
	//Բ�㲩ʿ:������Ԫ����
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*b_quad_halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*b_quad_halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*b_quad_halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*b_quad_halfT;  
	//
	//Բ�㲩ʿ:��һ������
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	if(norm==0) return 0;	
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	//
	//Բ�㲩ʿ:ŷ����ת��
	b_imu_roll=asin(-2*q1q3 + 2*q0q2)*57.30f;
  b_imu_pitch=atan2(2*q2q3 + 2*q0q1, -2*q1q1-2*q2q2 + 1)*57.30f; 
  b_imu_yaw=b_imu_yaw-gz*b_mpu6050_gyro_scale;
	//
	return 1;	
}
