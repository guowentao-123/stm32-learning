#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>
void Tick_Inc(void);
void SysTick_Init(void);
uint32_t SysTick_GetMs(void);
uint32_t SysTick_GetSec(void);

#endif 