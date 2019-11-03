/*
 * ecu.h
 *
 *  Created on: 22 Nov 2017
 *      Author: B49855
 */

#ifndef ECU_H_
#define ECU_H_

#include <stdint.h>
#include <stdbool.h>
#include "status.h"


/* Each ECU within the network is defined by this structure */
typedef struct
{
	uint32_t id:		 8;	/* ECU ID (max 256)*/
	uint32_t status:	 4;	/* Normal, Sleeping, Updating, Corrupted, Unresponsive, Faulty */
	uint32_t endianness: 1;	/* 1 = little endian, 0 = big endian */
	uint32_t security:	 1;	/* Supports HW security? */
	uint32_t protocol:	 4;	/* Communication protocol (CAN, LIN, ETH, ..) */
	uint32_t interface:	 4;	/* Peripheral interface (CAN0, CAN1, ..) */
	uint32_t fw_version: 8;	/* Current FW version (max 256) */
	uint32_t reserved:	 4; /* Reserved */
} ECU;

/* Set of requests for information that can be made to ECUs */
typedef enum {
	ECU_CMD_REQ_ALL	= 0x01, /* All ECU information */
	ECU_CMD_REQ_ID, 		/* ECU ID */
	ECU_CMD_REQ_FWVER, 		/* FW version */
	ECU_CMD_REQ_STATUS, 	/* Status */
	ECU_CMD_RESET, 			/* Reset */
	ECU_CMD_SLEEP			/* Sleep*/
	// ...
} ecu_cmd;

typedef enum {
	ECU_STAT_NORMAL = 0x01,	/* Normal/Operational */
	ECU_STAT_SLEEPING,		/* Sleeping state */
	ECU_STAT_UPDATING,		/* Updating FW */
	ECU_STAT_CORRUPTED,		/* FW Update corrupted */
	ECU_STAT_UNRESPONSIVE,	/* Not responding */
	ECU_STAT_FAULT			/* Faulty */
	// ...
} ecu_status;

/* Set of flags used for firmware update protocol */
typedef enum {
	RESET_CMD 	 = 0xFF,
	RESET_ACK 	 = 0xC4,
	BEGIN_SEC 	 = 0xFC,
	BEGIN_NONSEC = 0xFA,
	FRAME_ACK 	 = 0xC3,
	END_CMD 	 = 0xFD,
	END_ACK		 = 0xC1,
} fw_protocol_flag;


typedef struct {
	uint32_t 	engine_temp;
	uint16_t 	engine_rpm;
	uint8_t 	handbrake;
	uint8_t 	ignition;
} Cluster;

Cluster cluster;
ECU ecu;

uint8_t cmd_received;

#endif /* ECU_H_ */
