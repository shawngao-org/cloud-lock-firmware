//
// Created by ShawnGao on 2023/7/28.
//

#ifndef CLOUD_LOCK_OLED_H
#define CLOUD_LOCK_OLED_H

#include "stdlib.h"
#include "stm32f1xx_hal.h"
#include "delay.h"

#define WR_CMD 0x00
#define WR_DATA 0x40
#define SIZE 8
#define MAX_COLUMN 128

// SCL Pin
#define OLED_SCLK_CLR() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
#define OLED_SCLK_SET() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)

// SDA Pin
#define OLED_SDIN_CLR() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)
#define OLED_SDIN_SET() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)

/**
 * Enable OLED display
 */
void OLED_DISPLAY_ENABLE();
/**
 * Disable OLED display
 */
void OLED_DISPLAY_DISABLE();

/**
 * OLED initialization
 */
void OLED_INIT();
/**
 * Clear OLED
 */
void OLED_CLR();

/**
 * Display char
 * @param x X POS
 * @param y Y POS
 * @param c Char
 * @param size Char size
 */
void OLED_DISPLAY_CHAR(unsigned char x, unsigned char y, unsigned char c, unsigned char size);
/**
 * Display string (char array)
 * @param x X POS
 * @param y Y POS
 * @param arr String or char array
 * @param size Char size
 */
void OLED_DISPLAY_STR(unsigned char x, unsigned char y, unsigned char* arr, unsigned char size);
void OLED_DISPLAY_HEX(unsigned char x, unsigned char y, unsigned char hex);
/**
 * Display Chinese
 * @param x X POS
 * @param y Y POS
 * @param index Index of Chinese font array
 */
void OLED_DISPLAY_ZH_CN(unsigned char x, unsigned char y, unsigned char index);

/**
 * Set display position
 * @param x X
 * @param y Y
 */
void OLED_SET_POS(unsigned char x, unsigned char y);

void OLED_I2C_ENABLE();
void OLED_I2C_DISABLE();

/**
 * Write command or data
 * @param dat Data or cmd
 * @param mode Mode: 0x00 -> CMD, 0x40 -> Data
 */
void OLED_WR_I2C_CMD_DATA(unsigned char dat, unsigned char mode);
void OLED_WR_I2C_BYTE(unsigned char dat);

void OLED_I2C_WAIT_ACK();

#endif //CLOUD_LOCK_OLED_H
