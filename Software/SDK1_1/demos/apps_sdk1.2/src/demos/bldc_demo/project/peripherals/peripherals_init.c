/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2011 Freescale Semiconductor
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file   peripherals_init.c
*
* @brief  Init functions
*        
* @version 1.0.3.0
* 
* @date Nov-27-2013
* 
*******************************************************************************/
#include "peripherals_init.h"
/******************************************************************************
* Constants and macros
******************************************************************************/
#define TWO_POWER_N_SAMPLES  6  /* select carefully - COP timer not updated */

/******************************************************************************
* Local variables
******************************************************************************/
static MC33927_LATCH_T mudtStatusRegister0;
static MC33927_MODE_T  mudtStatusRegister1;
static MC33927_LATCH_T mudtStatusRegister2;
static MC33927_MODE_COMMAND_T mudtModeCommands;
static MC33927_LATCH_T mudtMaskInterrupts;
static MC33927_LATCH_T mudtClearFlags;
/******************************************************************************
* Local variables
******************************************************************************/
const Pwm_sChannelControl bldcCommutationTableComp[8] = { // mask, swap
                                                    {0x30,0x02},    // [0] - sector 0
                                                    {0x0C,0x04},    // [1] - sector 1
                                                    {0x03,0x04},    // [2] - sector 2
                                                    {0x30,0x01},    // [3] - sector 3
                                                    {0x0C,0x01},    // [4] - sector 4
                                                    {0x03,0x02},    // [5] - sector 5
                                                    {0x00,0x06},    // [6] - alignment vector (combination of sectors 0 & 1)
                                                    {0x3F,0x00},    // [7] - PWM Off
                                                  };

const unsigned short bldcAdcSectorCfg[8] = {ADC_CHANNEL_PHASEC, ADC_CHANNEL_PHASEB, ADC_CHANNEL_PHASEA,
                                            ADC_CHANNEL_PHASEC, ADC_CHANNEL_PHASEB, ADC_CHANNEL_PHASEA,
                                            ADC_CHANNEL_PHASEA, ADC_CHANNEL_PHASEA
                                           };   
/*
LED - PTD6 - ALT1

Brake control - PTE24  - ALT1
USER LED + SW2 - PTB0  - ALT1 (input)
SW1           - PTA4   - ALT1 (input)

MC33937 RESET   - PTC2  - ALT1
MC33937 DRV_EN  - PTD7  - ALT1
MC33937 DRV_INT - PTB3  - ALT1 (input)
MC33937 DRV_OC  - PTB2  - ALT1 (input)

SPI0_MISO - PTD3 - ALT2
SPI0_MOSI - PTD2 - ALT2
SPI0_/SS  - PTC0 - ALT7
SPI0_SCLK - PTC5 - ALT2

PWM0 - AT - PTC1  - ALT4
PWM1 - AB - PTE25 - ALT3
PWM2 - BT - PTC3  - ALT4
PWM3 - BB - PTC4  - ALT4
PWM4 - CT - PTD4  - ALT4
PWM5 - CB - PTD5  - ALT4
*/

/*************************************************************************
 * Function Name: Clock_init
 * Parameters: none
 * Return: none
 * Description: Clock initialization
 *************************************************************************/
void Clock_init(void)
{
  //Enable clock to port
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePortA);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePortB);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePortC);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePortD);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePortE);
 
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateAdc0);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateAdc1);
 
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateFtm0);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateFtm1);
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateFtm2);
 
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGatePdb0);

  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateUart1);
   
  SIM_HAL_EnableClock(SIM_BASE, kSimClockGateSpi0);
 
  CLOCK_SYS_SetOutDiv1(0);
  CLOCK_SYS_SetOutDiv4(2);// system clock = MCGOUTCLK ; Flash Clock (bus clock) = MCGOUTCLK / 3

  CLOCK_HAL_SetDcoRangeMode(MCG_BASE, kMcgDcoRangeSelMidHigh);
  CLOCK_HAL_SetDmx32(MCG_BASE, kMcgDmx32Fine);                                                           

}

/*************************************************************************
 * Function Name: ADC0_init
 * Parameters: none
 * Return: none
 * Description: ADC0 module initialization
 *************************************************************************/
