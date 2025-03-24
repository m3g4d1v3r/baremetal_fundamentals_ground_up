/*
 * spi.c
 *
 *  Created on: Mar 24, 2025
 *      Author: marlon
 */
#include <spi.h>
#include <gpio.h>

// NSS:  yellow - PA4
// SCK:  red    - PA5
// MISO: blue   - PA6
// MOSI: green  - PA7
#define SPI1_EN  (0b1 << 12)
#define IOPA_EN  (0b1 << 2)
#define CR1_BR   (0b1 << 3)
#define CR1_CPOL (0b1 << 1)
#define CR1_CPHA (0b1 << 0)
#define CR1_RXOY (0b1 << 10)
#define CR1_LSBF (0b1 << 7)
#define CR1_MSTR (0b1 << 2)
#define CR1_DFF  (0b1 << 11)
#define CR1_SSI  (0b1 << 8)
#define CR1_SSM  (0b1 << 9)
#define CR1_SPE  (0b1 << 6)
#define SR_TXE   (0b1 << 1)
#define SR_BUSY  (0b1 << 7)
#define SR_RXNE  (0b1 << 0)
#define GPIO_NSS (0b1 << 4)

void spi_gpio_init(void) {
	RCC->APB2ENR |= IOPA_EN;

	GPIOA->CRL &= ~(0b1111 << MODE4_OFFSET);
	GPIOA->CRL &= ~(0b1111 << MODE5_OFFSET);
	GPIOA->CRL &= ~(0b1111 << MODE6_OFFSET);
	GPIOA->CRL &= ~(0b1111 << MODE7_OFFSET);

	GPIOA->CRL |= (MODE_OUTPUT_50MHZ << MODE4_OFFSET);
	GPIOA->CRL |= (MODE_OUTPUT_50MHZ << MODE5_OFFSET);
	GPIOA->CRL |= (MODE_INPUT        << MODE6_OFFSET);
	GPIOA->CRL |= (MODE_OUTPUT_50MHZ << MODE7_OFFSET);

	GPIOA->CRL |= (CNF_OUTPUT_GNRL_PUSH_PULL << CNF4_OFFSET);
	GPIOA->CRL |= (CNF_OUTPUT_ALTR_PUSH_PULL << CNF5_OFFSET);
	GPIOA->CRL |= (CNF_INPUT_FLOAT           << CNF6_OFFSET);
	GPIOA->CRL |= (CNF_OUTPUT_ALTR_PUSH_PULL << CNF7_OFFSET);
}

void spi_config(void) {
	// Enable clock access to SPI1
	RCC->APB2ENR |= SPI1_EN;

	// Set clock to (Peripheral Clock / 4)
	SPI1->CR1 |= (0b111 << CR1_BR); // 30khz approximately.

	// Set CPOL to 0 and CPHA to 0
	SPI1->CR1 &= ~(0b1 << CR1_CPOL);
	SPI1->CR1 &= ~(0b1 << CR1_CPHA);

	// Enable full-duplex
	SPI1->CR1 &= ~(CR1_RXOY);

	// Set MSB first
	SPI1->CR1 &= ~(CR1_LSBF);

	// Set this device to be the Master
	SPI1->CR1 |= (CR1_MSTR);

	// Set the data size to 8-bit
	SPI1->CR1 &= ~(CR1_DFF);

	// Set to software management mode
	SPI1->CR1 |= (CR1_SSM);
	SPI1->CR1 |= (CR1_SSI);

	// Enable the SPI
	SPI1->CR1 |= CR1_SPE;
}

void spi_transmit(uint8_t *data, uint32_t size) {
	volatile uint8_t temp;

	for (uint32_t i = 0; i < size; ++i) {
		// Wait until TXE is set
		while ((SPI1->SR & SR_TXE) == 0);

		// Write the data to the data register
		SPI1->DR = data[i];

        // Wait for received byte (RXNE)
        while ((SPI1->SR & SR_RXNE) == 0);

        for (volatile int i = 0; i < 10000; ++i);
        // Read the received byte to clear RXNE
        data[i] = SPI1->DR;
	}
	// Wait until TXE is set
	while ((SPI1->SR & SR_TXE) == 0);

	// Wait for BUSY flag to reset
	while ((SPI1->SR & SR_BUSY) != 0);

	// Clear the overflow flag by reading from the DR and the SR
	temp = SPI1->DR;
	temp = SPI1->SR;

	(void) temp;
}

void spi_receive(uint8_t *data, uint32_t size) {
	volatile uint8_t temp;

	for (uint32_t i = 0; i < size; ++i) {
		// Send dummy data
		SPI1->DR = 0;

		// Wait until RXNE is set
		while ((SPI1->SR & SR_RXNE) == 0);

		// Read data from Data Register
		data[i] = SPI1->DR;
	}

	(void) temp;
}

void cs_enable(void) {
	// Not-Slave Select: PA4
	GPIOA->ODR &= ~(GPIO_NSS);
}

void cs_disable(void) {
	// Not-Slave Select: PA4
	GPIOA->ODR |= (GPIO_NSS);
}
