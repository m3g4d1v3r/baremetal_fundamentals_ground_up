#include <adc.h>
#include <usart.h>
#include <tim.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int __io_putchar(int ch) {
	usart1_write(ch);
    return ch;
}

uint32_t adc1_data_register;

static void delay_s(int seconds) {
	for (int i = 0; i < seconds; ++i)
		tim2_wait_interrupt();
}

int main() {
	usart1_init();
	adc1_interrupt_init();
	tim2_1hz_init();
	delay_s(2);

	printf("hello world\r\n");

	while (true) {
		adc1_start_conversion();
		printf("adc1_data_register: %lu\r\n", adc1_data_register);
		delay_s(2);
	}
}

void ADC1_2_IRQHandler(void) {
	// Check if it was ADC1 who interrupted
	if ((ADC1->SR & ADC_EOC) != 0) {
		adc1_data_register = (ADC1->DR);
	}
}