int ADC0_init(void)
{

  long tmp=0;
  long tmp1=0;
  long tmp2=0;
  register uint16 i = 0, numLoops = ((uint16) 1 << TWO_POWER_N_SAMPLES);
  tADC_CALIB adc0_calib;
  
    
  /* ADC calibration */
  ADC16_HAL_SetClkSrcMode(ADC0_BASE, kAdcClkSrcOfBusClk);
  ADC16_HAL_SetResolutionMode(ADC0_BASE, kAdcResolutionBitOfSingleEndAs12);
  ADC16_HAL_SetClkDividerMode(ADC0_BASE, kAdcClkDividerInputOf1);

  ADC16_HAL_SetHighSpeedCmd(ADC0_BASE, true);
  ADC16_HAL_SetLongSampleCycleMode(ADC0_BASE, kAdcLongSampleCycleOf4);// high-speed mode, 2 extra ADC clock cycles; 6 ADCK cycles sample time
  ADC16_HAL_SetRefVoltSrcMode(ADC0_BASE, kAdcRefVoltSrcOfVref);
  
  /* calibration loop */   
  while ((i++) < numLoops)
  { 
    /* The input channel, conversion mode continuous function, compare      */
    /* function, resolution mode, and differential/single-ended mode are    */
    /* all ignored during the calibration function.                         */
    ADC16_HAL_ConfigChn(ADC0_BASE, 0,false, false, ADC_CHANNEL_DISABLED);
    ADC16_HAL_SetHwAverageMode(ADC0_BASE, kAdcHwAverageCountOf4);
    ADC16_HAL_SetAutoCalibrationCmd(ADC0_BASE, true);
    while (!ADC16_HAL_GetChnConvCompletedCmd(ADC0_BASE, 0));
    if (ADC16_HAL_GetAutoCalibrationFailedCmd(ADC0_BASE)) 
    {
       ADC16_HAL_SetAutoCalibrationCmd(ADC0_BASE, true);
       return 1; 
    }
    tmp+= (short int) ADC16_HAL_GetOffsetValue(ADC0_BASE);
    tmp1+= ADC16_HAL_GetAutoPlusSideGainValue(ADC0_BASE);
    tmp2+= ADC16_HAL_GetAutoMinusSideGainValue(ADC0_BASE);
  }
    adc0_calib.OFS  = tmp >> TWO_POWER_N_SAMPLES;
    tmp1 = tmp1&(~0x8000);
    tmp1 = tmp1 >> TWO_POWER_N_SAMPLES;
    adc0_calib.PG  = (tmp1|0x8000);
    tmp2 = tmp2&(~0x8000);
    tmp2 = tmp2 >> TWO_POWER_N_SAMPLES;
    adc0_calib.MG  = (tmp2|0x8000);    
    ADC0->PG   = adc0_calib.PG; 
    ADC0->MG   = adc0_calib.MG; 
    ADC0->OFS  = adc0_calib.OFS;  
    
    /* End of ADC calibration */
    enable_irq(ADC0_IRQn);
    set_irq_priority(ADC0_IRQn, ISR_PRIORITY_ADC0);
    // Bus clock / 1 (24 MHz), 12-bit, Bus clock source
    ADC16_HAL_SetClkDividerMode(ADC0_BASE, kAdcClkDividerInputOf1);
    ADC16_HAL_SetResolutionMode(ADC0_BASE, kAdcResolutionBitOf12or13);
    ADC16_HAL_SetClkSrcMode(ADC0_BASE, kAdcClkSrcOfBusClk);
    ADC16_HAL_SetLowPowerCmd(ADC0_BASE, false);
    ADC16_HAL_SetLongSampleCmd(ADC0_BASE, false);
    // high-speed mode, 2 extra ADC clock cycles; 6 ADCK cycles sample time
    ADC16_HAL_SetHighSpeedCmd(ADC0_BASE, true);
    ADC16_HAL_SetLongSampleCycleMode(ADC0_BASE,kAdcLongSampleCycleOf4);
    ADC16_HAL_SetChnMuxMode(ADC0_BASE, kAdcChnMuxOfA);
    ADC16_HAL_SetAsyncClkCmd(ADC0_BASE, false);
    ADC16_HAL_SetHwCmpValue1(ADC0_BASE, 1);
    ADC16_HAL_SetHwCmpValue2(ADC0_BASE, 1);
    // H/W trigger, DMA disabled
    ADC16_HAL_SetHwTriggerCmd(ADC0_BASE, true);
    ADC16_HAL_SetRefVoltSrcMode(ADC0_BASE, kAdcRefVoltSrcOfVref);
    ADC16_HAL_SetHwCmpCmd(ADC0_BASE, false);
    ADC16_HAL_SetHwCmpGreaterCmd(ADC0_BASE, false);
    ADC16_HAL_SetHwCmpRangeCmd(ADC0_BASE, false);
    ADC16_HAL_SetDmaCmd(ADC0_BASE, false);
    
    ADC16_HAL_SetHwAverageMode(ADC0_BASE, kAdcHwAverageCountOf4);
    ADC16_HAL_SetHwAverageCmd(ADC0_BASE, false);
    ADC16_HAL_SetContinuousConvCmd(ADC0_BASE, false);
    ADC16_HAL_SetAutoCalibrationCmd(ADC0_BASE, false);
    
    ADC16_HAL_ConfigChn(ADC0_BASE, 0,false, false, ADC_CHANNEL_DCBV);
    ADC16_HAL_ConfigChn(ADC0_BASE, 1,true, false, ADC_CHANNEL_DCBV);
    
    return 0;     

}

