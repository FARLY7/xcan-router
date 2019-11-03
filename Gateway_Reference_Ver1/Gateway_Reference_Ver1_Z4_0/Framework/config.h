/*
 * config.h
 *
 *  Created on: 27 Mar 2018
 *      Author: B49855
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "ecu.h"
#include "pdu_router.h"
#include "task.h"

/* Number of ECUs connected to the GW */
#define NUM_OF_ECUS 				4U
/* Number of PDU descriptor entries in the PDU routing table */
#define NUM_OF_PDU_TYPES 			5U
/* Number of CAN MSG ID filter entries in the filter table */
#define NUM_OF_CANID_FILTERS		10U
/* Number of PDU elements held in PDU router queue */
#define PDU_QUEUE_SIZE 				5U
/* Number of PDU elements held in GW queue */
#define GW_QUEUE_SIZE 				10U
/* Number of PDU elements held in FW update queue */
#define FW_QUEUE_SIZE				10U
/* Enable debug functionality via Serial terminal. PDUs printed, ECUs printed. */
#define DEBUG_ENABLE

/* HSM/SHE Configuration */
/* Boot MAC key used in Secure Boot */
#define BOOT_MAC_KEY_VALUE	{0x6B, 0x65, 0x79, 0x30, 0x6B, 0x65, 0x79, 0x31, 0x6B, 0x65, 0x79, 0x32, 0x6B, 0x65, 0x79, 0x33}
/* Key 1 - Used for encrypting FW frames before sending to ECU */
#define KEY_1_VALUE			{0x6B, 0x65, 0x79, 0x30, 0x6B, 0x65, 0x79, 0x31, 0x6B, 0x65, 0x79, 0x32, 0x6B, 0x65, 0x79, 0x33}

/* Priority of PDU Router task */
#define	TASK_PRIORITY_PDU 		(tskIDLE_PRIORITY + 2)
/* Priority of Gateway task */
#define	TASK_PRIORITY_GW 		(tskIDLE_PRIORITY + 2)
/* Priority of LWIP task */
#define TASK_PRIORITY_LWIP 		(tskIDLE_PRIORITY + 1)
/* Priority of Heartbeat task */
#define TASK_PRIORITY_HEART 	(tskIDLE_PRIORITY + 2)
/* Priority of Heartbeat task */
#define TASK_PRIORITY_CONSOLE 	(tskIDLE_PRIORITY + 1)


/* Master list of information for each ECU. It is held in RAM and periodically backed up to SD card */
ECU ecu_list[NUM_OF_ECUS];

/* PDU Routing table */
PDU_Desc PDU_Descriptors[NUM_OF_PDU_TYPES];

/* SW filter for CAN IDs */
uint32_t CAN_ID_FilterTable[NUM_OF_CANID_FILTERS];


#endif /* CONFIG_H_ */
