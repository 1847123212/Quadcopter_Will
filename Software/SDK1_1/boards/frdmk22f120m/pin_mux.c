/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : pin_mux.c
**     Project     : twrk22f120m
**     Processor   : MK22FN512VMC12
**     Component   : PinSettings
**     Version     : Component 01.002, Driver 1.1, CPU db: 3.00.000
**     Compiler    : IAR ARM C Compiler
**     Date/Time   : 2013-12-10, 14:58, # CodeGen: 7
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         CAN                 - void pin_mux_CAN(uint32_t instance);
**         ENET                - void pin_mux_ENET(uint32_t instance);
**         GPIO                - void pin_mux_GPIO(uint32_t instance);
**         I2C                 - void pin_mux_I2C(uint32_t instance);
**         RTC                 - void pin_mux_RTC(uint32_t instance);
**         SDHC                - void pin_mux_SDHC(uint32_t instance);
**         SPI                 - void pin_mux_SPI(uint32_t instance);
**         UART                - void pin_mux_UART(uint32_t instance);
**
**     Copyright : 1997 - 2013 Freescale Semiconductor, Inc. All Rights Reserved.
**     SOURCE DISTRIBUTION PERMISSIBLE as directed in End User License Agreement.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/
/*!
** @file pin_mux.c
** @version 1.1
** @brief
**
*/         
/*!
**  @addtogroup pin_mux_module pin_mux module documentation
**  @{
*/         

/* MODULE pin_mux. */

#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "pin_mux.h"


void pin_mux_GPIO(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* PTA */
      /* PORTA_PCR1 */
      PORT_HAL_SetMuxMode(PORTA_BASE,1u,kPortMuxAsGpio);
      /* PORTA_PCR2 */
      PORT_HAL_SetMuxMode(PORTA_BASE,2u,kPortMuxAsGpio);
      break;
    case 1:                             /* PTB */
      /* PORTB_PCR16 */
      PORT_HAL_SetMuxMode(PORTB_BASE,16u,kPortMuxAsGpio);
      /* PORTB_PCR17 */
      PORT_HAL_SetMuxMode(PORTB_BASE,17u,kPortMuxAsGpio);
      break;
    case 2:                             /* PTC */
      /* PORTC_PCR0 */
      PORT_HAL_SetMuxMode(PORTC_BASE,0u,kPortMuxAsGpio);
      /* PORTC_PCR1 */
      PORT_HAL_SetMuxMode(PORTC_BASE,1u,kPortMuxAsGpio);
      /* PORTC_PCR3 */
      PORT_HAL_SetMuxMode(PORTC_BASE,3u,kPortMuxAsGpio);
      /* PORTC_PCR4 */
      PORT_HAL_SetMuxMode(PORTC_BASE,4u,kPortMuxAsGpio);
      /* PORTC_PCR7 */
      PORT_HAL_SetMuxMode(PORTC_BASE,7u,kPortMuxAsGpio);
      /* PORTC_PCR11 */
      PORT_HAL_SetMuxMode(PORTC_BASE,11u,kPortMuxAsGpio);
      break;
    case 3:
      /* PORTD_PCR0 */
      PORT_HAL_SetMuxMode(PORTD_BASE,0u,kPortMuxAsGpio);
      /* PORTD_PCR1 */
      PORT_HAL_SetMuxMode(PORTD_BASE,1u,kPortMuxAsGpio);
      /* PORTD_PCR4 */
      PORT_HAL_SetMuxMode(PORTD_BASE,4u,kPortMuxAsGpio);
      /* PORTD_PCR5 */
      PORT_HAL_SetMuxMode(PORTD_BASE,5u,kPortMuxAsGpio);      
      break;
    case 4:                             /* PTE */
      /* PORTE_PCR0 */
      PORT_HAL_SetMuxMode(PORTE_BASE,0u,kPortMuxAsGpio);
      /* PORTE_PCR1 */
      PORT_HAL_SetMuxMode(PORTE_BASE,1u,kPortMuxAsGpio);
      break;
    default:
      break;
  }
}

void pin_mux_I2C(uint32_t instance)
{
  switch(instance) {    
    case 0:                             /* I2C0 */
      /* PORTB_PCR2 */
      PORT_HAL_SetMuxMode(PORTB_BASE,2u,kPortMuxAlt2);
      PORT_HAL_SetOpenDrainCmd(PORTB_BASE,2u,true);
      /* PORTB_PCR3 */
      PORT_HAL_SetMuxMode(PORTB_BASE,3u,kPortMuxAlt2);
      PORT_HAL_SetOpenDrainCmd(PORTB_BASE,3u,true);
      break;
    case 1:                             /* I2C1 */
      /* PORTC_PCR10 */
      PORT_HAL_SetMuxMode(PORTC_BASE,10u,kPortMuxAlt2);
      PORT_HAL_SetOpenDrainCmd(PORTC_BASE,10u,true);
      /* PORTC_PCR11 */
      PORT_HAL_SetMuxMode(PORTC_BASE,11u,kPortMuxAlt2);
      PORT_HAL_SetOpenDrainCmd(PORTC_BASE,11u,true);
      break;
    default:
      break;
  }
}

