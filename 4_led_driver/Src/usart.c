/*
 * usart.c
 *
 *  Created on: Mar 21, 2025
 *      Author: marlon
 */
#include <usart.h>

#define USART1_EN (0b1 << 14)
#define IOPA_EN   (0b1 << 2)

void usart1_init() {
	// Configure UART GPIO pin
	// 1st: enable clock access to GPIO port A
	// 2nd: set PA9 and PA10 to alternate function mode
	// 3rd: set PA9 to TX
	// 4th: set PA10 to RX

	// 1st:
	RCC->APB2ENR |= IOPA_EN;

	// Configure USART module
	// 1st: Enable clock access to USART1
	// 2nd: Configure baud rate
	// 3rd: Configure the transfer direction
	// 4th: Enable USART1 module

	// 1st:
	RCC->APB2ENR |= USART1_EN;
}
