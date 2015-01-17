#include "quad_common.h"

void motor_pwm_reflash(uint32_t uDutyCyclePercent0 ,uint32_t uDutyCyclePercent1,
                       uint32_t uDutyCyclePercent2 ,uint32_t uDutyCyclePercent3 )
{
  uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
  uint32_t ftmBaseAddr = g_ftmBaseAddr[0];

//  //set the dutyCycle to 50% (no speed on motor) if RC is lock
//  if(isRCunlock == false)
//  {
//    uDutyCyclePercent0 = 50;
//    uDutyCyclePercent1 = 50;
//    uDutyCyclePercent2 = 50;
//    uDutyCyclePercent3 = 50;
//  }  //this function should be in motor_pid_control

  uMod = FTM_HAL_GetMod(ftmBaseAddr);
  uCnv0 = uMod * uDutyCyclePercent0 / 100;
  uCnv1 = uMod * uDutyCyclePercent1 / 100;
  uCnv2 = uMod * uDutyCyclePercent2 / 100;
  uCnv3 = uMod * uDutyCyclePercent3 / 100;

  /* For 100% duty cycle */
  if(uCnv0 >= uMod) 
  {
    uCnv0 = uMod + 1;
  }
  if(uCnv1 >= uMod)
  {
    uCnv1 = uMod + 1;
  }
  if(uCnv2 >= uMod)
  {
    uCnv2 = uMod + 1;
  }
  if(uCnv3 >= uMod)
  {
    uCnv3 = uMod + 1;
  }
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 0, uCnv0);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 1, uCnv1);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 2, uCnv2);
  FTM_HAL_SetChnCountVal(ftmBaseAddr, 3, uCnv3);

  FTM_HAL_SetSoftwareTriggerCmd(g_ftmBaseAddr[0], true);
}

//void motor_pid_control(uint32_t throttleDutyCycle,
//                       imu_float_euler_angle_t * expectAngel,
//                       imu_float_euler_angle_t * currentAngel,
//                       bool RCunlock )  //unfinished.20150105
//{
//  pitch_p,pitch_i,pitch_d;
//  yaw_p,yaw_i,yaw_d;
//  roll_p,roll_i,roll_d;
//
//
//
//
//  if(RCunlock == true)
//  {
//    motor_pwm0 = throttle + pitch_out + 0        + yaw_out ;
//    motor_pwm1 = throttle + 0         + roll_out - yaw_out ;
//    motor_pwm2 = throttle - pitch_out - roll_out + yaw_out ;
//    motor_pwm3 = throttle - pitch_out - 0        - yaw_out ;
//  }
//  else
//  {
//    motor_pwm0 = 50 ;
//    motor_pwm1 = 50 ;
//    motor_pwm2 = 50 ;
//    motor_pwm3 = 50 ;
//  }
//
//  motor_pwm_reflash(motor_pwm0,motor_pwm1,motor_pwm2,motor_pwm3);
//}


void IncPIDInit(pid_t *sptr) 
{ 
  sptr->SumError   = 0;     
  sptr->LastError = 0;    //Error[-1] 
  sptr->PrevError = 0;    //Error[-2] 
  
  sptr->Proportion = 0;    //�������� Proportional Const 
  sptr->Integral   = 0;    //���ֳ��� Integral Const 
  sptr->Derivative = 0;    //΢�ֳ��� Derivative Const 
  sptr->ExpectPoint   = 0;         
} 


////IncPIDCalc ( ) 
////ԭ        ��  int IncPIDCalc(int) 
////��        ��  ����ʽ PID ���� 
////�������  PID ���ڵ�ǰ����ֵ 
////�������  �������� 
////ͷ  ��  ��  Spmc75_PID.h 
////��  ��  ��  DigitalPID_V100 
////ע������  ��ʽ 2��5������ʽ PID �㷨��ʵ�֡� 
////��        ��  uiGoalvalue  ����  IncPIDCalc  (1998);  //λ��ʽ PID �����㷨ͨ������ʽ��
////���㷨����ʵ�֣���ǰ�����õ�ת�� 1998rpm��    
////����ʽ PID ������� 
//int IncPIDCalc(int CurrentPoint) //
//{ 
//  register int iError, iIncpid; 
//  //��ǰ��� 
//  iError = sptr->ExpectPoint - CurrentPoint; 
//  //�������� 
//  iIncpid = sptr->Proportion * iError               //E[k]�� 
//    - sptr->Integral   * sptr->LastError     //E[k��1]�� 
//      + sptr->Derivative * sptr->PrevError;   //E[k��2]�� 
//  //�洢�������´μ��� 
//  sptr->PrevError = sptr->LastError; 
//  sptr->LastError = iError; 
//  //��������ֵ 
//  return(iIncpid); 
//} 

int16_t IncPIDCalc(double CurrentPoint , pid_t *sptr)
{ 
  int16_t  iIncpid;    
  double iError;
  iError = sptr->ExpectPoint - CurrentPoint;    //��ǰ���
  
  //�������� 
  iIncpid =(int16_t)(sptr->Proportion * iError              //E[k]�� 
          - sptr->Integral   * sptr->LastError     //E[k��1]�� 
          + sptr->Derivative * sptr->PrevError);    //E[k��2]�� 
  
  //�洢�������´μ��� 
  sptr->PrevError = sptr->LastError; 
  sptr->LastError = iError; 
 
  //��������ֵ 
  return(iIncpid); 
} 