/*************************************************************************
 * Function Name: PDB_init
 * Parameters: none
 * Return: none
 * Description: PDB module initialization
 *************************************************************************/
void PDB_init(void)
{
  PDB_HAL_Init(PDB0_BASE);
  PDB_HAL_SetPreTriggerOutputCmd(PDB0_BASE, 0, 0, true);
  PDB_HAL_SetPreTriggerOutputCmd(PDB0_BASE, 0, 1, true);
  PDB_HAL_SetPreTriggerCmd(PDB0_BASE, 0, 0, true);
  PDB_HAL_SetPreTriggerCmd(PDB0_BASE, 0, 1, true);
  PDB_HAL_SetPreTriggerBackToBackCmd(PDB0_BASE, 0, 0, false);
  PDB_HAL_SetPreTriggerBackToBackCmd(PDB0_BASE, 0, 1, true);
  
  PDB_HAL_SetPreTriggerDelayCount(PDB0_BASE, 0, 0, (PWM_MODULO / 6));
  
  PDB_HAL_SetLoadMode(PDB0_BASE, kPdbLoadImmediately);
  PDB_HAL_Enable(PDB0_BASE);
  PDB_HAL_SetModulusValue(PDB0_BASE, 0x7fff);
  
  PDB_HAL_SetLoadMode(PDB0_BASE, kPdbLoadAtNextTrigger);
  PDB_HAL_SetSeqErrIntCmd(PDB0_BASE, true);
  PDB_HAL_SetPreDivMode(PDB0_BASE, kPdbClkPreDivBy1);
  PDB_HAL_SetTriggerSrcMode(PDB0_BASE, kPdbTrigger8);
  PDB_HAL_SetPreMultFactorMode(PDB0_BASE, kPdbMultFactorAs1);
  PDB_HAL_SetLoadRegsCmd(PDB0_BASE);
  
  enable_irq(PDB0_IRQn);
  set_irq_priority(PDB0_IRQn, ISR_PRIORITY_PDB0); 
}

/*************************************************************************
 * Function Name: FTM0_init
 * Parameters: none
 * Return: none
 * Description: FlexTimer 0 initialization
 *************************************************************************/
