/*************************************************************************************************************
Բ�㲩ʿС���������2014������Դ��������:
��Դ��������ο�,Բ�㲩ʿ����Դ�����ṩ�κ���ʽ�ĵ���,Ҳ������ʹ�ø�Դ��������ֵ���ʧ����.
�û�������ѧϰ��Ŀ���޸ĺ�ʹ�ø�Դ����.
���û����޸ĸ�Դ����ʱ,�����Ƴ��ò��ְ�Ȩ��Ϣ�����뱣��ԭ������.

������Ϣ������ʹٷ���վwww.etootle.com, �ٷ�����:http://weibo.com/xiaosizhou
**************************************************************************************************************/
#include "stm32f10x_lib.h"
#include <math.h>
#include "b_led.h"
#include "b_bluetooth.h"
#include "b_motor.h"
#include "b_adc.h"
#include "b_sys.h"
#include "b_mpu6050.h"
#include "b_imu.h"
#include "b_parameter.h"

#define IDLE 0
#define UPDATE_COMMAND 1
#define UPDATE_MOTOR   2
#define RESET_MOTOR    3
