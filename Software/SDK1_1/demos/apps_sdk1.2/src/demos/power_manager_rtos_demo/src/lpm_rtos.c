/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
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
#include <assert.h>
#include <stdio.h>
#include <string.h>

// SDK Included Files
#include "board.h"
#include "fsl_os_abstraction.h"
#include "fsl_smc_hal.h"
#include "fsl_adc16_driver.h"

// Project Included Files
#include "lpm_rtos.h"
#include "adc16_temperature.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// task prio
#define TASK_LPM_PRIO          6U
#define TASK_LED1_PRIO         8U
#define TASK_LED2_PRIO         7U

#define TASK_LPM_STACK_SIZE     0x400U
#define TASK_LED1_STACK_SIZE    0x200U
#define TASK_LED2_STACK_SIZE    0x200U

// task declare
extern void task_lpm(task_param_t param);
#if (!defined FSL_RTOS_BM)
extern void task_led1(task_param_t param);
extern void task_led2(task_param_t param);
#endif

// task define
OSA_TASK_DEFINE(task_lpm, TASK_LPM_STACK_SIZE);
#if (!defined FSL_RTOS_BM)
OSA_TASK_DEFINE(task_led1, TASK_LED1_STACK_SIZE);
OSA_TASK_DEFINE(task_led2, TASK_LED2_STACK_SIZE);
#endif
///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
#if (defined BOARD_USE_LPSCI)
static lpsci_state_t s_dbgState;
static lpsci_user_config_t s_dbgConfig;
#elif (defined BOARD_USE_LPUART)



#else
static uart_state_t s_dbgState;
static uart_user_config_t s_dbgConfig;
#endif // BOARD_USE_LPSCI


static osa_status_t s_result = kStatus_OSA_Error;
static adc16_user_config_t adcUserConfig;
static adc16_chn_config_t adcChnConfig;
static adc16_calibration_param_t adcCalibraitionParam;

/************************* Configure for MQX************************************/
#if (defined FSL_RTOS_MQX)&&(MQX_COMMON_CONFIG == MQX_LITE_CONFIG)
#if MQX_STDIO
#error "MQX Lite configuration is designed to work with tool provided STD Library.\
Remove reference to MQX STD library from your build tool project options:\
IAR:\
    Linker->Library->aditional_libraries                             - remove lib_mqx_stdlib.a path \
    C/C++ Compiler->Preprocessor->Additional include directories:    - remove mqx_stdlib \
\
KEIL: \
    Linker->Misc controls   - remove lib_mqx_stdlib.lib path \
    C/C++->Include Paths    - remove mqx_stdlib \
\
KDS: \
    C/C++ Build\Settings->Cross ARM C Linker\Miscellaneous    - remove lib_mqx_stdlib.a path\
    C/C++ Build\Settings->Cross ARM C Compiler\Includes       - remove mqx_stdlib (on 4th line)\
\
Atollic: \
    C/C++ Build\Settings->C Linker/Libraries->Librarie search path    - remove lib_mqx_stdlib \
    C/C++ Build\Settings->C Compiler/Directories->Include Paths       - remove mqx_stdlib \
CMAKE : \
    Remove following lines from CMakeList.txt: \
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../rtos/mqx/lib/twrk22f120m.armgcc/debug/mqx_stdlib) \
    INCLUDE_DIRECTORIES(${ProjDirPath}/../../../../../rtos/mqx/lib/twrk22f120m.armgcc/release/mqx_stdlib) \
    \
    TARGET_LINK_LIBRARIES(lpm_rtos_mqx ${ProjDirPath}/../../../../../rtos/mqx/lib/twrk22f120m.armgcc/debug/mqx_stdlib/lib_mqx_stdlib.a) \
    TARGET_LINK_LIBRARIES(lpm_rtos_mqx ${ProjDirPath}/../../../../../rtos/mqx/lib/twrk22f120m.armgcc/release/mqx_stdlib/lib_mqx_stdlib.a) \
."
#endif /* MQX_STDIO */
#elif (defined FSL_RTOS_MQX)&&(MQX_COMMON_CONFIG != MQX_LITE_CONFIG)
#define MAIN_TASK        8
void main_task(uint32_t param);
const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, main_task, 0xC00, 20, "main_task", MQX_AUTO_START_TASK},
   { 0L,        0L,        0L,    0L,  0L,         0L }
};
#endif /* (FSL_RTOS_MQX)&&(MQX_COMMON_CONFIG != MQX_LITE_CONFIG) */

///////////////////////////////////////////////////////////////////////////////
// Code
///////////////////////////////////////////////////////////////////////////////

char getInput()
{
    char ch;
    /* We use getchar() for BM and UART Blocking technic for other RTOS. */
#if (defined FSL_RTOS_BM)
    ch = getchar();
#else
#if (defined(BOARD_USE_LPSCI))
    LPSCI_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE,(uint8_t*)(&ch),1,OSA_WAIT_FOREVER);
#elif (defined(BOARD_USE_LPUART))
    LPUART_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE,(uint8_t*)(&ch),1,OSA_WAIT_FOREVER);
#else
    UART_DRV_ReceiveDataBlocking(BOARD_DEBUG_UART_INSTANCE, (uint8_t*)(&ch), 1, OSA_WAIT_FOREVER);
