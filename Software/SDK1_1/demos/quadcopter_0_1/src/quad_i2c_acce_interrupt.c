/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "quad_common.h"
//#include "quad_i2c_config.h"




i2c_master_state_t i2cMaster;
i2c_device_t accel_device =
  {
    .address = FXOS8700_ADDR,
    .baudRate_kbps = FXOS8700_BAUDRATE
  };
i2c_device_t gyro_device =
  {
    .address = GYRO_ADDR,
    .baudRate_kbps = GYRO_BAUDRATE
  };

i2c_status_t I2C_fxos8700Init(void)
{
  uint8_t write_value = 0;
  uint8_t accel_reg = 0;
  uint8_t read_temp = 0;

//  i2c_status_t run_result = kStatus_I2C_Fail;

//  PRINTF("\r\n==I2C Accelerometer Interrupt_Example ==\r\n\r\n");
//  PRINTF("If full-scale is 2g , then 1g = 4096 \r\n");
//  PRINTF("If full-scale is 4g , then 1g = 2048 \r\n");
//  PRINTF("If full-scale is 8g , then 1g = 1024 \r\n");

  I2C_DRV_MasterInit(FXOS8700_I2C_INSTANCE, &i2cMaster);

  //who am I
  accel_reg = FXOS8700_WHOAMI;
  I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &read_temp, 1, 100);
  if (read_temp != FXOS8700_WHOAMI_VAL )
  {
    PRINTF("no a FXO8700 chip");
    return kStatus_I2C_Fail;
  }

  // write 0000 0000 = 0x00 to accelerometer control register 1 to place FXOS8700 into standby mode ,
  // then you can do other settings.
  // [7-1] = 0000 000
  // [0]: active=0
  accel_reg = FXOS8700_CTRL_REG1;
  write_value = 0x00;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0001 1111 = 0x1F to magnetometer control register 1
  // [7]: m_acal=0:  0:auto calibration disabled 1:auto calibration enabled
  // [6]: m_rst=0: no one-shot magnetic reset
  // [5]: m_ost=0: no one-shot magnetic measurement
  // [4-2]: m_os=111=7: 8x oversampling (for 200Hz) to reduce magnetometer noise
  // [1-0]: m_hms=11=3: select hybrid mode with accel and magnetometer active
  accel_reg = FXOS8700_M_CTRL_REG1;
  write_value = 0x1F;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0010 0000 = 0x20 to magnetometer control register 2
  // [7]: reserved
  // [6]: reserved
  // [5]: hyb_autoinc_mode=1 to map the magnetometer registers to follow the accelerometer registers
  //the user may do a burst read of 12 bytes starting from OUT_X_MSB (address 0x1) to read out both
  //the current accelerometer and magnetometer data in one contiguous operation.
  // [4]: m_maxmin_dis=0 to retain default min/max latching even though not used
  // [3]: m_maxmin_dis_ths=0
  // [2]: m_maxmin_rst=0
  // [1-0]: m_rst_cnt=00 to enable magnetic reset each cycle
  accel_reg = FXOS8700_M_CTRL_REG2;
  write_value = 0x20;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0000 0001= 0x01 to XYZ_DATA_CFG register
  // [7]: reserved
  // [6]: reserved
  // [5]: reserved
  // [4]: hpf_out=0
  // [3]: reserved
  // [2]: reserved
  // [1-0]: fs=02 for accelerometer range of +/-8g range with 0.976mg/LSB
  accel_reg = FXOS8700_XYZ_DATA_CFG;
  write_value = 0x02;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  // write 0000 1101b = 0x0D to accelerometer control register 1
  // [7-6]: aslp_rate=00
  // [5-3]: dr=000 for 800Hz data rate //(when in hybrid mode)
  // [2]: lnoise=0 for normal mode  //low noise mode
  // [1]: f_read=0 for normal 16 bit reads
  // [0]: active=1 to take the part out of standby and enable sampling

  accel_reg = FXOS8700_CTRL_REG1;
  write_value = 0x01;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }
