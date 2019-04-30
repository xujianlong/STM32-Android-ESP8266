#include "DHT11.h"


/*
 * 函数名： static void Input_GPIO_Config(void);
 * 描述  : IO口接收配置
 * 输入  ：无
 * 输出  : 无
 */
static void Input_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//浮空输入
	GPIO_InitStructure.GPIO_Pin=DHT11_PIN;

	GPIO_Init(DHT11_PORT,&GPIO_InitStructure);
}


/*
* 函数名： static void Output_GPIO_Config(void);
* 描述  : IO口发送配置
* 输入  ：无
* 输出  : 无
*/
static void Output_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=DHT11_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_Init(DHT11_PORT,&GPIO_InitStructure);

}


/*
* 函数名：static void DHT11WriteStart(void);
* 描述  : //引导信号
* 输入  ：一维数组指针
* 输出  : 无
*/
static void DHT11WriteStart(void)
{
	//SysTick_Init();
	Output_GPIO_Config();//将IO口配置成输出模式
	OutPut(0);
	delay_us(25000);//延时超过18ms
	OutPut(1);
	delay_us(3);
}


/*
* 函数名：static uint8_t Byte_Receive(void);
* 描述  : 单总线8位数据接收
* 输入  ：无
* 输出  : 无
*/
static uint8_t  Byte_Receive(void)
{
	u8 temp=0,i=0,j=0;
	Input_GPIO_Config();
	for(i=0; i<8; i++)
	{
		temp<<=1;
		while(Bit_RESET== Read);//等待变为高电平
		while(Read==Bit_SET)//计算高电平时长
		{

			delay_us(1);//延时1us
			j++;
		}
		if(j>=9)        //超过30us确认为1
		{
			temp = temp|0x01;
			j=0;
		}
		j=0;

	}
	return temp;

}



/*
* 函数名：void DHT11Read(uint8_t *Data_Array);
* 描述  : 总线数据接收函数
* 输入  ：一维数组指针
* 输出  : 无
*/
void DHT11Read(uint8_t*Data_Array)
{

	uint8_t TH_temp,TL_temp,RH_temp,RL_temp,CK_temp;
	DHT11WriteStart();//读取前导信号
	Input_GPIO_Config(); //设置端口为输入
	while(Bit_SET==Read);
	if(!Read)
	{
		while(Bit_RESET == Read);//低电平响应信号
		while(Bit_SET == Read);//80us高电平准备信号
		RH_temp=Byte_Receive();//湿度高8位
		RL_temp=Byte_Receive();//湿度低8位
		TH_temp=Byte_Receive();//温度高8位
		TL_temp=Byte_Receive();//温度低8位
		CK_temp=Byte_Receive();//校验和

		Output_GPIO_Config();
		OutPut(1);

		//校验
		if(CK_temp==RH_temp+RL_temp+TH_temp+TL_temp)
		{
			*Data_Array=RH_temp;
			*(Data_Array+1)=RL_temp;
			*(Data_Array+2)=TH_temp;
			*(Data_Array+3)=TL_temp;
			*(Data_Array+4)=CK_temp;
		}

	}
}