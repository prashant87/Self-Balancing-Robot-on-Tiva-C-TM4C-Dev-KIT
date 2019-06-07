#ifndef  MOTOR_CONTROL_H_
#define  MOTOR_CONTROL_H_
#define MOVEBASE_PERIOD_MS          20

#include "debug.h"
xStatusTypeDef motor_control_serivce_init(void);
xStatusTypeDef motor_control_service_start(void);
xStatusTypeDef motor_set(float w[2]);

#endif
