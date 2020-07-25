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
static float left=1, right=1;
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

extern void PWM_Enable(uint8_t l, uint8_t r)
{

    if (l)
      {
          GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
      }
      else
      {
          GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
      }
      if (r)
      {
          GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
      }
      else
      {
          GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, ~GPIO_PIN_1);

      }
}
#ifdef DVR8412
extern void PWM_Run(uint8_t l,uint8_t r)
{

    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6, left*Period*l/100-1);
    PWMPulseWidthSet(PWM0_BASE,PWM_OUT_7, right*Period*r/100-1);
    PWMOutputState(PWM0_BASE,PWM_OUT_6_BIT|PWM_OUT_7_BIT, true);
}

#else
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


extern void PWM_Control(float l, float r)
{
    left=l;
    right=r;
}
extern void PWM_Checker(void)
{
    if((MPU6050_DegGet[0]<-80)||(MPU6050_DegGet[0]>80)) //
      {
//        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
//        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, ~GPIO_PIN_3);
        GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2,~( GPIO_PIN_2));
              GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2,~( GPIO_PIN_2));
              GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3,~GPIO_PIN_3);
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1,~GPIO_PIN_1);
//        SysCtlDelay(SysCtlClockGet()/3);
//        if((MPU6050_DegGet[0]>-80)&&(MPU6050_DegGet[0]<80))
//        {
//            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
//            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
//        }
      }
    if (PWM_left>95)
        {
            PWM_left=95;
        }
        if (PWM_left<5)
        {
            PWM_left=5;
        }
        if (PWM_right>95)
           {
               PWM_right=95;
           }
        if (PWM_right<5)
           {
               PWM_right=5;
           }
}
extern void PWM_Converter (void)
{
    if (PWM_left>50)
    {

    }
    else
    {

    }
    if (PWM_right>50)
    {

    }
    else
    {

    }
}
