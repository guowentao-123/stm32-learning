#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count=0;
void Encoder_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructurn;
	GPIO_InitStructurn.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructurn.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructurn.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructurn); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	
	EXTI_InitTypeDef EXTI_InitStructurn;
	
	EXTI_InitStructurn.EXTI_Line=EXTI_Line0|EXTI_Line1;
	EXTI_InitStructurn.EXTI_LineCmd=ENABLE;
	EXTI_InitStructurn.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructurn.EXTI_Trigger=EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_InitStructurn);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructurn;
	NVIC_InitStructurn.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructurn.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructurn.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructurn.NVIC_IRQChannelSubPriority=1;
	
	NVIC_Init(&NVIC_InitStructurn);
	
	NVIC_InitStructurn.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStructurn.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructurn.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructurn.NVIC_IRQChannelSubPriority=2;
	
	NVIC_Init(&NVIC_InitStructurn);
}

int16_t Get_Count(){
	int16_t Temp=Encoder_Count;
	Encoder_Count=0;
	return Temp;
}

void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0)==SET){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
			Encoder_Count--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line1)==SET){
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0){
			Encoder_Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
