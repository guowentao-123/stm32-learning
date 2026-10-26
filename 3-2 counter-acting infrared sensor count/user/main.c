#include "stm32f10x.h"                  // Device header
#include "OLED.H"
#include "COUNTSENSOR.h"
int main()
{
	OLED_Init();
	
	OLED_ShowString(1,1,"count:");
	CountSensor_Init(); 
	while(1){
		OLED_ShowNum(1,7,Get_Countsensor_count(),5);
	}
}
