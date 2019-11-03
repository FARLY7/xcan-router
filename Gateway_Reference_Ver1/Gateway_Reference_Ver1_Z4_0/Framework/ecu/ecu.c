/*
 * ecu.c
 *
 *  Created on: 24 Nov 2017
 *      Author: B49855
 */

#include "ecu.h"
#include "can.h"
#include "lin.h"
#include "ethernet.h"
#include "config.h"

/*
 * @brief: Interface for updating an ECU's firmware
 * @param ecu   	   : Pointer to ECU struct
 * @param fw 		   : Pointer to FW File to use
 * @param secureUpdate : Secure update (yes/no)
 * @return             : STATUS_SUCCESS, STATUS_ERROR
 */
status_t ECU_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate)
{
	switch(ecu_list[ecu_id].protocol) {
		case PROTOCOL_CAN: return CAN_updateFirmware(ecu_id, fw, secureUpdate); break;
		case PROTOCOL_LIN: return LIN_updateFirmware(ecu_id, fw, secureUpdate); break;
		case PROTOCOL_ETH: return ETH_updateFirmware(ecu_id, fw, secureUpdate); break;
		default: return STATUS_ERROR; break;
	}
}

/*
 * @brief: Interface for sending a command to ECU through correct peripheral
 * @param ecu : Pointer to ECU struct
 * @param req : Type of request
 * @return    : none
 */
void ECU_sendCommand(uint8_t ecu_id, ecu_cmd cmd)
{
	uint32_t msgId = CAN_ID_MASK_TYPE_CMD | ecu_id; /* ECU ID + Request Type */
	uint8_t data = cmd;

	switch(ecu_list[ecu_id].protocol) {
		case PROTOCOL_CAN: CAN_sendFrame(ecu_list[ecu_id].instance, msgId , &data, sizeof(data)); break;
		case PROTOCOL_LIN: LIN_sendFrame(ecu_list[ecu_id].instance, msgId , &data, sizeof(data)); break;
		//case PROTOCOL_ETH: ETH_sendFrame(); break;
		default: break;
	}
}

/*
 * @brief: Interface for sending a PDU to ECU through correct peripheral
 * @param ecu : Pointer to ECU struct
 * @param req : Type of request
 * @return    : none
 */
void ECU_sendPDU(uint8_t ecu_id, PDU *pdu)
{
	switch(ecu_list[ecu_id].protocol) {
		case PROTOCOL_CAN: CAN_sendFrame(ecu_list[ecu_id].instance, pdu->id, pdu->data, pdu->length); break;
		case PROTOCOL_LIN: LIN_sendFrame(ecu_list[ecu_id].instance, pdu->id, pdu->data, pdu->length); break;
		//case PROTOCOL_ETH: ETH_SendPDU(ecu_id, pdu); break;
		default: break;
	}
}

/*
 * @brief: Returns a pointer to ECU struct for given ECU ID
 * @param ecu_id : ID of ECU
 * @return       : Pointer to ECU within ecu_list
 */
ECU* ECU_getECU(uint8_t ecu_id)
{
	return &ecu_list[ecu_id];
}
