#include "../include.h"
#include "Timer.h"
//#include "../PID/PID.h"

typedef struct{
    float kp;
    float ki;
    float kd;
    float i_part_max;
    float i_part_min;
    float output_max;
    float output_min;
    float setpoint;
    float ouput_offset; // output = pid_output + pid->ouput_offset;

    float i_part;
    float prior_error;
    float is_first_compute;
}pid_t;
void pid_init(pid_t *pid);
float pid_compute(pid_t *pid, float input, float dt);
pid_t PID_Yaw={
            .kp=1,
            .ki=0,
            .kd=0,
            .i_part_max=100,
          .i_part_min=-100,
          .output_max=100,
          .output_min=-100,
          .setpoint=0,
          .ouput_offset=0
        };
extern void Timer_Init(void)
{
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()/1000-1);
        TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer_Isr);
        IntEnable(INT_TIMER1A);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        TimerEnable(TIMER1_BASE, TIMER_A);
        pid_init(&PID_Yaw);


}
static void Timer_Isr(void)
{
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
//    MPU6050_Complimentary_Angle(MPU6050_DegGet);
  // All control
//    PID_Deg(MPU6050_DegGet[0]);
//    PWM_Deg=0;
    Kalman_Filter_Process();
    MPU6050_Complimentary_Angle(MPU6050_DegGet);
    PWM_Deg= -pid_compute(&PID_Yaw,MPU6050_DegGet[0] ,10);
    PID_Velo_Left();
    PID_Velo_Right();
    PWM_left=50 +(PWM_SetV_Left+PWM_Deg);
//    if (PWM_left>50)
  //  {
    //    PWM_left=PWM_left-5;
    //}
    PWM_right=50+PWM_SetV_Right+PWM_Deg;
    //PWM_left=50+PWM_Deg;
   //?WM_right=50+PWM_Deg;
    PWM_Checker();
    PWM_Run(PWM_left,PWM_right);
}

void pid_init(pid_t *pid){
    pid->i_part = 0;
    pid->prior_error = 0;
}

float pid_compute(pid_t *pid, float input, float dt){
    float error = input - pid->setpoint;

    float p_part = pid->kp*error;

    pid->i_part += pid->ki*error*dt;
    pid->i_part = pid->i_part > pid->i_part_min ? pid->i_part : pid->i_part_min;
    pid->i_part = pid->i_part < pid->i_part_max ? pid->i_part : pid->i_part_max;

    float d_part = 0;
    if(!pid->is_first_compute){
        d_part = pid->kd*(error - pid->prior_error)*dt;
        pid->is_first_compute = false;
    }
    pid->prior_error = error;

    float pid_output = p_part + pid->i_part + d_part;
    pid_output = pid_output > pid->output_min ? pid_output : pid->output_min;
    pid_output = pid_output < pid->output_max ? pid_output : pid->output_max;

    return pid_output + pid->ouput_offset;
}

