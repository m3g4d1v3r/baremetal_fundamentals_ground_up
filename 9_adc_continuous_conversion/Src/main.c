#include <adc.h>
#include <led.h>
#include <usart.h>
#include <stdbool.h>
#include <string.h>

#define DELAY_CONST 400

void delay(uint64_t n) {
	for (uint64_t idx = 0; idx < (n * n); idx++);
}

double convert(char buffer[], int sensor_value) {
	double volt = sensor_value * 3.3 / 4095;
	// sprintf(buffer, "%f", volt);
	return volt;
}

int main() {
	led_init();
	usart1_init();
	adc1_init();

	uint32_t sensor_value;
	adc1_start_conversion();
	char buffer[10];
	int buffer_idx = 0;

	while (true) {
		// chr = usart1_read();
		sensor_value = adc1_read();
		led_off();
		delay(DELAY_CONST);
		led_on();
		// usart1_write(chr);
		usart1_write(sensor_value);
	}
}
