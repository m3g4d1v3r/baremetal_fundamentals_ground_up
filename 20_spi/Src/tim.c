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
	TIM2->PSC = 8000 - 1; // 8 000 000 / 8 000 = 1 000
	// Set the auto reload value
	TIM2->ARR = 1000 - 1; // 1 000 / 1 000 = 1
	// Clear the timer counter
	TIM2->CNT = 0;
	// Enable the timer
	TIM2->CR1 |= TIM2CTEN;
}

void tim2_1hz_interrupt_init(void) {
	// Enable Clock Access to Timer 2
	RCC->APB1ENR |= TIM2EN;

	// Set the prescaler value
	TIM2->PSC = 8000 - 1; // 8 000 000 / 8 000 = 1 000

	// Set the auto reload value
	TIM2->ARR = 1000 - 1; // 1 000 / 1 000 = 1

	// Clear the timer counter
	TIM2->CNT = 0;

	// Enable the timer
	TIM2->CR1 |= TIM2CTEN;

	// Enable TIM2 Interrupt
	TIM2->DIER |= DIER_UIE;

	// Enable TIM2 Interrupt in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
}

void tim2_output_compare(void) {
	// Enable Clock Access to Timer 2
	RCC->APB1ENR |= TIM2EN;

	// Set the prescaler value
	// TIM2->PSC = 800 - 1; // 8 000 000 / 800 = 10 000
	TIM2->PSC = 800 - 1; // 8 000 000 / 800 = 10 000
	// Set the auto reload value
	// TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1
	TIM2->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	// Set output compare toggle mode
	TIM2->CCMR1 |= (0b011 << 4);
	// Enable tim2 ch1 in compare mode
	TIM2->CCER |= CC1E;

	RCC->APB2ENR |= IOPA_EN;
	RCC->APB2ENR |= AFIO_EN;
	GPIOA->CRL &= ~(0b11 << MODE0_OFFSET);
	GPIOA->CRL &= ~(0b11 << CNF0_OFFSET);
	GPIOA->CRL |= (MODE_OUTPUT_50MHZ << MODE0_OFFSET);
	GPIOA->CRL |= (CNF_OUTPUT_ALTR_PUSH_PULL << CNF0_OFFSET);

	// Clear the timer counter
	TIM2->CNT = 0;
	// Enable the timer
	TIM2->CR1 |= TIM2CTEN;
}

void tim3_input_capture(void) {
	// Enable clock access to GPIOB
	RCC->APB2ENR |= IOPB_EN;
	RCC->APB2ENR |= AFIO_EN;

	// Free PB4 from JTAG
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

	// Set PB4 alternate function type to TIM3_CH1
	GPIOB->CRL &= ~(0b11 << MODE5_OFFSET);
	GPIOB->CRL &= ~(0b11 << CNF5_OFFSET);
	GPIOB->CRL |= (MODE_INPUT << MODE5_OFFSET);
	GPIOB->CRL |= (CNF_INPUT_PU_PD << CNF5_OFFSET);

	// Enable clock access to TIM4
	RCC->APB1ENR |= TIM3EN;

	// Set Prescaler
	TIM3->PSC = 8000 - 1; // 8 000 000 / 8 000
	// TIM3->ARR = 10000 - 1; // 10 000 / 10 000 = 1

	// Set CH1 to input capture
	TIM3->CCMR1 |= CCMR1_CC2S;

	// Set CH3 to capture at rising edge
	TIM3->CCER &= ~(CCER_CC2P);
	TIM3->CCER |= CCER_CC2E;

	// Enable TIM3
	TIM3->CR1 |= CR1_CEN;
}

void tim2_wait_interrupt(void) {
	// Wait for UIF
	while ((TIM2->SR & TIM2SR_UIF) == 0);
	// Clear UIF
	TIM2->SR &= ~(TIM2SR_UIF);
}

void tim3_wait_interrupt(void) {
	// Wait for CC1F
	while ((TIM3->SR & SR_CC2IF) == 0);

	// Clear CC1F
	TIM3->SR &= ~(SR_CC2IF);
	// Wait for CCR1 to toggle
}
