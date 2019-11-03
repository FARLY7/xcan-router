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
	xTaskCreate(Gateway_Task,	"Gateway", 		configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_GW,  NULL);

	/* Add Gateway Heartbeat task to the FreeRTOS scheduler */
	//xTaskCreate(Heartbeat_Task,	"GW_Heartbeat", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_HEART,  NULL);
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
			if(pdu.id & CAN_MSG_ID_MASK_TYPE_INFO)
			{
#ifdef DEBUG_ENABLE
				CONSOLE_printPDU(&pdu);
#endif
				/* Extract information from PDU and save to ECU struct */
				GW_parsePDU(&pdu);
			}
			else if(pdu.id & CAN_MSG_ID_MASK_TYPE_FW)
			{
				/* Send PDU directly to FW update process */
				xQueueSendToBack(fw_update_queue, &pdu, 0);
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
	const TickType_t xFrequency = portTICK_PERIOD_MS * 1000; /* Frequency = 1sec */

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
				ECU_sendRequest(i, ECU_REQ_STATUS);
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
	uint8_t ecu_id 	 = (pdu->id & CAN_MSG_ID_MASK_ECU_ID) >> 8; /* Extract ECU ID from MSG ID field */
	uint8_t req_type = (pdu->id & CAN_MSG_ID_MASK_TYPE2); /* Extract Request type from MSG ID field */
	uint8_t *data 	 = (pdu->data);

	//ECU *ecu = ECU_GetECU(ecu_id);

	///ECU *ecu =

	switch(req_type)
	{

//		case ECU_REQ_ALL: {
//
//			ecu->id = data;
//			ecu->fw_version = (data + 4);
//
//			memcpy(&(ecu->id), 	   data,  										 sizeof(ecu->id));
//			memcpy(&(ecu->fw_version), data + sizeof(ecu->id),  					 sizeof(ecu->fw_version));
//			//memcpy(&(ecu->health), data + sizeof(ecu->id) + sizeof(ecu->fw_ver), sizeof(ecu->health));
//			break;
//		}
//		case ECU_REQ_ID: 	 memcpy(&(ecu->id), 	data, sizeof(ecu->id));     break;
//		case ECU_REQ_FWVER:  memcpy(&(ecu->fw_version), data, sizeof(ecu->fw_version)); break;
		//case ECU_REQ_HEALTH: memcpy(&(ecu->health), data, sizeof(ecu->health)); break;
		// ...

//		case ECU_REQ_ALL: {
//
//			ecu->id = data;
//			ecu->fw_version = (data + 4);
//
//			break;
//		}
//		case ECU_REQ_ID: 	 ecu->id 		 = (uint8_t) data; break;
//		case ECU_REQ_FWVER:  ecu->fw_version = (uint8_t) data; break;
//		case ECU_REQ_STATUS: ecu->status 	 = (uint8_t) data; break;


		default: /* Request type not found */ break;
	}
}
