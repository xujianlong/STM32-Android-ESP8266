#include "driver/uart.h"
#include "user_main.h"
ETSTimer connect_timer;
void user_rf_pre_init(void)
{
}
void ICACHE_FLASH_ATTR Wifi_conned(void *arg)
{
	static uint8 count = 0;
	uint8 status;
	os_timer_disarm(&connect_timer);
	count++;
	status = wifi_station_get_connect_status();
	if (status == STATION_GOT_IP)
	{
		os_printf("Wifi connect success!");
		struct ip_info info;
		const char remote_ip [ 4 ] =
		{ 192, 168, 1, 100 };
		wifi_get_ip_info(STATION_IF, &info);
		//my_station_init((struct ip_addr *) remote_ip, &info.ip, 9999);

		//wifi开发板端口号
		server_init(&info.ip, 9998); //本地IP和端口号
		//wifi模块端口号
		server_init(&info.ip, 9999); //本地IP和端口号
		return;
	}
	else
	{
		if (count >= 7)
		{
			os_printf("Wifi connect fail！");
			return;
		}
	}
	os_timer_arm(&connect_timer, 2000, NULL);
}
void ICACHE_FLASH_ATTR scan_done(void *arg, STATUS status)
{
	uint8 ssid [ 33 ];
	struct station_config stationConf;

	if (status == OK)
	{
		struct bss_info *bss_link = (struct bss_info *) arg;
		bss_link = bss_link->next.stqe_next; //ignore first

		while (bss_link != NULL)
		{
			os_memset(ssid, 0, 33);
			if (os_strlen(bss_link->ssid) <= 32)
			{
				os_memcpy(ssid, bss_link->ssid, os_strlen(bss_link->ssid));
			}
			else
			{
				os_memcpy(ssid, bss_link->ssid, 32);
			}
			os_printf("+CWLAP:(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
					bss_link->authmode, ssid, bss_link->rssi,
					MAC2STR(bss_link->bssid), bss_link->channel);

			bss_link = bss_link->next.stqe_next;
		}
		/*以下为连接，上面部分为wifi扫描*/
		stationConf.bssid_set = 0; //need not check MAC address of AP
		/*os_memcpy(&stationConf.ssid, "796", 32);
		 os_memcpy(&stationConf.password, "1234567890", 64);*/
		os_memcpy(&stationConf.ssid, "nishishui", 32);
		os_memcpy(&stationConf.password, "niyashishuia", 64);
		wifi_station_set_config(&stationConf);
		wifi_station_connect();
		os_timer_setfn(&connect_timer, Wifi_conned, NULL);
		os_timer_arm(&connect_timer, 2000, NULL);
	}
	else
	{
		//  	os_sprintf(temp,"err, scan status %d\r\n", status);
		//  	uart0_sendStr(temp);

	}

}
void to_scan(void)
{
	wifi_station_scan(NULL, scan_done);
}
void user_init()
{
	uint8 opmode;
	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	opmode = wifi_get_opmode();
	os_printf("\r\n当前模式为：%d\r\n", opmode);
	wifi_set_opmode(0x01);
	opmode = wifi_get_opmode();
	os_printf("\r\n设置后的模式为：%d\r\n", opmode);
	system_init_done_cb(to_scan);
}
