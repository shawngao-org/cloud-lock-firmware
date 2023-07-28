//
// Created by ShawnGao on 2023/7/28.
//

#include "delay.h"

static unsigned long fac_us = 0;
static unsigned long fac_ms = 0;
static volatile unsigned long count_ms = 0;

void delay_init() {
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
#if USE_DELAY_INT
    fac_us = SystemCoreClock / 8000000;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = TIMING_MS * 1000 * fac_us;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    fac_ms = (unsigned int) fac_us * 1000;
#endif
}

void delay_us(uint32_t nus) {
    unsigned long ticks;
    unsigned long told, tnow, tcnt = 0;
    unsigned long reload = SysTick->LOAD;

    ticks = nus * fac_us;
    told = SysTick->VAL;
    while (1) {
        tnow = SysTick->VAL;
        if (tnow != told) {
            if (tnow < told) {
                tcnt += told - tnow;
            } else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) {
                break;
            }
        }
    }
}

void delay_ms(uint32_t nms) {
    unsigned long ticks;
    unsigned long told, tnow, tcnt = 0;
    unsigned long reload = SysTick->LOAD;
    unsigned char ms_500 = nms / 500;
    while (ms_500--) {
        ticks = fac_ms * 500;
        tcnt = 0;
        told = SysTick->VAL;
        while (1) {
            tnow = SysTick->VAL;
            if (tnow != told) {
                if (tnow < told) tcnt += told - tnow;
                else tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks) break;
            }
        }
    }
    if (nms % 500) {
        ticks = nms * fac_ms;
        tcnt = 0;
        told = SysTick->VAL;
        while (1) {
            tnow = SysTick->VAL;
            if (tnow != told) {
                if (tnow < told) tcnt += told - tnow;
                else tcnt += reload - tnow + told;
                told = tnow;
                if (tcnt >= ticks) break;
            }
        }
    }
}
