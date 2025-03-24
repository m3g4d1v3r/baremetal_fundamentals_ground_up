#include <led.h>
#include <usart.h>
#include <stdbool.h>
#include <string.h>

#define DELAY_CONST 400

void delay(uint64_t n) {
	for (uint64_t idx = 0; idx < (n * n); idx++);
}

uint32_t GetSystemClock(void) {
    uint32_t sysClk = 0;
    uint32_t pllMul = 0;
    uint8_t clkSrc = (RCC->CFGR & RCC_CFGR_SWS) >> 2;

    switch(clkSrc) {
        case 0: // HSI
            sysClk = 8000000; // 8 MHz
            break;
        case 1: // HSE
            sysClk = 8000000; // Typical for Bluepill
            break;
        case 2: // PLL
            pllMul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
            if (RCC->CFGR & RCC_CFGR_PLLSRC) {
                // HSE as PLL source
                sysClk = 8000000 * pllMul;
            } else {
                // HSI/2 as PLL source
                sysClk = 4000000 * pllMul;
            }
            break;
    }

    return sysClk;
}

int main() {
	GetSystemClock();
	led_init();
	usart1_init();
	unsigned char chr;
	int delay_acc = 50;

	while (true) {
		chr = usart1_read();
		if (chr == '1') {
			delay_acc += 100;
		}
		led_off();
		delay(DELAY_CONST + delay_acc);
		led_on();
		usart1_write(chr);
	}
}
