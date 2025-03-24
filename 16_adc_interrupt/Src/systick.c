/*
 * systick.c
 *
 *  Created on: Mar 22, 2025
 *      Author: marlon
 */
#include <systick.h>

void systick_delay_ms(int n) {
	// 1st: Reload with number of clocks per millisecond
	SysTick->LOAD = SYST_LOAD_VALUE;

	// 2nd: Clear systick current value register
	SysTick->VAL = 0;

    uint32_t calib_value = SysTick->CALIB & 0x00FFFFFF;
    if (calib_value != 0) {
        SysTick->LOAD = calib_value;
    } else {
        SysTick->LOAD = SYST_LOAD_VALUE;
    }

	// 3rd: Enable systick and select internal clock source
	SysTick->CTRL |= SYST_ENABLE;
	SysTick->CTRL |= SYST_PRCCLK;

	for (int i = 0; i < n; ++i)
		while ((SysTick->CTRL & SYST_CNTFLG) == 0);
	SysTick->CTRL = 0;
}
