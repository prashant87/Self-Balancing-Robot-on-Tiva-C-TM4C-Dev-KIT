
#include <drivers/qei.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "debug.h"
/*
 * QEI.c
 *
 *  Created on: Jun 18, 2016
 *      Author: TungNgo
 */

static void QEI_ISR_left(void);
static void QEI_ISR_right(void);
static float Velocity_left=0,Velocity_right=0;
void QEI_Init(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;
    HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) &= ~0x80;

    ROM_QEIConfigure(QEI0_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET
                | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP, 0xFFFFFFFF);
    ROM_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    ROM_GPIOPinConfigure(GPIO_PD6_PHA0);
    ROM_GPIOPinConfigure(GPIO_PD7_PHB0);

    SysCtlDelay(100);

    ROM_QEIVelocityConfigure(QEI0_BASE,QEI_VELDIV_1,ROM_SysCtlClockGet()/50);
    ROM_QEIVelocityEnable(QEI0_BASE);
    ROM_QEIIntEnable(QEI0_BASE,QEI_INTINDEX|QEI_INTTIMER);
    QEIIntRegister(QEI0_BASE, &QEI_ISR_left);
    ROM_QEIEnable(QEI0_BASE);

    ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_QEI1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
       ROM_QEIConfigure(QEI1_BASE, QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET
                        | QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP, 0xFFFFFFFF);
       ROM_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_4 |GPIO_PIN_5 | GPIO_PIN_6);
       ROM_GPIOPinConfigure(GPIO_PC5_PHA1);
       ROM_GPIOPinConfigure(GPIO_PC6_PHB1);
       ROM_GPIOPinConfigure(GPIO_PC4_IDX1);

        //  ROM_QEIVelocityConfigure(QEI1_BASE,QEI_VELDIV_1,ROM_SysCtlClockGet()/10);
         // ROM_QEIVelocityEnable(QEI1_BASE);
       ROM_QEIVelocityConfigure(QEI1_BASE,QEI_VELDIV_1,ROM_SysCtlClockGet()/50);
       ROM_QEIVelocityEnable(QEI1_BASE);
       ROM_QEIIntEnable(QEI1_BASE,QEI_INTINDEX|QEI_INTTIMER);
       QEIIntRegister(QEI1_BASE, &QEI_ISR_right);
       ROM_QEIEnable(QEI1_BASE);
       SysCtlDelay(100);

}
static void QEI_ISR_left(void)
{
        Velocity_left=(float)ROM_QEIVelocityGet(QEI0_BASE)*ROM_QEIDirectionGet(QEI0_BASE);
        ROM_QEIIntClear(QEI0_BASE,ROM_QEIIntStatus(QEI0_BASE,true));
}
static void QEI_ISR_right(void)
{
        Velocity_right=(float)ROM_QEIVelocityGet(QEI1_BASE)*ROM_QEIDirectionGet(QEI1_BASE);
        ROM_QEIIntClear(QEI1_BASE,ROM_QEIIntStatus(QEI1_BASE,true));
}

float QEI_Get(motor_t motor)
{
    switch (motor)
    {
        case LEFT:
            return Velocity_left;
        case RIGHT:
            return Velocity_right;
    }
    return 0;
}


