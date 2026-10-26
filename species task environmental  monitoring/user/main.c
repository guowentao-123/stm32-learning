#include "stm32f10x.h"                  // Device header
#include "OLED.H"
#include "KEY.h"
#include "ADC_Light.h"
#include "SysTick.h"
#include "DHT11.h"
#include "Buzzer.h"
#include "Delay.h"
#include "LED.h"
#define LIGHT_LIMIT_DFAULT 35
#define EYE_LIMIT_DEFAUT 1
#define HUMI_HIGH_LIMIT 85 //太湿
#define HUMI_LOW_LIMIT 15 //太干
#define PAGE_ENV 0
#define PAGE_EYE 1
#define PAGE_SET 2
#define PAGE_COUNT 3

static uint8_t g_page=PAGE_ENV;//页面标记
static uint8_t g_temperturn=0;//读取温度
static uint8_t g_humidity=0;//读取湿度
static uint8_t g_light=0;//ADC采集的光照
static uint8_t g_dht_ok=0;//DHT11读取成功的标志
static uint8_t g_dht_err=0;//DHT11错误码 0正常 1未应答 2数据超时 3校验错 4数值非法
static uint8_t g_light_limit=LIGHT_LIMIT_DFAULT;//光照报警阈值
static uint8_t g_eye_limit=EYE_LIMIT_DEFAUT;//护眼超时阈值
static uint32_t g_eye_sec=0;//用眼计时

static uint8_t g_alarm_light=0;//光照报警标志1报警0正常
static uint8_t g_alarm_eye=0;//护眼超时报警标志1报警0正常
static uint8_t g_alarm_humi=0;//湿度报警标志1报警0正常

static uint32_t s_dht_tick=0;//DHT11采样计时，用来控制多久读一次温湿度
static uint32_t s_adc_tick=0;//ADC光敏采集计时器，控制多久读一次光照
static uint32_t s_key_tick=0;//按键消抖
static uint32_t s_blink_tick=0;//LED闪烁计时
static uint32_t  s_eye_tick=0;//用眼计时节拍器
static uint8_t s_blink_on=0;//LED闪烁状态标记0灭1亮
static uint8_t g_alarm=0;
//报警模块
static void Alarm_Updata(void)
{
	g_alarm_light=(g_light<g_light_limit)?1:0;
	g_alarm_eye=(g_eye_sec>g_eye_limit*60)?1:0;
	g_alarm_humi=(g_dht_ok&&(g_humidity>HUMI_HIGH_LIMIT||g_humidity<HUMI_LOW_LIMIT))?1:0;
	g_alarm = (g_alarm_light | (g_alarm_eye << 1) | (g_alarm_humi << 2));
	
}

//展示页面
static void DisPlay_Env(void)
{
	OLED_ShowString(1,1,"T:");                      // T:13 H:50% L:43%  共 16 列
	if(g_dht_ok){ OLED_ShowNum(1,3,g_temperturn,2); }
	else        { OLED_ShowString(1,3,"--"); }       // 传感器故障时不显示旧数据
	OLED_ShowString(1,5," H:");
	if(g_dht_ok){ OLED_ShowNum(1,8,g_humidity,2); }
	else        { OLED_ShowString(1,8,"--"); }
	OLED_ShowString(1,10,"%");
	OLED_ShowString(1,11," L:");
	OLED_ShowNum(1,14,g_light,2);
	OLED_ShowString(1,16,"%");
	if(g_alarm_light)
	{
		OLED_ShowString(2,1,"Light LOW!");            // 10 字符
	}
	else
	{
		OLED_ShowString(2,1,"Light OK  ");            // 补齐 10 字符，防残影
	}
	if(!g_dht_ok)
	{
		OLED_ShowString(3,1,"DHT11 ERR    ");          // 9 + 4 空格 = 13，显示稳定不跳数
	}
	else if(g_alarm_humi)
	{
		OLED_ShowString(3,1,"Humidity WARN");         // 13
	}
	else
	{
		OLED_ShowString(3,1,"EVN OK       ");         // 6 + 7 空格 = 13
	}
	if(g_alarm)
	{
		OLED_ShowString(4,1,"alarm on   ");            // 8 + 3 空格 = 11
	}
	else
	{
		OLED_ShowString(4,1,"status,safe");            // 11
	}
}
static void DisPlay_Eye(void)
{
	uint32_t m=g_eye_sec/60;
	uint32_t s=g_eye_sec%60;
	OLED_ShowString(1,1,"EYE TIME :");
	OLED_ShowString(2,1,"USED:");
	OLED_ShowNum(2, 6, m, 2);
    OLED_ShowString(2, 8, "m");
    OLED_ShowNum(2, 9, s, 2);
    OLED_ShowString(2, 11, "s");
	OLED_ShowString(3, 1, "LIMIT:");
    OLED_ShowNum(3, 7, g_eye_limit, 2);
    OLED_ShowString(3, 9, "MIN");
	if(g_alarm_eye)
	{
		OLED_ShowString(4,1,"Take a Break! ");        // 补 1 空格 = 14，与下面等长
	}
	else
	{
		OLED_ShowString(4,1,"K2:RET K1:PAGE");        // 14
	}
	
}

