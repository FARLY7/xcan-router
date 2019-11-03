/*
 * console.h
 *
 *  Created on: 27 Feb 2018
 *      Author: B49855
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "Cpu.h"

void CONSOLE_Task(void *pvParameters);
void CONSOLE_initialise(uint8_t instance, const uart_user_config_t *config);
void CONSOLE_printLine(char *string);
void CONSOLE_printData(uint8_t *data, uint32_t len);

#endif /* COSNOLE_H_ */
