//
// Created by 高新 on 2023/8/28.
//

#include "rfid.h"

void WR_RFID_REG(unsigned char addr, unsigned char dat) {
    unsigned char addr_bits = (addr << 1) & 0x7e;
    RFID_SDA_CLR();
    HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 500);
    HAL_SPI_Transmit(&hspi1, &dat, 1, 500);
    RFID_SDA_SET();
}

unsigned char RD_RFID_REG(unsigned char addr) {
    unsigned char rx_bits;
    unsigned char addr_bits = ((addr << 1) & 0x7e) | 0x80;
    RFID_SDA_CLR();
    HAL_SPI_Transmit(&hspi1, &addr_bits, 1, 500);
    HAL_SPI_Receive(&hspi1, &rx_bits, 1, 500);
    RFID_SDA_SET();
    return rx_bits;
}

void RFID_SET_BIT_MSK(unsigned char reg, unsigned char msk) {
    unsigned char tmp;
    tmp = RD_RFID_REG(reg);
    WR_RFID_REG(reg, tmp | msk);
}

void RFID_CLR_BIT_MSK(unsigned char reg, unsigned char msk) {
    unsigned char tmp;
    tmp = RD_RFID_REG(reg);
    WR_RFID_REG(reg, tmp & (~msk));
}

void RFID_ANTENNA_ON() {
    RFID_SET_BIT_MSK(TxControlReg, 0x03);
}

void RFID_ANTENNA_OFF() {
    RFID_CLR_BIT_MSK(TxControlReg, 0x03);
}

void RFID_RST() {
    WR_RFID_REG(CommandReg, PCD_RST);
}

void RFID_INIT_GPIO() {
    GPIO_InitTypeDef  gpioInitTypeDef;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpioInitTypeDef.Pin = RFID_SDA_PIN_N;
    gpioInitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RFID_SDA_PIN_G, &gpioInitTypeDef);

    gpioInitTypeDef.Pin = RFID_SCK_PIN_N;
    gpioInitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RFID_SCK_PIN_G, &gpioInitTypeDef);

    gpioInitTypeDef.Pin = RFID_MISO_PIN_N;
    gpioInitTypeDef.Mode = GPIO_MODE_INPUT;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RFID_MISO_PIN_G, &gpioInitTypeDef);

    gpioInitTypeDef.Pin = RFID_MOSI_PIN_N;
    gpioInitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RFID_MOSI_PIN_G, &gpioInitTypeDef);

    gpioInitTypeDef.Pin = RFID_RST_PIN_N;
    gpioInitTypeDef.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RFID_RST_PIN_G, &gpioInitTypeDef);
}

void RFID_INIT() {
    RFID_RST_SET();
    RFID_RST_SET();
    RFID_RST();
    WR_RFID_REG(Status2Reg, 0x08);
    WR_RFID_REG(TModeReg, 0x8d);
    WR_RFID_REG(TPrescalerReg, 0x3e);
    WR_RFID_REG(TReloadRegL, 0x1e);
    WR_RFID_REG(TReloadRegH, 0x00);
    WR_RFID_REG(TxAutoReg, 0x40);
    WR_RFID_REG(RxSelReg, 0x86);
    WR_RFID_REG(RFCfgReg, 0x7f);
    WR_RFID_REG(ModeReg, 0x3d);
    HAL_Delay(1000);
    RFID_ANTENNA_ON();
}

unsigned char RFID_REQ(unsigned char req_mode, unsigned char *tag_type) {
    unsigned char status;
    unsigned int back_bits;
    WR_RFID_REG(BitFramingReg, 0x07);
    tag_type[0] = req_mode;
    status = RFID_2_CARD(PCD_TRX, tag_type,
                         1, tag_type,
                         &back_bits);
    return status != MI_OK || back_bits != 0x10 ? MI_ERR : status;
}

unsigned char RFID_2_CARD(unsigned char cmd, unsigned char *send_data,
                          unsigned char send_len, unsigned char *back_data,
                          unsigned int *back_len) {
    unsigned char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitIRq = 0x00;
    unsigned char last_bits;
    unsigned char n;
    unsigned int i;
    switch (cmd) {
        case PCD_AUTH: {
            irqEn = 0x12;
            waitIRq = 0x10;
            break;
        }
        case PCD_TRX: {
            irqEn = 0x77;
            waitIRq = 0x30;
            break;
        }
        default:
            break;
    }
    WR_RFID_REG(CommIEnReg, irqEn | 0x80);
    RFID_CLR_BIT_MSK(CommIrqReg, 0x80);
    RFID_SET_BIT_MSK(FIFOLevelReg, 0x80);
    WR_RFID_REG(CommandReg, PCD_IDLE);
    for (i = 0; i < send_len; ++i) {
        WR_RFID_REG(FIFODataReg, send_data[i]);
    }
    WR_RFID_REG(CommandReg, cmd);
    if (cmd == PCD_TRX) {
        RFID_SET_BIT_MSK(BitFramingReg, 0x80);
    }
    i = 2000;
    do {
        n = RD_RFID_REG(CommIrqReg);
        i--;
    } while (i != 0 && !(n & 0x01) && !(n & waitIRq));
    RFID_CLR_BIT_MSK(BitFramingReg, 0x80);
    if (i != 0) {
        if (!(RD_RFID_REG(ErrorReg) & 0x1b)) {
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOT_AGREE;
            }
            if (cmd == PCD_TRX) {
                n = RD_RFID_REG(FIFOLevelReg);
                last_bits = RD_RFID_REG(ControlReg) & 0x07;
                *back_len = last_bits ? (n - 1) * 8 + last_bits : n * 8;
                n = n == 0 ? 1 : n > MAX_LEN ? MAX_LEN : n;
                for (i = 0; i < n; ++i) {
                    back_data[i] = RD_RFID_REG(FIFODataReg);
                }
            }
        } else {
            status = MI_ERR;
        }
    } else {
        //
    }
    return status;
}

