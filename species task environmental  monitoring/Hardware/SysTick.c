#include "stm32f10x.h"
#include "SysTick.h"

/* 1ms tick counter, driven by SysTick interrupt. */
static volatile uint32_t s_ms = 0;

void Tick_Inc(void)
{
    s_ms++;
}

/* IMPORTANT: stm32f10x_it.c already defines an empty SysTick_Handler().
   Replace its body with:   Tick_Inc();                                */
void SysTick_Init(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 3);
}

uint32_t SysTick_GetMs(void)
{
    return s_ms;
}

uint32_t SysTick_GetSec(void)
{
    return s_ms / 1000;
}