//  ///fgz define
//
//  accel_reg = FXOS8700_HP_FILTER_CUTOFF;
//  write_value = 0x10;
//
//  if( kStatus_I2C_Success !=
//     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
//  {
//    //    PRINTF("fail0");
//    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
//    //    return kStatus_I2C_Fail;
//  }
//
  return kStatus_I2C_Success ;
}

extern bool isFXOS8700Int1Trigger ;
void I2C_fxos8700AutoCalibration(void)
{
  uint8_t write_value = 0;
  uint8_t accel_reg = 0;
  uint8_t read_temp = 0;

  static int16_t autoCalResetCtr = 0;

  accel_reg = 0x2B;
  write_value = 0x40;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) )
  {
    //    PRINTF("fail0");
    //    I2C_DRV_MasterDeinit(FXOS8700_I2C_INSTANCE);
    //    return kStatus_I2C_Fail;
  }

  uint32_t magThreshold = 1000;
  uint16_t magThresholdHi = (magThreshold & 0xFF00) >> 8;
  uint16_t magThresholdLo = magThreshold & 0xFF;

  accel_reg = 0x6A;
  write_value = ( 0x80 | magThresholdHi ) ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  accel_reg = 0x6B;
  write_value = ( magThresholdLo ) ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  accel_reg = 0x6C;
  write_value = 0x01 ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  accel_reg = 0x69;
  write_value = 0x7B ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;
  // enable interrupts for DRDY using CTRL_REG4
  accel_reg = 0x2D;
  write_value = 0x01 ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;
  // route interrupts to INT1 pin using CTRL_REG5
  accel_reg = 0x2e;
  write_value = 0x01 ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  // Setup device
  // via M_CTRL_REG1 (0x5B): Hybrid mode, OS = 32, Auto Cal
  // via M_CTRL_REG2 (0x5C): Hybrid auto increment, maxmin disable
  // threshold
  // via CTRL_REG1 (0x2A): ODR = 50Hz, ACTIVE mode
  accel_reg = 0x5b;
  write_value = 0x9f ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  accel_reg = 0x5c;
  write_value = 0x20 ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;

  accel_reg = 0x2a;
  write_value = 0x31 ;
  I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;


  while(1)
  {

    if(isFXOS8700Int1Trigger == true)
    {
      isFXOS8700Int1Trigger = false;

      accel_reg = 0x5e;
      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &read_temp, 1, 100);
      if((read_temp & 0x02) == 0x02)
      {
        PRINTF("Interrupt due to Magnetometer Vector Magnitude feature\r\n");
        PRINTF("Magnetic Jamming detected\r\n");
        autoCalResetCtr++;
      }
      else
      {
        autoCalResetCtr = 0;
      }

      accel_reg = 0x0c;
      I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &read_temp, 1, 100);
      if((read_temp & 0x01) == 0x01)
      {
        PRINTF("print Interrupt due to data ready\r\n");
        static  mems_data_t MemsRawData_cal;
        I2C_getAccelMangData(&MemsRawData_cal);
        double mx = MemsRawData_cal.magn_x;
        double my = MemsRawData_cal.magn_y;
        double mz = MemsRawData_cal.magn_z;
        my = -my;

        PRINTF("Magnetic Vector Magnitude = %4.1f uT" , (sqrt(mx*mx + my*my + mz*mz) / 10) );

        PRINTF("Heading ===> %.2f degrees from NORTH" , ( atan2(-my, mx) * 57.2957795));
      }
      if(autoCalResetCtr > 10)
      {
        PRINTF("Resetting Hard Iron Estimation...\r\n");
        accel_reg = 0x5c;
        write_value = 0x24 ;
        I2C_DRV_MasterSendDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1, &write_value, 1, 100) ;
        autoCalResetCtr = 0;
        break;
      }
    }
  }

}


