/*
 * led.c
 *
 *  Created on: Mar 20, 2025
 *      Author: marlon
 */
#include <led.h>

#define IOPC_EN    (0b1 << 4)
#define SET_PC13   (0b1 << 13)
#define UNSET_PC13 (0b1 << 29)

void led_init(void) {
	RCC->APB2ENR |= IOPC_EN;

	GPIOC->CRH &= ~(0b1111 << 20); // clear bits for PC13
	GPIOC->CRH |= (0b0011 << 20);
}

void led_on(void) {
	GPIOC->BSRR |= UNSET_PC13;
}

void led_off(void) {
	GPIOC->BSRR |= SET_PC13;
}

void led_toggle(void) {
	GPIOC->ODR ^= (0b1 << 13);
}
