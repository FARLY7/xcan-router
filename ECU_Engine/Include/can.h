/*
 * can.h
 *
 *  Created on: 25 Jan 2018
 *      Author: B49855
 */


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

/* CAN Frame Sizes */
#define FW_DATA_SIZE 		48U
#define FW_CMAC_SIZE 		16U
#define MSG_SIZE (FW_DATA_SIZE + FW_CMAC_SIZE)

/* Mail boxes */
#define TX_MB			0U
#define RX_MB_DATA		1U
#define RX_MB_CMD		2U
#define RX_MB_FW		3U

/* CAN MSG ID Masks */
#define CAN_ID_MASK_TYPE_DATA		0x10000000U
#define CAN_ID_MASK_TYPE_CMD		0x08000000U
#define CAN_ID_MASK_TYPE_FW			0x04000000U
#define CAN_ID_MASK_ECU_ID			0x000000FFU

typedef enum {
	ENGINE_RPM = 0,
	ENGINE_TEMP = 1,
} CAN_MSG_IDs;

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
 * @brief: Send data via CAN to the specified mailbox with the specified message id
 * @param mailbox   : Destination mailbox number
 * @param messageId : Message ID
 * @param data      : Pointer to the TX data
 * @param len       : Length of the TX data
 * @return          : None
 */
void CAN_sendFrame(can_instance_t instance, uint32_t mailbox, uint32_t messageId, uint8_t * data, uint32_t len);

/* User buffer containing CAN frame currently processed */
can_message_t user_rxBuff;

#endif /* CAN_H_ */