i2c_status_t I2C_l3g4200dInit(void)
{
  uint8_t write_value = 0;
  uint8_t gyro_reg = 0;
  uint8_t read_temp = 0;
  uint8_t x_msb = 0 ,y_msb = 0, z_msb = 0 ;
  uint8_t x_lsb = 0 ,y_lsb = 0, z_lsb = 0 ;
  uint8_t show_time = 0;

//  I2C_DRV_MasterInit(GYRO_I2C_INSTANCE, &i2cMaster);

  uint8_t who = 0;

  /*Init gyro chip*/
  gyro_reg = GYRO_WHO_AM_I;

  if( kStatus_I2C_Success !=
     I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &read_temp, 1, 1000) )
  {
    PRINTF("fail0");
  //  I2C_DRV_MasterDeinit(GYRO_I2C_INSTANCE);
    return kStatus_I2C_Fail;
  }
  if(GYRO_ID != read_temp)
  {
    PRINTF("no GYRO chip");
  }

  /* ODR: 100Hz, Cut-Off: 12.5,         */
  gyro_reg = GYRO_CTRL_REG1;
  write_value = 0x0f;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

  /* Continous update,full scale 2000dps */
  //FS1-FS0 bit5-bit4 Full Scale selection. Default value: 00
  //(00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
  gyro_reg = GYRO_CTRL_REG4;
  write_value = 0x20;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

  /* FIFO Enable                        */
  gyro_reg = GYRO_CTRL_REG5;
  write_value = 0x40;
  I2C_DRV_MasterSendDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1, &write_value, 1, 1000);

   return kStatus_I2C_Success ;
}

