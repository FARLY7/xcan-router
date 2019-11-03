/*
 * debug.c
 *
 *  Created on: 27 Feb 2018
 *      Author: B49855
 */

#include <dbg_console.h>
#include <string.h>
#include "config.h"
#include "ecu.h"
#include "pdu_router.h"

const char *cmds[] = {"ecus", "test"};

const uart_instance_t *uart_instance; /* Instance of UART */

void CONSOLE_initialise(const uart_instance_t *instance, const uart_user_config_t *config)
{
	uart_instance = instance;

	/* Initialize UART PAL over LINFlexD */
	UART_Init(instance, config);

	/* Create a FreeRTOS task which is responsible for running the debug console interface */
	xTaskCreate(CONSOLE_Task, "Serial Console", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_CONSOLE, NULL);
}

void CONSOLE_Task(void *pvParameters)
{
	/* Buffer to store the received message */
	//char rxBuff[5];
	//status_t stat;

	CONSOLE_printString("\e[32m\e[1m");

	CONSOLE_printString("\n\r============== GW REFERENCE PLATFORM ===============");
	CONSOLE_printString("\n\r============ Created by: Sean Farrelly =============");
	CONSOLE_printString("\n\r====================== V0.1 ========================\n\r");
	CONSOLE_printString("\e[0m");

	print_ECUs();

	while(1)
	{
//		UART_SendDataBlocking(uart, welcome, strlen(welcome), 1000U);
//
//		/* Get the message sent by user from the console, using blocking method, wait forever */
//		stat = UART_ReceiveDataBlocking(uart, (uint8_t*) rxBuff, 1, portMAX_DELAY);
//
//		if(stat == STATUS_SUCCESS)
//		{
//			if(rxBuff[0] == '1')
//			{
//				CMD_ecus();
//			}
//			else if(rxBuff[0] == '2')
//			{
//				CMD_test();
//			}
//			else if(rxBuff[0] == '3')
//			{
//				CMD_test();
//			}
//		}

		vTaskDelay(portMAX_DELAY);
	}
}


//void Debug_Task(void *pvParameters)
//{
//	/* Buffer to store the received message */
//	char rxBuff[50];
//	status_t stat;
//	uint32_t i = 0;
//
//	while(1)
//	{
//		/* Get the message sent by user from the console, using blocking method, wait forever */
//		stat = UART_ReceiveDataBlocking(uart, (uint8_t*) &rxBuff[i], 1, portMAX_DELAY);
//
//		if(stat == STATUS_SUCCESS)
//		{
//			if(rxBuff[i] == '\r') /* Execute command */
//			{
//				rxBuff[i] = '\0'; /* Insert null character for strcmp function */
//
//				if(strncmp(cmds[0], &rxBuff[0], i) == 0)
//				{
//					CMD_ecus();
//				}
//				else if(!strncmp(cmds[1], &rxBuff[0], i) == 0)
//				{
//					CMD_test();
//				}
//			}
//			i++;
//
//			if(i == sizeof(rxBuff))
//			{
//				UART_SendDataBlocking(uart, error_length, strlen(error_length), 1000U);
//				i = 0;
//			}
//		}
//	}
//}

void CONSOLE_printString(char *string)
{
	status_t stat;

	stat = UART_SendDataBlocking(uart_instance, (uint8_t*) string, strlen(string), 10000U);

	if(stat != STATUS_SUCCESS)
	{
		/* Useful debugger trap for checking if UART is being overloaded */
		stat = STATUS_ERROR;
	}
}

void CONSOLE_printData(char *data, uint32_t len)
{
	status_t stat;

	stat = UART_SendDataBlocking(uart_instance, (uint8_t*) data, len, 10000U);

	if(stat != STATUS_SUCCESS)
	{
		/* Useful debugger trap for checking if UART is being overloaded */
		stat = STATUS_ERROR;
	}
}

/* Appends and colour tags to message to ensure it is easily visable as an error message */
void CONSOLE_printError(char *string)
{
	char buff[strlen(string) + 20];

	sprintf(buff, "\e[31m\e[1m%s\e[0m", string);

	CONSOLE_printString(buff);
}

void print_ECUs()
{
	uint16_t i;
	char *status;
	char *protocol;
	char *endian;
	char buff[100];

	ECU *ecu;

	CONSOLE_printString("\n\r+-------+---------------+---------------+---------------+---------------+---------------+---------------+");
	CONSOLE_printString("\n\r| ID\t| Status\t| FW Ver\t| Protocol\t| Interface\t| Security\t| Endian\t|");
	CONSOLE_printString("\n\r+-------+---------------+---------------+---------------+---------------+---------------+---------------+");

	for(i = 0 ; i < NUM_OF_ECUS ; i++)
	{
		ecu = &ecu_list[i];

		/* Convert enums to their string equivalent */
		switch(ecu->status)
		{
			case ECU_STAT_NORMAL: 		status = "NORMAL  "; break;
			case ECU_STAT_SLEEPING: 	status = "SLEEP   "; break;
			case ECU_STAT_UPDATING: 	status = "UPDATING"; break;
			case ECU_STAT_CORRUPTED: 	status = "CORRUPTED"; break;
			case ECU_STAT_UNRESPONSIVE: status = "NO RESP  "; break;
			case ECU_STAT_FAULT:		status = "FAULTY   "; break;
			default: 					status = "N/A      "; break;
		}
		switch(ecu->protocol)
		{
			case PROTOCOL_CAN: 	 protocol = "CAN   "; break;
			case PROTOCOL_CANFD: protocol = "CAN-FD"; break;
			case PROTOCOL_LIN: 	 protocol = "LIN   "; break;
			case PROTOCOL_ETH: 	 protocol = "ETH   "; break;
			default: 			 protocol = "N/A   "; break;
		}
		switch(ecu->endianness)
		{
			case 0:  endian = "LITTLE"; break;
			case 1:  endian = "BIG   "; break;
			default: endian = "N/A   "; break;
		}
		/*********************************************/

		sprintf(buff, "\n\r| %d\t| %s    \t| %d\t\t| %s    \t| %d\t\t| %d\t\t| %s\t|",
				i, status, ecu->fw_version, protocol, ecu->instance, ecu->security, endian);

		CONSOLE_printString(buff);
	}

	CONSOLE_printString("\n\r+-------+---------------+---------------+---------------+---------------+---------------+---------------+\n\r");
}