void FTM0_init(void)
{  
  FTM_HAL_SetWriteProtectionCmd(FTM0_BASE, false);//false: Write-protection is disabled
  FTM_HAL_Enable(FTM0_BASE, true);//true: all registers including FTM-specific registers are available
  FTM_HAL_SetCounterInitVal(FTM0_BASE, (uint16_t)(-PWM_MODULO/2));
  FTM_HAL_SetMod(FTM0_BASE, (uint16_t)PWM_MODULO/2-1);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 0, 2);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 1, 2);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 2, 2);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 3, 2);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 4, 2);
  FTM_HAL_SetChnEdgeLevel(FTM0_BASE, 5, 2);
	
  FTM_HAL_SetMaxLoadingCmd(FTM0_BASE, true);//True to enable minimum loading point
  FTM_HAL_SetOutmaskPwmSyncModeCmd(FTM0_BASE, 1);//true if OUTMASK register is updated only by PWM sync\n
  FTM_HAL_SetDualChnPwmSyncCmd(FTM0_BASE, 0, true);//True to enable PWM synchronization
  FTM_HAL_SetDualChnPwmSyncCmd(FTM0_BASE, 1, true);
  FTM_HAL_SetDualChnPwmSyncCmd(FTM0_BASE, 2, true);
  FTM_HAL_SetDualChnDeadtimeCmd(FTM0_BASE, 0, true);//True to enable deadtime insertion, false to disable
  FTM_HAL_SetDualChnDeadtimeCmd(FTM0_BASE, 1, true);
  FTM_HAL_SetDualChnDeadtimeCmd(FTM0_BASE, 2, true);
  FTM_HAL_SetDualChnCompCmd(FTM0_BASE, 0, true);//True to enable complementary mode, false to disable
  FTM_HAL_SetDualChnCompCmd(FTM0_BASE, 1, true);
  FTM_HAL_SetDualChnCompCmd(FTM0_BASE, 2, true);
  FTM_HAL_SetDualChnCombineCmd(FTM0_BASE, 0, true);// True to enable channel pair to combine, false to disable
  FTM_HAL_SetDualChnCombineCmd(FTM0_BASE, 1, true);
  FTM_HAL_SetDualChnCombineCmd(FTM0_BASE, 2, true);
    
  FTM_HAL_SetDeadtimePrescale(FTM0_BASE, kFtmDivided1);
  FTM_HAL_SetDeadtimeCount(FTM0_BASE, FTM_DEADTIME_DTVAL(63)); // DTVAL - deadtime value (0-63): deadtime period = DTPS x DTVAL
  FTM_HAL_SetInitTriggerCmd(FTM0_BASE, true);//True to enable, false to disable
  FTM_HAL_SetChnOutputPolarity(FTM0_BASE, 0,  1);
  FTM_HAL_SetChnOutputPolarity(FTM0_BASE, 2,  1);
  FTM_HAL_SetChnOutputPolarity(FTM0_BASE, 4,  1);
  
  /* Following line configures:
  - enhanced PWM synchronization, FTM counter reset on SW sync
  - output SW control / polarity registers updated on PWM synchronization (not on rising edge of clock)
  - output SW control/inverting(swap)/mask registers updated from buffers on SW synchronization */  
  FTM_HAL_SetPwmSyncModeCmd(FTM0_BASE, true);// true means use Enhanced PWM synchronization\n
  FTM_HAL_SetCounterSoftwareSyncModeCmd(FTM0_BASE, true);//true means software trigger activates register sync\n
  FTM_HAL_SetSwoctrlPwmSyncModeCmd(FTM0_BASE, true);//true means SWOCTRL register is updated by PWM synch\n
  FTM_HAL_SetInvctrlPwmSyncModeCmd(FTM0_BASE, true);//true means INVCTRL register is updated by PWM synch\n
  FTM_HAL_SetSwoctrlSoftwareSyncModeCmd(FTM0_BASE, true);//true means software trigger activates register sync\n
  FTM_HAL_SetInvctrlSoftwareSyncModeCmd(FTM0_BASE, true);//true means software trigger activates register sync\n
  FTM_HAL_SetOutmaskSoftwareSyncModeCmd(FTM0_BASE, true);//true means software trigger activates register sync\n
  //Sets the FTM peripheral timer channel output mask.
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 0, 1);//Sets the FTM peripheral timer channel output mask.
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 1, 1);
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 2, 1);
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 3, 1);
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 4, 1);
  FTM_HAL_SetChnOutputMask(FTM0_BASE, 5, 1);
  // update of FTM settings
  // FTM_HAL_SetCntinPwmSyncModeCmd(FTM0_BASE, true);//true means CNTIN register is updated by PWM synch\n
  //Sets the FTM clock source.
  //clock  The FTM peripheral clock selection\n
  FTM_HAL_SetClockSource(FTM0_BASE, kClock_source_FTM_SystemClk);
  //Sets the FTM peripheral timer channel counter value.
  FTM_HAL_SetChnCountVal(FTM0_BASE, 0, (uint16_t)(-PWM_MODULO/4));
  FTM_HAL_SetChnCountVal(FTM0_BASE, 1,(uint16_t) PWM_MODULO/4);	
  FTM_HAL_SetChnCountVal(FTM0_BASE, 2, (uint16_t)(-PWM_MODULO/4));
  FTM_HAL_SetChnCountVal(FTM0_BASE, 3,(uint16_t) PWM_MODULO/4);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 4, (uint16_t)(-PWM_MODULO/4));
  FTM_HAL_SetChnCountVal(FTM0_BASE, 5,(uint16_t) PWM_MODULO/4);

  //Enables the FTM peripheral timer software trigger.
  FTM_HAL_SetSoftwareTriggerCmd(FTM0_BASE, 1);
  //Enables the loading of MOD, CNTIN and CV with values of their write buffer.
  FTM_HAL_SetPwmLoadCmd(FTM0_BASE, 1);

  // FTM0 PWM output pins
  PORT_HAL_SetMuxMode(PORTC_BASE, 1, kPortMuxAlt4);
  PORT_HAL_SetMuxMode(PORTC_BASE, 3, kPortMuxAlt4);
  PORT_HAL_SetMuxMode(PORTC_BASE, 4, kPortMuxAlt4);
  PORT_HAL_SetMuxMode(PORTD_BASE, 4, kPortMuxAlt4);
  PORT_HAL_SetMuxMode(PORTD_BASE, 5, kPortMuxAlt4);
  PORT_HAL_SetMuxMode(PORTE_BASE, 25, kPortMuxAlt3);
  
  GPIO_HAL_SetPinDir(GPIOC_BASE, 1, kGpioDigitalOutput); 
  GPIO_HAL_SetPinDir(GPIOC_BASE, 3, kGpioDigitalOutput);   
  GPIO_HAL_SetPinDir(GPIOC_BASE, 4, kGpioDigitalOutput);   
  
  GPIO_HAL_SetPinDir(GPIOD_BASE, 4, kGpioDigitalOutput); 
  GPIO_HAL_SetPinDir(GPIOD_BASE, 5, kGpioDigitalOutput); 
  GPIO_HAL_SetPinDir(GPIOE_BASE, 25, kGpioDigitalOutput); 
}

