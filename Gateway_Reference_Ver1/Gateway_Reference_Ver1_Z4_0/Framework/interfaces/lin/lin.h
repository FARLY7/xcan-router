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

void LIN_initialise(const uart_instance_t *instance, const uart_user_config_t *config);
void LIN_sendFrame(uint32_t instance, uint32_t messageId, uint8_t * data, uint32_t len);
void LIN_parseResponse(uint8_t *response, uint32_t length);
status_t LIN_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate);

#endif /* LIN_H_ */
