/*
 * client.h
 *
 *  Created on: 2015Äê7ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef TCP_CLIENT_APP_INCLUDE_CLIENT_H_
#define TCP_CLIENT_APP_INCLUDE_CLIENT_H_
#include "user_main.h"
#include "espconn.h"
#include "mem.h"
struct espconn user_tcp_conn;
void my_station_init(struct ip_addr *remote_ip,struct ip_addr *local_ip,int remote_port);


#endif /* TCP_CLIENT_APP_INCLUDE_CLIENT_H_ */
