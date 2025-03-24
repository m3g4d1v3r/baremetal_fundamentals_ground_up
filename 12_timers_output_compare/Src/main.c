#include <adc.h>
#include <led.h>
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

int main() {
	usart1_init();
	led_init();
	tim2_1hz_init();
	tim2_output_compare();

	while (true) {
		;
		// printf("A second has passed!\n");
		// led_off();
		// printf("A second has passed!\n");
		// led_on();
	}
}
