//
// Created by 高新 on 2023/8/28.
//

#ifndef CLOUD_LOCK_RFID_H
#define CLOUD_LOCK_RFID_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include "sys.h"

/**
 * Pin
 */
#define RFID_SDA_PIN_N GPIO_PIN_4
#define RFID_SDA_PIN_G GPIOA
#define RFID_SCK_PIN_N GPIO_PIN_5
#define RFID_SCK_PIN_G GPIOA
#define RFID_MOSI_PIN_N GPIO_PIN_7
#define RFID_MOSI_PIN_G GPIOA
#define RFID_MISO_PIN_N GPIO_PIN_6
#define RFID_MISO_PIN_G GPIOA
#define RFID_RST_PIN_N GPIO_PIN_0
#define RFID_RST_PIN_G GPIOB

#define RFID_SDA_CLR()  HAL_GPIO_WritePin(RFID_SDA_PIN_G, RFID_SDA_PIN_N, GPIO_PIN_RESET)
#define RFID_SDA_SET()  HAL_GPIO_WritePin(RFID_SDA_PIN_G, RFID_SDA_PIN_N, GPIO_PIN_SET)

#define RFID_SCK_CLR()  HAL_GPIO_WritePin(RFID_SCK_PIN_G, RFID_SCK_PIN_N, GPIO_PIN_RESET)
#define RFID_SCK_SET()  HAL_GPIO_WritePin(RFID_SCK_PIN_G, RFID_SCK_PIN_N, GPIO_PIN_SET)

#define RFID_MOSI_CLR() HAL_GPIO_WritePin(RFID_MOSI_PIN_G, RFID_MOSI_PIN_N, GPIO_PIN_RESET)
#define RFID_MOSI_SET() HAL_GPIO_WritePin(RFID_MOSI_PIN_G, RFID_MOSI_PIN_N, GPIO_PIN_SET)

#define RFID_MISO_CLR() HAL_GPIO_WritePin(RFID_MISO_PIN_G, RFID_MISO_PIN_N, GPIO_PIN_RESET)
#define RFID_MISO_SET() HAL_GPIO_WritePin(RFID_MISO_PIN_G, RFID_MISO_PIN_N, GPIO_PIN_SET)

#define RFID_RST_CLR()  HAL_GPIO_WritePin(RFID_RST_PIN_G, RFID_RST_PIN_N, GPIO_PIN_RESET)
#define RFID_RST_SET()  HAL_GPIO_WritePin(RFID_RST_PIN_G, RFID_RST_PIN_N, GPIO_PIN_SET)

/**
 * Command
 */
#define PCD_IDLE 0x00 // Cancel current command; No action
#define PCD_AUTH 0x0e // Auth Key
#define PCD_RX   0x08 // Receive data
#define PCD_TX   0x04 // Send data
#define PCD_TRX  0x0c // Send and receive data
#define PCD_RST  0x0f // Reset
#define PCD_CRC  0x03 // CRC calculation

/**
 * Card command
 */
#define PICC_REQIDL      0x26 // Did not go to sleep in the card hunting antenna area
#define PICC_REQALL      0x52 // All cards in the card antenna
#define PICC_ANTICOLL_1  0x93 // Anti-collision
#define PICC_SELECT_CARD 0x93 // Select Card
#define PICC_ANTICOLL_2  0x95 // Anti-collision
#define PICC_AUTH_A_KEY  0x60 // Auth Key A
#define PICC_AUTH_B_KEY  0x61 // Auth Key B
#define PICC_READ        0x30 // Read Block
#define PICC_WRITE       0xa0 // Write Block
#define PICC_DECREMENT   0xc0 // Chargeback
#define PICC_INCREMENT   0xc1 // Recharge
#define PICC_RESTORE     0xc2 // Load data into buffer
#define PICC_TRANSFER    0xb0 // Save buffer data
#define PICC_HALT        0x50 // HALT

/**
 * Length
 */
#define MAX_LEN 16

/**
 * Status code
 */
#define MI_OK        0
#define MI_NOT_AGREE 1
#define MI_ERR       2

/**
 * Register definition
 */
// Command and Status --- P0
#define Reserved00    0x00
#define CommandReg    0x01
#define CommIEnReg    0x02
#define DivIEnReg     0x03
#define CommIrqReg    0x04
#define DivIrqReg     0x05
#define ErrorReg      0x06
#define Status1Reg    0x07
#define Status2Reg    0x08
#define FIFODataReg   0x09
#define FIFOLevelReg  0x0a
#define WaterLevelReg 0x0b
#define ControlReg    0x0c
#define BitFramingReg 0x0d
#define CollReg       0x0e
#define Reserved01    0x0f
// Command --- P1
#define Reserved10     0x10
#define ModeReg        0x11
#define TxModeReg      0x12
#define RxModeReg      0x13
#define TxControlReg   0x14
#define TxAutoReg      0x15
#define TxSelReg       0x16
#define RxSelReg       0x17
#define RxThresholdReg 0x18
#define DemodReg       0x19
#define Reserved11     0x1A
#define Reserved12     0x1B
#define MifareReg      0x1C
#define Reserved13     0x1D
#define Reserved14     0x1E
#define SerialSpeedReg 0x1F
// CFG --- P2
#define Reserved20        0x20
#define CRCResultRegM     0x21
#define CRCResultRegL     0x22
#define Reserved21        0x23
#define ModWidthReg       0x24
#define Reserved22        0x25
#define RFCfgReg          0x26
#define GsNReg            0x27
#define CWGsPReg          0x28
#define ModGsPReg         0x29
#define TModeReg          0x2A
#define TPrescalerReg     0x2B
#define TReloadRegH       0x2C
#define TReloadRegL       0x2D
#define TCounterValueRegH 0x2E
#define TCounterValueRegL 0x2F
// TestRegister --- P3
#define Reserved30      0x30
#define TestSel1Reg     0x31
#define TestSel2Reg     0x32
#define TestPinEnReg    0x33
#define TestPinValueReg 0x34
#define TestBusReg      0x35
#define AutoTestReg     0x36
#define VersionReg      0x37
#define AnalogTestReg   0x38
#define TestDAC1Reg     0x39
#define TestDAC2Reg     0x3A
#define TestADCReg      0x3B
#define Reserved31      0x3C
#define Reserved32      0x3D
#define Reserved33      0x3E
#define Reserved34      0x3F

/**
 * Function
 */
void WR_RFID_REG(unsigned char, unsigned char);
unsigned char RD_RFID_REG(unsigned char);
void RFID_SET_BIT_MSK(unsigned char, unsigned char);
void RFID_CLR_BIT_MSK(unsigned char, unsigned char);
void RFID_ANTENNA_ON();
void RFID_ANTENNA_OFF();
void RFID_RST();
void RFID_INIT();
unsigned char RFID_REQ(unsigned char, unsigned char *);
unsigned char RFID_2_CARD(unsigned char, unsigned char *, unsigned char, unsigned char *, unsigned int *);
unsigned char RFID_ANTI_COLL(unsigned char *);
void RFID_CAL_CRC(const unsigned char *, unsigned char, unsigned char *);
unsigned char RFID_SELECT_TAG(const unsigned char *);
unsigned char RFID_AUTH(unsigned char, unsigned char, const unsigned char *, const unsigned char *);
unsigned char RFID_RD_CARD(unsigned char, unsigned char *);
unsigned char RFID_WR_CARD(unsigned char, const unsigned char *);
unsigned char RFID_HALT();
void RFID_STOP_CRYPT();
void RFID_WAIT_CARD();

#endif //CLOUD_LOCK_RFID_H
