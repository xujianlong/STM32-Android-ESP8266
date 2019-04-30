#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void SysTick_Init(void)
{
	/* SystemCoreClock / 1000    1ms中断一次
	 * SystemCoreClock / 100000	 10us中断一次
	 * SystemCoreClock / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000000))
	{
		/* Capture error */
		while (1);
	}
}


/*
 * 函数名：Delay_us
 * 描述  ：us延时程序,10us为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 *       ：外部调用
 */

void Delay_us(__IO u32 nTime)//已改为us
{
	TimingDelay = nTime;

	while(TimingDelay != 0);
}



/*
 * 函数名：Delay_ms
 * 描述  ：ms延时程序
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：
 *       ：外部调用
 */

void Delay_ms(__IO u32 nTime)
{
	TimingDelay = nTime*1000;

	while(TimingDelay != 0);
}


/*
 * 函数名：Delay_s
 * 描述  ：秒延时程序
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：
 *       ：外部调用
 */

void Delay_s(__IO u32 nTime)
{
	TimingDelay = nTime*1000000;

	while(TimingDelay != 0);
}



/*
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}


void SysTick_Configuration(void)
{
	/* Setup SysTick Timer for 10 msec interrupts  */
	if (SysTick_Config(SystemCoreClock / 1000000)) //SysTick配置函数
	{
		/* Capture error */
		while (1);
	}
	/* Configure the SysTick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置
}
