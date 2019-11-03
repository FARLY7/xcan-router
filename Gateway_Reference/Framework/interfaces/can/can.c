/*
 * can.c
 *
 *  Created on: 25 Jan 2018
 *      Author: B49855
 */

#include <string.h>
#include <stdio.h>

#include "can.h"
#include "config.h"
#include "ff.h"
#include "queue.h"

#define TIMEOUT 1000 /* 1 second */

/*
 * @brief: Initialise CAN peripheral
 * @param instance : Instance of CAN to initialise
 * @param *config  : Pointer to user configuration struct
 * @return     	   : none
 */
void CAN_initialise(uint8_t instance, const can_user_config_t *config)
{
    /* Initialize CAN PAL */
    CAN_Init(instance, config);

    /* Install Callback/ISR for when CAN interface Tx/Rx */
    CAN_InstallEventCallback(instance, (can_callback_t) &CAN_eventHandler, NULL);

    /* Set buffer configuration
     *  - Flexible data rate enabled
     *  - Bit rate switch enabled to use a different bitrate for the data segment
     *  - Use zeros for FD padding
     *  - Standard message ID
     *  - Not remote frame
    */
    can_buff_config_t buffConfig =
    {
        .enableFD = true,
        .enableBRS = true,
        .fdPadding = 0U,
        .idType = CAN_MSG_ID_EXT,
        .isRemote = false
    };

    /* Configure Rx MBs */
    CAN_ConfigRxBuff(instance, RX_MB, &buffConfig, CAN_MSG_ID_MASK_TYPE_INFO);
    //CAN_ConfigRxBuff(instance, RX_FW_MB,   &buffConfig, CAN_MSG_ID_MASK_TYPE_FW);

    /* Configure Rx MB filters */
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB, 0); /* Accept all CAN frames */
    //CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_FW_MB,   CAN_MSG_ID_MASK_TYPE_FW);

    /* Enable Rx interrupts for MBs */
    CAN_Receive(instance, RX_MB, &user_rxBuff);
    //CAN_Receive(instance, RX_FW_MB,   &user_rxBuff);
}

/*
 * @brief: CAN Event Handler (ISR). This is invoked on any CAN Tx/Rx activity.
 * 		 : It is of type can_event_t.
 * @param instance  	 : CAN peripheral instance
 * @param eventType  	 : CAN event
 * @param objIdx  		 : Message Buffer ID
 * @param *flexcanState  : Pointer to CAN state
 * @return     			 : none
 */
void CAN_eventHandler(uint8_t instance, can_event_t eventType, uint32_t objIdx, void *flexcanState)
{
	switch(eventType) {

		case CAN_EVENT_RX_COMPLETE:
		{
			/* CAN frame implemented in software. Returns true if frame is accepted */
			if(CAN_filterFrames(&user_rxBuff))
			{
				/* Package CAN frame into PDU and pass to PDU router */
				CAN_packagePDU(&user_rxBuff);
			}

			/* Re-enable Rx interrupt for CAN Interface */
			CAN_Receive(instance, objIdx, &user_rxBuff);

			break;
		}
		case CAN_EVENT_TX_COMPLETE: break;
		default: break;
	}
}

/*
 * @brief: CAN Frame ID filtering implemented in software
 * @param *frame  	 : CAN peripheral instance
 * @return     		 : True  (if frame accepted)
 * 					 : False (if frame rejected)
 */
bool CAN_filterFrames(can_message_t *frame)
{
	uint32_t i;

	/*! Replace with effecient binary search !*/
	/* Any CAN frames found within the filter table are rejected */
	for(i = 0 ; i < (sizeof(CAN_ID_FilterTable) / sizeof(CAN_ID_FilterTable[0])) ; i++)
	{
		if(frame->id == CAN_ID_FilterTable[i]) return true;
	}
	return false;
}

/*
 * @brief: Build PDU from CAN frame and pass to PDU router
 * @param *frame  : Pointer to CAN frame
 * @return     	  : none
 */
void CAN_packagePDU(can_message_t *frame)
{
	PDU pdu;
	memset(&pdu, 0, sizeof(pdu));

	/*	**** CAN Frame ****	  |	 	*** PDU ***
	 * 	uint32_t cs;		  |
	 * 	uint32_t id; 		--|-->	uint32_t id;
	 *  uint8_t  data[64]; 	--|-->	uint8_t  data[64];
	 * 	uint8_t  length; 	--|-->	uint8_t  length;
	 */

	pdu.id 		= frame->id;
	pdu.length  = frame->length;
	pdu.data 	= malloc(frame->length);

	/* Copy payload to allocated memory location */
	memcpy(pdu.data, &(frame->data), frame->length);

	xQueueSendToBackFromISR(PDU_queue, (void*) &pdu, 0);

	free(pdu.data);
}

/*
 * @brief: Implementation to request data from ECU over CAN
 * @param ecu  : Pointer to ECU struct
 * @param req  : Type of request
 * @return     : none
 */
