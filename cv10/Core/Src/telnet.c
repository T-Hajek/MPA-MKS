/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#include "lwip/opt.h"
#include <string.h>

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

#define telnet_THREAD_PRIO  ( tskIDLE_PRIORITY + 4 )
#define CMD_BUFFER_LEN 256


/*-----------------------------------------------------------------------------------*/

//Telnet

static void http_client(char *s, uint16_t size)
{
	struct netconn *client;
	struct netbuf *buf;
	ip_addr_t ip;
	uint16_t len = 0;
	IP_ADDR4(&ip, 147,229,144,124);
	const char *request = "GET /ip.php HTTP/1.1\r\n"
			"Host: www.urel.feec.vutbr.cz\r\n"
			"Connection: close\r\n"
			"\r\n\r\n";
	client = netconn_new(NETCONN_TCP);
	if (netconn_connect(client, &ip, 80) == ERR_OK) {
		netconn_write(client, request, strlen(request), NETCONN_COPY);
		// Receive the HTTP response
		s[0] = 0;
		while (len < size && netconn_recv(client, &buf) == ERR_OK) {
			len += netbuf_copy(buf, &s[len], size-len);
			s[len] = 0;
			netbuf_delete(buf);
		}
	} else {
		sprintf(s, "Chyba pripojeni\n");
	}
	netconn_delete(client);
}

static void telnet_process_command(char *cmd, struct netconn *conn) {
	char *token;
	token = strtok(cmd, " ");
	char s[CMD_BUFFER_LEN];

	// ------------ HELLO-------------
	if (strcasecmp(token, "HELLO") == 0) {
		sprintf(s,"Komunikace OK\n\r");
		netconn_write(conn, s, strlen(s), NETCONN_COPY);

	}else if (strcasecmp(token, "CLIENT") == 0) {
			http_client(&s, CMD_BUFFER_LEN);
			netconn_write(conn, s, strlen(s), NETCONN_COPY);

		//LED1

	} else if (strcasecmp(token, "LED1") == 0) {
		token = strtok(NULL, " ");
		if (strcasecmp(token, "ON") == 0) {
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
			sprintf(s,"ZAPINAM LED1\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);

		} else if (strcasecmp(token, "OFF") == 0) {
			HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
			sprintf(s,"OK\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
		//LED 2
	} else if (strcasecmp(token, "LED2") == 0) {
		token = strtok(NULL, " ");
		if (strcasecmp(token, "ON") == 0) {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
			sprintf(s,"ZAPINAM LED2\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);

		} else if (strcasecmp(token, "OFF") == 0) {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
			sprintf(s,"OK\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
	} else if (strcasecmp(token, "STATUS") == 0) {
		if (HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin)){
			sprintf(s,"LED1 ON\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}

		//LED 3
	} else if (strcasecmp(token, "LED3") == 0) {
		token = strtok(NULL, " ");
		if (strcasecmp(token, "ON") == 0) {
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
			sprintf(s,"ZAPINAM LED3\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);

		} else if (strcasecmp(token, "OFF") == 0) {
			HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
			sprintf(s,"OK\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
	} else if (strcasecmp(token, "STATUS") == 0) {
		if (HAL_GPIO_ReadPin(LD3_GPIO_Port, LD3_Pin)){
			sprintf(s,"LED3 ON\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}

		/*else{
			sprintf(s,"LED1 OFF\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
		if (HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin)){
			sprintf(s,"LED2 ON\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
		else{
			sprintf(s,"LED2 OFF\n\r");
			netconn_write(conn, s, strlen(s), NETCONN_COPY);
		}
		 */

	}
	else{
		sprintf(s,"Neplatny Prikaz\n\r");
		netconn_write(conn, s, strlen(s), NETCONN_COPY);
	}
}

static void telnet_byte_available(uint8_t c, struct netconn *conn)
{
	static uint16_t cnt;
	static char data[CMD_BUFFER_LEN];
	if (cnt < CMD_BUFFER_LEN && c >= 32 && c <= 127) data[cnt++] = c;
	if (c == '\n') {
		data[cnt] = '\0';
		telnet_process_command(data, conn);
		cnt = 0;
	}
}

static void telnet_thread(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err, accept_err;
	struct netbuf *buf;
	uint8_t *data;
	u16_t len;

	LWIP_UNUSED_ARG(arg);

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	if (conn!=NULL)
	{
		/* Bind connection to well known port number 7. */
		err = netconn_bind(conn, NULL, 23);

		if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
			netconn_listen(conn);

			while (1)
			{
				/* Grab new connection. */
				accept_err = netconn_accept(conn, &newconn);

				/* Process the new connection. */
				if (accept_err == ERR_OK)
				{

					while (netconn_recv(newconn, &buf) == ERR_OK)
					{

						netbuf_data(buf, (void**)&data, &len);
						while (len--) telnet_byte_available(*data++, newconn);

						netbuf_delete(buf);
					}

					/* Close connection and discard connection identifier. */
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else
		{
			netconn_delete(newconn);
		}
	}
}
/*-----------------------------------------------------------------------------------*/

void telnet_init(void)
{
	sys_thread_new("telnet_thread", telnet_thread, NULL, DEFAULT_THREAD_STACKSIZE, telnet_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
