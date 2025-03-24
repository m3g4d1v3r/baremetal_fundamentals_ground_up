/*
 * usart.h
 *
 *  Created on: Mar 21, 2025
 *      Author: marlon
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f1xx.h"
#include <stdlib.h>

void usart1_init();
unsigned char usart1_read();
void usart1_write(unsigned char chr);
void usart1_write_str(unsigned char str[], size_t len);

#endif /* USART_H_ */
