/*
 * tim.c
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */
#include <tim.h>

void tim2_1hz_init(void) {
	// Enable Clock Access to Timer 2
	RCC->APB1ENR |= TIM2EN;

	// Set the prescaler value
	TIM2->PSC = 800 - 1; // 8 000 000 / 800 = 10 000
	// Set the auto reload value
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1
	// Clear the timer counter
	TIM2->CNT = 0;
	// Enable the timer
	TIM2->CR1 |= TIM2CTEN;
}

void tim2_wait_interrupt(void) {
	// Wait for UIF
	while ((TIM2->SR & TIM2SR_UIF) == 0);
	// Cleared UIF
	TIM2->SR &= ~(TIM2SR_UIF);
}
