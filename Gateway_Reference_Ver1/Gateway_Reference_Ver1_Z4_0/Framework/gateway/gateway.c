/*
 * gateway.c
 *
 *  Created on: 26 Feb 2018
 *      Author: B49855
 */

#include <string.h>
#include "gateway.h"
#include "config.h"
#include "task.h"
#include "can.h" /* Including CAN for #defines of MASK. Perhaps need to decouple? */


void GW_initialise()
{
	/* Create a queue to hold 10 PDUs which are for GW */
	gateway_queue = xQueueCreate(GW_QUEUE_SIZE, sizeof(PDU));

	/* Add Gateway Task to the FreeRTOS scheduler */
	xTaskCreate(Gateway_Task, "Gateway", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_GW, NULL);
}

void Gateway_Task(void *pvParameters)
{
	uint16_t i;
	PDU pdu;

	/*********************** GATEWAY THREAD ********************/
	/***********************************************************/
	while(1)
	{
		/* GW Task: Receive any PDUs which are destined for GW, or require signal routing */
		if(xQueueReceive(gateway_queue, &pdu, portMAX_DELAY))
		{
			if(pdu.id & PDU_ID_MASK_TYPE_CMD)
			{
				/* Extract information from PDU and save to ECU struct */
				GW_parsePDU(&pdu);
				/* Discard PDU and free memory */
				PDU_discardPDU(&pdu);
			}
		}

		/* GW Task: Act upon any critical ECU statuses */
		for(i = 0 ; i < NUM_OF_ECUS ; i++)
		{
			switch(ecu_list[i].status)
			{
				case ECU_STAT_NORMAL: 		{ break; }
				case ECU_STAT_SLEEPING: 	{ break; }
				case ECU_STAT_UPDATING: 	{ break; }
				case ECU_STAT_CORRUPTED: 	{ break; }
				case ECU_STAT_UNRESPONSIVE: { break; }
				case ECU_STAT_FAULT: 		{ break; }
			}
		}
	}
}


/*
 * @brief: Task responsible for 'Heartbeat' functionality of GW.
 * 		   e.g. periodically ping all ECUs for status update.
 * @param *pvParameters : void parameter
 * @return         	    : none
 */
void Heartbeat_Task(void *pvParameters)
{
	uint16_t i;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = portTICK_PERIOD_MS * 3000; /* Frequency = 1sec */

	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	/******************** GW HEARTBEAT THREAD ******************/
	/***********************************************************/
	while(1)
	{
		// 1. Wait for the next cycle.
		vTaskDelayUntil( &xLastWakeTime, xFrequency );

		// 2. Request status update from all ECUs
		for(i = 0 ; i < NUM_OF_ECUS ; i++)
		{
			if(ecu_list[i].status != ECU_STAT_UPDATING)
			{
				ECU_sendCommand(i, ECU_CMD_REQ_STATUS);
				vTaskDelay(100);
			}
		}
	}
}

/*
 * @brief: Parse ECU response to extract ID, FW VER, Health, etc..
 * @param response  : Pointer to response to be parsed
 * @return          : none
 */
void GW_parsePDU(PDU *pdu)
{
	uint8_t ecu_id = pdu->id & PDU_ID_MASK_ECU_ID; /* Extract ECU ID from MSG ID field */
	uint8_t cmd = pdu->data[0]; /* Extract CMD type from payload */
	uint8_t *data = pdu->data;

	ECU *ecu = ECU_getECU(ecu_id);

	switch(cmd)
	{
		case ECU_CMD_REQ_ALL: 		memcpy(ecu, data, sizeof(ECU)); break;
		case ECU_CMD_REQ_ID: 		/* Process data */ break;
		case ECU_CMD_REQ_FWVER:		ecu->fw_version = data[1]; break;
		case ECU_CMD_REQ_STATUS:	ecu->status = data[1]; break;
		case ECU_CMD_RESET:	 		/* Process data */ break;
		case ECU_CMD_SLEEP:  		/* Process data */ break;
		// ...

		default: /* Request type not found */ break;
	}
}