i2c_status_t I2C_getAccelMangData(volatile mems_data_t * pMemsRawData)
{
  uint8_t fxos8700_buffer[FXOS8700_READ_LEN];
  uint8_t accel_reg = 0;
  accel_reg = FXOS8700_STATUS ; //0x00
  I2C_DRV_MasterReceiveDataBlocking(FXOS8700_I2C_INSTANCE, &accel_device, &accel_reg, 1,
                                    fxos8700_buffer, FXOS8700_READ_LEN, 100);

  //  accel_x = DataCombine(fxos8700_buffer[1],fxos8700_buffer[2]);
  //  accel_y = DataCombine(fxos8700_buffer[3],fxos8700_buffer[4]);
  //  accel_z = DataCombine(fxos8700_buffer[5],fxos8700_buffer[6]);
  //
  //  magn_x = DataCombine(fxos8700_buffer[7],fxos8700_buffer[8]);
  //  magn_x = DataCombine(fxos8700_buffer[9],fxos8700_buffer[10]);
  //  magn_x = DataCombine(fxos8700_buffer[11],fxos8700_buffer[12]);

  //OUT_X_MSB,OUT_X_LSB,OUT_Y_MSB,OUT_Y_LSB,OUT_Z_MSB,OUT_Z_LSB from 0x01 to 0x06
  //MOUT_X_MSB,MOUT_X_LSB,MOUT_Y_MSB,MOUT_Y_LSB,MOUT_Z_MSB,OMUT_Z_LSB from 0x33 to 0x38
  //already set the fxos8700 continuous read from 0x01 - 0x06 and 0x33 - 0x38,
  //see detailed in I2C_fxos8700Init()
//  pMemsRawData->accel_x = (double)((int16_t)((fxos8700_buffer[1]  << 6)  |  (fxos8700_buffer[2]>>2) ));
//  pMemsRawData->accel_y = (double)((int16_t)((fxos8700_buffer[3]  << 6)  |  (fxos8700_buffer[4]>>2) ));
//  pMemsRawData->accel_z = (double)((int16_t)((fxos8700_buffer[5]  << 6)  |  (fxos8700_buffer[6]>>2) ));

  pMemsRawData->accel_x = (double)(DataCombine(fxos8700_buffer[1],fxos8700_buffer[2]));
  pMemsRawData->accel_y = (double)(DataCombine(fxos8700_buffer[3],fxos8700_buffer[4]));
  pMemsRawData->accel_z = (double)(DataCombine(fxos8700_buffer[5],fxos8700_buffer[6]));

  pMemsRawData->magn_x  = (double)((int16_t)((fxos8700_buffer[7]  << 8)  |  fxos8700_buffer[8] ));
  pMemsRawData->magn_y  = (double)((int16_t)((fxos8700_buffer[9]  << 8)  |  fxos8700_buffer[10]));
  pMemsRawData->magn_z  = (double)((int16_t)((fxos8700_buffer[11] << 8)  |  fxos8700_buffer[12]));


//  static int16_t magn_min_x = 32767;
//  static int16_t magn_max_x = 0;
//  static int16_t magn_min_y = 32767;
//  static int16_t magn_max_y = 0;
//  static int16_t magn_reca_times = 0;
//  static bool reca_calculation = true;
//  if( pMemsRawData->magn_x < magn_min_x)
//  {
//    magn_min_x = pMemsRawData->magn_x ;
//  }
//  if( pMemsRawData->magn_y < magn_min_y)
//  {
//    magn_min_y = pMemsRawData->magn_x ;
//  }
//    if( pMemsRawData->magn_x > magn_max_x)
//  {
//    magn_max_x = pMemsRawData->magn_x ;
//  }
//  if( pMemsRawData->magn_y > magn_max_y)
//  {
//    magn_max_y = pMemsRawData->magn_x ;
//  }
//  static double xb,xs,yb,ys,xout,yout,angel;
//
//  if(magn_reca_times <= 200)
//  {
//     magn_reca_times++;
//  }
////  if(magn_reca_times > 200 && reca_calculation == true)
//  {
//    reca_calculation = false;
//    xs=1;
//    ys = (magn_max_x - magn_min_x) / (magn_max_y - magn_min_y);
//    xb = -xs*(magn_max_x + magn_min_x)/2;
//    yb = -ys * (magn_max_y + magn_min_y) /2 ;
//  }
//  xout = pMemsRawData->magn_x - xb;
//  yout = ys*pMemsRawData->magn_y + yb;
//  angel = atan2(-yout,xout) * 57.3 + 180;
//  PRINTF("xout = %5d , yout = %5d , angel = %3d \r\n " , (int16_t)xout,(int16_t)yout,(int16_t)angel);

//  PRINTF("accel_x = %5d , accel_y = %5d , accel_z = %5d\r\n" ,pMemsRawData->accel_x,pMemsRawData->accel_y,pMemsRawData->accel_z);
//  PRINTF("magn_x = %5d , magn_y = %5d , magn_z = %5d\r\n" ,pMemsRawData->magn_x,pMemsRawData->magn_y,pMemsRawData->magn_z);

  return kStatus_I2C_Success ;
}

int16_t gyro_y_before;
int16_t gyro_y_after;

