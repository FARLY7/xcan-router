/*
 * tcp_sockets.c
 *
 *  Created on: 22 Nov 2017
 *      Author: B49855
 */

#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/stats.h>
#include <lwip/debug.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include "lwip/sockets.h"
#include <status.h>

#include "ff.h"
#include "ecu.h"

#define SOCK_TARGET_HOST "192.168.1.1"
#define SOCK_TARGET_PORT 80
#define SOCK_FW_PORT 	 10

#define STREAM_BUFFER_SIZE 1024u

#define MAX_SERV	5

static void socket_init_client(void* arg);
static void socket_init_server(void* arg);

static status_t TCP_DownloadFirmwareHeader(int socket, ecu_fw_header *header);
static status_t TCP_DownloadFirmware(int socket, ecu_fw_header *header);

static ip_addr_t dstaddr;

/* Socket server will wait for connection from T-BOX to initiate FW transfer */
static void socket_init_server(void* arg)
{
	  int server, socket, ret;
	  struct sockaddr_in addr;
	  const ip_addr_t *ipaddr = (const ip_addr_t*)arg;
	  struct sockaddr cliaddr;
	  socklen_t clilen;

	  /* set up address to connect to */
	  memset(&addr, 0, sizeof(addr));
	  addr.sin_len = sizeof(addr);
	  addr.sin_family = AF_INET;
	  addr.sin_port = PP_HTONS(SOCK_FW_PORT);
	  addr.sin_addr.s_addr = PP_HTONL(INADDR_ANY);

	  /* create the socket */
	  server = lwip_socket(AF_INET, SOCK_STREAM, 0);
	  LWIP_ASSERT("server >= 0", server >= 0);

	  /* bind the socket */
	  ret = lwip_bind(server, &addr, sizeof(addr) - 1);

	  /* put the socket in listen mode */
	  if(lwip_listen(server, MAX_SERV) == -1) {
		  LWIP_ASSERT("Listen failed", 0);
	  }

	  socket = lwip_accept(server, &cliaddr, clilen);

	  /*
	   *
	   * Hi am ECU id
	   * Save socket int to ECU interface for later comms
	   *
	   */


	  if(socket >= 0)
	  {
		  ecu_fw_header header;
		  ECU *ecu;

		  while(1) {

			  TCP_DownloadFirmwareHeader(socket, &header);

			  /* Get ECU which the FW belongs to */
			  ecu = ECU_GetECUfromID(header.ecu_id);

			  //if((header.fw_ver > ecu->fw_ver_new) && (header.fw_ver > ecu->fw_ver)) /* FW is latest FW */
			  //{
				  TCP_DownloadFirmware(socket, &header);
				  lwip_close(socket);
			  //}
			  //else /* FW is not latet, just close socket? */
			  //{
				  lwip_close(socket);
			  //}
		  }
	  }
}


static status_t TCP_DownloadFirmwareHeader(int socket, ecu_fw_header *header)
{
	  lwip_read(socket, header, sizeof(header)); /* Read header */
	  return STATUS_SUCCESS;
}


static status_t TCP_DownloadFirmware(int socket, ecu_fw_header *header)
{
	uint8_t buffer[STREAM_BUFFER_SIZE];
	FIL fdst;
	FRESULT res;
	UINT bw;
	char path[30];

	sprintf(path, "0:%d/%d_fw_%d.bin", header->ecu_id, header->ecu_id, header->fw_ver);
	res = f_open(&fdst, path, FA_CREATE_ALWAYS | FA_WRITE);

	if(res == FR_OK)
	{
		f_write(&fdst, &header, sizeof(header), &bw); /* Write header to beginning of file */

		uint32_t remaining_bytes = header->length;
		UINT br = 0;

		/* There must be a more elagent way of implementation below */
		while(br != header->length)
		{
			if((remaining_bytes - STREAM_BUFFER_SIZE) > STREAM_BUFFER_SIZE)
			{
				br += lwip_read(socket, buffer, sizeof(buffer));
				f_write(&fdst, buffer, sizeof(buffer), &bw);
			}
			else
			{
				br += lwip_read(socket, buffer, remaining_bytes);
				f_write(&fdst, buffer, remaining_bytes, &bw);
			}
		}

		f_close(&fdst);

		return STATUS_SUCCESS;
	}
	else return STATUS_ERROR;
}



///* Socket client for making any connection requests */
//static void socket_init_client(void* arg)
//{
//	  int s;
//	  int ret;
//	  struct sockaddr_in addr;
//	  const ip_addr_t *ipaddr = (const ip_addr_t*)arg;
//
//	  /* set up address to connect to */
//	  memset(&addr, 0, sizeof(addr));
//	  addr.sin_len = sizeof(addr);
//	  addr.sin_family = AF_INET;
//	  addr.sin_port = PP_HTONS(SOCK_TARGET_PORT);
//	  inet_addr_from_ip4addr(&addr.sin_addr, ip_2_ip4(ipaddr));
//
//	  /* create the socket */
//	  s = lwip_socket(AF_INET, SOCK_STREAM, 0);
//	  LWIP_ASSERT("s >= 0", s >= 0);
//
//	  /* connect */
//	  ret = lwip_connect(s, (struct sockaddr*)&addr, sizeof(addr));
//	  /* should succeed */
//	  LWIP_ASSERT("ret == 0", ret == 0);
//
//	  /* write something */
//	  ret = lwip_write(s, "test", 4);
//	  LWIP_ASSERT("ret == 4", ret == 4);
//
//	  /* close */
//	  ret = lwip_close(s);
//	  LWIP_ASSERT("ret == 0", ret == 0);
//}


static void socket_test(void* arg)
{
  //socket_init_client(arg);
  socket_init_server(arg);

  /* Our job is done. Block forever, since some OSes don't support tasks exiting. */
  while (1) {
    sys_msleep(5000); /* time is arbitrary */
  }
}

void socket_init(void)
{

  IP_SET_TYPE_VAL(dstaddr, IPADDR_TYPE_V4);
  ip4addr_aton(SOCK_TARGET_HOST, ip_2_ip4(&dstaddr));

  sys_thread_new("socket_example_test", socket_test, &dstaddr, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

}
