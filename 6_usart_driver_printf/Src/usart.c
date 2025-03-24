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
#define SYS_FREQ  (72000000UL)
#define APB2_FREQ (72000000UL)
#define USART_BAUDRATE (115200)

static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate);
static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static void usart_write(unsigned char chr);

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
	GPIOA->CRH |= (CNF_INPUT_PU_PD           << CNF10_OFFSET);

	// Configure USART module
	// 1st: Enable clock access to USART1
	// 2nd: Configure baud rate
	// 3rd: Configure the transfer direction
	// 4th: Enable USART1 module

	RCC->APB2ENR |= USART1_EN;
	usart_set_baudrate(USART1, SYS_FREQ, USART_BAUDRATE);
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
}

static void usart_write(unsigned char chr) {
	// Make sure transmit data register is empty
	// Write to transmit data register
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = chr;
}

static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
	return (PeriphClk + (BaudRate / 2U)) / BaudRate;
}

static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}
