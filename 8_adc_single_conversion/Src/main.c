#include <adc.h>
#include <led.h>
#include <usart.h>
#include <stdbool.h>
#include <string.h>

#define DELAY_CONST 400

void delay(uint64_t n) {
	for (uint64_t idx = 0; idx < (n * n); idx++);
}

int main() {
	led_init();
	usart1_init();
	adc1_init();

	unsigned char chr;
	uint32_t sensor_value;

	while (true) {
		adc1_start_conversion();

		chr = usart1_read();
		sensor_value = adc1_read();
		led_off();
		delay(sensor_value);
		led_on();
		usart1_write(chr);
		usart1_write(sensor_value);
	}
}
