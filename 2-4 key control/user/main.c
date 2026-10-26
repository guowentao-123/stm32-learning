#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.H"

uint8_t KeyNum;
int main()
{
	LED_Init();
	Key_Init();
	while(1){
		KeyNum=key_GetNum();
		if(KeyNum==1)
		{
			LED1_turn();
		}
		if(KeyNum==2)
		{
			LED2_turn();
		}
	}

}