/*************************************************************************
 * Function Name: FTM1_init
 * Parameters: none
 * Return: none
 * Description: FlexTimer 1 initialization
 *************************************************************************/
void FTM1_init(void)
{
  FTM_HAL_SetWriteProtectionCmd(FTM1_BASE, false);//false: Write-protection is disabled
  FTM_HAL_Enable(FTM1_BASE, true);//true: all registers including FTM-specific registers are available
  FTM_HAL_SetMod(FTM1_BASE, (uint16_t)0xffff);// Free running timer
  FTM_HAL_SetClockSource(FTM1_BASE, kClock_source_FTM_SystemClk);//clock  The FTM peripheral clock selection\n
  FTM_HAL_SetClockPs(FTM1_BASE, kFtmDividedBy128); // system clock, divide by 128, 1.70666us @ 75 MHz clock, 1.7777us @ 72 MHz clock
  FTM_HAL_EnableChnInt(FTM1_BASE, 0);//Enables the FTM peripheral timer channel(n) interrupt.
  FTM_HAL_SetChnMSnBAMode(FTM1_BASE, 0, 1);//Sets the FTM peripheral timer channel mode.
  INT_SYS_EnableIRQ(FTM1_IRQn);
  set_irq_priority(FTM1_IRQn, ISR_PRIORITY_FORCED_CMT);
}

/*************************************************************************
 * Function Name: FTM2_init
 * Parameters: none
 * Return: none
 * Description: FlexTimer 2 initialization
 *************************************************************************/
void FTM2_init(void)
{
  FTM_HAL_SetWriteProtectionCmd(FTM2_BASE, false);//false: Write-protection is disabled
  FTM_HAL_Enable(FTM2_BASE, true);//true: all registers including FTM-specific registers are available
  FTM_HAL_SetMod(FTM2_BASE, (uint16_t)0xffff);// Free running timer
  FTM_HAL_SetClockSource(FTM2_BASE, kClock_source_FTM_SystemClk);//clock  The FTM peripheral clock selection\n
  FTM_HAL_SetClockPs(FTM2_BASE, kFtmDividedBy2); // system clock, divide by 2
  FTM_HAL_EnableChnInt(FTM2_BASE, 0);//Enables the FTM peripheral timer channel(n) interrupt.
  FTM_HAL_SetChnMSnBAMode(FTM2_BASE, 0, 1);//Sets the FTM peripheral timer channel mode.
  INT_SYS_EnableIRQ(FTM2_IRQn);
  set_irq_priority(FTM2_IRQn, ISR_PRIORITY_SLOW_TIMER);  

}

/*************************************************************************
 * Function Name: SPI0_init
 * Parameters: none
 * Return: none
 * Description: SPI initialization
 *************************************************************************/
