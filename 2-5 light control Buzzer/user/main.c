#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "BUZZER.h"
#include "LIGHT.H"
uint8_t KeyNum;
int main()
{
	buzzer_Init();
	LightControl_Init(); 
	while(1){
		if(LightSensor_Get()==1)
			{
				buzzer_ON();
			}
		else
		{
				buzzer_OFF();
		}
	}
}
