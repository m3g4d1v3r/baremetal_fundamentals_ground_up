#include "stm32f103x6.h"
#include <stdbool.h>

#define IOPC_EN    (0b1 << 4)
#define SET_PC13   (0b1 << 13)
#define UNSET_PC13 (0b1 << 29)

#define DELAY_CONST 600

void delay(uint64_t n) {
	for (uint64_t idx = 0; idx < (n * n); idx++);
}

int main() {
	RCC->APB2ENR |= IOPC_EN;

	GPIOC->CRH &= ~(0b1111 << 20); // clear bits for PC13
	GPIOC->CRH |= (0b0011 << 20);
	while (true) {
		GPIOC->BSRR |= UNSET_PC13;
		delay(DELAY_CONST);
		GPIOC->BSRR |= SET_PC13;
		delay(DELAY_CONST);
	}
}
