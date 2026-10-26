#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"                  // Device header
#include "stdint.h"
void Key_Init(void);
uint8_t key_Scan(void);


#endif