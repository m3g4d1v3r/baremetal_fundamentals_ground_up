#include <adc.h>
#include <usart.h>
#include <tim.h>
#include <led.h>
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

static char message[] = "Hello from Stm32 DMA transfer\r\n";

int main() {
	led_init();
	// adc1_interrupt_init();
	tim2_1hz_interrupt_init();
	delay_s(2);
	usart1_init();

	while (true) {
		;
	}
}

void DMA1_Channel4_IRQHandler(void) {
	// Check for the interrupt flag
	if ((DMA1->ISR & DMA_ISR_TCIF) != 0) {
		// Clear the flag
		DMA1->IFCR |= (DMA_IFCR_CTCIF);
		// adc1_start_conversion();
		// printf("adc1_data_register: %lu\r\n", adc1_data_register);
		led_toggle();
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
	char buffer[100];
	strcpy(buffer, message);
	dma_channel4_init(
		(uint32_t) buffer,
		(uint32_t) &USART1->DR,
		strlen(buffer)
	);
}
