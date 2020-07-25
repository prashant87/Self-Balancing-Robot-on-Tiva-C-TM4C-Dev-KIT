#include "include.h"

//

int data[10];

void main(void)


{

    SysCtlClockSet(SYSCTL_SYSDIV_2_5|  SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ) ;
    PWM_Init();
    CC1101_Init();
    SW_Init();
    Timer_Init();
    MPU6050_Init();
    IntMasterEnable();
    QEIL_Init();
    QEIR_Init();
    State_Banlance=5;
    V_left_set=00;
    V_right_set=00;
    PWM_Enable(1,1);
//    double i=3.14;
     while (1)
     {
//        PWM_Run(30,50);
//        Kalman_Filter_Process();
//        MPU6050_Complimentary_Angle(MPU6050_DegGet);
//         MPU6050_Kalman_Angle(MPU6050_DegGet);
        V_left=-QEIVeloGet_left();
        V_right=-QEIVeloGet_right();
        LED_Status();
//        UART_Control();

//4

//        sprintf((char*)data, "%d  \n",(int)(100*ahrs_get_yaw()));
        UART_C1101_Write(data,strlen((char*)data));
        SysCtlDelay(SysCtlClockGet()/30);
    }

}
