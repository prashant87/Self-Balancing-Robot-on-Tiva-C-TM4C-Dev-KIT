#include "TwoWheel.h"
#include "MPU/MPU6050.h"
#include "PWM/pwm.h"
#include "PID/PID.h"
#include "LED/led.h"
#include "Encoder/QEI.h"

pid_controller_t PID_Angle;

pid_controller_t PID_Velocity[2];




void TwoWheelModelInit(void)
{
    PWM_Init();
    MPU6050_Init();
    pid_init(&PID_Angle);
    ledInit();
    HBrigdeEnable(1,1);



    PID_Angle=(pid_controller_t){
        .kp=0.7,
        .ki=0.1,
        .kd=0,
        .i_part_max=100,
      .i_part_min=-100,
      .output_max=100,
      .output_min=-100,
      .ouput_offset=0
    };


}

void TwoWheelModelControlLoop(void)
{
    double  CurrentAngleInDouble[2];
    MPU6050_Complimentary_Angle(CurrentAngleInDouble);
    float   CurrentAngleInFloat = (float)CurrentAngleInDouble[0];
    float   uk=pid_compute(&PID_Angle, CurrentAngleInFloat, 100);
    PWM_Run(uk, uk);
    if(CurrentAngleInFloat>0)
    {
        ledControl(LEDBLUE, ON);
        ledControl(LEDGREEN, OFF);
    }
    else
    {
        ledControl(LEDBLUE, ON);
        ledControl(LEDGREEN, OFF);
    }

}

