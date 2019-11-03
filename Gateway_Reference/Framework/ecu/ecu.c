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
 * @brief: Interface for sending a request to ECU
 * @param ecu : Pointer to ECU struct
 * @param req : Type of request
 * @return    : none
 */
void ECU_sendRequest(uint8_t ecu_id, ecu_req req)
{
	switch(ecu_list[ecu_id].protocol) {
		case PROTOCOL_CAN: CAN_sendRequest(ecu_id, req); break;
		case PROTOCOL_LIN: LIN_sendRequest(ecu_id, req); break;
		case PROTOCOL_ETH: ETH_sendRequest(ecu_id, req); break;
		default: break;
	}
}

/*
 * @brief: Interface for sending a PDU to ECU
 * @param ecu : Pointer to ECU struct
 * @param req : Type of request
 * @return    : none
 */
void ECU_sendPDU(uint8_t ecu_id, PDU *pdu)
{
	switch(ecu_list[ecu_id].protocol) {
		case PROTOCOL_CAN: CAN_sendPDU(ecu_id, pdu); break;
		//case PROTOCOL_CANFD: CAN_SendPDU(ecu_id, pdu); break;
		//case PROTOCOL_LIN: LIN_SendPDU(ecu_id, pdu); break;
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
//	uint8_t i;
//
//	/* REPLACE WITH BINARY SEARCH? */
//	for(i = 0 ; i < NUM_OF_ECUS; i++) {
//		if(ecu_list[i].id == ecu_id)
//			return &ecu_list[i];
//	}

	return &ecu_list[ecu_id];

	//return NULL;
}
