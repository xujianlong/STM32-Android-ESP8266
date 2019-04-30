#include "Timer.h"

/***************************************TIM2定时器配置**********************************************/

/*
 * 函数名：TIM2_GPIO_Config
 * 描述  ：配置TIM2输出的PWM信号的通道
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */

static void TIM4_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM2 clock enable */
	//PCLK1经过2倍频后作为TIM4的时钟源等于36MHz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/*  GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*****************************PB6上下舵机  PB7左右舵机*************************/
	/*GPIOA Configuration: TIM4 channel 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * 函数名：TIM2_Mode_Config
 * 描述  ：TIM2 输出PWM信号初始化，只要调用这个函数
 *         TIM2的二个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */

static void TIM4_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* -----------------------------------------------------------------------
	    TIM2 Configuration: generate 2 PWM signals with 2 different duty cycles:
	    TIM2CLK = 36 MHz, Prescaler = 0x0, TIM2 counter clock = 36 MHz
	    TIM2 ARR Register = 999 => TIM2 Frequency = TIM2 counter clock/(ARR + 1)
	    TIM2 Frequency = 36 KHz.
	    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR)* 100 = 50%
	    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR)* 100 = 37.5%
	    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR)* 100 = 25%
	    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR)* 100 = 12.5%
	  ----------------------------------------------------------------------- */

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (20000-1);       //当定时器从0计数到19999，即为20000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1);	    //设置预分频：不预分频，即为36MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1

	/* PWM1 Mode configuration: Channel3 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =750;	//设置通道3的电平跳变值，输出另外一个占空比的PWM

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse =750;	//设置通道4的电平跳变值，输出另外一个占空比的PWM

	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	//使能通道4

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

// TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM2重载寄存器ARR

	/* TIM2 enable counter */
	TIM_Cmd(TIM4, ENABLE);                   //使能定时器2
}

/*
 * 函数名：TIM4_PWM_Init
 * 描述  ：TIM4初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM4_PWM_Init(void)
{
	TIM4_GPIO_Config();
	TIM4_Mode_Config();
}
