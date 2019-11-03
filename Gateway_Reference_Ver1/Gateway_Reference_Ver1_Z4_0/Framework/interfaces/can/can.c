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
#include "dbg_console.h"

#define TIMEOUT 1000 /* 1 second */

can_instance_t can_ip_type;

/*
 * @brief: Initialise CAN peripheral
 * @param instance : Instance of CAN to initialise
 * @param *config  : Pointer to user configuration struct
 * @return     	   : none
 */
void CAN_initialise(const can_instance_t *instance, const can_user_config_t *config)
{
    /* Initialize CAN PAL */
    CAN_Init(instance, config);

    /* Set the global CAN IP type */
    can_ip_type.instType = instance->instType;

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
    CAN_ConfigRxBuff(instance, RX_MB_DATA,  &buffConfig, CAN_ID_MASK_TYPE_DATA);
    CAN_ConfigRxBuff(instance, RX_MB_CMD,	&buffConfig, CAN_ID_MASK_TYPE_CMD);
    CAN_ConfigRxBuff(instance, RX_MB_FW,    &buffConfig, CAN_ID_MASK_TYPE_FW);

    /* Configure Rx MB filters */
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_DATA, 	CAN_ID_MASK_TYPE_DATA);
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_CMD,	CAN_ID_MASK_TYPE_CMD);
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_FW,   	CAN_ID_MASK_TYPE_FW);

    /* Enable Rx interrupts for MBs */
    CAN_Receive(instance, RX_MB_DATA,	&user_rxBuff);
    CAN_Receive(instance, RX_MB_CMD,	&user_rxBuff);
    CAN_Receive(instance, RX_MB_FW,		&user_rxBuff);
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
void CAN_eventHandler(uint32_t instance, can_event_t eventType, uint32_t objIdx, void *flexcanState)
{
	switch(eventType) {

		case CAN_EVENT_RX_COMPLETE:
		{
			/* Filter the incoming CAN frame using CAN ID Filter implementing in software */
			if(CAN_filterFrame(&user_rxBuff))
			{
				CAN_buildPDU(&user_rxBuff);
			}

			/* SDK BETA 0.9.0 BUG */
			can_instance_t can_instance = { instance, CAN_INST_TYPE_FLEXCAN };

			/* Re-enable Rx interrupt for CAN Interface */
			CAN_Receive(&can_instance, objIdx, &user_rxBuff);
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
bool CAN_filterFrame(can_message_t *frame)
{
	uint32_t i, tempId;

	tempId = frame->id & ~CAN_ID_MASK_TYPE_DATA;

	/*! Replace with effecient binary search !*/
	/* Any CAN frames found within the filter table are rejected */
	for(i = 0 ; i < (sizeof(CAN_ID_FilterTable) / sizeof(CAN_ID_FilterTable[0])) ; i++)
	{
		if(tempId == CAN_ID_FilterTable[i]) return true;
	}
	return true; /* WARNING: Just return true regardless right now. */
}


/*
 * @brief: Build PDU from CAN frame and pass to PDU router
 * @param *frame  : Pointer to CAN frame
 * @return     	  : none
 */
void CAN_buildPDU(can_message_t *frame)
{
	if(!xQueueIsQueueFullFromISR(PDU_queue))
	{
		PDU pdu;

		memset(&pdu, 0, sizeof(pdu)); /* Clear PDU */

		/*	**** CAN Frame ****	  |	 	*** PDU ***
		 * 	uint32_t cs;		  |
		 * 	uint32_t id; 		--|-->	uint32_t id;
		 * 	uint8_t  length; 	--|-->	uint8_t  length;
		 *  uint8_t  data[64]; 	--|-->	uint8_t  *data;
		 */

		pdu.id 		= frame->id;
		pdu.length  = frame->length;
		pdu.data 	= malloc(frame->length);
		//pdu.data 	= pvPortMalloc(frame->length);

		/* Memory is allocated for incoming frames using malloc()
		 * Copy the CAN frame payload to this memory pointer below.
		 */
		memcpy(pdu.data, &(frame->data), frame->length);

		if(errQUEUE_FULL == xQueueSendToBackFromISR(PDU_queue, (void*) &pdu, NULL))
		{
			/* Error: PDU queue is full. CAN frame is dropped at the interface.
			 * In this case, immediately free() the memory which was allocated.
			 */
			free(pdu.data);
		} else {
			/* Success: The CAN frame has been added to the PDU queue for processing.
			 * WARNING: Upper layers must free() the buffer space once PDU has been processed.
			 */
		}
	}
	else {
		/* Error: PDU queue is full. CAN frame is dropped at the interface. */
	}
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
	PDU pdu;

	uint32_t msgId = CAN_ID_MASK_TYPE_FW | ecu_id; /* ECU ID + FW Update Type */

	BaseType_t stat;
	UINT i, br; 	/* loop counter */

	uint8_t rnd[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Random number generated */
	uint8_t iv[16]  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Initialisation vector */

	uint32_t interface = ecu->instance;

	/*********************** FLEXCAN UPDATE FLOW **************************/
	/**********************************************************************/
	/*********************** 1. Send ECU reset ****************************/
	buffer[0] = RESET_CMD;
	CAN_sendFrame(interface, msgId, buffer, 1);

	/*********************** 2. Wait for UP_RESET ACK *********************/
	stat = xQueueReceive(fw_update_queue, &pdu, TIMEOUT);
	if(stat != pdTRUE || pdu.data[0] != RESET_ACK)
		return stat;
	free(pdu.data);
	/*********************** 3. Begin FW Update ***************************/
	if(secureUpdate) buffer[0] = BEGIN_SEC;
	else 			 buffer[0] = BEGIN_NONSEC;

	CAN_sendFrame(interface, msgId, buffer, 1);

	/*********************** 4. Wait for Random Number ********************/
	stat = xQueueReceive(fw_update_queue, &pdu, TIMEOUT);
	if(stat != pdTRUE)
		return stat;

	if(secureUpdate)
	{
		memcpy(rnd, pdu.data, sizeof(rnd));
	}
	else
	{
		if(pdu.data[0] != 0xFF) /* ERROR */
			return stat;
	}
	free(pdu.data);

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
		SECURITY_EncryptCbcBlocking(SECURITY_INSTANCE0, SECURITY_KEY_1, buffer, sizeof(buffer), iv, buffer, TIMEOUT);
		//HSM_DRV_EncryptCBC(HSM_KEY_1, buffer, sizeof(buffer), iv, buffer, TIMEOUT);

		/* Send 1 CAN Frame containing FW */
		CAN_sendFrame(interface, msgId, buffer, MSG_SIZE);

		/* Wait for ACK */
		stat = xQueueReceive(fw_update_queue, &pdu, TIMEOUT);
		if(stat != pdTRUE || pdu.data[0] != FRAME_ACK) /* ERROR */
			return stat;
		free(pdu.data);
		f_read(fw, &buffer[0], FW_DATA_SIZE, &br); /* Read 48 bytes (or less) from file */
	};

	/*********************** 6. Send FW End Flag **************************/
	buffer[0] = END_CMD;
	CAN_sendFrame(interface, msgId, buffer, 1);

	stat = xQueueReceive(fw_update_queue, &pdu, TIMEOUT);
	if(stat != pdTRUE || pdu.data[0] != END_ACK) /* ERROR */
		return stat;
	free(pdu.data);
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
void CAN_sendFrame(uint32_t instance, uint32_t messageId, uint8_t * data, uint32_t len)
{
	status_t stat;

    can_buff_config_t buffConfig2 =
    {
        .enableFD = true,
        .enableBRS = true,
        .fdPadding = 0U,
        .idType = CAN_MSG_ID_EXT,
        .isRemote = false
    };

    can_instance_t can_instance = {
    		.instIdx = instance,
    		.instType = can_ip_type.instType
    };

    can_message_t sendMsg = {
      .id 	  = messageId,
      .length = len
    };

    /* Copy data into can_message_t data buffer */
    memcpy(sendMsg.data, data, len);

    /* Configure Rx MBs */
    CAN_ConfigTxBuff(&can_instance, TX_MB, &buffConfig2);

    /* Send data using buffer configured for transmission */
    stat = CAN_SendBlocking(&can_instance, TX_MB, &sendMsg, 1U); /* Timeout 100ms */

#ifdef DEBUG_ENABLE
    switch(stat)
    {
    	case STATUS_SUCCESS: CAN_printFrame(&can_instance, &sendMsg, true); break;
		default: CAN_errorHandler(&can_instance, TX_MB, &sendMsg, stat); break;
    }
#endif
}

/*
 * @brief: CAN Error Handler. Called anytime CAN send frame API call returns non-SUCCESS
 * @param instance  : CAN Instance
 * @param mailbox 	: MB ID
 * @param *msg      : can_message_t pointer
 * @param stat      : status
 * @return          : None
 */
void CAN_errorHandler(const can_instance_t *instance, uint32_t mailbox, can_message_t *msg, status_t stat)
{
	/* In this error handler you may want to:
	 * 1. Count the number of errors.
	 * 2. Time the occurancy of errors
	 * 3. Act on these errors, reset/cooldown/escalate?
	 */

	/* Just print an error message for now */
	CAN_printError(instance, msg, stat);
}

/*
 * @brief: Constructs error message to print on debug console.
 * @param instance  : CAN Instance
 * @param *msg      : can_message_t pointer
 * @param stat      : status
 * @return          : None
 */
void CAN_printError(const can_instance_t *instance, can_message_t *msg, status_t stat)
{
	char buff[100];
	char *status;
	uint8_t i;

    switch(stat)
    {
		case STATUS_BUSY: 	 status = "BUSY    ";	  break;
		case STATUS_TIMEOUT: status = "TIMEOUT ";  break;
		case STATUS_ERROR:	 status = "ERROR   "; 		  break;
		case STATUS_CAN_BUFF_OUT_OF_RANGE: status = "MB OUT OF RANGE"; break;
		default: 		 	 status = "N/A"; break;
    }

	sprintf(buff, "\n\r[CAN%d][Tx] %s\t0x%08X\t0x%02X\t", instance->instIdx, status, msg->id, msg->length);
	CONSOLE_printError(buff);

	for (i = 0; i < msg->length; i++)
	{
		sprintf(&buff[i*3], "%02X ", msg->data[i]);
	}
	CONSOLE_printError(buff);
}

/*
 * @brief: CAN frame toString() and print to debug console.
 * @param instance  : CAN Instance
 * @param *msg      : can_message_t pointer
 * @param tx      	: true = Transmit, false = Receive
 * @return          : None
 */
void CAN_printFrame(const can_instance_t *instance, can_message_t *msg, bool tx)
{
	char buff[50 + (msg->length)*3];
	char *dir;
	char *colour;
	char *label;
	uint8_t i;

	switch(tx) /* Set direction of message */
	{
		case true:  dir = "Tx"; colour = "\e[33m"; break; /* Yellow foreground */
		case false: dir = "Rx"; colour = "\e[0m"; break;  /* White foreground */
	}

	uint32_t ecuId = msg->id & PDU_ID_MASK_ECU_ID;

	if(msg->id & CAN_ID_MASK_TYPE_DATA) {
		label = PDU_Descriptors[ecuId].label;
	}
	else if(msg->id & CAN_ID_MASK_TYPE_CMD) {
		label = "COMMAND  ";
	}
	else if(msg->id & CAN_ID_MASK_TYPE_FW) {
		label = "FIRMWARE  ";
	}
	else {
		label = "N/A";
	}

	sprintf(buff, "\n\r%s[CAN%d][%s] %s\t0x%08X\t0x%02X\t", colour, instance->instIdx, dir, label, msg->id, msg->length);
	CONSOLE_printString(buff);

	for (i = 0; i < msg->length; i++)
	{
		sprintf(&buff[i*3], "%02X ", msg->data[i]);
	}
	CONSOLE_printString(buff);
}

