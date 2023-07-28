//
// Created by ShawnGao on 2023/7/28.
//

#ifndef CLOUD_LOCK_DELAY_H
#define CLOUD_LOCK_DELAY_H

#include "stm32f1xx_hal.h"
#include <stdint-gcc.h>

#define TIMING_MS 1
#define USE_DELAY_INT 1

void delay_init();
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);

#endif //CLOUD_LOCK_DELAY_H
