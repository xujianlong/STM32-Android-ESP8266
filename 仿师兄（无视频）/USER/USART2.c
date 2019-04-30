#include "USART2.h"




//重定义fputc函数
int fputc(int ch, FILE *f)
{
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕
	USART2->DR = (u8) ch;
	return ch;
}






//配置中断优先级
void NVIC2_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	//优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//中断源
	NVIC_InitStructure.NVIC_IRQChannel=DEBUG_USART2_IRQ;

	//主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;

	//子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;

	//中断使能
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;

	//调用NVIC初始化函数
	NVIC_Init(&NVIC_InitStructure);   //注释后可在主函数中扫描接收的数据控制RGB，不注释可以以中断的方式控制RGB
}

//USART初始化函数
void USART2_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;


	//开串口时钟
	DEBUG_USART2_APBxClkCmd (DEBUG_USART2_CLK,ENABLE);



	//初始化GPIO端口
	//开端口时钟
	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK,ENABLE);
	//选择接收引脚
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_RX_GPIO_PIN;
	//设置接收模式为浮空输入
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	//调用GPIO初始化函数
	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT,&GPIO_InitStructure);

	//选择发送引脚
	GPIO_InitStructure.GPIO_Pin=DEBUG_USART2_TX_GPIO_PIN;
	//设置发送模式为复用输出
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	//设置速度为50M
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	//调用GPIO初始化函数,完成配置
	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT,&GPIO_InitStructure);


	//USART设置
	//设置波特率115200
	USART_InitStructure.USART_BaudRate=DEBUG_USART2_BAUDRATE;
	//设置字长
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity=USART_Parity_No;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	//调用串口初始化函数,完成配置
	USART_Init(DEBUG_USART2,&USART_InitStructure);


	//串口中断优先级配置
	NVIC2_Config();	//注释后可在主函数中扫描接收的数据控制RGB，不注释可以以中断的方式控制RGB

	//使能接收中断
	USART_ITConfig(DEBUG_USART2,USART_IT_RXNE, ENABLE);


	//使能串口
	USART_Cmd(DEBUG_USART2,ENABLE);

}

//编写发送八位数函数
void Send2_Byte(USART_TypeDef* PUSARTx, uint8_t Data)
{
	//调用库函数，发送数据
	USART_SendData(PUSARTx,Data);
	//判断数据是否发送结束
	while(USART_GetFlagStatus(PUSARTx, USART_FLAG_TXE)==RESET);
}

//发送十六位数函数
void Send2_HalfWord(USART_TypeDef* PUSARTx, uint16_t Data)
{
	uint8_t tem_h=0,tem_l=0;
	tem_h|=(Data>>8);

	tem_l|=Data;
	//调用库函数，发送数据
	USART_SendData(PUSARTx,tem_h);
	//判断数据是否发送结束
	while(USART_GetFlagStatus(PUSARTx, USART_FLAG_TXE)==RESET);

	//调用库函数，发送数据
	USART_SendData(PUSARTx,tem_l);
	//判断数据是否发送结束
	while(USART_GetFlagStatus(PUSARTx, USART_FLAG_TXE)==RESET);
}


//发送八位数组
void Send2_Array(USART_TypeDef* PUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	for(i=0; i<num; i++)
	{
		Send2_Byte(PUSARTx, array[i]);
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(PUSARTx,USART_FLAG_TC)==RESET);
}


//发送字符串
void Send2_Str(USART_TypeDef* PUSARTx,char* str)
{
	uint16_t i=0;
	do
	{
		Send2_Byte(PUSARTx, *(str+i));
		i++;
	}
	while(*(str+i)!='\0');

	/* 等待发送完成 */
	while(USART_GetFlagStatus(PUSARTx,USART_FLAG_TC)==RESET) {}
}









