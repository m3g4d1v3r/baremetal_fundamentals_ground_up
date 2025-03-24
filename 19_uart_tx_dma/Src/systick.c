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

	// 3rd: Enable SysTick and select internal clock source
    SysTick->CTRL |= SYST_ENABLE;
}

void systick_1hz_interrupt(void) {
	SysTick->VAL = 0;
    SysTick->LOAD = 1600000 - 1;
	SysTick->CTRL |= (SYST_ENABLE | SYST_TICKINT);
}