void SPI0_init(void)
{
  dspi_baud_rate_divisors_t divisors;
  divisors.doubleBaudRate = 0;
  divisors.baudRateDivisor = 3;
  divisors.prescaleDivisor = 3;
  
  dspi_data_format_config_t config;
  config.clkPhase = kDspiClockPhase_SecondEdge;
  config.bitsPerFrame = 8;  
  config.clkPolarity = kDspiClockPolarity_ActiveHigh;
  config.direction = kDspiMsbFirst;
    
  PORT_HAL_SetMuxMode(PORTC_BASE,2,kPortMuxAsGpio);// MC33937 RESET
  GPIO_HAL_SetPinDir(GPIOC_BASE, 2, kGpioDigitalOutput);
  PORT_HAL_SetMuxMode(PORTD_BASE,7,kPortMuxAsGpio);// MC33937 DRV_EN
  GPIO_HAL_SetPinDir(GPIOD_BASE, 7, kGpioDigitalOutput); 
   
  GPIO_HAL_SetPinOutput(GPIOC_BASE, 2);//MC33937_RESET_HIGH;
  GPIO_HAL_SetPinOutput(GPIOD_BASE, 7);//MC33937_ENABLE_HIGH;
  
  DSPI_HAL_StopTransfer(SPI0_BASE);// halt SPI before SPI setting
  DSPI_HAL_Enable(SPI0_BASE);//Enables the DSPI peripheral and sets the MCR MDIS to 0.
  DSPI_HAL_SetMasterSlaveMode(SPI0_BASE, kDspiMaster);//Enable Master Mode
  DSPI_HAL_SetPcsPolarityMode(SPI0_BASE, kDspiPcs0,kDspiPcs_ActiveLow);//The setting for either "active high, inactive low (0)"  or "active low, inactive high(1)" of type dspi_pcs_polarity_config_t.
  DSPI_HAL_SetFifoCmd(SPI0_BASE, false, false);//Disable the DSPI FIFOs.    
  DSPI_HAL_PresetTransferCount(SPI0_BASE, 0x0000);//Pre-sets the transfer count.
 
  DSPI_HAL_SetDelay(SPI0_BASE, kDspiCtar0,3,0, kDspiPcsToSck); // CTAR0 selection option for master or slave mode
  DSPI_HAL_SetDelay(SPI0_BASE, kDspiCtar0,2,0, kDspiLastSckToPcs);
  DSPI_HAL_SetDelay(SPI0_BASE, kDspiCtar0,0,2, kDspiAfterTransfer); 
  
  DSPI_HAL_SetBaudDivisors(SPI0_BASE, kDspiCtar0, &divisors);
  DSPI_HAL_SetDataFormat(SPI0_BASE, kDspiCtar0, &config);  
   
  DSPI_HAL_ClearStatusFlag(SPI0_BASE ,kDspiTxComplete);///*!< TCF status/interrupt enable */
  DSPI_HAL_ClearStatusFlag(SPI0_BASE ,kDspiEndOfQueue);///*!< EOQF status/interrupt enable*/
  DSPI_HAL_ClearStatusFlag(SPI0_BASE ,kDspiTxFifoUnderflow);///*!< TFUF status/interrupt enable*/
  DSPI_HAL_ClearStatusFlag(SPI0_BASE ,kDspiTxFifoFillRequest);///*!< TFFF status/interrupt enable*/
  DSPI_HAL_ClearStatusFlag(SPI0_BASE ,kDspiRxFifoOverflow);///*!< RFOF status/interrupt enable*/
  
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiTxComplete, false);/*!< TCF status/interrupt disable */
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiEndOfQueue, false);/*!< EOQF status/interrupt disable */
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiTxFifoUnderflow, false);/*!< TFUF status/interrupt disable*/
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiTxFifoFillRequest, false);/*!< TFFF status/interrupt disable*/
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiRxFifoOverflow, false);/*!< RFOF status/interrupt disable*/
  DSPI_HAL_SetIntMode(SPI0_BASE,kDspiRxFifoDrainRequest, false);/*!< RFDF status/interrupt disable*/
  DSPI_HAL_StartTransfer(SPI0_BASE);// Starts the DSPI transfers, clears HALT bit in MCR. 

  PORT_HAL_SetMuxMode(PORTD_BASE, 3, kPortMuxAlt2);
  PORT_HAL_SetMuxMode(PORTD_BASE, 2, kPortMuxAlt2);
  PORT_HAL_SetMuxMode(PORTC_BASE, 0, kPortMuxAlt7);
  PORT_HAL_SetMuxMode(PORTC_BASE, 5, kPortMuxAlt2);  

} 

/*************************************************************************
 * Function Name: UART1_init
 * Parameters: none
 * Return: none
 * Description: UART port configuration
 *************************************************************************/
void UART1_init(void)
{
  UART_HAL_DisableTransmitter(UART1_BASE);
  UART_HAL_DisableReceiver(UART1_BASE);/* disable uart operation */
  UART_HAL_SetStopBitCount(UART1_BASE, kUartOneStopBit);
  UART_HAL_SetBaudRate(UART1_BASE, BUS_CLOCK, UART_BAUDRATE);
  //Configures the number of bits per character in the UART controller./
  UART_HAL_SetBitCountPerChar(UART1_BASE, kUart8BitsPerChar);/*!< 8-bit data characters */
  UART_HAL_SetParityMode(UART1_BASE, kUartParityDisabled);/*!< parity disabled */
  UART_HAL_SetWaitModeOperation(UART1_BASE, kUartOperates);/*!< UART continues to operate normally */
  UART_HAL_SetLoopCmd(UART1_BASE, false);/*��<Disable The UART loopback mode configuration */
  UART_HAL_SetReceiverSource(UART1_BASE, kUartLoopBack);/*!< Internal loop back mode. */
  UART_HAL_SetReceiverWakeupMethod(UART1_BASE, kUartIdleLineWake);/*!< The idle-line wakes UART receiver from standby */
  BW_UART_C1_ILT(UART1_BASE, 0);/*!<Idle character bit count starts after start bit.*/
  
  UART_HAL_EnableTransmitter(UART1_BASE);
  UART_HAL_EnableReceiver(UART1_BASE);/* Enable uart operation */
  // Configure UART1 port pins: PTD0, PTD1 - ALT5
  PORT_HAL_SetMuxMode(PORTD_BASE, 0, kPortMuxAlt5);
  PORT_HAL_SetMuxMode(PORTD_BASE, 1, kPortMuxAlt5);  
}

