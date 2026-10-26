#include "stm32f10x.h"
#include "core_cm3.h"
#define DEMCR      (*(volatile uint32_t *)0xE000EDFC)  // 调试异常与监控控制寄存器
#define DWT_CTRL   (*(volatile uint32_t *)0xE0001000)  // DWT 控制寄存器
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)  // DWT 周期计数器

#define DEMCR_TRCENA        (1UL << 24)   // 全局调试使能位
#define DWT_CTRL_CYCCNTENA  (1UL << 0)    // CYCCNT 启动位

/* DWT cycle counter based delay. Call Delay_Init() once after startup. */

void Delay_Init(void)
{
    DEMCR |= DEMCR_TRCENA;
    DWT_CYCCNT = 0;
    DWT_CTRL |= DWT_CTRL_CYCCNTENA;
}

/* xus * 72 assumes 72MHz system clock */
void Delay_us(uint32_t xus)
{
    uint32_t start = DWT_CYCCNT;
    while (DWT_CYCCNT - start < xus * 72);
}

void Delay_ms(uint32_t xms)
{
    while (xms--)
    {
        Delay_us(1000);
    }
}

void Delay_s(uint32_t xs)
{
    while (xs--)
    {
        Delay_ms(1000);
    }
}