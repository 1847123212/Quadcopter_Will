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

void motor_cnv_reflash(uint16_t uCnv0 ,uint16_t uCnv1,
                       uint16_t uCnv2 ,uint16_t uCnv3 )
{
//  uint16_t uMod, uCnv0, uCnv1,uCnv2,uCnv3;
  uint16_t uMod;
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
//  uCnv0 = uMod * uDutyCyclePercent0 / 100;
//  uCnv1 = uMod * uDutyCyclePercent1 / 100;
//  uCnv2 = uMod * uDutyCyclePercent2 / 100;
//  uCnv3 = uMod * uDutyCyclePercent3 / 100;

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
  
  sptr->LastDError = 0;    //dError[-1] 
   
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

int32_t LocPIDCalc(double CurrentPoint , pid_t *sptr , double gyro_d_value ,double dError_lowpass_percent) 
{ 
  double  iError,dError,dError_for_use; 
  int32_t re_value;
  iError = sptr->ExpectPoint - CurrentPoint;       //ƫ�� 
  sptr->SumError += iError;               //���� 
  
  //�����޷�
//  if(sptr->SumError > ftm_cnv_max_global)
//  { sptr->SumError  = ftm_cnv_max_global ;}
//  if(sptr->SumError < ((-1)*ftm_cnv_max_global))
//  { sptr->SumError  = (-1)*ftm_cnv_max_global) ;}
    sptr->SumError = 0; 
  
  dError = iError - sptr->LastError;     //΢�� 
 
  dError_for_use = dError * dError_lowpass_percent + sptr->LastDError*(1-dError_lowpass_percent); 
 // dError = -gyro_d_value;
  sptr->LastError = iError; 
  sptr->LastDError = dError; 
     
  re_value = (int32_t)(sptr->Proportion * iError  //������ 
           + sptr->Integral * sptr->SumError   //������ 
           + sptr->Derivative * dError_for_use);
  return(re_value);      //΢���� 
} 


pid_t pitch_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 10,        
  .Integral   = 0.10,//0.1 ,
  .Derivative = 9,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};
////////////////////roll
pid_t roll_pid00 = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 6,        
  .Integral   = 0,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

pid_t roll_pid11 = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 6,//8,        
  .Integral   = 0,//0.1,//0.1,//0.05,//0.05 ,
  .Derivative = 2 ,//4.5,//,//5,//,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};
////////////////////pitch 
pid_t pitch_pid00 = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 6,//3,        
  .Integral   = 0,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

pid_t pitch_pid11 = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 6,//,//8,        
  .Integral   = 0,//0.1,//0.1,//0.05,//0.05 ,
  .Derivative = 2 ,//4.5,//4.5,//,//5,//,//0.08 * 100,//0.05,    
  
  .LastError = 0,         
  .PrevError = 0,        
};
//////////////////////////
pid_t yaw_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 0,        
  .Integral   = 0  ,
  .Derivative = 0,    
  
  .LastError = 0,         
  .PrevError = 0,        
};

uint16_t motor_pwm0_cnv  = 0;
uint16_t motor_pwm1_cnv  = 0;
uint16_t motor_pwm2_cnv  = 0;
uint16_t motor_pwm3_cnv  = 0;

