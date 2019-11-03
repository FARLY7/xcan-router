/*
 * pdu_router.h
 *
 *  Created on: 22 Feb 2018
 *      Author: B49855
 */

#ifndef PDU_ROUTER_H_
#define PDU_ROUTER_H_

#include "FreeRTOS.h"
#include "queue.h"

#define PDU_MASK_REQRESP_TYPE 0x8000000

/* PDU structure defines properties of PDU processed by GW */
typedef struct {
	uint8_t  *data;  /* Pointer to data */
	uint32_t id; 	 /* PDU ID */
	uint8_t  length; /* Data length */
} PDU;

/* This struct represents a PDU which is shared across the network.
 * A collection of PDU_Desc structures create the PDU routing table
 * The PDU routing table describes to which ECU each PDU should be routed
 */
typedef struct {
	char 	*label;		/* PDU label for user readability */
	uint8_t destCount;	/* No of destinations for PDU */
	uint8_t *dest;		/* List of all ECU IDs to route PDU */
} PDU_Desc;

/* PDU Router task responsible for receiving PDUs and forwarding them to
 * correct peripherals/interfaces for sending */
void PDU_Task(void *pvParameters);

/* Initialise the PDU router and create PDU queues */
void PDU_initialise(void);

QueueHandle_t PDU_queue;
QueueHandle_t fw_update_queue;

#endif /* PDU_ROUTER_H_ */
