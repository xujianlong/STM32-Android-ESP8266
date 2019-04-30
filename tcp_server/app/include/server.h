/*
 * server.h
 *
 *  Created on: 2015Äê7ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef TCP_CLIENT_APP_INCLUDE_SERVER_H_
#define TCP_CLIENT_APP_INCLUDE_SERVER_H_
#include "user_main.h"
void server_init(struct ip_addr *local_ip,int port);

struct espconn esp_conn;

#endif /* TCP_CLIENT_APP_INCLUDE_SERVER_H_ */
