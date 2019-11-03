/*
 * debug.c
 *
 *  Created on: 27 Feb 2018
 *      Author: B49855
 */

#include <string.h>
#include "console.h"
#include "config.h"
#include "ecu.h"
#include "pdu_router.h"


const char *cmds[] = {"ecus", "test"};

uint8_t uart; /* Instance of UART */

void CONSOLE_initialise(uint8_t instance, const uart_user_config_t *config)
{
	uart = instance;

	/* Initialize UART PAL over LINFlexD */
	UART_Init(instance, config);

	xTaskCreate(CONSOLE_Task, "Serial Console", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_CONSOLE, NULL);
}

void CONSOLE_Task(void *pvParameters)
{
	/* Buffer to store the received message */
	//char rxBuff[5];
	//status_t stat;

	CONSOLE_printLine("\n\r============== GW REFERENCE PLATFORM ===============");
	CONSOLE_printLine("\n\r============ Created by: Sean Farrelly =============");
	CONSOLE_printLine("\n\r====================== V0.1 ========================\n\r");

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

		vTaskDelay(5000);
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

void CONSOLE_printLine(char *string)
{
	UART_SendDataBlocking(uart, (uint8_t*) string, strlen(string), 1000U);
}

void CONSOLE_printData(uint8_t *data, uint32_t len)
{
	UART_SendDataBlocking(uart, (uint8_t*) data, len, 1000U);
}


void print_ECUs()
{
	uint16_t i;
	char *status;
	char *protocol;
	char *endian;
	char buff[100];

	ECU *ecu;

	CONSOLE_printLine("\n\r===================== ECU LIST =====================");

	for(i = 0 ; i < NUM_OF_ECUS ; i++)
	{
		ecu = &ecu_list[i];

		/* Convert enums to string equivalent */
		switch(ecu->status)
		{
			case ECU_STAT_NORMAL: 		status = "NORMAL"; break;
			case ECU_STAT_SLEEPING: 	status = "SLEEP"; break;
			case ECU_STAT_UPDATING: 	status = "UPDATING"; break;
			case ECU_STAT_CORRUPTED: 	status = "CORRUPTED"; break;
			case ECU_STAT_UNRESPONSIVE: status = "NO RESP"; break;
			case ECU_STAT_FAULT:		status = "FAULTY"; break;
			default: 					status = "N/A"; break;
		}
		switch(ecu->protocol)
		{
			case PROTOCOL_CAN: 	 protocol = "CAN"; break;
			case PROTOCOL_CANFD: protocol = "CAN-FD"; break;
			case PROTOCOL_LIN: 	 protocol = "LIN"; break;
			case PROTOCOL_ETH: 	 protocol = "ETH"; break;
			default: 			 protocol = "N/A"; break;
		}
		switch(ecu->endianness)
		{
			case 0:  endian = "LITTLE"; break;
			case 1:  endian = "BIG"; break;
			default: endian = "N/A"; break;
		}

		sprintf(buff, "\n\rID: %d\tStatus: %s\tFW Ver: %d\tProtocol: %s\tInterface: %d\tSecurity: %d\tEndian: %s",
				i, status, ecu->fw_version, protocol, ecu->interface, ecu->security, endian);

		CONSOLE_printLine(buff);
	}

	CONSOLE_printLine("\n\r====================================================");
}



void CONSOLE_printPDU(PDU *pdu)
{
	uint16_t i;
	char buff[64];

	sprintf(buff, "\n\r[PDU][Rx] PDU ID: %d (%x)\tLength: %d\tData: %x", pdu->id, pdu->id, pdu->length);
	CONSOLE_printLine(buff);

	sprintf(buff, "%x", pdu->data);
	CONSOLE_printData(buff, sizeof(buff));


//	for(i = 0 ; i < pdu->length ; i++)
//	{
//		printf("%x02 ", pdu->data);
//		pdu->data++;
//
//		if((i % 7) == 0) {
//			printf("\n\r");
//		}
//	}
}
