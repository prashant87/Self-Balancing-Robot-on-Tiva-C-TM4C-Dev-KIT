#ifndef PID_h_
#define PID_h_


//90 25 2.5
// PID Degree
    double Kp=100;
    double Ki=0;
    double Kd=0;
    double u;
    double u_;
    double e;
    double e_;
    double e__;
    double Ts=0.001;
    unsigned char pwm_low=5;
    unsigned char pwm_high=95;
    unsigned char pwm_div=100;
    extern void PID_Deg(double Deg_Status);
    static double PID_Deg_Process(double error);
    static double PID_Error_Deg(double Deg_Status);
// PID Velocity 8 0.6 0.1
                double Kpv=6;
               double Kiv=0.5;
               double Kdv=0.05;
               double Kpr=6;
               double Kir=0.5;
               double Kdr=0.05;
        double uvl,uvl_,evl,evl_,evl__;
        double uvr,uvr_,evr,evr_,evr__;
        double pwm_velo_div=1000;
        double T=0.001;
        double PID_Velo_Left_Process(double error);
        extern void PID_Velo_Left(void);
        double PID_Velo_Right_Process(double error);
        extern void PID_Velo_Right(void);
#endif
