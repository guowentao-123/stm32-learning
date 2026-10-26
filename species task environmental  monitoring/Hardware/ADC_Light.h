#ifndef __ADC_LIGHT_H
#define __ADC_LIGHT_H

#include "stdint.h"
void ADC_Light_Init(void);
uint16_t ADC_Light_ReadRaw(void);
uint16_t ADC_Light_ReadPercent(void);

#endif