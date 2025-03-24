#include <adc.h>
#include <usart.h>
#include <tim.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <systick.h>

int __io_putchar(int ch) {
	usart1_write(ch);
    return ch;
}

static uint32_t adc1_data_register;

void delay_s(int n) {
	for (int i = 0; i < n * 10000; ++i)
		;
}

int main() {
	usart1_init();
	adc1_interrupt_init();
	tim2_1hz_interrupt_init();
	delay_s(2);

	printf("hello world\r\n");
	while (true) {
		;
	}
}

void ADC1_2_IRQHandler(void) {
	// Check if it was ADC1 who interrupted
	if ((ADC1->SR & ADC_EOC) != 0) {
		adc1_data_register = (ADC1->DR);
	}
}

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~(TIM2SR_UIF);
	adc1_start_conversion();
	printf("adc1_data_register: %lu\r\n", adc1_data_register);
}
