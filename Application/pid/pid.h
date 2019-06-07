#ifndef __PID__H__
#define __PID__H__

#include <stdbool.h>

typedef struct{
    float kp;
    float ki;
    float kd;
    float i_part_max;
    float i_part_min;
    float output_max;
    float output_min;
    float ouput_offset; // output = pid_output + pid->ouput_offset;
    float error_max;
    float default_output;
    float i_part;
    float prior_error;
    float is_first_compute;
}pid_controller_t;

void pid_init(pid_controller_t *pid);
float pid_compute(pid_controller_t *pid, float feedback, float dt);

#endif
