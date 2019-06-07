/*
 * pwm.c
 *
 *  Created on: May 5, 2018
 *      Author: rockm
 */

#include "pwm.h"
//#include "../GPIO/gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/qei.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "debug.h"
static uint32_t Period;

void PWM_Init(void){
    Period= SysCtlClockGet()/40000;   //Clock 200Khz


    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypePWM(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    GPIOPinConfigure(GPIO_PD0_M0PWM6);
    GPIOPinConfigure(GPIO_PD1_M0PWM7);
    PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,Period);
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
 // disable H brigde for firstboot
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
       GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3);
       GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1|GPIO_PIN_2);
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2,~( GPIO_PIN_2));
       GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2,~( GPIO_PIN_2));
       GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3,~GPIO_PIN_3);
       GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,~GPIO_PIN_1);
       SysCtlDelay(100);
}


extern void HBrigdeEnable(motor_t motor, state_t state)
{
    switch (motor)
    {
    case LEFT:
        if (state)GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
        else  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
//        GPIOPinWrite(GPIO_PORTD_BASE, HBRIDGE_ENABLE_MORTOR_LEFT, HBRIDGE_ENABLE_MORTOR_LEFT&state);
        break;
    case RIGHT:
            if (state)GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
            else  GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
//        GPIOPinWrite(GPIO_PORTE_BASE, HBRIDGE_ENABLE_MORTOR_RIGHT, HBRIDGE_ENABLE_MORTOR_RIGHT&&state);
        break;
    }



}

extern void motor_set_speed(motor_t motor, float duty)
{
       // limit pwm
    duty = (float)duty/2 +50;
           duty = duty > 5 ? duty : 5;
           duty = duty < 95 ? duty : 95;
    //pulse pwm
           switch (motor)
           {
           case LEFT:
               PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, duty*Period/100-1);
               PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
               break;
           case RIGHT:
               PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, duty*Period/100-1);
               PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
               break;
           }


}




