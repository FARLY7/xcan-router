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

#include "pdu_router.h"
#include "status.h"
#include "ff.h"

/* Each ECU within the network is defined by this structure
 * NOTE: The ID of the ECU is implicitly defined by its index within the ecu_list array */
typedef struct
{
//	uint32_t id:		 8;	/* ECU ID (max 256)*/
	uint32_t status:	 4;	/* Normal, Sleeping, Updating, Corrupted, Unresponsive, Faulty */
	uint32_t endianness: 1;	/* 1 = big endian, 0 = little endian */
	uint32_t security:	 1;	/* Supports HW security? */
	uint32_t protocol:	 4;	/* Communication protocol (CAN, LIN, ETH, ..) */
	uint32_t interface:	 4;	/* Peripheral interface (CAN0, CAN1, ..) */
	uint32_t fw_version: 8;	/* Current FW version (max 256) */
	uint32_t reserved:	 4; /* Reserved */
} ECU;

/* ECU Communication Protocol types
 * These are used by communication interface to determine which protocol to use */
typedef enum {
	PROTOCOL_CANFD  = 0x01,
	PROTOCOL_CAN 	= 0x02,
	PROTOCOL_LIN 	= 0x03,
	PROTOCOL_ETH 	= 0x04,
	// ...
} protocols;

/* Set of requests for information that can be made to ECUs */
typedef enum {
	ECU_REQ_ALL	   	= 0x01, /* All ECU information */
	ECU_REQ_ID	   	= 0x02, /* ECU ID */
	ECU_REQ_FWVER  	= 0x03, /* Current FW version */
	ECU_REQ_STATUS 	= 0x04, /* MISC - Health (faults, status?) */
	ECU_REQ_RESET  	= 0x05, /* Reset request */
	ECU_REQ_SLEEP	= 0x06	/* Sleep request */
	// ...
} ecu_req;

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

//#define FW_HEADER_SIZE 	28u /* 28 bytes */
//#define FW_HEADER 		0x5757

/* Each FW file stored on SD begins with a FW header containing important info about FW */
typedef struct {
	uint32_t	opcode; 	/* Header/Packet opcode? */
	uint32_t 	fw_ver;		/* FW version no */
	uint32_t 	length;		/* Length of FW (bytes) */
	uint32_t 	ecu_id;		/* ECU ID the FW belongs to */
	uint8_t 	cmac[16];	/* CMAC of FW */
} ecu_fw_header;

/* Interface for updating the firmware of ECU connected to the gateway */
status_t ECU_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate);
/* Interface for sending a reques to an ECU connected to the gateway */
void ECU_sendRequest(uint8_t ecu_id, ecu_req req);
/* Returns a pointer to the ECU structure for the equivalent ECU ID */
ECU* ECU_getECU(uint8_t ecu_id);

void ECU_sendPDU(uint8_t ecu_id, PDU *pdu);


#endif /* ECU_H_ */
