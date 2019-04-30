#include "hwjs.h"
#include "delay.h"



u32 hw_jsm;	  //定义一个32位数据变量，保存接收码
u8  hw_jsbz;  //定义一个8位数据的变量，用于指示接收标志
uint8_t Data_Array[4]= {0};

/*******************************************************************************
* 函 数 名         : hwjs_init
* 函数功能		   : 红外端口初始化函数	  时钟端口及外部中断初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void hwjs_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO时钟及管脚复用时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//红外接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOG,&GPIO_InitStructure);


	/* 设置外部中断的模式 */
	EXTI_ClearITPendingBit(EXTI_Line15);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource15); //选择GPIO管脚用作外部中断线路
	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* 设置NVIC参数 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;   //打开EXTI15的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   //使能
	NVIC_Init(&NVIC_InitStructure);

}


/*******************************************************************************
* 函 数 名         : HW_jssj
* 函数功能		   : 高电平持续时间，将记录的时间保存在t中返回，其中一次大约20us
* 输    入         : 无
* 输    出         : t
*******************************************************************************/
u8 HW_jssj()
{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15)==1)//高电平
	{
		t++;
		delay_us(20);

		if(t>=250) return t;//超时溢出
	}

	return t;
}
