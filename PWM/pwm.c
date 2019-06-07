/*
 * pwm.c
 *
 *  Created on: May 5, 2018
 *      Author: rockm
 */

#include "../include.h"
#include "pwm.h"
//#include "../GPIO/gpio.h"

#define DVR8412
static uint32_t Period;

void PWM_Init(void){
    Period= SysCtlClockGet()/10000;   //Clock 200Khz

#ifdef DVR8412
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
}

#else
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypePWM(GPIO_PORTD_BASE,GPIO_PIN_0|GPIO_PIN_1);
        GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_2|GPIO_PIN_3);
        SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        GPIOPinConfigure(GPIO_PD0_M0PWM6);
        GPIOPinConfigure(GPIO_PD1_M0PWM7);
        GPIOPinConfigure(GPIO_PF2_M1PWM6);
        GPIOPinConfigure(GPIO_PF3_M1PWM7);
        PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
        PWMGenConfigure(PWM1_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN|PWM_GEN_MODE_NO_SYNC);
        PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,Period);
        PWMGenPeriodSet(PWM1_BASE,PWM_GEN_3,Period);
        PWMGenEnable(PWM0_BASE, PWM_GEN_3);
        PWMGenEnable(PWM1_BASE, PWM_GEN_3);

#endif

extern void HBrigdeEnable(bool left_enable, bool right_enable)
{

    if (left_enable)
      {
          GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
      }
      else
      {
          GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
      }
      if (right_enable)
      {
          GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
      }
      else
      {
          GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, ~GPIO_PIN_1);

      }
}
#ifdef DVR8412
extern void PWM_Run(float left_duty,float right_duty)
{
       // limit pwm
    left_duty = ((float)left_duty)/2+ 50;
    right_duty= ((float)right_duty)/2+ 50;
    left_duty = left_duty > 5 ? left_duty : 5;
    left_duty = left_duty < 95 ? left_duty : 95;
    right_duty = right_duty > 5 ? right_duty : 5;
    right_duty = right_duty < 95 ? right_duty : 95;

    //pulse pwm
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, left_duty*Period/100-1);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, right_duty*Period/100-1);
    PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
}

#else       // define for L298N H brigde
extern void PWM_Run(uint8_t f,uint8_t g,uint8_t h, uint8_t k)
{

        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, Period*f/100-1);
        PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, Period*g/100-1);
        PWMPulseWidthSet(PWM1_BASE,PWM_OUT_6, Period*h/100-1);
        PWMPulseWidthSet(PWM1_BASE,PWM_OUT_7, Period*k/100-1);
        PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
        PWMOutputState(PWM1_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
}
#endif


