#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "SysTick.h"
#include "delay.h"

//数据IO口选择
#define DHT11_PORT             GPIOG
#define DHT11_CLK              RCC_APB2Periph_GPIOG
#define DHT11_PIN              GPIO_Pin_11

//读取IO
#define Read                GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)


//a=1:输出高电平
//a=0:输出低电平
#define OutPut(a)  if(a)\
															      GPIO_SetBits(DHT11_PORT,DHT11_PIN); \
															else \
																		GPIO_ResetBits(DHT11_PORT,DHT11_PIN)


extern void DHT11Read(uint8_t*Data_Array);


#endif /*__DHT11_H*/