#endif
#endif // FSL_RTOS_BM
    return ch;
}

#if (defined FSL_RTOS_MQX) && (MQX_COMMON_CONFIG != MQX_LITE_CONFIG)
    void main_task(uint32_t param)
#else /* (FSL_RTOS_MQX) && (MQX_COMMON_CONFIG != MQX_LITE_CONFIG) */
    int main(void)
#endif /* (FSL_RTOS_MQX) && (MQX_COMMON_CONFIG != MQX_LITE_CONFIG) */
{
    memset(&s_dbgState, 0, sizeof(s_dbgState));
    memset(&s_dbgConfig, 0, sizeof(s_dbgConfig));

    OSA_Init();

    // init the uart module with base address and config structure
    s_dbgConfig.baudRate = 9600;
#if (defined BOARD_USE_LPSCI)
    s_dbgConfig.bitCountPerChar = kLpsci8BitsPerChar;
    s_dbgConfig.parityMode = kLpsciParityDisabled;
    s_dbgConfig.stopBitCount = kLpsciOneStopBit;
    LPSCI_DRV_Init(BOARD_DEBUG_UART_INSTANCE, &s_dbgState, &s_dbgConfig);
#elif (defined BOARD_USE_LPUART)
    s_dbgConfig.bitCountPerChar = kLpuart8BitsPerChar;
    s_dbgConfig.parityMode = kLpuartParityDisabled;
    s_dbgConfig.stopBitCount = kLpuartOneStopBit;
    LPUART_DRV_Init(BOARD_DEBUG_UART_INSTANCE, &s_dbgState, &s_dbgConfig);
#else
    s_dbgConfig.bitCountPerChar = kUart8BitsPerChar;
    s_dbgConfig.parityMode = kUartParityDisabled;
    s_dbgConfig.stopBitCount = kUartOneStopBit;
    UART_DRV_Init(BOARD_DEBUG_UART_INSTANCE, &s_dbgState, &s_dbgConfig);
#endif // BOARD_USE_LPSCI

    hardware_init();
    // Initializes GPIO driver for LEDs and buttons
#if (defined FSL_RTOS_BM)
    GPIO_DRV_Init(switchPins, 0);
#else
    GPIO_DRV_Init(switchPins, ledPins);
#endif

    NVIC_SetPriority(PM_DBG_UART_IRQn, 6U);

#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    NVIC_SetPriority(RTC_IRQn, 6U);
#endif
    NVIC_SetPriority(LPTMR0_IRQn, 6U);
    NVIC_SetPriority(ADC_IRQ_N, 6U);
#if ((defined FRDM_KL03Z48M)|| (defined(TWR_KV10Z75M)))
    NVIC_SetPriority(LLWU_IRQn, 6U);
#else
    NVIC_SetPriority(LLW_IRQn, 6U);
#endif

#if (defined FSL_RTOS_MQX)
    OSA_InstallIntHandler(PM_DBG_UART_IRQn, PM_DBG_UART_IRQ_HANDLER);

#if (defined PM_RTOS_DEMO_RTC_FUNC_INSTANCE)
    OSA_InstallIntHandler(RTC_IRQn, MQX_RTC_IRQHandler);
#endif
    OSA_InstallIntHandler(LPTMR0_IRQn, MQX_LPTMR0_IRQHandler);
#if ((defined FRDM_KL03Z48M)|| (defined(TWR_KV10Z75M)))
    OSA_InstallIntHandler(LLWU_IRQn, MQX_LLWU_IRQHandler);
#else
    OSA_InstallIntHandler(LLW_IRQn, MQX_LLW_IRQHandler);
#endif
    OSA_InstallIntHandler(ADC_IRQ_N, MQX_ADC1_IRQHandler);
#endif

    dbg_uart_init();

    adc16Init(&adcUserConfig, &adcChnConfig, &adcCalibraitionParam);

    // Low power manager task.
    s_result = OSA_TaskCreate(task_lpm,
                (uint8_t *)"lpm",
                TASK_LPM_STACK_SIZE,
                task_lpm_stack,
                TASK_LPM_PRIO,
                (task_param_t)0,
                false,
                &task_lpm_task_handler);
    if (s_result != kStatus_OSA_Success)
    {
         printf("Failed to create lpm task\r\n");
    }

    // These tasks will not start in BM.
#if (!defined FSL_RTOS_BM)
    s_result = OSA_TaskCreate(task_led2,
                (uint8_t *)"led2",
                TASK_LED2_STACK_SIZE,
                task_led2_stack,
                TASK_LED2_PRIO,
                (task_param_t)0,
                false,
                &task_led2_task_handler);
    if (s_result != kStatus_OSA_Success)
    {
        printf("Failed to create led2 task\r\n");
    }

    s_result = OSA_TaskCreate(task_led1,
                (uint8_t *)"led1",
                TASK_LED1_STACK_SIZE,
                task_led1_stack,
                TASK_LED1_PRIO,
                (task_param_t)0,
                false,
                &task_led1_task_handler);
    if (s_result != kStatus_OSA_Success)
    {
        printf("Failed to create led1 task\r\n");
    }
#endif

    OSA_Start();

    for(;;) {}                    // Should not achieve here
}
///////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////
