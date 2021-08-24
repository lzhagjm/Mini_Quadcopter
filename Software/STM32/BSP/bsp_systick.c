#include "bsp_systick.h"


static uint64_t SysRunTimeMs = 0;

void SysTick_Init(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
    uint32_t         cnts;

    RCC_GetClocksFreq ( &rcc_clocks );

    cnts = ( uint32_t ) rcc_clocks.HCLK_Frequency / TICK_PER_SECOND;
    cnts = cnts / 8;

    SysTick_Config ( cnts );
    SysTick_CLKSourceConfig ( SysTick_CLKSource_HCLK_Div8 );
}
void SysTick_Handler(void)
{
	SysRunTimeMs++;
}
uint32_t GetSysRunTimeMs(void)
{
	return SysRunTimeMs;
}
uint32_t GetSysRunTimeUs(void)
{
	register uint32_t ms;
    u32 value;
	do
	{
    ms = SysRunTimeMs;
    value = ms * TICK_US + ( SysTick->LOAD - SysTick->VAL ) * TICK_US / SysTick->LOAD;
	}
	while(ms != SysRunTimeMs);
	return value;
}

void Delay_us ( uint32_t us )
{
    uint32_t now = GetSysRunTimeUs();
    while ( GetSysRunTimeUs() - now < us );
}
void Delay_ms(u32 time)
{
	while ( time-- )
        Delay_us (1000);
}




