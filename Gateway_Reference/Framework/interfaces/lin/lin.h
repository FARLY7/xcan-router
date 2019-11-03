/*
 * lin.h
 *
 *  Created on: 6 Feb 2018
 *      Author: B49855
 */

#ifndef LIN_H_
#define LIN_H_

#include <stdbool.h>
#include "Cpu.h"
#include "ecu.h"
#include "status.h"

void LIN_Task(void* pvParameters);
void LIN_initialise(uint32_t instance, linflexd_uart_state_t *uartStatePtr,
        			const linflexd_uart_user_config_t *uartUserConfig);
void LIN_sendRequest(ECU *ecu, ecu_req req);
void LIN_parseResponse(uint8_t *response, uint32_t length);
status_t LIN_updateFirmware(ECU *ecu, FIL *fw, bool secureUpdate);

#endif /* LIN_H_ */
