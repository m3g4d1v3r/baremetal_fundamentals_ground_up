/*
 * exti.h
 *
 *  Created on: Mar 23, 2025
 *      Author: marlon
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f1xx.h"
#include <gpio.h>

#define EXTICR1_EXTI2 (0b1 << 8)
#define EXTIIMR_MR3   (0b1 << 3)
#define EXTIRTSR_TR3  (0b1 << 3)

#define LINE3         (0b1 << 3)

void pa3_exti_init(void);

#endif /* EXTI_H_ */
