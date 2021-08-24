
#ifndef _BSP_H_I2C_H_
#define _BSP_H_I2C_H_
/****************************** Includes *****************************/
#include "sys.h"




/****************************** Defines *******************************/

#define SENSORS_I2C_FAIL 			  1
#define SENSORS_I2C_SUCCESS 	  0


#define I2C_SPEED                 400000
#define I2C_OWN_ADDRESS           0x00


//毫秒级延时(需要定时器支持)，或者重写Delay宏
#define Delay 		Delay_ms

 
#define I2Cx_FLAG_TIMEOUT             ((uint32_t) 900) //0x1100
#define I2Cx_LONG_TIMEOUT             ((uint32_t) (300 * I2Cx_FLAG_TIMEOUT)) //was300
 
 
/*引脚定义*/ 

#define SENSORS_I2C_SCL_GPIO_PORT         GPIOB
#define SENSORS_I2C_SCL_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN          GPIO_Pin_8
#define SENSORS_I2C_SCL_GPIO_PINSOURCE    GPIO_PinSource8
 
#define SENSORS_I2C_SDA_GPIO_PORT         GPIOB
#define SENSORS_I2C_SDA_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN          GPIO_Pin_9
#define SENSORS_I2C_SDA_GPIO_PINSOURCE    GPIO_PinSource9

#define SENSORS_I2C_AF                    GPIO_AF_I2C1

#define SENSORS_I2C              					 I2C1
#define SENSORS_I2C_RCC_CLK               RCC_APB1Periph_I2C1



/*信息输出*/
#define I2C_DEBUG_ON         1
#define I2C_DEBUG_FUNC_ON    0

#define I2C_INFO(fmt,arg...)           printf("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          printf("<<-I2C-ERROR->> "fmt"\n",##arg)
#define I2C_DEBUG(fmt,arg...)          do{\
                                          if(I2C_DEBUG_ON)\
                                          printf("<<-I2C-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define I2C_DEBUG_FUNC()               do{\
                                         if(I2C_DEBUG_FUNC_ON)\
                                         printf("<<-I2C-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


																			 
/*函数定义*/
#define I2C_Config() I2cMaster_Init();
																			 
void I2cMaster_Init(void);
void Set_I2C_Retry(unsigned short ml_sec);
unsigned short Get_I2C_Retry(void);
																			 
int H_I2C_Read(unsigned char slave_addr,
                                       unsigned char reg_addr,
                                       unsigned short len, 
                                       unsigned char *data_ptr);
int H_I2C_Write(unsigned char slave_addr,
                                        unsigned char reg_addr,
                                        unsigned short len, 
                                        const unsigned char *data_ptr);



u8 H_I2C_Write_Byte(u8 addr,u8 cmd,u8 data);
u8 H_I2C_Read_Byte(u8 addr,u8 cmd);																			

#endif // __I2C_H__


