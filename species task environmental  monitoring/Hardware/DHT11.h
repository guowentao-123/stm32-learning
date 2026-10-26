#ifndef __DHT11_H
#define __DHT11_H
#include "Stdint.h"
static void DHT11_SetOutput(void);
static void DHT11_SetInput(void);
void DHT11_Init(void);
uint8_t DHT11_ReadData(uint8_t *Temperaturn,uint8_t*Humidity);

#endif 