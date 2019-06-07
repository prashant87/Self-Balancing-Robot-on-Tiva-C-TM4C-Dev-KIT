#include "include.h"


#include "TwoWheel/TwoWheel.h"
#include "SysTickTimer/SysTimer.h"



/**
 * main.c
 */
int main(void)
{
   SysCtlClockSet(SYSCTL_SYSDIV_5|  SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ) ; //80mhz
   TwoWheelModelInit();

   IntMasterEnable();

   while(1)

   {



   }


}
