#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

uint32_t DWT_Delay_Init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
