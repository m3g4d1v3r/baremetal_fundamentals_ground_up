/*
 * usart.h
 *
 *  Created on: Mar 21, 2025
 *      Author: marlon
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f1xx.h"

void usart1_init();
void usart1_write(unsigned char chr);

#endif /* USART_H_ */
