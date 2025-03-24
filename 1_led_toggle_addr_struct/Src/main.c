#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define PERIPH_BASE	       (0x40000000UL)
#define APB2_OFFSET        (0x00010000UL)
#define APB2_BASE	       (PERIPH_BASE + APB2_OFFSET)
#define AHB_OFFSET         (0x00018000UL)
#define AHB_BASE           (PERIPH_BASE + AHB_OFFSET)

#define GPIOC_OFFSET       (0x00001000UL)
#define GPIOC_BASE	       (APB2_BASE + GPIOC_OFFSET)
#define RCC_OFFSET         (0x00009000UL)
#define RCC_BASE           (AHB_BASE + RCC_OFFSET)

#define __IO volatile

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

typedef struct {
	__IO uint32_t CRL;
	__IO uint32_t CRH;
	__IO uint32_t IDR;
	__IO uint32_t ODR;
	__IO uint32_t BSRR;
	__IO uint32_t BRR;
	__IO uint32_t LCKR;
} GPIO_t;

typedef struct {
	__IO uint32_t DUMMY1[5];
	__IO uint32_t AHBENR;
	__IO uint32_t APB2ENR;
	__IO uint32_t APB1ENR;
} RCC_t;

void delay(uint64_t n) {
	for (uint64_t i = 0; i < n; i++) {
		for (uint64_t j = 0; j < n; j++) {
			;
		}
	}
}

#define RCC   ((RCC_t *) RCC_BASE)
#define GPIOC ((GPIO_t *) GPIOC_BASE)

int main(void) {
	RCC->APB2ENR |= 0b1 << 4; // IOPC EN

	GPIOC->CRH &= ~(0xf << 20); // clear bits for pin 13
	GPIOC->CRH |= (0b0011 << 20);

	while (true) {
		GPIOC->BSRR |= (0b1 << 29);
		delay(250);
		GPIOC->BSRR |= (0b1 << 13);
		delay(250);
	}
}
