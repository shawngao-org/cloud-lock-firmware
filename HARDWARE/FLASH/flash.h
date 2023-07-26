#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x.h"

#define FLASH_KEY1         ((uint32_t)0x45670123)
#define FLASH_KEY2         ((uint32_t)0xCDEF89AB)
#define FLASH_PAGE_SIZE    ((uint16_t)0x800)

#define WRITE_START_ADDR  ((uint32_t)0x08008000)
#define WRITE_END_ADDR    ((uint32_t)0x0800C000)

void flash_wr(uint32_t wr_addr, uint32_t wr_data);
uint32_t flash_rd(uint32_t rd_addr);

#endif
