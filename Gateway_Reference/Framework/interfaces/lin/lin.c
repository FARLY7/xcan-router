/*
 * lin.c
 *
 *  Created on: 6 Feb 2018
 *      Author: B49855
 */

#include <lin.h>
#include "Cpu.h"

#define TIMEOUT 5000

/*
 * @brief: Dedicated thread to handle any LIN communication received
 * @param pvParameters	: Parameters
 * @return          	: none
 */
void LIN_Task(void* pvParameters)
{

	while(1)
	{

	}

	//vTaskDelete(NULL);
}


void LIN_initialise(uint32_t instance, linflexd_uart_state_t *uartStatePtr,
        			const linflexd_uart_user_config_t *uartUserConfig)
{
	LINFLEXD_UART_DRV_Init(INST_LIN_0, &lin_0_State, &lin_0_InitConfig0);
}

/*
 * @brief: Implementation requesting information from ECU on LIN network
 * @param ecu : Pointer to ECU struct
 * @param fw  : Type of request
 * @return    : STATUS_SUCCESS, STATUS_ERROR
 */
void LIN_sendRequest(ECU *ecu, ecu_req req)
{
	uint8_t rxBuff[30];

	/* Send Request */
	LINFLEXD_UART_DRV_SendDataBlocking(ecu->interface, (void*) req, sizeof(req), TIMEOUT);
	/* WAIT for response */
	LINFLEXD_UART_DRV_ReceiveDataBlocking(ecu->interface, rxBuff, sizeof(rxBuff), TIMEOUT);

	LIN_ParseResponse(rxBuff, sizeof(rxBuff));
}

/*
 * @brief: Implementation for updating ECU firmware over LIN with LIN bootloader
 * @param ecu   		: Pointer to ECU struct
 * @param fw 			: Pointer to FW File to use
 * @param secureUpdate  : Secure update (yes/no)
 * @return          	: STATUS_SUCCESS, STATUS_ERROR
 */
status_t LIN_updateFirmware(ECU *ecu, FIL *fw, bool secureUpdate)
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
