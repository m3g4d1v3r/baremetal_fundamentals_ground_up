#include <adc.h>
#include <usart.h>
#include <tim.h>
#include <led.h>
#include <spi.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <systick.h>
#define ARRAY_LEN(x) sizeof(x) / sizeof(x[0])

int __io_putchar(int ch) {
	usart1_write(ch);
    return ch;
}

static char send_msg[] = "Hello from STM32!!";
static char msg[100];

int main() {
	led_init();
	tim2_1hz_init();
	spi_gpio_init();
	spi_config();

	cs_disable();
	while (true) {
		strncpy(msg, send_msg, ARRAY_LEN(send_msg));
		cs_enable();
		spi_transmit((uint8_t *)msg, ARRAY_LEN(send_msg));
		cs_disable();
		tim2_wait_interrupt();
		led_toggle();
	}
}
