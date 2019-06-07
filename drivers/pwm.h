#ifndef pwm_h_
#define pwm_h_

#include "debug.h"
#include <math.h>
#include  <stdint.h>
#include <stdbool.h>

#define HBRIDGE_ENABLE_MORTOR_LEFT      GPIO_PIN_2
#define HBRIDGE_ENABLE_MORTOR_RIGHT     GPIO_PIN_1
#define HBRIDGE_OUTPUT_MORTOR_LEFT       PWM_OUT_6
#define HBRIDGE_OUTPUT_MORTOR_RIGHT      PWM_OUT_7


static uint32_t Period;
typedef enum {OFF=0,ON} state_t;
extern void HBrigdeEnable(motor_t motor, state_t state);
extern void motor_set_speed(motor_t motor, float duty);
extern void PWM_Init(void);


#endif
