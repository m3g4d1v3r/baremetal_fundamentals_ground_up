/*
 * usart.c
 *
 *  Created on: Mar 21, 2025
 *      Author: marlon
 */
#include <usart.h>
#include <gpio.h>

#define USART1_EN (0b1 << 14)
#define IOPA_EN   (0b1 << 2)
#define AFIO_EN	  (0b1 << 0)
#define SYS_FREQ  (8000000UL)
#define APB2_FREQ (8000000UL)
#define USART_BAUDRATE (2400)

static void usart1_set_baudrate(uint32_t BaudRate);

void dma_channel4_init(uint32_t src, uint32_t dst, uint32_t len) {
	// Enable clock access to DMA
	RCC->AHBENR |= RCC_AHBENR_DMA1;

	// Disable DMA Channel 4
	DMA1_Channel4->CCR &= ~(DMA_CCRX_EN);
	while ((DMA1_Channel4->CCR & DMA_CCRX_EN) != 0);

	// Clear all interrupt flags of Channel 4
	DMA1_Channel4->CCR &= ~(0b111 << 1);

	// Set the source buffer
	DMA1_Channel4->CMAR = (src);

	// Set the destination buffer
	DMA1_Channel4->CPAR = (dst);

	// Set the length
	DMA1_Channel4->CNDTR = (len);

	// Enable memory increment
	DMA1_Channel4->CCR |= (DMA_CCRX_MINC);

	// Configure transfer direction
	DMA1_Channel4->CCR |= (DMA_CCRX_DIR); // from memory to peripheral

	// Enable Transfer Complete interrupt
	DMA1_Channel4->CCR |= (0b1 << 1);

	// Enable DMA1 Channel4
	DMA1_Channel4->CCR |= (DMA_CCRX_EN);

	// Enable UART1 transmitter DMA
	USART1->CR3 |= (USART_CR3_DMAT);

	// DMA Interrupt Enable in NVIC
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void usart1_init() {
	// Configure UART GPIO pin
	// 1st: enable clock access to GPIO port A
	// 2nd: set PA9 and PA10 to alternate function mode
	// 3rd: set PA9 to TX
	// 4th: set PA10 to RX

	RCC->APB2ENR |= IOPA_EN;
	RCC->APB2ENR |= AFIO_EN;
	GPIOA->CRH &= ~(0b11 << MODE9_OFFSET);  // clear MODE9
	GPIOA->CRH &= ~(0b11 << CNF9_OFFSET);   // clear CNF9
	GPIOA->CRH &= ~(0b11 << MODE10_OFFSET); // clear MODE10
	GPIOA->CRH &= ~(0b11 << CNF10_OFFSET);  // clear CNF10
	GPIOA->CRH |= (MODE_OUTPUT_50MHZ         << MODE9_OFFSET);
	GPIOA->CRH |= (CNF_OUTPUT_ALTR_PUSH_PULL << CNF9_OFFSET);
	GPIOA->CRH |= (MODE_INPUT                << MODE10_OFFSET);
	GPIOA->CRH |= (CNF_INPUT_FLOAT           << CNF10_OFFSET);

	// Configure USART module
	// 1st: Enable clock access to USART1
	// 2nd: Configure baud rate
	// 3rd: Configure the transfer direction
	// 4th: Enable USART1 module

	RCC->APB2ENR |= USART1_EN;
	usart1_set_baudrate(USART_BAUDRATE);
    // USART1->CR1 &= ~(USART_CR1_M); // 8 data bits
    // USART1->CR2 &= ~(USART_CR2_STOP); // 1 stop bit

	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;

	while (!(USART1->SR & USART_SR_TC));
}

void usart1_init_interrupt(void) {
	__disable_irq();
	// Configure UART GPIO pin
	// 1st: enable clock access to GPIO port A
	// 2nd: set PA9 and PA10 to alternate function mode
	// 3rd: set PA9 to TX
	// 4th: set PA10 to RX

	RCC->APB2ENR |= IOPA_EN;
	RCC->APB2ENR |= AFIO_EN;
	GPIOA->CRH &= ~(0b11 << MODE9_OFFSET);  // clear MODE9
	GPIOA->CRH &= ~(0b11 << CNF9_OFFSET);   // clear CNF9
	GPIOA->CRH &= ~(0b11 << MODE10_OFFSET); // clear MODE10
	GPIOA->CRH &= ~(0b11 << CNF10_OFFSET);  // clear CNF10
	GPIOA->CRH |= (MODE_OUTPUT_50MHZ         << MODE9_OFFSET);
	GPIOA->CRH |= (CNF_OUTPUT_ALTR_PUSH_PULL << CNF9_OFFSET);
	GPIOA->CRH |= (MODE_INPUT                << MODE10_OFFSET);
	GPIOA->CRH |= (CNF_INPUT_FLOAT           << CNF10_OFFSET);

	// Configure USART module
	// 1st: Enable clock access to USART1
	// 2nd: Configure baud rate
	// 3rd: Configure the transfer direction
	// 4th: Enable USART1 module

	RCC->APB2ENR |= USART1_EN;
	usart1_set_baudrate(USART_BAUDRATE);
    // USART1->CR1 &= ~(USART_CR1_M); // 8 data bits
    // USART1->CR2 &= ~(USART_CR2_STOP); // 1 stop bit

	// Configure the transfer direction
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

	// Enable RXNE interrupt
	USART1->CR1 |= CR1_RXNEIE;

	// DO NOT: Enable TXE interrupt
	// USART1->CR1 |= CR1_TXEIE;

	// DO NOT: Enable TCIE interrupt
	// USART1->CR1 |= CR1_TCIE;

	// Enable USART1 interrupt in NVIC
	NVIC_EnableIRQ(USART1_IRQn);

	// Enable USART module
	USART1->CR1 |= USART_CR1_UE;

	while (!(USART1->SR & USART_SR_TC));
	USART1->SR &= ~(USART_SR_TC);
	__enable_irq();
}

unsigned char usart1_read() {
	while (!(USART1->SR & USART_SR_RXNE));
	return USART1->DR;
}

void usart1_write(unsigned char chr) {
	// Make sure transmit data register is empty
	// Write to transmit data register
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = chr;
	USART1->SR &= ~(USART_SR_TC);
}

void usart1_write_str(unsigned char str[], size_t len) {
	for (size_t idx = 0; idx < len; ++idx) {
		usart1_write(str[idx]);
	}
}

static void usart1_set_baudrate(uint32_t BaudRate) {
    uint16_t divider = (APB2_FREQ + (BaudRate/2)) / BaudRate;
    USART1->BRR = divider;
}