void CAN_sendRequest(uint8_t ecu_id, ecu_req req)
{
	ECU *ecu = &ecu_list[ecu_id];

	uint8_t txData = req; /* Empty payload */
	uint32_t msgId = CAN_MSG_ID_MASK_TYPE_INFO | (ecu_id << 8) |  req; /* ECU ID + Request Type */

	/******************** Send Request **********************/
	CAN_sendFrame(ecu->interface, TX_MB, msgId, &txData, 1);
}

/*
 * @brief: Implementation for updating ECU firmware over CAN with CAN bootloader
 * @param ecu   		: Pointer to ECU struct
 * @param fw 			: Pointer to FW File to use
 * @param secureUpdate  : Secure update (yes/no)
 * @return          	: STATUS_SUCCESS, STATUS_ERROR
 */
status_t CAN_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate)
{
	ECU *ecu = &ecu_list[ecu_id];

	uint8_t buffer[MSG_SIZE]; /* Buffer for CAN frame */
	PDU rxBuff;

	uint32_t msgId = CAN_MSG_ID_MASK_TYPE_FW | (ecu_id << 8); /* ECU ID + FW Update Type */

	BaseType_t stat;
	UINT i, br; 	/* loop counter */

	uint8_t rnd[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Random number generated */
	uint8_t iv[16]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Initialisation vector */

	uint32_t interface = ecu->interface;

	/*********************** FLEXCAN UPDATE FLOW **************************/
	/**********************************************************************/
	/*********************** 1. Send ECU reset ****************************/
	buffer[0] = RESET_CMD;
	CAN_sendFrame(interface, TX_MB, msgId, buffer, 1);

	/*********************** 2. Wait for UP_RESET ACK *********************/
	stat = xQueueReceive(fw_update_queue, &rxBuff, TIMEOUT);
	if(stat != pdTRUE || rxBuff.data[0] != RESET_ACK)
		return stat;

	/*********************** 3. Begin FW Update ***************************/
	if(secureUpdate) buffer[0] = BEGIN_SEC;
	else 			 buffer[0] = BEGIN_NONSEC;

	CAN_sendFrame(interface, TX_MB, msgId, buffer, 1);

	/*********************** 4. Wait for Random Number ********************/
	stat = xQueueReceive(fw_update_queue, &rxBuff, TIMEOUT);
	if(stat != pdTRUE)
		return stat;

	if(secureUpdate)
	{
		memcpy(rnd, rxBuff.data, sizeof(rnd));
	}
	else
	{
		if(rxBuff.data[0] != 0xFF) /* ERROR */
			return stat;
	}

	/*********************** 5. Send Firmware ****************************/
	f_read(fw, buffer, FW_DATA_SIZE, &br); /* Read 48 bytes from file */

	while(br != 0) /* Exit loop when sent last remaining packet */
	{
		for(i = (uint32_t) br ; i < FW_DATA_SIZE ; i++) { /* Pad packet with 0s if end of file */
			buffer[i] = 0;
		}

		for(i = 0 ; i < FW_CMAC_SIZE ; i++) /* Insert random number */
		{
			buffer[FW_DATA_SIZE + i] = rnd[i];
		}

		/* Encrypt CAN Frame */
		HSM_DRV_EncryptCBC(HSM_KEY_1, buffer, sizeof(buffer), iv, buffer, TIMEOUT);

		/* Send 1 CAN Frame containing FW */
		CAN_sendFrame(interface, TX_MB, msgId, buffer, MSG_SIZE);

		/* Wait for ACK */
		stat = xQueueReceive(fw_update_queue, &rxBuff, TIMEOUT);
		if(stat != pdTRUE || rxBuff.data[0] != FRAME_ACK) /* ERROR */
			return stat;

		f_read(fw, &buffer[0], FW_DATA_SIZE, &br); /* Read 48 bytes (or less) from file */
	};

	/*********************** 6. Send FW End Flag **************************/
	buffer[0] = END_CMD;
	CAN_sendFrame(interface, TX_MB, msgId, buffer, 1);

	stat = xQueueReceive(fw_update_queue, &rxBuff, TIMEOUT);
	if(stat != pdTRUE || rxBuff.data[0] != END_ACK) /* ERROR */
		return stat;

	/******************** FLEXCAN UPDATE FLOW FINISHED *******************/
	/*********************************************************************/
	return STATUS_SUCCESS;
}

/*
 * @brief: Send data via CAN to the specified mailbox with the specified message id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void CAN_sendFrame(can_instance_t instance, uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len)
{
    /* Buffer configuration */
    can_buff_config_t buffConfig = {
      .enableFD  = 1U,
      .enableBRS = 1U,
      .fdPadding = 0U,
      .idType 	 = CAN_MSG_ID_EXT,
      .isRemote  = false
    };

    can_message_t sendMsg = {
      .id 	  = messageId,
      .length = len
    };

    /* Copy data into can_message_t data buffer */
    memcpy(sendMsg.data, data, len);

    /* Configure mailbox for transmission */
    CAN_ConfigTxBuff(instance, mailbox, &buffConfig);

    /* Send data using buffer configured for transmission */
    CAN_Send(instance, mailbox, &sendMsg);
}


void CAN_sendPDU(uint8_t ecu_id, PDU *pdu)
{
	CAN_sendFrame(ecu_list[ecu_id].interface, TX_MB, pdu->id, pdu->data, pdu->length);
}