void motor_pid_control(uint32_t throttleDutyCycle,
                       imu_float_euler_angle_t * expectAngel,
                       imu_float_euler_angle_t * currentAngel,
                       pid_t *pitch_pid0,
                       pid_t *pitch_pid1,                     
                       pid_t *yaw_pid,
                       pid_t *roll_pid0,
                       pid_t *roll_pid1,                  
                       bool RCunlock )
{
  /*          �����pitch�ᣬ����Ҹߣ�pitchֵΪ��ֵ�����Խ��ֵԽ�󡣷���-180�Ⱥ��Ϊ179...
  *               |
  *      Motor0   |    Motor1
  *         �v    |    ��
  *           �v  |  ��
  *   __________�v|��___________�����ROLL�ᣬǰ���ϣ��ߣ����£��ͣ�ROLLֵΪ�������Խ��ֵԽ�󡣷���180�Ⱥ��Ϊ-179...
  *             ��|�v
  *           ��  |  �v
  *         ��    |    �v
  *      Motor3   |    Motor2
  *               |
  */ 
  //  
  static int32_t pitch_out0 = 0,pitch_out1 = 0,roll_out0 = 0,roll_out1 = 0,yaw_out = 0;
////  static uint16_t pitch_out = 0,roll_out = 0,yaw_out = 0;
////  static int16_t motor_pwm0_duty  = 0;
////  static int16_t motor_pwm1_duty  = 0;
////  static int16_t motor_pwm2_duty  = 0;
////  static int16_t motor_pwm3_duty  = 0;
//  static uint16_t motor_pwm0_cnv  = 0;
//  static uint16_t motor_pwm1_cnv  = 0;
//  static uint16_t motor_pwm2_cnv  = 0;
//  static uint16_t motor_pwm3_cnv  = 0;
  
  pitch_pid0->ExpectPoint = expectAngel->imu_pitch;
  roll_pid0->ExpectPoint  = expectAngel->imu_roll;
  
  if(throttleDutyCycle > THROTTLE_DUTY_MAX) {throttleDutyCycle = THROTTLE_DUTY_MAX;} ////�������ֵ���ƣ�������
  
  //��һ��ת�������� umod=59999;����PWMռ�ձ�50%~100%�� 30000 - 60000 ������Ҫ 
  //throttleDutyCycle = throttleDutyCycle * 600 
  uint32_t throttleDutyCycle_cnv = throttleDutyCycle * (ftm_uMod_global+1) / 100 ; 

  if((gyro_offset_done == true)&&(RCunlock == true) && (throttleDutyCycle_cnv > ftm_cnv_min_global))// && (isAngleProtected == false))
  {
    roll_out0 = (int32_t)(LocPIDCalc(currentAngel->imu_roll ,roll_pid0,gyro_roll_global,1));
    roll_pid1->ExpectPoint  = -roll_out0;
    roll_out1  =(int32_t)(LocPIDCalc( (double)(gyro_roll_global*Gyro_G) ,roll_pid1,gyro_roll_global,0.5));
    
    pitch_out0 = (int32_t)(LocPIDCalc(currentAngel->imu_pitch ,pitch_pid0,gyro_pitch_global,1));
    pitch_pid1->ExpectPoint  = -pitch_out0;
    pitch_out1  =(int32_t)(LocPIDCalc( (double)(gyro_pitch_global*Gyro_G) ,pitch_pid1,gyro_pitch_global,0.5));
    
    
    sendLineX(0x1f,(((float)roll_out1)/100));
    sendLineX(0x2f,(((float)pitch_out1)/100));
//    sendLineX(0x2f,(((float)motor_pwm1_cnv)/ftm_uMod_global));
//    sendLineX(0x3f,(((float)motor_pwm2_cnv)/ftm_uMod_global));
//    sendLineX(0x4f,(((float)motor_pwm3_cnv)/ftm_uMod_global));
    
    
    motor_pwm0_cnv  = (uint16_t)(throttleDutyCycle_cnv - pitch_out1 - roll_out1 - yaw_out) ;
    motor_pwm1_cnv  = (uint16_t)(throttleDutyCycle_cnv + pitch_out1 - roll_out1 + yaw_out) ;
    motor_pwm2_cnv  = (uint16_t)(throttleDutyCycle_cnv + pitch_out1 + roll_out1 - yaw_out) ;
    motor_pwm3_cnv  = (uint16_t)(throttleDutyCycle_cnv - pitch_out1 + roll_out1 + yaw_out) ;   
    
    if(motor_pwm0_cnv > ftm_cnv_max_global){ motor_pwm0_cnv = ftm_cnv_max_global;}
    if(motor_pwm1_cnv > ftm_cnv_max_global){ motor_pwm1_cnv = ftm_cnv_max_global;}
    if(motor_pwm2_cnv > ftm_cnv_max_global){ motor_pwm2_cnv = ftm_cnv_max_global;}
    if(motor_pwm3_cnv > ftm_cnv_max_global){ motor_pwm3_cnv = ftm_cnv_max_global;}
    
    if(motor_pwm0_cnv < ftm_cnv_min_global){ motor_pwm0_cnv = ftm_cnv_min_global;}
    if(motor_pwm1_cnv < ftm_cnv_min_global){ motor_pwm1_cnv = ftm_cnv_min_global;}
    if(motor_pwm2_cnv < ftm_cnv_min_global){ motor_pwm2_cnv = ftm_cnv_min_global;}
    if(motor_pwm3_cnv < ftm_cnv_min_global){ motor_pwm3_cnv = ftm_cnv_min_global;}
  }   
  else
  {
    motor_pwm0_cnv  = ftm_cnv_stop_global;
    motor_pwm1_cnv  = ftm_cnv_stop_global;
    motor_pwm2_cnv  = ftm_cnv_stop_global;
    motor_pwm3_cnv  = ftm_cnv_stop_global;
  }

//  PRINTF("pwm0 = %3d ,pwm1 = %3d ,pwm2 = %3d ,pwm3 = %3d\r\n" ,motor_pwm0_duty,motor_pwm1_duty,motor_pwm2_duty,motor_pwm3_duty);
//   PRINTF("cnv0 = %3d ,cnv1 = %3d ,cnv2 = %3d ,cnv3 = %3d\r\n" ,motor_pwm0_cnv,motor_pwm1_cnv,motor_pwm2_cnv,motor_pwm3_cnv); 

  motor_cnv_reflash((uint16_t)motor_pwm0_cnv,
                    (uint16_t)motor_pwm1_cnv,
                    (uint16_t)motor_pwm2_cnv,
                    (uint16_t)motor_pwm3_cnv);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
