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

static unsigned char key = '\0';

int main() {
	// usart1_init();
	usart1_init_interrupt();
	led_init();
	tim2_1hz_init();
	pa3_exti_init();
	tim2_wait_interrupt();
	tim2_wait_interrupt();
	tim2_wait_interrupt();
	tim2_wait_interrupt();

	printf("hello world\r\n");

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

void USART1_IRQHandler(void) {
	static bool flag = false;
	// __disable_irq(); // start of critical session
	// Check if RXNE is set: if so, then Receive Data
	if ((USART1->SR & USART_SR_RXNE) != 0) {
		key = USART1->DR;
		if (key != '\0') {
			if (key == '1') {
				led_on();
				// usart1_write('1');
				// usart1_write('\n');
				printf("stage1\r\n");
				tim2_wait_interrupt();
				tim2_wait_interrupt();
				flag = true;
			}
			else {
				led_off();
			}
		}
	}
	// Check if TXE is set: if so, then Transmit Data
	// else if ((USART1->SR & USART_SR_TXE) != 0) {
	// }
	// Check if TC is set: if so, then Transmit is Completed
	if ((USART1->SR & USART_SR_TC) != 0) {
		if (flag == true) {
			// usart1_write('2');
			// usart1_write('\n');
			printf("stage2\r\n");
			flag = false;
		}
	}
	// __enable_irq(); // end of critical session
}