static void DisPaly_Set(void)
{
	OLED_ShowString(1,1,"L-LIMIT:");
	OLED_ShowNum(1,14,g_light_limit,2);
	OLED_ShowString(2,1,"EYE-LIMIT:");
	OLED_ShowNum(2,14,g_eye_limit,2);
	OLED_ShowString(3, 1, "K2:+5  K1:PAGE");
    OLED_ShowString(4, 1, "PAGE2:RST EYE ");
}
static void DisPlay_Refresh(void)
{
	if(g_page==PAGE_ENV)
	{DisPlay_Env();}
	else if(g_page==PAGE_EYE)
	{DisPlay_Eye();}
	else 
	{DisPaly_Set();}
}

static void Alarm_Output(uint32_t now)
{
	if(!g_alarm)
	{
		buzzer_OFF();
		LED1_OFF();
		LED2_OFF();
		return;
	}
	if(now-s_blink_tick>=500)
	{
		s_blink_tick=now;
		s_blink_on=!s_blink_on;
	}
	if(s_blink_on)
	{
		buzzer_ON();
		LED1_ON();
		LED2_ON();
	}
	else
	{
		buzzer_OFF();
		LED1_OFF();
		LED2_OFF();
	}
	if(g_alarm_light)
	{
		LED2_ON();
	}
}
int main()
{
	uint8_t key;
	uint32_t now;
	
	Delay_Init();
	SysTick_Init();
	OLED_Init();
	LED_Init();
	ADC_Light_Init();
	buzzer_Init();
	DHT11_Init();
	Key_Init();
	OLED_Clear();
    OLED_ShowString(1, 1, "  HEALTH SENTRY ");
    OLED_ShowString(2, 1, "  DESK MONITOR  ");
    OLED_ShowString(3, 1, "  LOADING...    ");
    OLED_ShowString(4, 1, "  K1 NEXT K2 +  ");
    Delay_ms(1500);
    OLED_Clear();
	while(1){
		now=SysTick_GetMs();
		if(now-s_key_tick>=10)
		{
			s_key_tick=now;
			key=key_Scan();
			if(key==1)
			{
					g_page++;
				if(g_page>=PAGE_COUNT)
				{
					g_page=0;
				}
			OLED_Clear();
			}
			else if(key==2)
			{
				if(g_page==PAGE_SET)
				{
					g_light_limit += 5;
                    if (g_light_limit > 90) 
					{	
						g_light_limit = 10;
					}
				}
					else if(g_page==PAGE_EYE){
						g_eye_sec=0;
					}
				}
		}
		if(now-s_dht_tick>=2000){
			s_dht_tick=now;
			g_dht_err=DHT11_ReadData(&g_temperturn,&g_humidity);
			// 校验和全 0 也能通过（线松时读到全 0），必须再做量程合理性检查
			if(g_dht_err==0 && !(g_humidity>=10 && g_humidity<=100 && g_temperturn<=60))
			{
				g_dht_err=4;       // 数据"合法"但物理上不可能
			}
			g_dht_ok=(g_dht_err==0)?1:0;
		}
		if(now-s_adc_tick>=200){
			s_adc_tick=now;
			g_light=ADC_Light_ReadPercent();
		}
		if(now-s_eye_tick>=1000){
			s_eye_tick=now;
			g_eye_sec++;
		}
		Alarm_Updata();
		Alarm_Output(now);
		DisPlay_Refresh();
		
		Delay_ms(50);
	}
}
