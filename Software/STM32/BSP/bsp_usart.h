#ifndef _BSP_USART_H_
#define _BSP_USART_H_
#include "sys.h"


#define USE_USART1  0
#define USE_USART2  0
#define USE_USART3  1
#define USE_USART6  1

#if USE_USART1
void usart1_init(u32 br_num);
void DrvUart1SendBuf(unsigned char *DataToSend, u8 data_num);
#endif
#if USE_USART2
void usart2_init(u32 br_num);
void DrvUart2SendBuf(unsigned char *DataToSend, u8 data_num);
#endif
#if USE_USART3
void usart3_init(u32 br_num);
void DrvUart3SendBuf(unsigned char *DataToSend, u8 data_num);
#endif

#if USE_USART6
void usart6_init(u32 br_num);
void DrvUart6SendBuf(unsigned char *DataToSend, u8 data_num);
#endif
void DrvUartDataCheck(void);
void drvU6DataCheck(void);

#endif  



