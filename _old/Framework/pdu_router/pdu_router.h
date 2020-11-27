/*
 * pdu_router.h
 *
 *  Created on: 22 Feb 2018
 *      Author: B49855
 */

#ifndef PDU_ROUTER_H_
#define PDU_ROUTER_H_

#include <stdbool.h>
#include "FreeRTOS.h"
#include "queue.h"

#define PDU_ID_MASK_TYPE_DATA		0x10000000U
#define PDU_ID_MASK_TYPE_CMD		0x08000000U
#define PDU_ID_MASK_TYPE_FW			0x04000000U
#define PDU_ID_MASK_ECU_ID			0x000000FFU

/* PDU object which is processed by the PDU Router */
typedef struct {
	uint32_t id; 	 /* PDU ID */
	uint8_t  length; /* Data length */
	uint8_t  *data;  /* Pointer to data */
} PDU;

/* The PDU_Desc struct defines a routing entry to the PDU routing table
 * which described how a PDU should be routed by the PDU router.
 */
typedef struct {
	char 	*label;		/* PDU label for user readability */
	uint8_t destCount;	/* Num of destinations of PDU */
	uint8_t *dest;		/* Pointer to list of ECU IDs to route PDU */
} PDU_Desc;


//typedef struct {
//	uint32_t pdu_id;	/* PDU ID which signal is to be copied too */
//	uint32_t position;	/* Position within the PDU where signal should be copied */
//} tSignalDestDescr;
//
//
//typedef struct {
//	uint32_t position;	/* Position of the signal within the PDU */
//	uint32_t size;		/* Size of the signal within the PDU */
//	tSignalDestDescr *destinations; /* List of destinations where signal should be copied */
//} tSignalDescr;
//
//typedef struct {
//
//	uint32_t id;	 /* ID of frame */
//	uint32_t length; /* Expected length of frame */
//
//	uint8_t num_of_signals; /* 0 = no signals (PDU routing) */
//	tSignalDescr *signalDesc;
//
//} tRxPDUDesc;
//
//typedef struct {
//	uint32_t id;
//	uint8_t *dest;
//
//} tTxPDUDescr;

/*
 * @brief: PDU Router (FreeRTOS task). This is responsible for recieving PDUs
 * 		   from the PDU queue and forwarding them to the correct ECU.
 * @param *pvParameters 	 : void parameter
 * @return     		 		 : none
 */
void PDU_Task(void *pvParameters);
/*
 * @brief: Initialise the PDU router by creating FreeRTOS task
 * 		   and creating the queues which hold PDUs.
 * @param void  	 : none
 * @return     		 : none
 */
void PDU_initialise(void);
/*
 * @brief: Convert PDU to string equivalent and print to debug console.
 * @param *PDU  	 : Pointer to PDU struct
 * @param tx		 : True = Transmit, False = Receive
 * @return     		 : none
 */
void PDU_printPDU(PDU *pdu, bool tx);

/*
 * @brief: This function frees the memory which was previously malloc()'d by interface.
 * 		   it is IMPORTANT this operation is not interrupted by a context switch
 * 		   or else an IVOR1 will be triggered.
 * @param *PDU  	 : Pointer to PDU struct
 * @return     		 : none
 */
void PDU_discardPDU(PDU *pdu);

/* FreeRTOS Queue object which holds all PDUs to be processed by PDU router */
QueueHandle_t PDU_queue;
/* FreeRTOS Queue object which holds all PDUs specific to FW update process */
QueueHandle_t fw_update_queue;


#endif /* PDU_ROUTER_H_ */