int32_t LocPIDCalc(int16_t CurrentPoint , pid_t *sptr) 
{ 
  double  iError,dError; 
  int16_t re_value;
  iError = sptr->ExpectPoint - CurrentPoint;       //ƫ�� 
  sptr->SumError += iError;               //���� 
  dError = iError - sptr->LastError;     //΢�� 
  sptr->LastError = iError; 
     
  re_value = (int32_t)(sptr->Proportion * iError  //������ 
           + sptr->Integral * sptr->SumError   //������ 
           + sptr->Derivative * dError);
  return(re_value);      //΢���� 
} 


pid_t pitch_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 10,        
  .Integral   = 0 ,
  .Derivative = 3 ,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

pid_t roll_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 10,        
  .Integral   = 0 ,
  .Derivative = 3 ,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

pid_t yaw_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 0.5,        
  .Integral   = 0  ,
  .Derivative = 1.5,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

void motor_pid_control(uint32_t throttleDutyCycle,
                       imu_float_euler_angle_t * expectAngel,
                       imu_float_euler_angle_t * currentAngel,
                       pid_t *pitch_pid,
                       pid_t *yaw_pid,
                       pid_t *roll_pid,
                       bool RCunlock )//unfinished.20150105
{
/*          �����ROLL�ᣬ����Ҹߣ�ROLLֵΪ�������Խ��ֵԽ�󡣷���180�Ⱥ��Ϊ-179...
*               |
*      Motor3   |    Motor2
*         �v    |    ��
*           �v  |  ��
*   __________�v|��___________�����pitch�ᣬǰ���ϣ��ߣ����£��ͣ�pitchֵΪ�������Խ��ֵԽ�󡣷���180�Ⱥ��Ϊ-179...
*             ��|�v
*           ��  |  �v
*         ��    |    �v
*      Motor0   |    Motor1
*               |
*/ 
  
  static int32_t pitch_out = 0,roll_out = 0,yaw_out = 0;
//  static uint16_t pitch_out = 0,roll_out = 0,yaw_out = 0;
  static int16_t motor_pwm0_duty  = 0;
  static int16_t motor_pwm1_duty  = 0;
  static int16_t motor_pwm2_duty  = 0;
  static int16_t motor_pwm3_duty  = 0;
  
  pitch_pid->ExpectPoint = expectAngel->imu_pitch;
  roll_pid->ExpectPoint  = expectAngel->imu_roll;
  
//  pitch_out += IncPIDCalc(currentAngel->imu_pitch , pitch_pid);
//  roll_out  += IncPIDCalc(currentAngel->imu_roll  , roll_pid);
  
  pitch_out =(int32_t)((LocPIDCalc(currentAngel->imu_pitch ,pitch_pid)) / 100) ;
  roll_out  =(int32_t)((LocPIDCalc(currentAngel->imu_roll ,roll_pid )) / 100);
  
//  if(pitch_out > 3) pitch_out = 3;
 // if(roll_out > 3) roll_out = 3;
//  if(pitch_out < -5) pitch_out = -5;
//  if(roll_out < -5) roll_out = -5;
  
  if(throttleDutyCycle > 75) throttleDutyCycle = 75; ////��ʱ��С������
  
  if((RCunlock == true) && (throttleDutyCycle > 60) )
//  throttleDutyCycle = 65;
//    if(1)
  {
    motor_pwm0_duty  = throttleDutyCycle - pitch_out - roll_out - yaw_out ;
    motor_pwm1_duty  = throttleDutyCycle - pitch_out + roll_out + yaw_out ;
    motor_pwm2_duty  = throttleDutyCycle + pitch_out + roll_out - yaw_out ;
    motor_pwm3_duty  = throttleDutyCycle + pitch_out - roll_out + yaw_out ;
    
    if(motor_pwm0_duty > 98){ motor_pwm0_duty = 98;}
    if(motor_pwm1_duty > 98){ motor_pwm1_duty = 98;}
    if(motor_pwm2_duty > 98){ motor_pwm2_duty = 98;}
    if(motor_pwm3_duty > 98){ motor_pwm3_duty = 98;}
    
    if(motor_pwm0_duty < 50){ motor_pwm0_duty = 50;}
    if(motor_pwm1_duty < 50){ motor_pwm1_duty = 50;}
    if(motor_pwm2_duty < 50){ motor_pwm2_duty = 50;}
    if(motor_pwm3_duty < 50){ motor_pwm3_duty = 50;}
  }
  else
  {
    motor_pwm0_duty  = 50 ;
    motor_pwm1_duty  = 50 ;
    motor_pwm2_duty  = 50 ;
    motor_pwm3_duty  = 50 ;
  }

  PRINTF("pwm0 = %3d ,pwm1 = %3d ,pwm2 = %3d ,pwm3 = %3d\r\n" ,motor_pwm0_duty,motor_pwm1_duty,motor_pwm2_duty,motor_pwm3_duty);
  
  motor_pwm_reflash(motor_pwm0_duty ,
                    motor_pwm1_duty ,
                    motor_pwm2_duty ,
                    motor_pwm3_duty );
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
