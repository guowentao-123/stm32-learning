#include "stm32f10x.h"                  // Device header
#define ADC_Light_Min 300
#define ADC_Light_Max 3900

void ADC_Light_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructurn;
	ADC_InitTypeDef ADC_InitStructurn;
	
	GPIO_InitStructurn.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructurn.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructurn.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructurn);
	
	ADC_InitStructurn.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructurn.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructurn.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructurn.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructurn.ADC_NbrOfChannel=1;
	ADC_InitStructurn.ADC_ScanConvMode=DISABLE;
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	ADC_Init(ADC1,&ADC_InitStructurn);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);// 给 ADC 上电。它操作的是 CR2 寄存器的 ADEN 位
	ADC_ResetCalibration(ADC1);//把校准寄存器复位
	while (ADC_GetResetCalibrationStatus(ADC1) == SET){
	;
	}//死等第②步做完
    ADC_StartCalibration(ADC1);//启动真正的校准
    while (ADC_GetCalibrationStatus(ADC1) == SET);//同样死等，这次等 CAL 位被硬件清 0——校准完成。

}

//读取光敏传感器的原始采样值
uint16_t ADC_Light_ReadRaw(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	return ADC_GetConversionValue(ADC1);
	
}

//处理原始数据
// 注意：本光敏模块 AO 是反相的——越暗电压越高（捂住时 raw 反而到最大），
// 所以最后一步做 100 - pct 取反，让"越暗 = 百分比越低"符合直觉。
uint16_t ADC_Light_ReadPercent(void)
{
	uint16_t raw=ADC_Light_ReadRaw();
	uint16_t span=ADC_Light_Max-ADC_Light_Min;
	uint8_t pct;
	
	if(raw<=ADC_Light_Min)
	{
		pct=0;
	}
	else if(raw>=ADC_Light_Max)
	{
		pct=100;
	}
	else
	{
		pct=(uint8_t)(((uint32_t)(raw - ADC_Light_Min) * 100) / span);
	}
	return 100-pct;   // 反相修正
}