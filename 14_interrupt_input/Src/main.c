#include <adc.h>
#include <exti.h>
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
	pa3_exti_init();

	while (true) {
		;
	}
}

static void exti_callback(void) {
	led_off();
	tim2_wait_interrupt();
	led_on();
	tim2_wait_interrupt();
	printf("BTN pressed...\n");
}

void EXTI3_IRQHandler(void) {
	if ((EXTI->PR & LINE3) != 0) {
		// Clear PR flag
		EXTI->PR |= LINE3;
		exti_callback();
	}
}
