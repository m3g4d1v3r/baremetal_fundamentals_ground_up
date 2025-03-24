/*
 * i2c.c
 *
 *  Created on: Mar 23, 2025
 *      Author: marlon
 */
#include <i2c.h>
#include <gpio.h>

#define APB1ENR_I2C1   (0b1 << 21)
#define APB2ENR_GPIOB  (0b1 << 3)
#define SWRST          (0b1 << 15)
#define CR1_PE         (0b1 << 0)
#define CR1_START      (0b1 << 8)
#define CR1_STOPF      (0b1 << 9)
#define CR1_ACKF       (0b1 << 10)
#define SR1_STARTF     (0b1 << 0)
#define SR1_ADDRF      (0b1 << 1)
#define SR1_BTF        (0b1 << 2)
#define SR1_RXNE       (0b1 << 6)
#define SR1_TXEF       (0b1 << 7)
#define SR2_BUSY       (0b1 << 1)
#define WR_BIT         (0b0)
#define RD_BIT         (0b1)
// SPI1_SCL: Alternate function open-drain, PB6, yellow
// SPI1_SDA: Alternate function open-drain, PB7, red

void i2c1_init(void) {
	// Enable clock access to GPIOB
	RCC->APB2ENR |= APB2ENR_GPIOB;

	// Set PB6 and PB7 to alternate function open-drain
	GPIOB->CRL &= ~(0b11 << CNF6_OFFSET);
	GPIOB->CRL &= ~(0b11 << CNF7_OFFSET);
	GPIOB->CRL &= ~(0b11 << MODE6_OFFSET);
	GPIOB->CRL &= ~(0b11 << MODE7_OFFSET);

	GPIOB->CRL |= (CNF_OUTPUT_ALTR_OPEN_DRAIN << CNF6_OFFSET);
	GPIOB->CRL |= (CNF_OUTPUT_ALTR_OPEN_DRAIN << CNF7_OFFSET);
	GPIOB->CRL |= (MODE_OUTPUT_50MHZ << MODE6_OFFSET);
	GPIOB->CRL |= (MODE_OUTPUT_50MHZ << MODE7_OFFSET);


	// Enable pull-ups for PB6 and PB7
	// Done by hardware, not internally by software

	// Enable clock access to I2C1
	RCC->APB1ENR |= APB1ENR_I2C1;

	// Reset the I2C
	I2C1->CR1 |= SWRST;

	// Come-out of the reset I2C
	I2C1->CR1 &= ~(SWRST);

	// Set Peripheral Clock frequency
	I2C1->CR2 |= (0b1 << 3); // 8MHz

	// Configure 100khz for SPI
	// I2C1->CCR |= (8000000 / (2 * (100000)) << 0);
	// 50khz
	I2C1->CCR = 80; // 8MHz / (2 * 50kHz)

	// TRISE configuration
	I2C1->TRISE = 9;  // Maximum rise time + 1

	// Enable I2C
	I2C1->CR1 |= (CR1_PE);
}

void i2c1_byte_read(char slave_addr, char memory_addr, char *data) {
	volatile int temp;

	while (I2C1->SR2 & SR2_BUSY);

	I2C1->CR1 |= CR1_START;

	// Wait for the Start Flag
	while ((I2C1->SR1 & SR1_STARTF) == 0);

	// Transmit Slave Address + Write
	I2C1->DR |= (slave_addr << 1) | (WR_BIT << 0); // 7 bits

	// Wait for the Address Flag
	while ((I2C1->SR1 & SR1_ADDRF) == 0);

	// Clear the Address Flag
	temp = I2C1->SR2;

	// Send Master Address
	I2C1->DR = memory_addr;

	// Wait until transmitter is empty
	while ((I2C1->SR1 & SR1_TXEF) == 0);

	// Generate restart
	I2C1->CR1 |= CR1_START;

	// Wait for the Start Flag
	while ((I2C1->SR1 & SR1_STARTF) == 0);

	// Transmit Slave Address + Read
	I2C1->DR |= (slave_addr << 1) | (RD_BIT << 0); // 7 bits

	// Wait for the Address Flag
	while ((I2C1->SR1 & SR1_ADDRF) == 0);

	// Disable the ACK
	I2C1->CR1 &= ~(CR1_ACKF);

	// Clear the Address Flag
	temp = I2C1->SR2;

	// Generate stop after data received
	I2C1->CR1 |= CR1_STOPF;

	// Wait until DR is not empty
	while ((I2C1->SR1 & SR1_RXNE) == 0);

	*data++ = I2C1->DR;
	(void) temp;
}

