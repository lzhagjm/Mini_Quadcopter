#ifndef _SYS_H_
#define _SYS_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/***************中断优先级******************/
																				
#define NVIC_GROUP NVIC_PriorityGroup_3 //中断分组选择  NVIC_PriorityGroup_3  | 0-7  |   0-1  |

#define NVIC_TIM5_P 0 //TIM5中断配置   步进电机
#define NVIC_TIM5_S 0

//#define NVIC_systick_P 0  //滴答定时器
//#define NVIC_systick_S 0

#define NVIC_UART6_P  6    //k210数据
#define NVIC_UART6_S  0

#define NVIC_TIM7_P 0 //TIM7中断配置   核心定时器
#define NVIC_TIM7_S 0

#define NVIC_UART3_P 1 //串口3中断配置   //上位机
#define NVIC_UART3_S 0

#define NVIC_EXTI2_P 6 //EXTI2中断     //超声波
#define NVIC_EXTI2_S 0

#define NVIC_TIM4_P 6 //TIM4中断配置   编码器2
#define NVIC_TIM4_S 0

#define NVIC_TIM2_P 6 //TIM2中断配置   编码器1
#define NVIC_TIM2_S 1


//u8 CYCLE = 5;

//核心运行周期
#define CYCLE   10  //MS   max = 65.536ms   min = 0.001ms

/*           系统驱动               */
#include "Scheduler.h" 
#include "Task.h"
#include "core_run.h"

/*           BSP驱动库               */
#include "bsp_led.h"
#include "bsp_s_i2c.h"
#include "bsp_h_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_mpu6050.h"
#include "bsp_adc.h"
#include "bsp_bluetooth.h"
#include "bsp_uc_debug.h"	
#include "bsp_hollow_cup.h"


/*              算法库               */
#include "math_lib.h"
#include "FIR_lib.h"
#include "pid_lib.h"



/*           mpu_6050 Lib库驱动库               */
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"



/*****************灵活库********************/




/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			 {p->BSRRL=i;}		//设置为高电平
#define digitalLo(p,i)			 {p->BSRRH=i;}		//输出低电平
#define digitalToggle(p,i)	 {p->ODR ^=i;}		//输出反转状态


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).M4同M3类似,只是寄存器地址变了.
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入




#define TICK_PER_SECOND	1000
#define TICK_US	(1000000/TICK_PER_SECOND)


u8 All_Init(void);

#endif







