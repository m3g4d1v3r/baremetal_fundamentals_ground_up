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

#define MODE0_OFFSET               (0)
#define CNF0_OFFSET				   (2)
#define MODE1_OFFSET               (4)
#define CNF1_OFFSET				   (6)
#define MODE3_OFFSET               (12)
#define CNF3_OFFSET				   (14)

#define MODE4_OFFSET               (16)
#define CNF4_OFFSET                (18)
#define MODE5_OFFSET               (20)
#define CNF5_OFFSET                (22)
#define MODE6_OFFSET               (24)
#define CNF6_OFFSET                (26)
#define MODE7_OFFSET               (28)
#define CNF7_OFFSET                (30)

#define MODE8_OFFSET               (0)
#define CNF8_OFFSET                (2)
#define MODE9_OFFSET               (4)
#define CNF9_OFFSET                (6)
#define MODE10_OFFSET              (8)
#define CNF10_OFFSET               (10)

#define IOPA_EN                    (0b1 << 2)
#define IOPB_EN    				   (0b1 << 3)
#define IOPC_EN    				   (0b1 << 4)
#define AFIO_EN	                   (0b1 << 0)

#endif /* GPIO_H_ */