void i2c1_byte_burst_read(char slave_addr, char memory_addr, int n, char *data) {
	volatile int temp;

	// Wait until bus not busy
	while (I2C1->SR2 & SR2_BUSY);

	// Generate START
	I2C1->CR1 |= CR1_START;

	// Wait for the Start Flag
	while ((I2C1->SR1 & SR1_STARTF) == 0);

	// Transmit Slave Address + Write
	I2C1->DR = (slave_addr << 1) | (WR_BIT << 0); // 7 bits

	// Wait for the Address Flag
	while ((I2C1->SR1 & SR1_ADDRF) == 0);

	// Clear the Address Flag
	temp = I2C1->SR2;

	// Wait until transmitter is empty
	while ((I2C1->SR1 & SR1_TXEF) == 0);

	// Send Master Address
	I2C1->DR = memory_addr;

	// Wait until transmitter is empty
	while ((I2C1->SR1 & SR1_TXEF) == 0);

	// Generate Restart
	I2C1->CR1 |= CR1_START;

	// Wait for the Restart Flag
	while ((I2C1->SR1 & SR1_STARTF) == 0);

	// Transmit Slave Address + Read
	I2C1->DR = (slave_addr << 1) | (RD_BIT << 0); // 7 bits

	// Wait for the Address Flag
	while ((I2C1->SR1 & SR1_ADDRF) == 0);

	// Clear the Address Flag
	temp = I2C1->SR2;

	// Enable the ACK
	I2C1->CR1 |= (CR1_ACKF);

	while (n > 0U) {
		if (n == 1U) {
			// Disable the ACK
			I2C1->CR1 &= ~(CR1_ACKF);

			// Generate the Stop Condition
			I2C1->CR1 |= CR1_STOPF;
		}
		// Wait until DR is not empty
		while ((I2C1->SR1 & SR1_RXNE) == 0);

		*data = I2C1->DR;
		if (*data == '\0') {
			n == 2;
		}
		data++;
		n--;
	}
	(void) temp;
}

void i2c1_byte_burst_write(char slave_addr, char memory_addr, int n, char *data) {
	volatile int temp;

	// Wait until bus not busy
	while (I2C1->SR2 & SR2_BUSY);

	// Generate START
	I2C1->CR1 |= CR1_START;

	// Wait for the Start Flag
	while ((I2C1->SR1 & SR1_STARTF) == 0);

	// Transmit Slave Address + Write
	I2C1->DR = (slave_addr << 1) | (WR_BIT << 0); // 7 bits

	// Wait for the Address Flag
	while ((I2C1->SR1 & SR1_ADDRF) == 0);

	// Clear the Address Flag
	temp = I2C1->SR2;

	// Wait until Data Register is empty
	while ((I2C1->SR1 & SR1_TXEF) == 0);

	// Send Master Address
	I2C1->DR = memory_addr;

	for (int i = 0; i < n; ++i) {
		// Wait until Data Register is empty
		while ((I2C1->SR1 & SR1_TXEF) == 0);

		I2C1->DR = *data++;
	}

	// Wait until Byte Transfer is finished
	while ((I2C1->SR1 & SR1_BTF) == 0);

	// Generate the Stop Condition
	I2C1->CR1 |= CR1_STOPF;

	(void) temp;
}
