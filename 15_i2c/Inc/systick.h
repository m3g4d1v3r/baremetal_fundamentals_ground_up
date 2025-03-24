/*
 * systick.h
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "stm32f1xx.h"

#define SYST_LOAD_VALUE  (8000 - 1)
#define SYST_ENABLE_Pos	 (0)
#define SYST_CLKSRC_Pos  (2)
#define SYST_CNTFLG_Pos  (16)
#define SYST_ENABLE	     (0b1 << SYST_ENABLE_Pos)
#define SYST_PRCCLK      (0b1 << SYST_CLKSRC_Pos)
#define SYST_CNTFLG      (0b1 << SYST_CNTFLG_Pos)

void systick_delay_ms(int n);

#endif /* SYSTICK_H_ */
