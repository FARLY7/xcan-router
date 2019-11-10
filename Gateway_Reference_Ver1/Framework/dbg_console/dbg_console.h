/*
 * console.h
 *
 *  Created on: 27 Feb 2018
 *      Author: B49855
 */

#ifndef DBG_CONSOLE_H_
#define DBG_CONSOLE_H_

#include "Cpu.h"



void CONSOLE_Task(void *pvParameters);
/* Initialise debug console */
void CONSOLE_initialise(const uart_instance_t *instance, const uart_user_config_t *config);
/* Print string to debug console */
void CONSOLE_printString(char *string);
/* Print data to debug console */
void CONSOLE_printData(char *data, uint32_t len);
/* Print string as error in debug console (red foreground) */
void CONSOLE_printError(char *string);
/* Print CAN frame to debug console */
//void CONSOLE_printFrame(uint8_t instance, can_message_t *msg, bool tx)
/* Print PDU frame to debug console */
//void CONSOLE_printPDU(PDU *pdu, bool tx)
/* Print generic data packet to debug console */
//void CONSOLE_printDebug(char *interface, uint32_t id, uint8_t len, uint8_t *data, bool tx)
/* Converts byte array to hex string equivalent and prints to debug console */
//void print_hex(uint8_t *data, uint32_t len);

#endif /* DBG_COSNOLE_H_ */
