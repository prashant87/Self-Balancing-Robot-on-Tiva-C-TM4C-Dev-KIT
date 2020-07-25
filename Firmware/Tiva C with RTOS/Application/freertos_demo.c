//*****************************************************************************
//
// freertos_demo.c - Simple FreeRTOS example.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "drivers/led.h"
#include "drivers/switches.h"
#include "driverlib/interrupt.h"
#include "inc/hw_nvic.h"
#include "timers.h"
#include "debug.h"
#include "drivers/MPU6050.h"
#include "TwoWheel/twowheel.h"
#include "motor_control/motor_control.h"
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>FreeRTOS Example (freertos_demo)</h1>
//!
//! This application demonstrates the use of FreeRTOS on Launchpad.
//!
//! The application blinks the user-selected LED at a user-selected frequency.
//! To select the LED press the left button and to select the frequency press
//! the right button.  The UART outputs the application status at 115,200 baud,
//! 8-n-1 mode.
//!
//! This application utilizes FreeRTOS to perform the tasks in a concurrent
//! fashion.  The following tasks are created:
//!
//! - An LED task, which blinks the user-selected on-board LED at a
//!   user-selected rate (changed via the buttons).
//!
//! - A Switch task, which monitors the buttons pressed and passes the
//!   information to LED task.
//!
//! In addition to the tasks, this application also uses the following FreeRTOS
//! resources:
//!
//! - A Queue to enable information transfer between tasks.
//!
//! - A Semaphore to guard the resource, UART, from access by multiple tasks at
//!   the same time.
//!
//! - A non-blocking FreeRTOS Delay to put the tasks in blocked state when they
//!   have nothing to do.
//!
//! For additional details on FreeRTOS, refer to the FreeRTOS web page at:
//! http://www.freertos.org/
//
//*****************************************************************************
//#define UART_KIT

#ifdef UART_KIT
#define UART               SYSCTL_PERIPH_UART0
#define UART_GPIO          SYSCTL_PERIPH_GPIOA
#define UART_RX            GPIO_PA0_U0RX
#define UART_TX            GPIO_PA1_U0TX
#define GPIO_BASE          GPIO_PORTA_BASE
#define GPIO_PIN           (GPIO_PIN_0 | GPIO_PIN_1)
#define UART_BASE          UART0_BASE
#define Baudrate           115200
#define UART_Config        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)
#define UART_INT           INT_UART0
#define UART_INT_Config    (UART_INT_RX | UART_INT_RT)

#else
//            |   TIVA C   |        |  MODULE CC1101 |
//            |         3v3|------->|VCC             |
//            |         GND|------->|GND             |
//            |         PB0|------->|TXD             |
//            |         PB1|------->|RXD             |

#define UART               SYSCTL_PERIPH_UART1
#define UART_GPIO          SYSCTL_PERIPH_GPIOB
#define UART_RX            GPIO_PB0_U1RX
#define UART_TX            GPIO_PB1_U1TX
#define GPIO_BASE          GPIO_PORTB_BASE
#define GPIO_PIN           (GPIO_PIN_0 | GPIO_PIN_1)
#define UART_BASE          UART1_BASE
#define Baudrate           115200
#define UART_Config        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)
#define UART_INT           INT_UART1
#define UART_INT_Config    (UART_INT_RX | UART_INT_RT)

#endif
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}

#endif

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    ROM_SysCtlPeripheralEnable(UART_GPIO);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(UART);

    //
    // Configure GPIO Pins for UART mode.
    //
    ROM_GPIOPinConfigure(UART_RX);
    ROM_GPIOPinConfigure(UART_TX);
    ROM_GPIOPinTypeUART(GPIO_BASE,GPIO_PIN);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(1, 115200, 16000000);
}

xSemaphoreHandle    serialPortMutex;
static void Task1(void *pvParameters);
  // Queue task

/* Declare a variable of type xQueueHandle.  This is used to store the queue
that is accessed by all three tasks. */
xQueueHandle xQueue;
enum {
               Task_1=0,
               Task_2
} task_t ;
/* Define the structure type that will be passed on the queue. */
typedef struct
{
    unsigned char message[20];
    unsigned int taskSource;
} Data_t;

float speed[2]={1000,1000};

unsigned char Data;

//*****************************************************************************
//
// Initialize FreeRTOS and start the initial set of tasks.
//
//*****************************************************************************
int
main(void)
{

        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);

      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ConfigureUART();
    DBG("system starting \n");
    xStatusTypeDef xStatus = motor_control_serivce_init();
    DBG("Motor service init \n",xStatus);
    xStatus = motor_control_service_start();
    DBG("Motor service start \n ",xStatus);
//        xTaskCreate( Task1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
    xStatus = mpu_service_init();
    DBG("mpu service init \n",xStatus);
    xStatus = twowheel_service_control_init();
    DBG("twowheel_service_control_init \n",xStatus);
    xStatus = twowheel_service_control_start();
       DBG("twowheel_service_control_start \n",xStatus);


    vTaskStartScheduler();
    while(1)
    {
    }
}
static void Task1(void *pvParameters)
{
        while(1)
        {
//            DBG("Task 1 is running \n");
            motor_set(speed);
            vTaskDelay(2000);
            speed[LEFT] = speed[LEFT] +100;
            if (speed[LEFT]>1800) speed[LEFT] = 200;
            speed[RIGHT] = speed[RIGHT] +100;
                        if (speed[RIGHT]>1800) speed[RIGHT] = 200;
        }
}
void vApplicationIdleHook( void )
{
    UARTprintf(" Hook is running\r\n ");

}

void vApplicationTickHook( void )
{
    UARTprintf(" tick\r\n ");
}














