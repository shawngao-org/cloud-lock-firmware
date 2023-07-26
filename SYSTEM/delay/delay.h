/*************************************************************************
 * @file    Delay.h
 * @author  lyf
 * @version V1.1
 * @date    2014.7.6
 * @brief   
 **************************************************************************/

#ifndef __DELAY_H__
#define __DELAY_H__

#include "sys.h"

#define TIMING_MS         1        //�δ�ʱ���Ķ�ʱʱ��
#define USE_DELAY_INT     1        //1:ʹ����ʱ�ж� 0����ʹ���ж�
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
u32 GetTime(void);
u32 GetTime_us(void);
u32 Delta_T(u32 lasttime);
void SysTick_Handler_Deal(void);   //�жϴ��������ڵδ�ʱ�����жϺ�������

#endif

