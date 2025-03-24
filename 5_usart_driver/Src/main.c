#include <led.h>
#include <usart.h>
#include <stdbool.h>

#define DELAY_CONST 150

void delay(uint64_t n) {
	for (uint64_t idx = 0; idx < (n * n); idx++);
}

int main() {
	led_init();
	usart1_init();
	usart1_write('Y');

	while (true) {
		led_off();
		delay(DELAY_CONST);
		led_on();
		delay(DELAY_CONST);
	}
}
