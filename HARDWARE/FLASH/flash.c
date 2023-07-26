#include "flash.h"

/**
 * Write data to flash
 * @param wr_addr Write address
 * @param wr_data Write data
 */
void flash_wr(uint32_t wr_addr, uint32_t wr_data) {
	// Number of pages erased
	uint32_t EraseCounter = 0x00;
	// Number of pages written
	uint32_t NbrOfPage = 0x00;
	// Flash status
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	// Unlock flash, enable access
	FLASH_Unlock();
	// Calculate the number of pages to erase
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	// Clear all flag bit
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	// Erase by page
	for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++) {
		FLASHStatus = FLASH_ErasePage(WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
	}
	// Write data in units of word
	while ((wr_addr < WRITE_END_ADDR) && (FLASHStatus == FLASH_COMPLETE)) {
		FLASHStatus = FLASH_ProgramWord(wr_addr, wr_data);
		wr_addr += 4;
	}
	// Lock flash, disable access
	FLASH_Lock();
}

uint32_t flash_rd(uint32_t rd_addr) {
	if (rd_addr < WRITE_END_ADDR) {
		return (*(__IO uint32_t*) rd_addr);
	}
	return 0xFF;
}