i2c_status_t I2C_getGyroData(volatile mems_data_t * pMemsRawData)
{
  uint8_t gyro_buffer[GYRO_READ_LEN];
  uint8_t gyro_reg = 0;

  /*******************
  In order to read multiple bytes, it is necessary to assert the most significant bit of the sub-
  address field. In other words, SUB(7) must be equal to 1, while SUB(6-0) represents the
  address of the first register to be read.
  *******************/
  gyro_reg = (GYRO_OUT_X_LSB | 0x80) ;

  I2C_DRV_MasterReceiveDataBlocking(GYRO_I2C_INSTANCE, &gyro_device, &gyro_reg, 1,
                                    gyro_buffer, GYRO_READ_LEN, 100);

  //  gyro_x = DataCombine(gyro_buffer[1],gyro_buffer[0]);
  //  gyro_y = DataCombine(gyro_buffer[3],gyro_buffer[2]);
  //  gyro_z = DataCombine(gyro_buffer[5],gyro_buffer[4]);

  //OUT_X_L,OUT_X_H,OUT_Y_L,OUT_Y_H,OUT_Z_L,OUT_Z_H from 0x28 to 0x2d
//  pMemsRawData->gyro_x = (double)((-1)*((double)((int16_t)((gyro_buffer[1] << 8) | gyro_buffer[0]))));
//  pMemsRawData->gyro_y = (double)((-1)*((double)((int16_t)((gyro_buffer[3] << 8) | gyro_buffer[2]))));
//  pMemsRawData->gyro_z = (double)((-1)*((double)((int16_t)((gyro_buffer[5] << 8) | gyro_buffer[4]))));

  pMemsRawData->gyro_x = (double)((-1)*((double)(DataCombine_gyro(gyro_buffer[1] , gyro_buffer[0]))));
  pMemsRawData->gyro_y = (double)((-1)*((double)(DataCombine_gyro(gyro_buffer[3] , gyro_buffer[2]))));
  pMemsRawData->gyro_z = (double)((-1)*((double)(DataCombine_gyro(gyro_buffer[5] , gyro_buffer[4]))));

  static int16_t gyro_aver_times = 0 ;
  static bool gyro_aver_cal_flag = true;
  static double gyro_x_aver = 0 ;
  static double gyro_y_aver = 0 ;
  static double gyro_z_aver = 0 ;

  static double gyro_x_last = 0 ;
  static double gyro_y_last = 0 ;
  static double gyro_z_last = 0 ;

  static double gyro_x_current = 0 ;
  static double gyro_y_current = 0 ;
  static double gyro_z_current = 0 ;

  gyro_x_current = pMemsRawData->gyro_x;
  gyro_y_current = pMemsRawData->gyro_y;
  gyro_z_current = pMemsRawData->gyro_z;

  static uint32_t aver_times = 500;
  if((gyro_aver_times < aver_times) && (gyro_aver_cal_flag == true))
  {
    if(   (gyro_x_current - gyro_x_last) < 20 && (gyro_x_current - gyro_x_last) > -20
       && (gyro_y_current - gyro_y_last) < 20 && (gyro_y_current - gyro_y_last) > -20
       && (gyro_z_current - gyro_z_last) < 20 && (gyro_z_current - gyro_z_last) > -20 )
    {
      gyro_x_aver += gyro_x_current;
      gyro_y_aver += gyro_y_current;
      gyro_z_aver += gyro_z_current;
      gyro_aver_times++;
    }
    else
    {
      gyro_x_aver = 0;
      gyro_y_aver = 0;
      gyro_z_aver = 0;
      gyro_aver_times = 0;
    }
  }
  if((gyro_aver_times >= aver_times) && (gyro_aver_cal_flag == true))
  {
    gyro_x_aver = gyro_x_aver/aver_times;
    gyro_y_aver = gyro_y_aver/aver_times;
    gyro_z_aver = gyro_z_aver/aver_times;
    gyro_aver_cal_flag = false;
    gyro_offset_done = true;
  }

  pMemsRawData->gyro_x -= gyro_x_aver;//gyro_x_aver;
  pMemsRawData->gyro_y -= gyro_y_aver;//gyro_y_aver;
  pMemsRawData->gyro_z -= gyro_z_aver;//gyro_z_aver;

  gyro_x_last = gyro_x_current;
  gyro_y_last = gyro_y_current;
  gyro_z_last = gyro_z_current;
  //  gyro_y_before =   pMemsRawData->gyro_y;
  //
  //  pMemsRawData->gyro_x = (int16_t)KalmanFilter1(pMemsRawData->gyro_x,10,10,1);
  //  pMemsRawData->gyro_y = (int16_t)KalmanFilter2(pMemsRawData->gyro_y,15,15,4);
  //  pMemsRawData->gyro_z = (int16_t)KalmanFilter3(pMemsRawData->gyro_z,10,10,1);
  //
  //  gyro_y_after =   pMemsRawData->gyro_y;
  //  int16_t kal_gyro_x =0;
  //  kal_gyro_x = (int16_t)KalmanFilter(pMemsRawData->gyro_x,10,10,1);
  //
  //  PRINTF("gyro_x = %5d , gyro_y = %5d , gyro_z = %5d , kal_gyro_x = %d\r\n\r\n" ,pMemsRawData->gyro_x,pMemsRawData->gyro_y,pMemsRawData->gyro_z,kal_gyro_x);

  return kStatus_I2C_Success ;
}

