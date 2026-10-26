#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define DHT11_PORT  GPIOA
#define DHT11_PIN GPIO_Pin_11
#define DHT11_CLK RCC_APB2Periph_GPIOA

//推挽输出，（单片机传送信息给DHT11）
static void DHT11_SetOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructurn;
	GPIO_InitStructurn.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructurn.GPIO_Pin=DHT11_PIN;
	GPIO_InitStructurn.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStructurn);
	
}


//上拉输入，（单片机接收DHT11传送来的信息）
static void DHT11_SetInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructurn;
	GPIO_InitStructurn.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructurn.GPIO_Pin=DHT11_PIN;
	GPIO_InitStructurn.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT,&GPIO_InitStructurn);
}

void DHT11_Init(void)
{
	RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);
	DHT11_SetOutput();
	GPIO_SetBits(DHT11_PORT,DHT11_PIN);
}

uint8_t DHT11_ReadData(uint8_t *Temperaturn,uint8_t*Humidity)
{
	//Data[0]是湿度整数，[1]是湿度小数,[2]是温度整数,[3]是温度小数,[4]是校验和
	uint8_t Data[5]={0,0,0,0,0};
	uint8_t i,j;
	uint16_t timeout;
	DHT11_SetOutput();
	GPIO_ResetBits(DHT11_PORT,DHT11_PIN);
	Delay_ms(20);
	GPIO_SetBits(DHT11_PORT,DHT11_PIN);
	Delay_us(30);
	DHT11_SetInput();
	timeout=0;
		while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==1)
	{
		Delay_us(1);
		if(++timeout>200)
			return 1;              // E1：传感器没有应答（供电/接线/DAT 落点）
	}
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			uint16_t cnt=0;    // 计"数据位高电平"持续的微秒数
			timeout=0;
			// 等上一位/响应的高电平结束（54us 的低电平到来）
			while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==1)
			{
				Delay_us(1);
				if(++timeout>200){
					return 2;      // E2：等"高电平结束"超时
				}
			}
			timeout=0;
			// 等低电平结束（本数据位的高电平开始）
			while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==0)
			{
				Delay_us(1);
				if(++timeout>200)
					return 2;      // E2：等"低电平结束"超时（应答后数据中断）
			}
			// 直接数高电平挂了多少微秒：0 位≈26us，1 位≈70us
			while(GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)==1)
			{
				Delay_us(1);
				if(++cnt>100)
					return 2;
			}
			if(cnt>40)             // 阈值取两档中间：>40us 判 1
			{
				Data[i]|=(uint8_t)(1<<(7 - j));
			}
		}
	}
	if(Data[4]!=(uint8_t)(Data[0]+Data[1]+Data[2]+Data[3]))
		return 3;                  // E3：收到数据但校验和不对（干扰/线缆）
	*Humidity=Data[0];
	*Temperaturn=Data[2];
	return 0;
}


