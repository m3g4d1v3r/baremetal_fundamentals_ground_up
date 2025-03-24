/*
 * tim.h
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f1xx.h"
#include <gpio.h>

#define TIM4_BASE  (APB1PERIPH_BASE + 0x00000800UL)
#define TIM4       ((TIM_TypeDef *)TIM4_BASE)

#define TIM2EN     (0b1 << 0)
#define TIM3EN     (0b1 << 1)
#define TIM4EN     (0b1 << 2)
#define TIM2CTEN   (0b1 << 0)
#define TIM2SR_UIF (0b1 << 0)
#define CC1E       (0b1 << 0)
#define IOPA_EN    (0b1 << 2)
#define IOPB_EN    (0b1 << 3)
#define CCMR1_CC1S (0b1 << 0)
#define CCMR1_CC2S (0b1 << 8)
#define CCER_CC1E  (0b1 << 0)
#define CCER_CC1P  (0b1 << 1)
#define CCER_CC2E  (0b1 << 4)
#define CCER_CC2P  (0b1 << 5)
#define CR1_CEN    (0b1 << 0)
#define SR_CC1IF   (0b1 << 1)
#define SR_CC2IF   (0b1 << 2)
#define DIER_UIE   (0b1 << 0)

void tim2_1hz_init(void);
void tim2_wait_interrupt(void);
void tim2_output_compare(void);
void tim2_1hz_interrupt_init(void);
void tim3_input_capture(void);
void tim3_wait_interrupt(void);

#endif /* TIM_H_ */
