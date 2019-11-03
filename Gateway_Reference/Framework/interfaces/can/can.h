/*
 * can.h
 *
 *  Created on: 25 Jan 2018
 *      Author: B49855
 */

#ifndef CAN_H_
#define CAN_H_

#include <stdbool.h>
#include "Cpu.h"
#include "pdu_router.h"
#include "ecu.h"


/* CAN Frame Sizes */
#define FW_DATA_SIZE 		48U
#define FW_CMAC_SIZE 		16U
#define MSG_SIZE (FW_DATA_SIZE + FW_CMAC_SIZE)

/* Mail boxes */
//#define TX_FW_MB 	1
//#define RX_FW_MB 	2
//#define TX_INFO_MB	3
//#define RX_INFO_MB 	4
#define TX_MB		0U
#define RX_MB		1U


/* CAN MSG ID Masks */
#define CAN_MSG_ID_MASK_TYPE_FW		0x00010000 /* FW Frames */
#define CAN_MSG_ID_MASK_TYPE_INFO	0x00020000 /* Info request/response Frames */
#define CAN_MSG_ID_MASK_ECU_ID		0x0000FF00
#define CAN_MSG_ID_MASK_TYPE2 		0x000000FF

/*
 * @brief: Initialise CAN peripheral
 * @param instance : Instance of CAN to initialise
 * @param *config  : Pointer to user configuration struct
 * @return     	   : none
 */
void CAN_initialise(uint8_t instance, const can_user_config_t *config);

/*
 * @brief: CAN Event Handler (ISR). This is invoked on any CAN Tx/Rx activity.
 * 		 : It is of type can_event_t.
 * @param instance  	 : CAN peripheral instance
 * @param eventType  	 : CAN event
 * @param objIdx  		 : Message Buffer ID
 * @param *flexcanState  : Pointer to CAN state
 * @return     			 : none
 */
void CAN_eventHandler(uint8_t instance, can_event_t eventType, uint32_t objIdx, void *flexcanState);

/*
 * @brief: CAN Frame ID filtering implemented in software
 * @param *frame  	 : CAN peripheral instance
 * @return     		 : True  (if frame accepted)
 * 					 : False (if frame rejected)
 */
bool CAN_filterFrames(can_message_t *frame);

/*
 * @brief: Build PDU from CAN frame and pass to PDU router
 * @param *frame  : Pointer to CAN frame
 * @return     	  : none
 */
void CAN_packagePDU(can_message_t *response);

/*
 * @brief: Implementation to request data from ECU over CAN
 * @param ecu  : Pointer to ECU struct
 * @param req  : Type of request
 * @return     : none
 */
void CAN_sendRequest(uint8_t ecu_id, ecu_req req);

/*
 * @brief: Implementation for updating ECU firmware over CAN with CAN bootloader
 * @param ecu   		: Pointer to ECU struct
 * @param fw 			: Pointer to FW File to use
 * @param secureUpdate  : Secure update (yes/no)
 * @return          	: STATUS_SUCCESS, STATUS_ERROR
 */
status_t CAN_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate);

/*
 * @brief: Send data via CAN to the specified mailbox with the specified message id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void CAN_sendFrame(can_instance_t instance, uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len);

void CAN_sendPDU(uint8_t ecu_id, PDU *pdu);

/* User buffer containing CAN frame currently processed */
can_message_t user_rxBuff;

#endif /* CAN_H_ */