int16_t DataCombine_gyro(uint8_t msb, uint8_t lsb)
{
  int16_t sign = 1;
  uint16_t value = 0;
  /*follow the rules of MISRA-2004*/
  sign = (msb & 0x80) ? (-1) : 1;
  value = msb & 0x7F;
  value = (value << 8U) + (lsb);
  if (sign == -1)
  {
    value = ((uint16_t)(~value + 1)) & 0x7FFFU;
  }
  value *= sign;
  return value;
}


int16_t DataCombine(uint8_t msb, uint8_t lsb)
{
  int16_t sign = 1;
  uint16_t value = 0;
  /*follow the rules of MISRA-2004*/
  sign = (msb & 0x80) ? (-1) : 1;
  value = msb & 0x7F;
  value = (value << 6U) + ((lsb) >> 2U);
  if (sign == -1)
  {
    value = ((uint16_t)(~value + 1)) & 0x1FFFU;
  }
  value *= sign;
  return value;
}

double KalmanFilter(const double ResrcData,
                    double ProcessNiose_Q,
                    double MeasureNoise_R,
                    double InitialPrediction)
{
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;

        static double x_last;

        double x_mid = x_last;
        double x_now;

        static double p_last;

        double p_mid ;
        double p_now;
        double kg;

        x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)

        p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

        kg = p_mid / ( p_mid + R ); //kg为kalman filter，R为噪声

        x_now = x_mid + kg * ( ResrcData - x_mid );//估计出的最优值

        p_now = (1 - kg) * p_mid;//最优值对应的covariance

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;
}


double KalmanFilter1(const double ResrcData,
                    double ProcessNiose_Q,
                    double MeasureNoise_R,
                    double InitialPrediction)
{
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;

        static double x_last;

        double x_mid = x_last;
        double x_now;

        static double p_last;

        double p_mid ;
        double p_now;
        double kg;

        x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)

        p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

        kg = p_mid / ( p_mid + R ); //kg为kalman filter，R为噪声

        x_now = x_mid + kg * ( ResrcData - x_mid );//估计出的最优值

        p_now = (1 - kg) * p_mid;//最优值对应的covariance

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;
}


double KalmanFilter2(const double ResrcData,
                    double ProcessNiose_Q,
                    double MeasureNoise_R,
                    double InitialPrediction)
{
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;

        static double x_last;

        double x_mid = x_last;
        double x_now;

        static double p_last;

        double p_mid ;
        double p_now;
        double kg;

        x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)

        p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

        kg = p_mid / ( p_mid + R ); //kg为kalman filter，R为噪声

        x_now = x_mid + kg * ( ResrcData - x_mid );//估计出的最优值

        p_now = (1 - kg) * p_mid;//最优值对应的covariance

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;
}

double KalmanFilter3(const double ResrcData,
                    double ProcessNiose_Q,
                    double MeasureNoise_R,
                    double InitialPrediction)
{
        double R = MeasureNoise_R;
        double Q = ProcessNiose_Q;

        static double x_last;

        double x_mid = x_last;
        double x_now;

        static double p_last;

        double p_mid ;
        double p_now;
        double kg;

        x_mid = x_last; //x_last=x(k-1|k-1),x_mid=x(k|k-1)

        p_mid = p_last + Q; //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

        kg = p_mid / ( p_mid + R ); //kg为kalman filter，R为噪声

        x_now = x_mid + kg * ( ResrcData - x_mid );//估计出的最优值

        p_now = (1 - kg) * p_mid;//最优值对应的covariance

        p_last = p_now; //更新covariance值
        x_last = x_now; //更新系统状态值

        return x_now;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
