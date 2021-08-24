#ifndef _BSP_SYSTICK_H_
#define _BSP_SYSTICK_H_
#include "sys.h"

void SysTick_Init(void);
uint32_t GetSysRunTimeMs(void);
uint32_t GetSysRunTimeUs(void);
void Delay_us ( uint32_t us );
void Delay_ms(u32 time);


#endif
