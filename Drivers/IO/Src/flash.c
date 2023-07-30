//
// Created by ShawnGao on 2023/7/30.
//

#include "flash.h"

unsigned char FLASH_WR(uint32_t start, uint32_t end, uint32_t dat) {
    uint32_t num_of_page;
    uint32_t SECTORErr = 0;

    FLASH_EraseInitTypeDef status;
    HAL_FLASH_Unlock();
    num_of_page = (WR_END_ADDR - WR_START_ADDR) / FLASH_PAGE_SIZE;
    status.TypeErase = FLASH_TYPEERASE_PAGES;
    status.NbPages = num_of_page;
    status.PageAddress = WR_START_ADDR;

    if (HAL_FLASHEx_Erase(&status, &SECTORErr) != HAL_OK) {
        // Error handler
        return 1;
    }
    while (start < WR_END_ADDR && end < WR_END_ADDR && start <= end) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, start, dat) == HAL_OK) {
            start += 4;
        } else {
            // Error handler
            return 1;
        }
    }
    HAL_FLASH_Lock();
    return 0;
}

uint32_t FLASH_RD(uint32_t addr) {
    if (addr < WR_END_ADDR) {
        return (*(__IO uint32_t *) addr);
    }
    return 0xff;
}
