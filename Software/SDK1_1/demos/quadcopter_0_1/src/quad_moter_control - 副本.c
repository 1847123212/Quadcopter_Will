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
  
  sptr->Proportion = 0;    //比例常数 Proportional Const 
  sptr->Integral   = 0;    //积分常数 Integral Const 
  sptr->Derivative = 0;    //微分常数 Derivative Const 
  sptr->ExpectPoint   = 0;         
} 


////IncPIDCalc ( ) 
////原        形  int IncPIDCalc(int) 
////描        述  增量式 PID 计算 
////输入参数  PID 调节当前采样值 
////输出参数  计算增量 
////头  文  件  Spmc75_PID.h 
////库  文  件  DigitalPID_V100 
////注意事项  （式 2－5）增量式 PID 算法的实现。 
////例        子  uiGoalvalue  ＋＝  IncPIDCalc  (1998);  //位置式 PID 控制算法通过增量式控
////制算法递推实现，当前采样得到转速 1998rpm。    
////增量式 PID 控制设计 
//int IncPIDCalc(int CurrentPoint) //
//{ 
//  register int iError, iIncpid; 
//  //当前误差 
//  iError = sptr->ExpectPoint - CurrentPoint; 
//  //增量计算 
//  iIncpid = sptr->Proportion * iError               //E[k]项 
//    - sptr->Integral   * sptr->LastError     //E[k－1]项 
//      + sptr->Derivative * sptr->PrevError;   //E[k－2]项 
//  //存储误差，用于下次计算 
//  sptr->PrevError = sptr->LastError; 
//  sptr->LastError = iError; 
//  //返回增量值 
//  return(iIncpid); 
//} 

int16_t IncPIDCalc(double CurrentPoint , pid_t *sptr)
{ 
  int16_t  iIncpid;    
  double iError;
  iError = sptr->ExpectPoint - CurrentPoint;    //当前误差
  
  //增量计算 
  iIncpid =(int16_t)(sptr->Proportion * iError              //E[k]项 
          - sptr->Integral   * sptr->LastError     //E[k－1]项 
          + sptr->Derivative * sptr->PrevError);    //E[k－2]项 
  
  //存储误差，用于下次计算 
  sptr->PrevError = sptr->LastError; 
  sptr->LastError = iError; 
 
  //返回增量值 
  return(iIncpid); 
} 