void pin_mux_RTC(uint32_t instance)
{
  /* PORTE_PCR26 */
  PORT_HAL_SetMuxMode(PORTE_BASE,0u,kPortMuxAlt7);
}

void pin_mux_SPI(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* SPI0 */
      /* PORTD_PCR0 */
      /*PORT_HAL_SetMuxMode(PORTD_BASE,0u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC_BASE,4u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR3 */
      //PORT_HAL_SetMuxMode(PORTD_BASE,3u,kPortMuxAlt2);
      PORT_HAL_SetMuxMode(PORTC_BASE,5u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR1 */
      /*PORT_HAL_SetMuxMode(PORTD_BASE,1u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC_BASE,6u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR2 */
      /*PORT_HAL_SetMuxMode(PORTD_BASE,2u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC_BASE,7u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      break;
    case 1:                             /* SPI1 */
      /* PORTD_PCR4 */
      PORT_HAL_SetMuxMode(PORTD_BASE,4u,kPortMuxAlt7);
      /* PORTD_PCR5 */
      PORT_HAL_SetMuxMode(PORTD_BASE,5u,kPortMuxAlt7);
      /* PORTD_PCR6 */
      PORT_HAL_SetMuxMode(PORTD_BASE,6u,kPortMuxAlt7);
      /* PORTD_PCR7 */
      PORT_HAL_SetMuxMode(PORTD_BASE,7u,kPortMuxAlt7);
      break;
    default:
      break;
  }
}

void pin_mux_UART(uint32_t instance)
{
  switch(instance) {
    case 1:                             /* UART1 BT */
      /* PORTE_PCR0 */
      PORT_HAL_SetMuxMode(PORTE_BASE,0u,kPortMuxAlt3);
      /* PORTE_PCR1 */
      PORT_HAL_SetMuxMode(PORTE_BASE,1u,kPortMuxAlt3);
      break;
    default:
      break;
  }
}

void pin_mux_I2S(uint32_t instance)
{
  /* MCLK output */
  PORT_HAL_SetMuxMode(PORTC_BASE,8u,kPortMuxAlt4);
  /* RX Data */
  PORT_HAL_SetMuxMode(PORTC_BASE,5u,kPortMuxAlt4);
  /* RX Frame Sync */
  PORT_HAL_SetMuxMode(PORTC_BASE,7u,kPortMuxAlt4);
  /* RX Bit clock */
  PORT_HAL_SetMuxMode(PORTC_BASE,9u,kPortMuxAlt4);
  /* TX Bit Clock */
  PORT_HAL_SetMuxMode(PORTA_BASE,5u,kPortMuxAlt6);
  /* TX Frame Sync */
  PORT_HAL_SetMuxMode(PORTA_BASE,13u,kPortMuxAlt6);
  /* TX Data */
  PORT_HAL_SetMuxMode(PORTA_BASE,12u,kPortMuxAlt6);
}

void pin_mux_SPI_CS0(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTD_BASE,0u,kPortMuxAsGpio);
}

void pin_mux_SPI_CS1(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTD_BASE,4u,kPortMuxAsGpio);
}

void pin_mux_FTM(uint32_t instance)
{
    switch(instance) 
    {    
        case 0:
            PORT_HAL_SetMuxMode(PORTD_BASE,5u,kPortMuxAlt4);
            break;
        default:
            break;
    }
}

void pin_mux_GPIO_I2C(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTB_BASE,2u,kPortMuxAsGpio);
  PORT_HAL_SetOpenDrainCmd(PORTB_BASE,2u,true);
  PORT_HAL_SetMuxMode(PORTB_BASE,3u,kPortMuxAsGpio);
  PORT_HAL_SetOpenDrainCmd(PORTB_BASE,3u,true);
}

void pin_mux_GPIO_UART(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTE_BASE,0u,kPortMuxAsGpio);
  PORT_HAL_SetMuxMode(PORTE_BASE,1u,kPortMuxAsGpio);
}

void configure_cmp_pins(uint32_t instance)
{
    switch (instance)
    {
        case 0U:
            PORT_HAL_SetMuxMode(PORTC_BASE, 6U, kPortPinDisabled); /* PTC6 - CMP0_IN0. */
            break;
        default:
            break;
    }
}

/* END pin_mux. */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
