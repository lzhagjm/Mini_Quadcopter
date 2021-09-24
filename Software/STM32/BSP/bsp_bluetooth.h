#ifndef _BSP_BLUETOOTH_H_
#define _BSP_BLUETOOTH_H_
#include "sys.h"




/* 串口相关 */
#define USARTx						USART1
#define USARTx_CLK				RCC_APB2Periph_USART1
#define USARTx_TX_CLK    	RCC_APB2Periph_GPIOA
#define USARTx_TX_PORT    GPIOA
#define USARTx_TX_PIN			GPIO_Pin_9
#define USARTx_RX_CLK			RCC_APB2Periph_GPIOA
#define USARTx_RX_PORT		GPIOA
#define USARTx_RX_PIN			GPIO_Pin_10

#define RX_BUFFER_SIZE				(3+4)    //添加数据需改
#define TX_BUFFER_SIZE				7     
#define BUFFER_HEAD				0xA5
#define BUFFER_END				0x5A




typedef struct{											//添加数据需改
	uint8_t rx_buffer[RX_BUFFER_SIZE];


//	uint8_t P1[4];
//	uint8_t I1[4];
//	uint8_t D1[4];
//	uint8_t P2[4];
//	uint8_t I2[4];
//	uint8_t D2[4];
//	  u8 x[4];
//		u8 y[4];
	
		u8 s1[4];
//		u8 s2[4];
//		u8 s3[4];
//		u8 s4[4];
//	uint8_t k[4];
//	uint8_t k1[4];
//	uint8_t Q[4];
//	uint8_t R[4];
//	uint8_t Q2[4];
//	uint8_t R2[4];
//		uint8_t seep1[2];
//		uint8_t seep2[2];
	//发送
	uint8_t head;
	uint8_t Angle[4];
	uint8_t cs;
	uint8_t end;
}BlueTooth;


void BlueTooth_Init(void);
void calculate_cs(void);
uint8_t BlueTooth_Write_data(BlueTooth* Debug); //中断调用（防止数据打乱）
#endif

