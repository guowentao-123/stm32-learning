#ifndef __COUNTSENSOR_H
#define __COUNTSENSOR_H
#include "stdint.h"
void CountSensor_Init();
void EXTI15_10_IRQHandler();
uint16_t Get_Countsensor_count();
#endif