/*
 * adc.h
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f1xx.h"
#include <gpio.h>
#include <stdint.h>

#define ADC1EN         (0b1 << 9)
#define IOPAEN         (0b1 << 2)
#define ADC1_CH1       (0b1 << 0)
#define ADC1_SEQ_LEN_1 (0b0)
#define ADC1_ADON      (0b1 << 0)
#define ADC_SWSTART    (0b1 << 22)
#define ADC_EOC		   (0b1 << 1)

void adc1_init();
void adc1_start_conversion();
uint32_t adc1_read(void);

#endif /* ADC_H_ */
