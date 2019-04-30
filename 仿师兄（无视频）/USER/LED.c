#include "led.h"

void LED_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12;				 //LED0-->PB.13 端口配置
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
//	GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_12);						 //PB.12 13 输出高
//GPIO_ResetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
}

