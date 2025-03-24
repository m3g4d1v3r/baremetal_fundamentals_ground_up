/*
 * adc.c
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */

#include <adc.h>

void adc1_init() {
	// 1st: Enable clock access to GPIOA
	RCC->APB2ENR |= IOPAEN;

	// 2nd: Set the mode of PA1 to analog
	GPIOA->CRL &= ~(0b11 << MODE1_OFFSET);
	GPIOA->CRL &= ~(0b11 << CNF1_OFFSET);
	GPIOA->CRL |= (MODE_INPUT << MODE1_OFFSET);
	GPIOA->CRL |= (CNF_INPUT_ANALOG << CNF1_OFFSET);

	// 3rd: Enable clock access to ADC
	RCC->APB2ENR |= ADC1EN;

	// 4th: Set EXTSEL to use SWSTART
	ADC1->CR2 |= (0b111 << 17);

	// 5th: Enable External Trigger
	ADC1->CR2 |= (0b1 << 20);

	// 6th: Conversion sequence start
	ADC1->SQR3 = ADC1_CH1;

	// 7th: Configure sequence length
	ADC1->SQR1 = ADC1_SEQ_LEN_1;

	// 8th: Enable ADC1
	ADC1->CR2 |= ADC1_ADON;

	// 9th: Calibrate the ADC1
	ADC1->CR2 |= (0b1 << 2);
	while ((ADC1->CR2 & 0b1 << 2) != 0);
}

void adc1_start_conversion() {
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_SWSTART;
}

uint32_t adc1_read(void) {
	// 1st: Wait for conversion to be complete
	while ((ADC1->SR & ADC_EOC) == 0);

	// 2nd: Read converted result
	return (ADC1->DR);
}
