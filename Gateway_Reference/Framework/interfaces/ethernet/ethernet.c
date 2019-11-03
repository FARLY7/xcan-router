/*
 * ethernet.c
 *
 *  Created on: 20 Feb 2018
 *      Author: B49855
 */

#include "ethernet.h"

void ETH_socketServer(void* pvParameters)
{
	//int socket = pvParameters[0];

	//vTaskDelete(NULL);
}

void ETH_sendRequest(ECU *ecu, ecu_req req)
{
//	uint8_t socket = ecu->interface;
//	uint32_t ret;
//	ret = lwip_write(socket, (void*) req, sizeof(req));
}



status_t ETH_updateFirmware(ECU *ecu, FIL *fw, bool secureUpdate)
{
	//uint8_t socket = ecu->interface;

	return STATUS_SUCCESS;
}
