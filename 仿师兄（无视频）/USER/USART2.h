
#ifndef __USART2_H
#define __USART2_H


#include "stm32f10x.h"
#include "stdio.h"
#include "sys.h"


//串口2-USART2
#define  DEBUG_USART2                   USART2
#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART2_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd

#define  DEBUG_USART2_TX_GPIO_PORT       GPIOA
#define  DEBUG_USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART2_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART2_IRQ                USART2_IRQn
#define  DEBUG_USART2_IRQHandler         USART2_IRQHandler



extern void USART2_Config(void);
extern void Send2_Byte(USART_TypeDef* USARTx, uint8_t Data);
extern void Send2_HalfWord(USART_TypeDef* PUSARTx, uint16_t Data);
extern void Send2_Array(USART_TypeDef* PUSARTx, uint8_t *array,uint8_t num);
extern void Send2_Str(USART_TypeDef* PUSARTx,char* str);
extern  void NVIC2_Config(void);

#endif /*__USART2_H*/
