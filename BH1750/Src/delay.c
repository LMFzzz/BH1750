//
// Created by LMF on 2024/1/13.
//
#include "delay.h"
static uint8_t fac_us=0;
static uint16_t fac_ms=0;
void delay_Init(){
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
    fac_us=SystemCoreClock/8000000;
    fac_ms=(uint16_t)fac_us*1000;
}
//��ʱnus
void delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD=nus*fac_us;
    SysTick->VAL=0x00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    do {
        temp=SysTick->CTRL;
    } while ((temp&0x01)&&!(temp&(1<<16)));
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL=0x00;
}
//��ʱnms
void delay_ms(uint16_t nms)
{
    uint32_t temp;
    SysTick->LOAD=(uint32_t)nms*fac_ms;
    SysTick->VAL=0x00;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
    do {
        temp=SysTick->CTRL;
    } while ((temp&0x01)&&!(temp&(1<<16)));
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL=0x00;
}