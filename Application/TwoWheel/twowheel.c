
#include "FreeRTOS.h"
#include "timers.h"
#include "pid/pid.h"
#include "debug.h"
#include "motor_control/motor_control.h"
#include "drivers/MPU6050.h"
#include "drivers/pwm.h"
pid_controller_t PID;
static TimerHandle_t xTimersAngleBase;

float a_get[2];
float a_out[2];
void twowheel_service_control_loop(TimerHandle_t xTimer)
{

    MPU6050_Complimentary_Angle(a_get);

    a_out[LEFT]=pid_compute(&PID,- a_get[0] ,MOVEBASE_PERIOD_MS);
    a_out[RIGHT]= a_out[LEFT];

    motor_set(a_out);

    DBG("A: %d \n",(int)a_get[0]);
}


xStatusTypeDef twowheel_service_control_init(void)
{
    PID = (pid_controller_t)
        {
            .kp=20,
            .ki=000,
            .kd=0.05,//0.006
            .i_part_max = 100,
            .i_part_min = -80,
            .output_max = 80,
            .output_min = -100,
            .ouput_offset = 0
        };
    xTimersAngleBase = xTimerCreate("Angle base", MOVEBASE_PERIOD_MS, pdTRUE, ( void * ) 0, twowheel_service_control_loop);


    return OK;
}
xStatusTypeDef twowheel_service_control_start(void)
{
    xTimerStart( xTimersAngleBase, 0 );
    HBrigdeEnable(LEFT, ON);
        HBrigdeEnable(RIGHT,ON);
    return OK;

}

