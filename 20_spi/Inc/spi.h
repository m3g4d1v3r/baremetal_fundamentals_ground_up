/*
 * spi.h
 *
 *  Created on: Mar 24, 2025
 *      Author: marlon
 */

#ifndef SPI_H_
#define SPI_H_

#include <stm32f1xx.h>

void spi_gpio_init(void);
void spi_config(void);
void spi_transmit(uint8_t *data, uint32_t size);
void spi_receive(uint8_t *data, uint32_t size);
void cs_enable(void);
void cs_disable(void);

#endif /* SPI_H_ */
