//
// Created by ShawnGao on 2023/7/30.
//

#ifndef CLOUD_LOCK_FLASH_H
#define CLOUD_LOCK_FLASH_H

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#define WR_START_ADDR ((uint32_t) 0x08008000)
#define WR_END_ADDR ((uint32_t) 0x0800c000)

/**
 * Write data to flash.
 * @param start Start address
 * @param end End address
 * @param dat Data
 * @return 0 means normal, non-zero means abnormal.
 */
unsigned char FLASH_WR(uint32_t start, uint32_t end, uint32_t dat);

/**
 * Read data on flash.
 * @param addr Read address
 * @return 0xFF means err, else normal.
 */
uint32_t FLASH_RD(uint32_t addr);

#endif //CLOUD_LOCK_FLASH_H
