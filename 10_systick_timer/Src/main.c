#include <adc.h>
#include <led.h>
#include <usart.h>
#include <stdbool.h>
#include <string.h>
#include <systick.h>

double convert(char buffer[], int sensor_value) {
	double volt = sensor_value * 3.3 / 4095;
	return volt;
}

int main() {
	led_init();

	while (true) {
		systick_delay_ms(50);
		led_off();
		systick_delay_ms(50);
		led_on();
	}
}
