/*
 * include.h
 *
 *  Created on: Mar 12, 2016
 *      Author: Satu
 */

#ifndef INCLUDE_H_
#define INCLUDE_H_


//thu vien ho tro ve bool, int, string va toan hoc math
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "UART/UART.h"

//thu vien driver API

#include "driverlib/gpio.h"
#include "driverlib/can.h"
#include "driverlib/eeprom.h"
#include "driverlib/i2c.h"
#include "driverlib/lcd.h"

#include "driverlib/mpu.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"

//thu vien ho tro phan cung
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "GPIO/gpio.h"
#include "PWM/pwm.h"
#include "Timer/Timer.h"
#include "ADC/ADC.h"
#include "MPU/MPU6050.h"
#include "QEI/QEI.h"
#include "SPI/SPI.h"
//
double MPU6050_DegGet[2];
double PWM_left,PWM_right,PWM_Deg;
double State_Banlance;
double V_left,V_right;
double V_left_set,V_right_set;
double PWM_SetV_Left;
double PWM_SetV_Right;

#endif /* INCLUDE_H_ */
