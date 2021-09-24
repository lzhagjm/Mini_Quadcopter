#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_	
#include "sys.h" 

#define RHEOSTAT_NOFCHANEL      2
 							   
void adc1_init(void); 				//ADC通道初始化
 
float Get_temperature(void);

#endif 
