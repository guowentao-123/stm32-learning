#include "stm32f10x.h"

void Delay_ms(uint32_t xms)
{
	uint32_t i;
	while (xms--)
	{
		i = 7200;
		while (i--);
	}
}

void Delay_s(uint32_t xs)
{
	while (xs--)
	{
		Delay_ms(1000);
	}
}
