/*
 * ethernet.h
 *
 *  Created on: 20 Feb 2018
 *      Author: B49855
 */

#ifndef ETHERNET_H_
#define ETHERNET_H_

#include <stdbool.h>
#include "ecu.h"


void ETH_socketServer(void* pvParameters);
void ETH_sendCommand(uint8_t ecu_id, ecu_cmd cmd);
status_t ETH_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate);

#endif /* ETHERNET_H_ */
