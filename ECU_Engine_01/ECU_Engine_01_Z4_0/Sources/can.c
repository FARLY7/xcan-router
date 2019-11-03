/*
 * can.c
 *
 *  Created on: 25 Jan 2018
 *      Author: B49855
 */

#include <string.h>
#include <stdio.h>
#include "can.h"
#include "ecu.h"


#define TIMEOUT 1000 /* 1 second */

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
    CAN_ConfigRxBuff(instance, RX_MB_CMD,	&buffConfig, CAN_ID_MASK_TYPE_CMD | ecu.id); /* WARNING: Should OR with ECU ID */
    CAN_ConfigRxBuff(instance, RX_MB_FW,    &buffConfig, CAN_ID_MASK_TYPE_FW  | ecu.id);	/* WARNING: Should OR with ECU ID */

    /* Configure Rx MB filters */
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_DATA, 	CAN_ID_MASK_TYPE_DATA);
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_CMD,	CAN_ID_MASK_TYPE_CMD | ecu.id); 	/* WARNING: Should OR with ECU ID */
    CAN_SetRxFilter(instance, CAN_MSG_ID_EXT, RX_MB_FW,   	CAN_ID_MASK_TYPE_FW  | ecu.id);	/* WARNING: Should OR with ECU ID */

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
void CAN_eventHandler(uint8_t instance, can_event_t eventType, uint32_t objIdx, void *flexcanState)
{
	switch(eventType) {

		case CAN_EVENT_RX_COMPLETE:
		{
			if(objIdx == RX_MB_DATA) /* Received CAN DATA frame */
			{

			}
			else if(objIdx == RX_MB_CMD) /* Received CAN Command frame */
			{
				cmd_received = user_rxBuff.data[0];
			}
			if(objIdx == RX_MB_FW) /* Received CAN Firmware frame */
			{

			}

			can_instance_t can_instance = {
					.instIdx = instance,
					.instType = CAN_INST_TYPE_FLEXCAN
			};

			/* Re-enable Rx interrupt for CAN Interface */
			CAN_Receive(&can_instance, objIdx, &user_rxBuff);

			break;
		}
		case CAN_EVENT_TX_COMPLETE: break;
		default: break;
	}
}

/*
 * @brief: Send data via CAN to the specified mailbox with the specified message id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void CAN_sendFrame(const can_instance_t *instance, uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len)
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
    CAN_SendBlocking(instance, mailbox, &sendMsg, 1000U);
}


