/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : pin_mux.c
**     Project     : frdmk22f120mk02
**     Processor   : MK02FN128VLH10 on FRDM-K22F120M
**     Component   : PinSettings
**     Version     : Component 01.002, Driver 1.1, CPU db: 3.00.000
**     Compiler    : IAR ARM C Compiler
**     Date/Time   : 2013-12-10, 14:58, # CodeGen: 7
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         GPIO                - void pin_mux_GPIO(uint32_t instance);
**         I2C                 - void pin_mux_I2C(uint32_t instance);
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


void configure_gpio_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* PTA */
      /* PORTA_PCR1 */
      PORT_HAL_SetMuxMode(PORTA,1u,kPortMuxAsGpio);
      /* PORTA_PCR2 */
      PORT_HAL_SetMuxMode(PORTA,2u,kPortMuxAsGpio);
      break;
    case 1:                             /* PTB */
      /* PORTB_PCR17 */
      PORT_HAL_SetMuxMode(PORTB,17u,kPortMuxAsGpio);
      break;
    case 2:                             /* PTC */
      /* PORTC_PCR0 */
      PORT_HAL_SetMuxMode(PORTC,0u,kPortMuxAsGpio);
      /* PORTC_PCR1 */
      PORT_HAL_SetMuxMode(PORTC,1u,kPortMuxAsGpio);
      /* PORTC_PCR3 */
      PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAsGpio);
      /* PORTC_PCR4 */
      PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAsGpio);
      /* PORTC_PCR7 */
      PORT_HAL_SetMuxMode(PORTC,7u,kPortMuxAsGpio);
      /* PORTC_PCR11 */
      PORT_HAL_SetMuxMode(PORTC,11u,kPortMuxAsGpio);
      break;
    case 3:                             /* PTD */
      /* PORTD_PCR0 */
      PORT_HAL_SetMuxMode(PORTD,0u,kPortMuxAsGpio);
      /* PORTD_PCR1 */
      PORT_HAL_SetMuxMode(PORTD,1u,kPortMuxAsGpio);
      /* PORTD_PCR0 */
      PORT_HAL_SetMuxMode(PORTD,2u,kPortMuxAsGpio);
      /* PORTD_PCR1 */
      PORT_HAL_SetMuxMode(PORTD,3u,kPortMuxAsGpio);
      /* PORTD_PCR4 */
      PORT_HAL_SetMuxMode(PORTD,4u,kPortMuxAsGpio);
      /* PORTD_PCR5 */
      PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAsGpio);
      break;
    case 4:                             /* PTE */
      /* PORTE_PCR0 */
      PORT_HAL_SetMuxMode(PORTE,0u,kPortMuxAsGpio);
      /* PORTE_PCR1 */
      PORT_HAL_SetMuxMode(PORTE,1u,kPortMuxAsGpio);
      break;
    default:
      break;
  }
}

void configure_i2c_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* I2C0 */
      /* PORTB_PCR2 */
      PORT_HAL_SetMuxMode(PORTB,2u,kPortMuxAlt2);/*I2C0_SCL */
      PORT_HAL_SetOpenDrainCmd(PORTB,2u,true);
      /* PORTB_PCR3 */
      PORT_HAL_SetMuxMode(PORTB,3u,kPortMuxAlt2);/*I2C0_SDA*/
      PORT_HAL_SetOpenDrainCmd(PORTB,3u,true);
      break;
           /* I2C1 */
      /* Not supported, only I2C0 module*/
    default:
      break;
  }
}

void configure_spi_pins(uint32_t instance)
{
  switch(instance) {
    case 0:                             /* SPI0 */
      /* PORTD_PCR0 */
      /*PORT_HAL_SetMuxMode(g_portBaseAddr[3],0u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR3 */
      //PORT_HAL_SetMuxMode(g_portBaseAddr[3],3u,kPortMuxAlt2);
      PORT_HAL_SetMuxMode(PORTC,5u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR1 */
      /*PORT_HAL_SetMuxMode(g_portBaseAddr[3],1u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC,6u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      /* PORTD_PCR2 */
      /*PORT_HAL_SetMuxMode(g_portBaseAddr[3],2u,kPortMuxAlt2);*/
      PORT_HAL_SetMuxMode(PORTC,7u,kPortMuxAlt2);   /*** Temporary work around until next board spin. ***/
      break;
      /* SPI1 */
      /* Not supported, only SPI0 module*/
    default:
      break;
  }
}

void configure_spi_cs0_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTC,4u,kPortMuxAsGpio);
}

void configure_spi_cs1_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTC,3u,kPortMuxAsGpio);
}

void configure_gpio_i2c_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTB,2u,kPortMuxAsGpio);
  PORT_HAL_SetOpenDrainCmd(PORTB,2u,true);
  PORT_HAL_SetMuxMode(PORTB,3u,kPortMuxAsGpio);
  PORT_HAL_SetOpenDrainCmd(PORTB,3u,true);
}

void configure_uart_pins(uint32_t instance)
{
  switch(instance) {
    case 1:                             /* UART1 BT */
      /* PORTE_PCR0 */
      PORT_HAL_SetMuxMode(PORTE,0u,kPortMuxAlt3);
      /* PORTE_PCR1 */
      PORT_HAL_SetMuxMode(PORTE,1u,kPortMuxAlt3);
      break;
    default:
      break;
  }
}

void configure_ftm_pins(uint32_t instance)
{
    switch(instance)
    {
        case 0:                         /*FTM0_CH5*/
            PORT_HAL_SetMuxMode(PORTD,5u,kPortMuxAlt4);
            break;
        default:
            break;
    }
}

void configure_gpio_uart_pins(uint32_t instance)
{
  PORT_HAL_SetMuxMode(PORTE,0u,kPortMuxAsGpio);
  PORT_HAL_SetMuxMode(PORTE,1u,kPortMuxAsGpio);
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
