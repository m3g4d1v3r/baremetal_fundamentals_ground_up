/*
 * exti.c
 *
 *  Created on: Mar 23, 2025
 *      Author: marlon
 */

#include <exti.h>

void pa3_exti_init(void) {
	// Disable global interrupt
	__disable_irq();

	// Enable clock access for GPIOA
	RCC->APB2ENR |= IOPA_EN;

	// Enable clock access to AFIO
	RCC->APB2ENR |= AFIO_EN;

	// Set PA3 as input
	GPIOA->CRL &= ~(0b11 << MODE3_OFFSET);
	GPIOA->CRL &= ~(0b11 << CNF3_OFFSET);
	GPIOA->CRL |= (MODE_INPUT << MODE3_OFFSET);
	GPIOA->CRL |= (CNF_INPUT_PU_PD << CNF3_OFFSET);

	// Select PORTA for EXTI3
	AFIO->EXTICR[0] &= ~(0b1111 << 8);

	// Un-mask EXTI3
	EXTI->IMR |= EXTIIMR_MR3;

	// Select Rising Edge trigger
	EXTI->RTSR |= EXTIRTSR_TR3;

	// Enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI3_IRQn);

	// Re-enable global interrupt
	__enable_irq();
}
