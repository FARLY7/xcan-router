/*
 * router.c
 *
 *  Created on: 21 Feb 2018
 *      Author: B49855
 */

#include "pdu_router.h"
#include "gateway.h"
#include "config.h"
#include <stdlib.h>

#define TIMEOUT 1000

void PDU_initialise()
{
	/* Create queue to hold PDU objects */
	PDU_queue = xQueueCreate(PDU_QUEUE_SIZE, sizeof(PDU));
	/* Create queue to hold 'FW Update' specific PDUs */
    fw_update_queue = xQueueCreate(FW_QUEUE_SIZE, sizeof(PDU));

    /* Add the PDU Router task to the FreeRTOS scheduler */
	xTaskCreate(PDU_Task, 		"PDU Router", 	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_PDU, NULL);
}

void PDU_Task(void *pvParameters)
{
	PDU pdu;
	uint8_t i, ecu_id = 0;

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
		if( xQueueReceive(PDU_queue, &pdu, portMAX_DELAY) ) /* PDU available */
		{
			if(!(pdu.id & PDU_MASK_REQRESP_TYPE)) /* PDU Type: Normal */
			{
				for(i = 0 ; i < PDU_Descriptors[pdu.id].destCount ; i++)
				{
					ecu_id = PDU_Descriptors[pdu.id].dest[i];
#ifdef DEBUG_ENABLE
					CONSOLE_printPDU(&pdu);
#endif
					/* Tx PDU to ECU */
					ECU_sendPDU(ecu_id, &pdu);
				}
			}
			else if(pdu.id & PDU_MASK_REQRESP_TYPE) /* PDU Type: Request/Response */
			{
				/*
				 * Throw PDU to upper GW layer which will extract ECU response.
				 * and save data to ECU properties structure.
				 */
				xQueueSendToBack(gateway_queue, &pdu, 0);
			}
		}
	}
	/***********************************************************/
	/***********************************************************/
}
