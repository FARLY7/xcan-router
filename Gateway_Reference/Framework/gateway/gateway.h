/*
 * gateway.h
 *
 *  Created on: 26 Feb 2018
 *      Author: B49855
 */

#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "pdu_router.h"
#include "FreeRTOS.h"
#include "queue.h"


void Gateway_Task(void *pvParameters);
void Heartbeat_Task(void *pvParameters);

void GW_initialise(void);
void GW_parsePDU(PDU *pdu);

QueueHandle_t gateway_queue;

#endif /* GATEWAY_H_ */
