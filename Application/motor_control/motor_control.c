
#include "motor_control.h"

#include "debug.h"
#include "drivers/pwm.h"
#include "drivers/qei.h"
#include "pid/pid.h"

#include "driverlib/sysctl.h"

#include "FreeRTOS.h"
#include "timers.h"
//#include "os_task.h"
//#include "os_timer.h"

pid_controller_t  PID_Velocity[2];


static float w_get[2];
static float w_set[2];
static float w_run[2];
static TimerHandle_t xTimersMoveBase;


void motor_control_service_loop(TimerHandle_t xTimer);


xStatusTypeDef   motor_control_serivce_init(void)
{
    PWM_Init();
    HBrigdeEnable(LEFT, OFF);
    HBrigdeEnable(RIGHT, OFF);
    QEI_Init();



    PID_Velocity[LEFT]=(pid_controller_t)
               {
                   .kp=0.07,
                   .ki=00.0004,
                   .kd=0.001,//0.006
                   .i_part_max = 100,
                      .i_part_min = -80,
                      .output_max = 80,
                      .output_min = -100,
                      .ouput_offset = 0
               };
    PID_Velocity[RIGHT]=(pid_controller_t)
               {

                .kp=0.07,
                .ki=00.0004,
                .kd=0.001,//0.006
                .i_part_max = 100,
                   .i_part_min = -80,
                   .output_max = 80,
                      .output_min = -100,
                      .ouput_offset = 0
               };

    pid_init(&PID_Velocity[LEFT]);
    pid_init(&PID_Velocity[RIGHT]);

    xTimersMoveBase = xTimerCreate("Move base", MOVEBASE_PERIOD_MS, pdTRUE, ( void * ) 0, motor_control_service_loop);
//    xTimersMoveBase = xTimerCreate("Move base", MOVEBASE_PERIOD_MS, pdTRUE, ( void * ) 0, motor_control_service_loop);

    SysCtlDelay(100);


    return OK;
}
xStatusTypeDef motor_control_service_start(void)
{
    xTimerStart( xTimersMoveBase, 0 );
    return OK;
}
void motor_control_service_loop(TimerHandle_t xTimer)
{
    w_get[LEFT]=-QEI_Get(LEFT);
    w_get[RIGHT]=-QEI_Get(RIGHT);

    w_run[LEFT]=pid_compute(&PID_Velocity[LEFT], w_set[LEFT] - w_get[LEFT],MOVEBASE_PERIOD_MS );
    w_run[RIGHT]=pid_compute(&PID_Velocity[RIGHT], w_set[RIGHT] - w_get[RIGHT],MOVEBASE_PERIOD_MS );

    motor_set_speed(LEFT, w_run[LEFT]);
    motor_set_speed(RIGHT, w_run[RIGHT]);

   // DBG("DBG1: %d DBG2: %d %d \n ",(int)w_get[LEFT],(int)w_set[LEFT],(int)w_get[RIGHT]);
//    DBG("DBG2: %d\n ",(int)w_set[LEFT]);
//    DBG("DBG: En:%d  \n ",(int)w_get[RIGHT]);
//    DBG("DBG: PWM_OUT_1: %d \n ",(int)w_run[LEFT]);
//    DBG("DBG: PWM_OUT_2: %d  \n ",(int)w_run[RIGHT]);
}

xStatusTypeDef motor_set(float w[2])
{
    w_set[LEFT]=w[LEFT];
    w_set[RIGHT]=w[RIGHT];
    return OK;
}



