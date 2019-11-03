/*
 * router.c
 *
 *  Created on: 21 Feb 2018
 *      Author: B49855
 */

#include "pdu_router.h"
#include "FreeRTOS.h"
#include "gateway.h"
#include "config.h"
#include "dbg_console.h"
#include "can.h"
#include <stdlib.h>

#define TIMEOUT 1000


void PDU_discardPDU(PDU *pdu);
/*
 * @brief: Initialise the PDU router by creating FreeRTOS task
 * 		   and creating the queues which hold PDUs.
 * @param void  	 : none
 * @return     		 : none
 */
void PDU_initialise()
{
	/* Create queue to hold PDU objects */
	PDU_queue = xQueueCreate(PDU_QUEUE_SIZE, sizeof(PDU));

	/* Create queue to hold 'FW Update' specific PDUs */
    fw_update_queue = xQueueCreate(FW_QUEUE_SIZE, sizeof(PDU));

    /* Add the PDU Router task to the FreeRTOS scheduler */
	xTaskCreate(PDU_Task, "PDU Router", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_PDU, NULL);
}

/*
 * @brief: PDU Router (FreeRTOS task). This is responsible for recieving PDUs
 * 		   from the PDU queue and forwarding them to the correct ECU.
 * @param *pvParameters 	 : void parameter
 * @return     		 		 : none
 */
void PDU_Task(void *pvParameters)
{
	PDU pdu;
	uint8_t i, ecu_id = 0;

	BaseType_t stat;
	/********************* PDU ROUTER THREAD *******************/
	/***********************************************************/
	/*
	 * 1. Receive PDU from queue.
	 * 2. Mask PDU ID to determine type (Normal, Req/Resp, FW).
	 * 2. Consult PDU table to find destinations of PDU.
	 * 3. Send PDU to correct ECU interface.
	 * NOTE: Only route PDU if it is on different interface. (Only possible if Publish ECU is recorded?)
	 */
	while(1)
	{
		stat = xQueueReceive(PDU_queue, &pdu, portMAX_DELAY);
		if( stat == pdTRUE ) /* PDU available */
		{

#ifdef DEBUG_ENABLE
			PDU_printPDU(&pdu, false);
#endif

			/****************** PDU Type = NORMAL ******************/
			if(pdu.id & PDU_ID_MASK_TYPE_DATA)
			{
				/* Remove PDU_TYPE field, leaving only the Frame ID */
				uint32_t pdu_id = pdu.id & ~PDU_ID_MASK_TYPE_DATA;

				for(i = 0 ; i < PDU_Descriptors[pdu_id].destCount ; i++)
				{
					ecu_id = PDU_Descriptors[pdu_id].dest[i];

					/* Send PDU to ECU */
					ECU_sendPDU(ecu_id, &pdu);
				}

				PDU_discardPDU(&pdu);
			}
			/***************** PDU Type = COMMAND *****************/
			else if(pdu.id & PDU_ID_MASK_TYPE_CMD)
			{
				/*  Send CMD PDU to upper GW layer for further processing */
				if(errQUEUE_FULL == xQueueSendToBack(gateway_queue, &pdu, 0))
				{
					/* The queue is full so the PDU is dropped and will never
					 * be processed. In this case, immediately free() the memory
					 * for another Rx Frame.
					 */
					PDU_discardPDU(&pdu);
					CONSOLE_printError("ERROR: Gateway queue is full!");
				} else {
					/* The PDU has been successfully added to the Gateway queue for processing.
					 * WARNING: Upper layers must free() the buffer space once PDU has been processed.
					 */
				}
			}
			/***************** PDU Type = FIRMWARE ****************/
			else if(pdu.id & PDU_ID_MASK_TYPE_FW)
			{
				/*
				 * Send PDU directly to FW update queue.
				 */
				if(errQUEUE_FULL == xQueueSendToBack(fw_update_queue, &pdu, 0))
				{
					/* The queue is full so the PDU is dropped and will never
					 * be processed. In this case, immediately free() the memory
					 * for another Rx Frame.
					 */
					PDU_discardPDU(&pdu);
					CONSOLE_printError("ERROR: Firmware queue is full!");
				} else {
					/* The PDU has been successfully added to the firmware queue for processing.
					 * WARNING: Upper layers must free() the buffer space once PDU has been processed.
					 */
				}
			}
			else
			{
				/* PDU did not fall into any of the conditions for further processing.
				 * Discard the PDU and free() the data buffer.
				 */
				PDU_discardPDU(&pdu);
			}
		}
	}
	/****************** END PDU ROUTER THREAD ******************/
	/***********************************************************/
}

/*
 * @brief: Convert PDU to string equivalent and print to debug console.
 * @param *PDU  	 : Pointer to PDU struct
 * @param tx		 : True = Transmit, False = Receive
 * @return     		 : none
 */
void PDU_printPDU(PDU *pdu, bool tx)
{
	char buff[50 + (pdu->length)*3];
	char *dir;
	char *colour;
	char *label;
	uint8_t i;

	/* Set direction of message */
	switch(tx)
	{
		case true:  dir = "Tx"; colour = "\e[33m"; break; /* Yellow foreground */
		case false: dir = "Rx"; colour = "\e[0m"; break;  /* White foreground */
	}

	uint32_t ecuId = pdu->id & PDU_ID_MASK_ECU_ID;

	if(pdu->id & PDU_ID_MASK_TYPE_DATA) {
		label = PDU_Descriptors[ecuId].label;
	}
	else if(pdu->id & PDU_ID_MASK_TYPE_CMD) {
		label = "COMMAND  ";
	}
	else if(pdu->id & PDU_ID_MASK_TYPE_FW) {
		label = "FIRMWARE  ";
	}
	else {
		label = "N/A";
	}

	sprintf(buff, "\n\r%s[PDU ][%s] %s\t0x%08x\t0x%02x\t", colour, dir, label, pdu->id, pdu->length);
	CONSOLE_printString(buff);

	for (i = 0; i < pdu->length; i++)
	{
		sprintf(&buff[i*3], "%02X ", pdu->data[i]);
	}
	CONSOLE_printString(buff);
}


/*
 * @brief: This function frees the memory which was previously malloc()'d by interface.
 * 		   it is IMPORTANT this operation is not interrupted by a context switch
 * 		   or else an IVOR1 will be triggered.
 * @param *PDU  	 : Pointer to PDU struct
 * @return     		 : none
 */
void PDU_discardPDU(PDU *pdu)
{
	taskENTER_CRITICAL();
	free(pdu->data);
	//vPortFree(pdu->data);
	taskEXIT_CRITICAL();
}
