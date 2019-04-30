#include "Servo.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"

u8 Angle[8]= {0};
u8 num;


/* 函数名：SetServoAngle
* 描述  ：舵机角度赋值
* 输入  ：servoNum,Angle
* 输出  ：无
* 调用  ：外部调用
*/
/*****************舵机角度赋值********************/
void SetServoAngle(u16 servoNum,u8 angle)
{
	u16 pwm_value;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	if(servoNum>2)return;

	if(angle>165)return;//限制最大角度防止舵机烧毁
	if(angle<1)return;  //限制最小角度防止舵机烧毁

	Angle[servoNum-1]=angle;
	pwm_value=2500-(angle*(2000/180));//角度值转换计数值

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	                        //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwm_value-1;	                       //设置通道3的电平跳变值，输出另外一个占空比的PWM
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;            //当定时器计数值小于CCR1_Val时为高电平

	switch(servoNum)//舵机切换
	{
	case 1:
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		return;
	case 2:
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		return;
	default:
		return;
	}
}


/*
 * 函数名：Init_Steer
 * 描述  ：舵机初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Init_Steer()
{
	u8 i;
	ee_ReadBytes(Angle,0x10,2);//从eeprom读取角度值
	if((Angle[0]==0xff)&&(Angle[1]==0xff))//如果从未保存过角度
	{
		for(i=0; i<2; i++)
		{
			Angle[i]=0x5a;//赋初值90度
		}
		ee_WriteBytes(Angle,0x10,2);//保存
	}

	TIM4_PWM_Init();  //TIME4初始化

	for(i=0; i<2; i++)
	{
		SetServoAngle(i+1,Angle[i]);
	}
}
