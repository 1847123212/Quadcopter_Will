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
///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

 // Standard C Included Files
#include <stdio.h>
// SDK Included Files
#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_i2c_slave_driver.h"

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////
/*!
* @brief The i2c slave
* The function runs i2c slave with interrupt active mode. Slave receive data from
* master and echo back to master
*/
void main(void)
{
    // Number byte data will be transfer
    uint32_t count = 0;
    // Buffer store data to transfer
    uint8_t dataBuff[100] = {0};
    // state of slave
    i2c_slave_state_t slave;
    // user configuration
    i2c_slave_user_config_t userConfig =
    {
        .address        = 0x7FU,
        .slaveCallback  = NULL,
        .callbackParam  = NULL,
        .slaveListening = false,
#if FSL_FEATURE_I2C_HAS_START_STOP_DETECT
        .startStopDetect  = false,
#endif
#if FSL_FEATURE_I2C_HAS_STOP_DETECT
        .stopDetect       = false,
#endif
    };

    // Initialize hardware
    hardware_init();
    // Configure pin for i2c slave
    configure_i2c_pins(BOARD_I2C_COMM_INSTANCE);
    // Initialize uart to debug
    dbg_uart_init();
    // Initialize OSA
    OSA_Init();

    printf("Slave is running ...");

    // Initialize slave
    I2C_DRV_SlaveInit(BOARD_I2C_COMM_INSTANCE, &userConfig, &slave);

    // Loop transfer
    while(1)
    {
        // Slave receive buffer from master
        I2C_DRV_SlaveReceiveData(BOARD_I2C_COMM_INSTANCE, (uint8_t*)&count, 1);

        /* Wait until transfer is successful */
        while (I2C_DRV_SlaveGetReceiveStatus(BOARD_I2C_COMM_INSTANCE, NULL) != kStatus_I2C_Success);

        // Slave receive buffer from master
        I2C_DRV_SlaveReceiveData(BOARD_I2C_COMM_INSTANCE, dataBuff, count);

        /* Wait until transfer is successful */
        while (I2C_DRV_SlaveGetReceiveStatus(BOARD_I2C_COMM_INSTANCE, NULL) != kStatus_I2C_Success);

        // Slave send buffer received from master
        I2C_DRV_SlaveSendData(BOARD_I2C_COMM_INSTANCE, dataBuff, count);

        /* Wait until transfer is successful */
        while (I2C_DRV_SlaveGetTransmitStatus(BOARD_I2C_COMM_INSTANCE, NULL) != kStatus_I2C_Success);

        OSA_TimeDelay(1);
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
