#include <i2c.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <usart.h>
#include <tim.h>
#define ARRAY_LEN(x) sizeof(x) / sizeof(x[0])

int __io_putchar(int ch) {
	usart1_write(ch);
    return ch;
}

int main() {
	tim2_1hz_init();
	i2c1_init();
	char message[100];
	char to_send[] = "STM32 Hello";

	while (true) {
		i2c1_byte_burst_read(0x55, 0x55, ARRAY_LEN(message), message);
		i2c1_byte_burst_write(0x55, 0x55, ARRAY_LEN(to_send), to_send);
		tim2_wait_interrupt();
	}
}
