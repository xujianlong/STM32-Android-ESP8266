/*
 * server.c
 *
 *  Created on: 2015年7月22日
 *      Author: Administrator
 */
#include "server.h"
void ICACHE_FLASH_ATTR server_recv(void *arg, char *pdata, unsigned short len)
{
	os_printf("*#%s", pdata);
	//espconn_sent((struct espconn *) arg, "已经收到", strlen("已经收到"));
	//espconn_send((struct espconn *) arg, pdata, strlen(pdata));
}
void ICACHE_FLASH_ATTR server_sent(void *arg)
{
	//os_printf("发送成功！");
}
void ICACHE_FLASH_ATTR server_discon(void *arg)
{
	os_printf("*#%d",0xff);
}

void ICACHE_FLASH_ATTR server_listen(void *arg)
{
	struct espconn *pespconn = arg;
	espconn_regist_recvcb(pespconn, server_recv);
	espconn_regist_sentcb(pespconn, server_sent);
	espconn_regist_disconcb(pespconn, server_discon);
}
void ICACHE_FLASH_ATTR server_recon(void *arg, sint8 err)
{
	//os_printf("连接错误，错误代码为：%d\r\n", err);
}
void ICACHE_FLASH_ATTR server_init(struct ip_addr *local_ip, int port)
{

	//初始化espconn参数
	esp_conn.type = ESPCONN_TCP;
	esp_conn.state = ESPCONN_NONE;
	esp_conn.proto.tcp = (esp_tcp *) os_malloc(sizeof(esp_tcp));
	os_memcpy(esp_conn.proto.tcp->local_ip, local_ip, 4);
	esp_conn.proto.tcp->local_port = port;

	//注册连接成功回调函数和重新连接回调函数
	espconn_regist_connectcb(&esp_conn, server_listen);
	espconn_regist_reconcb(&esp_conn, server_recon);

	espconn_accept(&esp_conn);
}

