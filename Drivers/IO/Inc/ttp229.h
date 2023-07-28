//
// Created by ShawnGao on 2023/7/28.
//

#ifndef CLOUD_LOCK_TTP229_H
#define CLOUD_LOCK_TTP229_H

#include "stm32f1xx_hal.h"
#include <stdint-gcc.h>
#include "sys.h"
#include "delay.h"

#define	SDA_IN()	{GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0X80000000;}
#define	SDA_OUT()	{GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= 0X30000000;}

#define	TTP_SCL		PBout(6)
#define	TTP_SDO		PBout(7)
#define	TTP_SDI		PBin(7)

extern uint16_t TTP_RD();
extern void TTP_INIT();
extern uint8_t TTP_SCAN();

#endif //CLOUD_LOCK_TTP229_H
