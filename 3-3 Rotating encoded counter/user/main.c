#include "stm32f10x.h"                  // Device header
#include "OLED.H"
#include "ENCODER.h"

int16_t Num;
int main()
{
	OLED_Init();
	Encoder_Init();
	
	OLED_ShowString(1,1,"Num:");
	
	while(1){
		Num+=Get_Count();
		OLED_ShowSignedNum(1,5,Num,5);
	}
}
