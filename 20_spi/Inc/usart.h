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

#define CR1_RXNEIE	    (0b1 << 5)
#define CR1_TCIE        (0b1 << 6)
#define CR1_TXEIE       (0b1 << 7)
#define RCC_AHBENR_DMA1 (0b1 << 0)
#define DMA_CCRX_EN		(0b1 << 0)
#define DMA_CCRX_DIR	(0b1 << 4)
#define DMA_CCRX_MINC	(0b1 << 7)
#define DMA_ISR_TCIF    (0b1 << 13)
#define DMA_IFCR_CTCIF  (0b1 << 13)
// #define USART_CR3_DMAR  (0b1 << 6)
// #define USART_CR3_DMAT  (0b1 << 7)

void usart1_init();
unsigned char usart1_read();
void usart1_write(unsigned char chr);
void usart1_write_str(unsigned char str[], size_t len);
void usart1_init_interrupt(void);
void dma_channel4_init(uint32_t src, uint32_t dst, uint32_t len);

#endif /* USART_H_ */
