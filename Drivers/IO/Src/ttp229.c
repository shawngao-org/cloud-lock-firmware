//
// Created by ShawnGao on 2023/7/28.
//

#include "ttp229.h"

void TTP_INIT() {
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.Pin = GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    SDA_OUT();
    TTP_SDO = 0;
    TTP_SCL = 0;
    TTP_RD();
}

uint16_t TTP_RD() {
    uint8_t i = 0;
    uint16_t real_value = 0;
    SDA_OUT();
    TTP_SDO = 1;
    delay_us(100);
    TTP_SDO = 0;
    delay_us(20);
    SDA_IN();
    for (i = 0; i < 16; i++) {
        TTP_SCL = 1;
        delay_us(100);
        TTP_SCL = 0;
        delay_us(1);
        if (!TTP_SDI) {
            real_value |= (1 << i);
        }
    }
    delay_ms(2);
    return real_value;
}

uint16_t Previous = 0;
uint16_t Current = 0;

uint8_t TTP_SCAN() {
    uint8_t key;
    Current = TTP_RD();
    if ((Current & 0x0001) && !(Previous & 0x0001)) {
        key = '1';
        delay_ms(10);
    } else if ((Current & 0x0002) && !(Previous & 0x0002)) {
        key = '2';
        delay_ms(10);
    } else if ((Current & 0x0004) && !(Previous & 0x0004)) {
        key = '3';
        delay_ms(10);
    } else if ((Current & 0x0008) && !(Previous & 0x0008)) {
        key = 'A';
        delay_ms(10);
    } else if ((Current & 0x0010) && !(Previous & 0x0010)) {
        key = '4';
        delay_ms(10);
    } else if ((Current & 0x0020) && !(Previous & 0x0020)) {
        key = '5';
        delay_ms(10);
    } else if ((Current & 0x0040) && !(Previous & 0x0040)) {
        key = '6';
        delay_ms(10);
    } else if ((Current & 0x0080) && !(Previous & 0x0080)) {
        key = 'B';
        delay_ms(10);
    } else if ((Current & 0x0100) && !(Previous & 0x0100)) {
        key = '7';
        delay_ms(10);
    } else if ((Current & 0x0200) && !(Previous & 0x0200)) {
        key = '8';
        delay_ms(10);
    } else if ((Current & 0x0400) && !(Previous & 0x0400)) {
        key = '9';
        delay_ms(10);
    } else if ((Current & 0x0800) && !(Previous & 0x0800)) {
        key = 'C';
        delay_ms(10);
    } else if ((Current & 0x1000) && !(Previous & 0x1000)) {
        key = '*';
        delay_ms(10);
    } else if ((Current & 0x2000) && !(Previous & 0x2000)) {
        key = '0';
        delay_ms(10);
    } else if ((Current & 0x4000) && !(Previous & 0x4000)) {
        key = '#';
        delay_ms(10);
    } else {
        key = 'D';
        delay_ms(10);
    }
    Previous = Current;
    return key;
}
