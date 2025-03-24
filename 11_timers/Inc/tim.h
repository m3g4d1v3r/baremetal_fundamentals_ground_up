/*
 * tim.h
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f1xx.h"

#define TIM2EN     (0b1 << 0)
#define TIM2CTEN   (0b1 << 0)
#define TIM2SR_UIF (0b1 << 0)

void tim2_1hz_init(void);
void tim2_wait_interrupt(void);

#endif /* TIM_H_ */
