/*
 * lin.c
 *
 *  Created on: 6 Feb 2018
 *      Author: B49855
 */

#include <lin.h>
#include "Cpu.h"
#include "config.h"

#define TIMEOUT 1000

uart_instance_t uart_ip_type;


void LIN_initialise(const uart_instance_t *instance, const uart_user_config_t *config)
{
	UART_Init(instance, config);

	/* Save the UART IP type used by the device */
	uart_ip_type.instType = instance->instType;
}

/*
 * @brief: Implementation requesting information from ECU on LIN network
 * @param ecu : Pointer to ECU struct
 * @param fw  : Type of request
 * @return    : STATUS_SUCCESS, STATUS_ERROR
 */
void LIN_sendFrame(uint32_t instance, uint32_t messageId, uint8_t * data, uint32_t len)
{
	uint8_t txBuff[30];

	uart_instance_t uart_instance = {
			.instType = uart_ip_type.instType,
			.instIdx = instance
	};


	//UART_SetTxBuffer(&uart_instance, txBuff, sizeof(txBuff));
	// or
	//UART_SendDataBlocking(&uart_instance, txBuff, sizeof(txBuff), TIMEOUT);

}

/*
 * @brief: Implementation for updating ECU firmware over LIN with LIN bootloader
 * @param ecu   		: Pointer to ECU struct
 * @param fw 			: Pointer to FW File to use
 * @param secureUpdate  : Secure update (yes/no)
 * @return          	: STATUS_SUCCESS, STATUS_ERROR
 */
status_t LIN_updateFirmware(uint8_t ecu_id, FIL *fw, bool secureUpdate)
{
	return 0;
}


/*
 * @brief: Parse ECU response to extract information
 * @param response  : Pointer to response to be parsed
 * @return          : non
 */
void LIN_parseResponse(uint8_t *response, uint32_t length)
{

}
