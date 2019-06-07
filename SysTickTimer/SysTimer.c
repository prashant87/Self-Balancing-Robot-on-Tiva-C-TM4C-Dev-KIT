#include "../include.h"
#include "SysTimer.h"

uint16_t Timercount=0;

bool  TimerOverflowFlag = 0;

void SystickInit(void)
{
    SysTickPeriodSet(SysCtlClockGet()/100);
    SysTickIntRegister(&Systick_ISR);
    SysTickIntEnable();
    SysTickEnable();
}

void Systick_ISR(void)
{
    if (Timercount<10)
        {
        Timercount++;
        }
    else{

        TimerOverflowFlag=1;
        Timercount=0;
    }

}
bool SystickFlagReturn(void)
{
    return TimerOverflowFlag;
}
void SystickFlagSet(bool Flag)
{
    TimerOverflowFlag = Flag;
}

