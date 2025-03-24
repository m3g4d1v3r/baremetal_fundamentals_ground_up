#include <adc.h>
#include <led.h>
#include <usart.h>
#include <tim.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <systick.h>

int __io_putchar(int ch) {
	usart1_write(ch);
    return ch;
}

void test_direct_connection(void) {
    // Setup clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    // Free PB4 from JTAG
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    // Configure PA0 as simple GPIO output (not timer)
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x3 << 0);  // Output, 50MHz

    // Configure PB4 as simple GPIO input with pull-down
    GPIOB->CRL &= ~(0xF << 16);
    GPIOB->CRL |= (0x8 << 16);  // Input with pull-down

    // Set up LED on PC13
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x3 << 20);  // Output, 50MHz

    // Initial states
    GPIOA->BSRR = (1 << 0);  // PA0 high initially
    GPIOC->BSRR = (1 << 13); // LED off initially

    while(1) {
        // Toggle PA0 at a visible rate
        GPIOA->ODR ^= (1 << 0);

        // Delay (adjust based on your clock speed)
        for(volatile int i = 0; i < 100000; i++);

        // Check if PB4 matches PA0
        if(((GPIOA->ODR & (1 << 0)) >> 0) == ((GPIOB->IDR & (1 << 4)) >> 4)) {
            // Connection works - LED off
            GPIOC->BSRR = (1 << 13);
        } else {
            // Connection doesn't work - LED on
            GPIOC->BRR = (1 << 13);
        }
    }
}

void setup_loopback_test(void) {
    // Enable clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure PC13 for LED
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x3 << 20);  // Output, 50MHz

    // Configure PA0 as GPIO output initially
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x3 << 0);  // Output, 50MHz

    // Generate a pulse manually
    GPIOA->BSRR = (1 << 0);  // Set PA0 high
    for(volatile int i = 0; i < 1000; i++);
    GPIOA->BRR = (1 << 0);   // Set PA0 low

    // Now reconfigure PA0 for input capture
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |= (0x4 << 0);  // Input floating

    // Configure TIM2 for input capture
    TIM2->CR1 = 0;
    TIM2->CCMR1 = 0;
    TIM2->CCER = 0;

    // Set prescaler
    TIM2->PSC = 7999;  // 8MHz/8000 = 1kHz

    // Configure channel 1 for input capture
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0;  // CC1S = 01: Map to TI1

    // Configure for rising edge detection
    TIM2->CCER &= ~TIM_CCER_CC1P;  // Rising edge
    TIM2->CCER |= TIM_CCER_CC1E;   // Enable capture

    // Clear flags
    TIM2->SR = 0;

    // Start timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // Generate another pulse manually using a different pin
    GPIOA->CRL &= ~(0xF << 4);     // Configure PA1
    GPIOA->CRL |= (0x3 << 4);      // Output, 50MHz

    GPIOA->BSRR = (1 << 1);        // Set PA1 high
    for(volatile int i = 0; i < 1000; i++);
    GPIOA->BRR = (1 << 1);         // Set PA1 low

    // Now manually connect PA1 to PA0 with a jumper

    // Wait for capture
    while(!(TIM2->SR & TIM_SR_CC1IF));

    // Capture occurred - turn on LED
    GPIOC->BRR = (1 << 13);  // LED on (active low)
}

int main() {
	// test_direct_connection();
	usart1_init();
	led_init();
	tim2_1hz_init();
	tim2_output_compare();
	tim3_input_capture();

	while (true) {
		tim3_wait_interrupt();
		printf("A second has passed!\n");
		led_off();
		tim3_wait_interrupt();
		printf("A second has passed!\n");
		led_on();
	}
}