unsigned char RFID_ANTI_COLL(unsigned char *ser_num) {
    unsigned char status;
    unsigned char i;
    unsigned char ser_num_chk = 0;
    unsigned int len;
    WR_RFID_REG(BitFramingReg, 0x00);
    ser_num[0] = PICC_ANTICOLL_1;
    ser_num[1] = 0x20;
    status = RFID_2_CARD(PCD_TRX, ser_num, 2,
                         ser_num, &len);
    if (status == MI_OK) {
        for (i = 0; i < 4; ++i) {
            ser_num_chk ^= ser_num[i];
        }
        if (ser_num_chk != ser_num[i]) {
            status = MI_ERR;
        }
    }
    return status;
}

void RFID_CAL_CRC(const unsigned char *in_data, unsigned char len,
                  unsigned char *out_data) {
    unsigned char i, n;
    RFID_CLR_BIT_MSK(DivIrqReg, 0x04);
    RFID_SET_BIT_MSK(FIFOLevelReg, 0x80);
    for (i = 0; i < len; ++i) {
        WR_RFID_REG(FIFODataReg, *(in_data + i));
    }
    WR_RFID_REG(CommandReg, PCD_CRC);
    i = 0xff;
    do {
        n = RD_RFID_REG(DivIrqReg);
        i--;
    } while (i != 0 && !(n & 0x04));
    out_data[0] = RD_RFID_REG(CRCResultRegL);
    out_data[1] = RD_RFID_REG(CRCResultRegM);
}

unsigned char RFID_SELECT_TAG(const unsigned char *ser_num) {
    unsigned char i;
    unsigned char status;
    unsigned char size;
    unsigned int recv_bits;
    unsigned char buffer[9];
    buffer[0] = PICC_SELECT_CARD;
    buffer[1] = 0x70;
    for (i = 0; i < 5; ++i) {
        buffer[i + 2] = *(ser_num + i);
    }
    RFID_CAL_CRC(buffer, 7, &buffer[7]);
    status = RFID_2_CARD(PCD_TRX, buffer,
                         9, buffer,
                         &recv_bits);
    size = status == MI_OK && recv_bits == 0x18 ? buffer[0] : 0;
    return size;
}

unsigned char RFID_AUTH(unsigned char auth_mode, unsigned char block_addr,
                        const unsigned char *secret_key,
                        const unsigned char *ser_num) {
    unsigned char status;
    unsigned int recv_bits;
    unsigned char i;
    unsigned char buffer[12];
    buffer[0] = auth_mode;
    buffer[1] = block_addr;
    for (i = 0; i < 6; ++i) {
        buffer[i + 2] = *(secret_key + i);
    }
    for (i = 0; i < 4; ++i) {
        buffer[i + 8] = *(ser_num + i);
    }
    status = RFID_2_CARD(PCD_AUTH, buffer,
                         12, buffer,
                         &recv_bits);
    return status != MI_OK || !(RD_RFID_REG(Status2Reg) & 0x08) ? MI_ERR : status;
}

unsigned char RFID_RD_CARD(unsigned char block_addr, unsigned char *dat) {
    unsigned char status;
    unsigned int len;
    dat[0] = PICC_READ;
    dat[1] = block_addr;
    RFID_CAL_CRC(dat, 2, &dat[2]);
    status = RFID_2_CARD(PCD_TRX, dat,
                         4, dat,
                         &len);
    return status != MI_OK || len != 0x90 ? MI_ERR : status;
}

unsigned char RFID_WR_CARD(unsigned char block_addr, const unsigned char *dat) {
    unsigned status;
    unsigned int recv_bits;
    unsigned char i;
    unsigned char buffer[18];
    buffer[0] = PICC_WRITE;
    buffer[1] = block_addr;
    RFID_CAL_CRC(buffer, 2, &buffer[2]);
    status = RFID_2_CARD(PCD_TRX, buffer,
                         4, buffer,
                         &recv_bits);
    if (status != MI_OK) {
        status = MI_ERR;
    }
    if (status == MI_OK) {
        for (i = 0; i < 16; ++i) {
            buffer[i] = *(dat + i);
        }
        RFID_CAL_CRC(buffer, 16, &buffer[16]);
        status = RFID_2_CARD(PCD_TRX, buffer,
                             18, buffer,
                             &recv_bits);
        if (status != MI_OK) {
            status = MI_ERR;
        }
    }
    return status;
}

unsigned char RFID_HALT() {
    unsigned char status;
    unsigned int len;
    unsigned char buffer[4];
    buffer[0] = PICC_HALT;
    buffer[1] = 0;
    RFID_CAL_CRC(buffer, 2, &buffer[2]);
    status = RFID_2_CARD(PCD_TRX, buffer,
                         4, buffer,
                         &len);
    if (status != MI_OK) {
        status = MI_ERR;
    }
    return status;
}

void RFID_STOP_CRYPT() {
    RFID_CLR_BIT_MSK(Status2Reg, 0x08);
}

void RFID_WAIT_CARD() {
    unsigned char status;
    unsigned char tag_type[2];
    while (1) {
        status = RFID_REQ(PICC_REQIDL, tag_type);
        if (status) {
            status = RFID_REQ(PICC_REQIDL, tag_type);
            if (status) {
                status = RFID_REQ(PICC_REQIDL, tag_type);
                if (status) {
                    return;
                }
            }
        }
        delay_ms(1000);
    }
}