/*************************************************************************
 * Function Name: GPIO_init
 * Parameters: none
 * Return: none
 * Description: I/O pins configuration
 *************************************************************************/
void GPIO_init(void)
{  
  // Brake control
  PORT_HAL_SetMuxMode(PORTE_BASE, 24, kPortMuxAsGpio);
  PORT_HAL_SetPullMode(PORTE_BASE, 24, kPortPullDown);
  GPIO_HAL_SetPinDir(GPIOE_BASE, 24, kGpioDigitalOutput);
  GPIO_HAL_ClearPinOutput(GPIOE_BASE, 24);
    
  // LED
  PORT_HAL_SetMuxMode(PORTD_BASE, 6, kPortMuxAsGpio);  
  PORT_HAL_SetPullMode(PORTD_BASE, 6, kPortPullDown);
  GPIO_HAL_SetPinDir(GPIOD_BASE, 6, kGpioDigitalOutput);
  GPIO_HAL_SetPinOutput(GPIOC_BASE, 1);// Turn off LED
  
  // Push buttons: PDB0 = SW2, PTA4 = SW1
  PORT_HAL_SetMuxMode(PORTA_BASE, 4, kPortMuxAsGpio);
  PORT_HAL_SetPullMode(PORTA_BASE, 4, kPortPullDown);
  PORT_HAL_SetMuxMode(PORTB_BASE, 0, kPortMuxAsGpio);
  PORT_HAL_SetPullMode(PORTB_BASE, 0, kPortPullDown);    

}

/*************************************************************************
 * Function Name: SPI_Send
 * Parameters: none
 * Return: none
 * Description: SPI send data handler
 *************************************************************************/
unsigned char SPI_Send(unsigned char spi_data)
{    
  dspi_command_config_t command;
  command.whichPcs = kDspiPcs0;
  command.clearTransferCount = false;
  command.isChipSelectContinuous = false;
  command.isEndOfQueue = false;
  command.whichCtar = kDspiCtar0;
  DSPI_HAL_WriteDataMastermodeBlocking(SPI0_BASE, &command, spi_data);
  return DSPI_HAL_ReadData(SPI0_BASE);
  
}

/*************************************************************************
 * Function Name: MC33927_readStatus
 * Parameters: none
 * Return: none
 * Description: Reads MC33937 driver status registers
 *************************************************************************/
void MC33927_readStatus(void)
{
  /* This command reads Status Register 0 of MC33927 and stores it in the mudtStatusRegister0 structure */
  MC33927_GetSR0(&mudtStatusRegister0);
    
  /* This command reads Status Register 1 of MC33927 and stores it in the mudtStatusRegister1 structure */    
  MC33927_GetSR1(&mudtStatusRegister1);
  
  /* This command reads Status Register 2 of MC33927 and stores it in the mudtStatusRegister2 structure */
  MC33927_GetSR2(&mudtStatusRegister2);
}

/*************************************************************************
 * Function Name: MC33927_config
 * Parameters: none
 * Return: none
 * Description: Initializes the MC33937 driver
 *************************************************************************/
