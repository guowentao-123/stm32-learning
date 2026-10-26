#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_count=0;
void CountSensor_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructurn;
	GPIO_InitStructurn.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructurn.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructurn.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructurn); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructurn;
	
	EXTI_InitStructurn.EXTI_Line=EXTI_Line14;
	EXTI_InitStructurn.EXTI_LineCmd=ENABLE;
	EXTI_InitStructurn.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructurn.EXTI_Trigger=EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStructurn);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructurn;
	NVIC_InitStructurn.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructurn.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructurn.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructurn.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructurn);
}

uint16_t Get_Countsensor_count(){
	return CountSensor_count;
}

void EXTI15_10_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line14)==SET){
		CountSensor_count++;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}

}