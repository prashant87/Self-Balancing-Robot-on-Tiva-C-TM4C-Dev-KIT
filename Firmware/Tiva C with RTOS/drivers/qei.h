/*
 * QEI.h
 *
 *  Created on: Jul 31, 2018
 *      Author: rockm
 */

#ifndef QEI_QEI_H_
#define QEI_QEI_H_
#include "debug.h"
//
//typedef struct {
//    float left;
//    float right;
//} velocity_t;

void QEI_Init(void);

float QEI_Get(motor_t motor);




#endif /* QEI_QEI_H_ */
