#include "PID.h"
#include "../include.h"
double PWM_SetD;

extern void PID_Deg(double Deg_Status)
{

      static double error;
      error=State_Banlance-Deg_Status;
      PWM_SetD= PID_Deg_Process(error);
//      if(PWM_SetD<pwm_low)
//          PWM_SetD=pwm_low;
//      if(PWM_SetD>pwm_high)
//          PWM_SetD=pwm_high;
      PWM_Deg=PWM_SetD;
//          PWM=PWM_Set;
  // return PWM_Set;
}

double PID_Deg_Process(double error)
{
        e__ = e_;
        e_ = e;
        e = error;
        u_ = u;
        u = u_ + Kp * (e - e_)
               + Ki * Ts * (e + e_)
               + (Kd / Ts) * (e - (2 * e_) + e__);
        return (u/pwm_div);
}
double PID_Error_Deg(double Deg_Status)
{
        return State_Banlance-Deg_Status;
}
extern void PID_Velo_Left(void)
{
    static double V_Erorr_Left;
    V_Erorr_Left=V_left_set-V_left;
    PWM_SetV_Left=PID_Velo_Left_Process(V_Erorr_Left);
}

double PID_Velo_Left_Process(double error)
{
       evl__ = evl_;
       evl_ = evl;
       evl = error;
       uvl_ = uvl;
       uvl = uvl_ + Kpv * (evl - evl_)
               + Kiv * T * (evl + evl_)
               + (Kdv / T) * (evl - (2 * evl_) + evl__);
        return (uvl/pwm_velo_div);
}
extern void PID_Velo_Right(void)
{
    static double V_Erorr_Right;
    V_Erorr_Right=V_right_set-V_right;
    PWM_SetV_Right=PID_Velo_Right_Process(V_Erorr_Right);
}

double PID_Velo_Right_Process(double error)
{
       evr__ = evr_;
       evr_ = evr;
       evr = error;
       uvr_ = uvr;
       uvr = uvr_ + Kpr * (evr - evr_)
               + Kir * T * (evr + evr_)
               + (Kdr / T) * (evr - (2 * evr_) + evr__);
        return (uvr/pwm_velo_div);
}

