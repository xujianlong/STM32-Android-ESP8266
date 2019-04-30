#include "function.h"
#include "led.h"
#include "delay.h"
#include "beep.h"


extern u8 USART_RX_BUF[200];//´®¿Ú»º´æ


void AutoControlWindow(float temp)
{
	if(temp < 1)
	{
		LED1=1;
		LED0=0;
		//sound1();

	}
	else if(temp > 2.2)
	{
		LED0=1;
		LED1=0;  //´ò¿ª´°Á±
		//sound2();
	}
	else
	{
		LED0=1;
		LED1=1;
	}

}
