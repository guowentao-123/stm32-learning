#ifndef __ENCODER_H
#define __ENCODER_H
#include "stdint.h"

void Encoder_Init();
void EXTI0_IRQHandler();
void EXTI1_IRQHandler();
int16_t Get_Count();
#endif