#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "USART2.h"
#include "adc.h"
#include "function.h"
#include "GENERAL.h"
#include "hwjs.h"
#include "beep.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "SysTick.h"
#include "DHT11.h"
#include "Servo.h"
#include "Timer.h"


uint8_t Data_Array_Dht11[5]= {0};


extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符


u16 adcx;
float temp;
float temp_fan;
u16 temp_int= 0;
u16 temp_first = 0;
u16 temp_second = 0;

uint8_t  window_sta=0,beep_sta=0;

//延时函数
void SOFT_DELAY(uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}




/*窗帘操作函数*/
void window_control(void)
{
	if(window_sta!=0)
	{


		AutoControlWindow(temp);
		//printf("%f\n",temp);
//		adcx=Get_Adc_Average(ADC_Channel_0,1);
//			temp=(float)adcx*(3.3/4096);
//			temp_fan=(3.3-(float)adcx*(3.3/4096));
//			temp_int=temp_fan*1000;
//			AutoControlWindow(temp);
//			printf("%f\n",temp);
//			temp_first = temp_int/256;
//			temp_second = temp_int- temp_first*256;

	}

	else
	{

		LED0=1;
		LED1=1;
	}
}
/*蜂鸣器操作函数*/
void beep_control(void)
{
	if(beep_sta!=0)
	{

		sound1();

	}
}


uint8_t ReadData[256]= {0};
uint8_t I2c_Write[256]= {0,1,2,3,4,5,6,7,8,9};

int main(void)
{
	u8 t=0;
	char sStr [ 100 ] = { 0 },cStr [ 100 ] = { 0 };
	/* 设置系统时钟为72M */
	SystemInit();
	SysTick_Configuration();  //SysTick中断配置，主要用来定时

	SysTick_Init();


	LED_Init();   //初始化LED
	delay_init();	    	 //延时函数初始化
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	USART2_Config();
	Adc_Init();
	GENERAL_TIM_Init();    //初始化PWM
	hwjs_init();   //红外接口初始化
	BEEP_Init();	//端口初始化
	Init_Steer();  //舵机及角度初始化


	printf("t:%d\n",t);
	USART_SendData(USART2,'2Q');
	USART_SendData(USART1,'e');
	while(1)
	{
		
		printf("测试跑飞程序:%d\n",t);
		
		
		delay_ms(300);
		
		adcx=Get_Adc_Average(ADC_Channel_0,1);
		temp=(float)adcx*(3.3/4096);
		switch(USART_RX_BUF[0])
		{
		case 0x01:
			switch (USART_RX_BUF[1])
			{
			case 0x0F:
				printf("%d\r\n",0x10);
				window_sta=0x01;



//					AutoControlWindow(temp);
//			printf("%f\n",temp);
				break;
			default :
				window_sta=0x0;
				printf("%d\r\n",0x1f);
				break;

			}
//			adcx=Get_Adc_Average(ADC_Channel_0,1);
//			temp=(float)adcx*(3.3/4096);
//			temp_fan=(3.3-(float)adcx*(3.3/4096));
//			temp_int=temp_fan*1000;
//			AutoControlWindow(temp);
//			printf("%f\n",temp);
//			temp_first = temp_int/256;
//			temp_second = temp_int- temp_first*256;
			break;

		case 0x02:
			SetServoAngle(USART_RX_BUF[1],USART_RX_BUF[3]);
			SetServoAngle(USART_RX_BUF[2],USART_RX_BUF[4]);
			break;

		case 0x03:
			//SOFT_DELAY(0XFFFFFF);
			DHT11Read(Data_Array_Dht11);
			//前两位为湿度整数和小数，后两位为温度整数和小数

			sprintf(sStr,"%d%d%d%d",
			        Data_Array_Dht11[0],Data_Array_Dht11[1],Data_Array_Dht11[2],
			        Data_Array_Dht11[3]);
			printf("%s\r\n",sStr);

//			printf("湿度=%d.%d%%RH\r\n温度=%d.%d℃\r\n校验和=%d\r\n",
//		Data_Array_Dht11[0],Data_Array_Dht11[1],Data_Array_Dht11[2],
//		Data_Array_Dht11[3],Data_Array_Dht11[4]);
			break;

		case 0x04:

			if((USART_RX_BUF[1]!=0)&&(USART_RX_BUF[2]!=0)&&(USART_RX_BUF[3]!=0))
			{
				SET_Color(USART_RX_BUF[1]-1,USART_RX_BUF[2]-1,USART_RX_BUF[3]-1);
			}
			else
			{

				SET_Color(USART_RX_BUF[1],USART_RX_BUF[2],USART_RX_BUF[3]);
			}

			break;

		case 0x05:
//			if(hw_jsbz==1)	//如果红外接收到
//			{
//				hw_jsbz=0;	   //清零
//				printf("红外接收码 %0.8X\r\n",hw_jsm);	//打印
//				printf("红0 %x\r\n红1 %x\r\n红2 %x\r\n红3 %x\r\n",Data_Array[0],Data_Array[1],Data_Array[2],Data_Array[3]);
//				hw_jsm=0;					//接收码清零

//			}
			break;

		case 0x06:
			switch(USART_RX_BUF[1])
			{
			case 0xf0:
				beep_sta=0x01;
				printf("%d\r\n",0x60);
				break;
			default :
				printf("%d\r\n",0x6f);
				beep_sta=0x00;
				break;

			}

			break;

		default :

			break;
		}
		for(uint16_t i=0; i<=USART_REC_LEN; i++)
		{
			USART_RX_BUF[i]=0;
		}


		window_control();
		beep_control();


	}
}



