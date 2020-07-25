/*
 * gpio.c
 *
 *  Created on: Apr 24, 2018
 *      Author: rockm
 */
#include "../include.h"
#include "gpio.h"
//#include "../PWM/pwm.h"
unsigned char Status;
extern void SW_Init(void)
{
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x01;
        HWREG(GPIO_PORTF_BASE + GPIO_O_AFSEL) &= ~0x01;
        GPIOPinTypeGPIOInput(PORTF, SW1|SW2);
        GPIOPadConfigSet(PORTF, SW1|SW2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
        GPIOPinWrite(PORTF,  SW1|SW2,  SW1|SW2);
        GPIOIntRegister(PORTF, &SW_Isr);
        GPIOIntEnable(PORTF, SW1|SW2);
        IntEnable(INT_GPIOF);
        IntMasterEnable();
}
extern void SW_Isr(void)
{
    GPIOIntClear(PORTF, GPIOIntStatus(PORTF, true));
    Status = ~Status; // 0 == Off
                      // 1 == Run
    //TimerEnable(TIMER1_BASE, TIMER_A);
}
extern unsigned char SW_Status(void){
    return Status;
}
extern void LED_Status(void)
{
        if(MPU6050_DegGet[0]>0)
           {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, ~GPIO_PIN_3);

           }
           else
           {
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
               GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
           }
}

