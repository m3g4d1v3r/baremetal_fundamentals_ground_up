/*
 * i2c.h
 *
 *  Created on: Mar 23, 2025
 *      Author: marlon
 */

#ifndef I2C_H_
#define I2C_H_

#include <stm32f1xx.h>

void i2c1_init(void);
void i2c1_byte_read(char slave_addr, char master_addr, char *data);
void i2c1_byte_burst_read(char slave_addr, char memory_addr, int n, char *data);
void i2c1_byte_burst_write(char slave_addr, char memory_addr, int n, char *data);

#endif /* I2C_H_ */
