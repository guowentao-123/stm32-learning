#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}
uint8_t key_Scan(void)
{
	static uint8_t lock=0;
	uint8_t KeyNum=0;
	
	//防止误触
	if(lock)
	{
		lock--;
		return 0;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
		lock=2;
		KeyNum=1;
	}
	else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		lock=2;
		KeyNum=2;
	}
	return KeyNum;
}