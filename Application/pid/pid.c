#include "pid.h"

void pid_init(pid_controller_t *pid){
    pid->i_part = 0;
    pid->prior_error = 0;
}

float pid_compute(pid_controller_t *pid, float error, float dt){


            float p_part = (pid->kp)*error;

            pid->i_part += (pid->ki)*error*dt;
            pid->i_part = pid->i_part > pid->i_part_min ? pid->i_part : pid->i_part_min;
            pid->i_part = pid->i_part < pid->i_part_max ? pid->i_part : pid->i_part_max;

            float d_part = 0;
            if(!pid->is_first_compute){
                    d_part = (pid->kd)*(error - pid->prior_error)*dt;
                    pid->is_first_compute = false;
            }
            pid->prior_error = error;

            float pid_output = p_part + pid->i_part + d_part;
            pid_output = pid_output > pid->output_min ? pid_output : pid->output_min;
            pid_output = pid_output < pid->output_max ? pid_output : pid->output_max;

            return pid_output + pid->ouput_offset;
}
float pid_compute_2(pid_t *pid,float error,float dt){
    pid->e__ = pid -> e_;
    pid->e_  = pid -> e ;
    pid->e   = error;
    pid->u_  = pid ->u;
    pid->u   = pid->u_ +
            (pid->kp)*(pid->e - pid->e_)+
            (pid->ki)*(pid->e + pid->e_)*(pid->Ts)+
            (pid->kd)*(pid->e -(2*pid->e_)+pid->e__)/(pid->Ts);
            return pid->u;
}
