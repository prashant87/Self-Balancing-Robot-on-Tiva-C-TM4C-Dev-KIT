#ifndef pwm_h_
#define pwm_h_


#include <math.h>
#include  <stdint.h>
#include <stdbool.h>

static uint32_t Period;

extern void HBrigdeEnable(bool left_enable, bool right_enable);
extern void PWM_Run(float left_duty,float right_duty);
extern void PWM_Init(void);


#endif
