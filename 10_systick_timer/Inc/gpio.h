/*
 * gpio.h
 *
 *  Created on: Mar 21, 2025
 *      Author: marlon
 */

#ifndef GPIO_H_
#define GPIO_H_

#define CNF_INPUT_ANALOG           (0b00)
#define CNF_INPUT_FLOAT            (0b01)
#define CNF_INPUT_PU_PD            (0b10)

#define CNF_OUTPUT_GNRL_PUSH_PULL  (0b00)
#define CNF_OUTPUT_GNRL_OPEN_DRAIN (0b01)
#define CNF_OUTPUT_ALTR_PUSH_PULL  (0b10)
#define CNF_OUTPUT_ALTR_OPEN_DRAIN (0b11)

#define MODE_INPUT                 (0b00)
#define MODE_OUTPUT_10MHZ          (0b01)
#define MODE_OUTPUT_2MHZ           (0b10)
#define MODE_OUTPUT_50MHZ          (0b11)

#define MODE1_OFFSET               (4)
#define CNF1_OFFSET				   (6)
#define MODE9_OFFSET               (4)
#define CNF9_OFFSET                (6)
#define MODE10_OFFSET              (8)
#define CNF10_OFFSET               (10)

#endif /* GPIO_H_ */