void MC33927_config(void)
{
    /* This command reads Status Register 0 of MC33927 and stores it in the mudtStatusRegister0 structure */
    MC33927_GetSR0(&mudtStatusRegister0);
    
    /* This command reads Status Register 1 of MC33927 and stores it in the mudtStatusRegister1 structure */    
    MC33927_GetSR1(&mudtStatusRegister1);
    
    /* This command reads Status Register 2 of MC33927 and stores it in the mudtStatusRegister2 structure */
    MC33927_GetSR2(&mudtStatusRegister2);

    /* This command reads Status Register 3 (deadtime) of MC33927 and stores it in the mbytDeadtime variable */
    MC33927_GetSR3();
    
    /***** Mode setup *****/
    mudtModeCommands.B.Lock = 0; /* 0: allows further writing; 1: locks the configuration until reset */
    mudtModeCommands.B.FullOnMode = 0; /* 0: BLDC, PMSM, ACIM; 1: SR motors */    
    mudtModeCommands.B.DesatFaultMode = 0; /* 0: when desaturation or phase error
                                            on any MOSFET turns off all MOSFETs; 1: desat faults ignored */
    
    /* This command controls the Desaturation Fault Mode, Full On Mode and Lock Mode bits */
    MC33927_ModeCommand(&mudtModeCommands);
    
    /***** Interrupts setup *****/    
    mudtMaskInterrupts.B.TLim = 0; /* overtemperature interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.Desat = 0; /* dasaturation interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.LowVLS = 0; /* VLS undervoltage interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.Overcurrent = 0; /* overcurrent interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.PhaseError = 0; /* phase error interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.FramingError = 0; /* SPI framing error interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.WriteError = 0; /* write after lock is on interrupt 0: disables; 1: enables */
    mudtMaskInterrupts.B.ResetEvent = 0; /* driver set or disabled interrupt 0: disables; 1: enables */
    
    /* This command controls interrupt mask registers */
    MC33927_MaskInterrupts(&mudtMaskInterrupts);

    /***** Flags clearing *****/    
    mudtClearFlags.B.TLim = 1; /* overtemperature flag 0: untouched; 1: clears */
    mudtClearFlags.B.Desat = 1; /* dasaturation flag 0: untouched; 1: clears */
    mudtClearFlags.B.LowVLS = 1; /* VLS undervoltage flag 0: untouched; 1: clears */
    mudtClearFlags.B.Overcurrent = 1; /* overcurrent flag 0: untouched; 1: clears */
    mudtClearFlags.B.PhaseError = 1; /* phase error flag 0: untouched; 1: clears */
    mudtClearFlags.B.FramingError = 1; /* SPI framing error flag 0: untouched; 1: clears */
    mudtClearFlags.B.WriteError = 1; /* write after lock is on flag 0: untouched; 1: clears */
    mudtClearFlags.B.ResetEvent = 1; /* driver set upon exiting /RST flag 0: untouched; 1: clears */
    
#ifdef KEIL
     for (int ind=0; ind <10000;ind++) __nop();//asm("nop"); // Delay has to be inserted, otherwise 3PPA driver didn't eat the command
    /* This command clears the flag registers */
    MC33927_ClearFlags(&mudtClearFlags);
    for (int ind=0; ind <10000;ind++) __nop();//asm("nop"); // Delay has to be inserted, otherwise 3PPA driver didn't eat the command
#else

    for (int ind=0; ind <10000;ind++) asm("nop"); // Delay has to be inserted, otherwise 3PPA driver didn't eat the command
    /* This command clears the flag registers */
    MC33927_ClearFlags(&mudtClearFlags);
    for (int ind=0; ind <10000;ind++) asm("nop"); // Delay has to be inserted, otherwise 3PPA driver didn't eat the command
#endif
    /* This command sets the zero deadtime */
    MC33927_ZeroDeadtime();
    
    /* Reading the status registers to check if it was set correctly */
    MC33927_GetSR0(&mudtStatusRegister0); /* SR0 */
    MC33927_GetSR1(&mudtStatusRegister1); /* SR1 */
    MC33927_GetSR2(&mudtStatusRegister2); /* SR2 */
    MC33927_GetSR3(); /* SR3, deadtime value */
}

/*************************************************************************
 * Function Name: FTM0_SetDutyCycle
 * Parameters: duty cycle value
 * Return: none
 * Description: Set PWM dutycycle
 *************************************************************************/
void FTM0_SetDutyCycle(int16 inpDuty)
{
  int FirstEdge, SecondEdge, duty;

  duty = MLIB_Mul_F16(inpDuty, PWM_MODULO/4);

  FirstEdge = -PWM_MODULO/4 - duty;
  if (FirstEdge < (-PWM_MODULO/2)) FirstEdge = -PWM_MODULO/2;
  
  SecondEdge = PWM_MODULO/4 + duty;
  if (SecondEdge > (PWM_MODULO/2)) SecondEdge = PWM_MODULO/2;

  FTM_HAL_SetChnCountVal(FTM0_BASE, 0, FirstEdge);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 1, SecondEdge);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 2, FirstEdge);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 3, SecondEdge);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 4, FirstEdge);
  FTM_HAL_SetChnCountVal(FTM0_BASE, 5, SecondEdge);
  FTM_HAL_SetPwmLoadCmd(FTM0_BASE, true);

}

/*************************************************************************
 * Function Name: FTM0_SetPwmOutput
 * Parameters: sector number
 * Return: none
 * Description: set PWM output configuration based on selected sector
 *************************************************************************/
void FTM0_SetPwmOutput(int16 sector)
{ 
  FTM_HAL_SetInvctrlReg(FTM0_BASE, bldcCommutationTableComp[sector].swap);
  FTM_HAL_SetOutmaskReg(FTM0_BASE, bldcCommutationTableComp[sector].mask);
  FTM_HAL_SetSoftwareTriggerCmd(FTM0_BASE, true);
}

/*************************************************************************
 * Function Name: MCU_init
 * Parameters: none
 * Return: none
 * Description: initialization of peripheral modules after reset 
 *************************************************************************/
void MCU_init(void)
{
  Clock_init();  
  ADC0_init();
  PDB_init();
  FTM0_init();
  FTM1_init();
  FTM2_init();
  SPI0_init();
  UART1_init();
  GPIO_init();
  MC33927_config();
  
}
             

