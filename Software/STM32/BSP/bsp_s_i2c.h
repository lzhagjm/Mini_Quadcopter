#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_
#include "sys.h"
typedef enum{
	S_IIC_OK = 0,
	S_IIC_ERROR,
}S_IIC_DEBUG;


//软件IIC引脚定义
//SCL    PB6
#define S_I2C_SCL_RCC_PORT			RCC_APB2Periph_GPIOB
#define S_I2C_SCL_PORT					GPIOB
#define S_I2C_SCL_Pin						GPIO_Pin_6

//SDA    PB7
#define S_I2C_SDA_RCC_PORT			RCC_APB2Periph_GPIOB
#define S_I2C_SDA_PORT					GPIOB
#define S_I2C_SDA_Pin						GPIO_Pin_7

#define SDA_1()  		GPIO_SetBits(S_I2C_SDA_PORT,S_I2C_SDA_Pin)
#define SDA_0()  		GPIO_ResetBits(S_I2C_SDA_PORT,S_I2C_SDA_Pin)
#define SCL_1()  		GPIO_SetBits(S_I2C_SCL_PORT,S_I2C_SCL_Pin)
#define SCL_0()  		GPIO_ResetBits(S_I2C_SCL_PORT,S_I2C_SCL_Pin)
#define SDA_In()    GPIO_ReadInputDataBit(S_I2C_SDA_PORT,S_I2C_SDA_Pin)

#define S_I2C_WR		0
#define S_I2C_RD		1

S_IIC_DEBUG S_I2C_Write(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size);
S_IIC_DEBUG S_I2C_Read(uint8_t Addr,uint8_t cmd,uint8_t *data,uint8_t size);
u8 S_I2C_Write_Byte(u8 addr,u8 cmd,u8 data);
u8 S_I2C_Read_Byte(u8 addr,u8 cmd);
void S_I2C_Init(void);

#endif  



