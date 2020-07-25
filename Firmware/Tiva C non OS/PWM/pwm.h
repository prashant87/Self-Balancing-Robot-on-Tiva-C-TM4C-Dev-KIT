#ifndef pwm_h_
#define pwm_h_
static uint32_t Period;

extern void PWM_Enable(uint8_t l, uint8_t r); // ham nay de bat cau H
extern void PWM_Run(uint8_t l,uint8_t r); //
extern void PWM_Control(float l, float r);
extern void PWM_Init(void);
extern void PWM_Checker(void);
void PWM_Converter (void);
#endif