int32_t LocPIDCalc(double CurrentPoint , pid_t *sptr , double gyro_d_value) 
{ 
  double  iError,dError; 
  int32_t re_value;
  iError = sptr->ExpectPoint - CurrentPoint;       //偏差 
  sptr->SumError += iError;               //积分 
  
  if(sptr->SumError > 20000)
  { sptr->SumError  = 20000 ;}
  if(sptr->SumError < -20000)
  { sptr->SumError  = -20000 ;}
  
//  sptr->SumError = 0;
  
  dError = iError - sptr->LastError;     //微分 
 // dError = -gyro_d_value;
  sptr->LastError = iError; 
     
  re_value = (int32_t)(sptr->Proportion * iError  //比例项 
           + sptr->Integral * sptr->SumError   //积分项 
           + sptr->Derivative * dError);
  return(re_value);      //微分项 
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

pid_t roll_pid = {
  .ExpectPoint = 0,       
  .SumError    = 0,          
  
  .Proportion = 8,//8,        
  .Integral   = 0,//0.1,//0.05,//0.05 ,
  .Derivative = 0,//,//5,//,//0.08 * 100,//0.05,    
  
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

uint16_t motor_pwm0_cnv  = 0;
uint16_t motor_pwm1_cnv  = 0;
uint16_t motor_pwm2_cnv  = 0;
uint16_t motor_pwm3_cnv  = 0;

void motor_pid_control(uint32_t throttleDutyCycle,
                       imu_float_euler_angle_t * expectAngel,
                       imu_float_euler_angle_t * currentAngel,
                       pid_t *pitch_pid,
                       pid_t *yaw_pid,
                       pid_t *roll_pid,
                       bool RCunlock )//unfinished.20150105
{
/*          这个是ROLL轴，左低右高，ROLL值为正，差距越大值越大。翻过180度后变为-179...
*               |
*      Motor3   |    Motor2
*         ╲    |    ／
*           ╲  |  ／
*   __________╲|／___________这个是pitch轴，前（上）高，（下）低，pitch值为正，差距越大值越大。翻过180度后变为-179...
*             ／|╲
*           ／  |  ╲
*         ／    |    ╲
*      Motor0   |    Motor1
*               |
*/ 
  
  static int32_t pitch_out = 0,roll_out = 0,yaw_out = 0;
//  static uint16_t pitch_out = 0,roll_out = 0,yaw_out = 0;
//  static int16_t motor_pwm0_duty  = 0;
//  static int16_t motor_pwm1_duty  = 0;
//  static int16_t motor_pwm2_duty  = 0;
//  static int16_t motor_pwm3_duty  = 0;
//  static uint16_t motor_pwm0_cnv  = 0;
//  static uint16_t motor_pwm1_cnv  = 0;
//  static uint16_t motor_pwm2_cnv  = 0;
//  static uint16_t motor_pwm3_cnv  = 0;
  
  pitch_pid->ExpectPoint = expectAngel->imu_pitch;
  roll_pid->ExpectPoint  = expectAngel->imu_roll;
  
//  pitch_out += IncPIDCalc(currentAngel->imu_pitch , pitch_pid);
//  roll_out  += IncPIDCalc(currentAngel->imu_roll  , roll_pid);
  
  pitch_out =(int32_t)(LocPIDCalc(currentAngel->imu_pitch ,pitch_pid,gyro_pitch_global));
  roll_out  =(int32_t)(LocPIDCalc(currentAngel->imu_roll ,roll_pid,gyro_roll_global));
  
//  if(pitch_out > 3) pitch_out = 3;
 // if(roll_out > 3) roll_out = 3;
//  if(pitch_out < -5) pitch_out = -5;
//  if(roll_out < -5) roll_out = -5;
  
  if(throttleDutyCycle > 80) throttleDutyCycle = 80; ////临时调小做保护
  throttleDutyCycle = throttleDutyCycle * 600 ; //umod=59999;所以50%是 30000 - 60000，到时候再改。
  
  if((RCunlock == true) && (throttleDutyCycle > (60*600) ))
//  throttleDutyCycle = 65;
//    if(1)
  {
//    motor_pwm0_duty  = throttleDutyCycle - pitch_out - roll_out - yaw_out ;
//    motor_pwm1_duty  = throttleDutyCycle - pitch_out + roll_out + yaw_out ;
//    motor_pwm2_duty  = throttleDutyCycle + pitch_out + roll_out - yaw_out ;
//    motor_pwm3_duty  = throttleDutyCycle + pitch_out - roll_out + yaw_out ;
    
//    motor_pwm0_cnv  = (uint16_t)(throttleDutyCycle - pitch_out - roll_out - yaw_out) ;
//    motor_pwm1_cnv  = (uint16_t)(throttleDutyCycle - pitch_out + roll_out + yaw_out) ;
//    motor_pwm2_cnv  = (uint16_t)(throttleDutyCycle + pitch_out + roll_out - yaw_out) ;
//    motor_pwm3_cnv  = (uint16_t)(throttleDutyCycle + pitch_out - roll_out + yaw_out) ;   
    
//    motor_pwm0_cnv  = (uint16_t)(throttleDutyCycle - pitch_out);// - roll_out - yaw_out) ;
    motor_pwm1_cnv  = (uint16_t)(throttleDutyCycle - roll_out);// + roll_out + yaw_out) ;
//    motor_pwm2_cnv  = (uint16_t)(throttleDutyCycle + pitch_out);// + roll_out - yaw_out) ;
    motor_pwm3_cnv  = (uint16_t)(throttleDutyCycle + roll_out);// - roll_out + yaw_out) ; 
    
//    if(motor_pwm0_duty > 98){ motor_pwm0_duty = 98;}
//    if(motor_pwm1_duty > 98){ motor_pwm1_duty = 98;}
//    if(motor_pwm2_duty > 98){ motor_pwm2_duty = 98;}
//    if(motor_pwm3_duty > 98){ motor_pwm3_duty = 98;}
//    
//    if(motor_pwm0_duty < 50){ motor_pwm0_duty = 50;}
//    if(motor_pwm1_duty < 50){ motor_pwm1_duty = 50;}
//    if(motor_pwm2_duty < 50){ motor_pwm2_duty = 50;}
//    if(motor_pwm3_duty < 50){ motor_pwm3_duty = 50;}
  }
  else
  {
//    motor_pwm0_duty  = 50 ;
//    motor_pwm1_duty  = 50 ;
//    motor_pwm2_duty  = 50 ;
//    motor_pwm3_duty  = 50 ;
        motor_pwm0_cnv  = 30000 ;
        motor_pwm1_cnv  = 30000 ;
        motor_pwm2_cnv  = 30000 ;
        motor_pwm3_cnv  = 30000 ;
    
  }

//  PRINTF("pwm0 = %3d ,pwm1 = %3d ,pwm2 = %3d ,pwm3 = %3d\r\n" ,motor_pwm0_duty,motor_pwm1_duty,motor_pwm2_duty,motor_pwm3_duty);
//   PRINTF("cnv0 = %3d ,cnv1 = %3d ,cnv2 = %3d ,cnv3 = %3d\r\n" ,motor_pwm0_cnv,motor_pwm1_cnv,motor_pwm2_cnv,motor_pwm3_cnv); 
  motor_cnv_reflash(motor_pwm0_cnv,
                    motor_pwm1_cnv,
                    motor_pwm2_cnv,
                    motor_pwm3_cnv);
//  motor_pwm_reflash(motor_pwm0_duty ,
//                    motor_pwm1_duty ,
//                    motor_pwm2_duty ,
//                    motor_pwm3_duty );
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
