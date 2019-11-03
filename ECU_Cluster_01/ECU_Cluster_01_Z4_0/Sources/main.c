/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "pin_mux.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "can_pal1.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "can.h"
#include "ecu.h"

#define LED_PORT        PTG
#define LED0             2U
#define LED1             3U

volatile int exit_code = 0;

void clearLEDs();
void processCommand();


ECU ecu = {
		.id = 2,
		.fw_version = 1,
		.status = ECU_STAT_FAULT
};

int main(void)
{
  /* Write your local variable definition here */
  uint32_t i;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    /* Initialize and configure clocks
     *  -   Setup system clocks, dividers
     *  -   Configure FlexCAN clock, GPIO, LPSPI
     *  -   see clock manager component for more details
     */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
            g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_FORCIBLE);

    cluster.engine_rpm = 0;
    cluster.engine_temp = 0;

    /* Initialize LED1 and FlexCAN RX and TX pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Set Output value LEDs */
    PINS_DRV_SetPins(LED_PORT, (1 << LED0) | (1 << LED1));

    CAN_Initialise(&can_pal1_instance, &can_pal1_Config0);

    cmd_received = 0;
    uint16_t rpm = 0;

    while(1)
    {
    	rpm = cluster.engine_rpm;

    	clearLEDs();

    	if(rpm >= 0)  	PINS_DRV_WritePin(PTA, 4,  0);
    	if(rpm > 511)  	PINS_DRV_WritePin(PTA, 0,  0);
    	if(rpm > 1024) 	PINS_DRV_WritePin(PTJ, 4,  0);
    	if(rpm > 1534) 	PINS_DRV_WritePin(PTH, 5,  0);
    	if(rpm > 2045) 	PINS_DRV_WritePin(PTC, 4,  0);
    	if(rpm > 2556) 	PINS_DRV_WritePin(PTH, 13,  0);
    	if(rpm > 3067) 	PINS_DRV_WritePin(PTA, 7, 0);
    	if(rpm > 3578) 	PINS_DRV_WritePin(PTA, 10,  0);

    	for(i = 0 ; i < 10000 ; i++)
    	{
    		processCommand();
    	}
    }



//    /* Receive message */
//    CAN_Receive(INST_CAN_PAL1, RX_BUFFER, &recvMsg);
//    /* Wait for message to be received */
//    while (CAN_GetTransferStatus(INST_CAN_PAL1, RX_BUFFER) == STATUS_BUSY);
//
//    /* Check if data was received correctly */
//    switch(recvMsg.id & CAN_MSG_ID_MASK_TYPE2) {
//
//		sendMsg.cs = 0;
//		sendMsg.id = recvMsg.id;
//
//		case ECU_REQ_ALL: {
//
//			memcpy(sendMsg.data, &ecu, 9);
//			sendMsg.length = 9;
//
//			break;
//		};
//
//		case ECU_REQ_ID: {break;};
//		case ECU_REQ_FWVER: {break;};
//		default: break;
//
//    };
//
//    /* Send message */
//    CAN_Send(INST_CAN_PAL1, TX_BUFFER, &sendMsg);

	//PINS_DRV_ClearPins(LED_PORT, (1 << LED0));
	//PINS_DRV_ClearPins(LED_PORT, (1 << LED1));

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/


void clearLEDs()
{
	PINS_DRV_WritePin(PTA, 4,  1);
	PINS_DRV_WritePin(PTA, 0,  1);
	PINS_DRV_WritePin(PTJ, 4,  1);
	PINS_DRV_WritePin(PTH, 5,  1);
	PINS_DRV_WritePin(PTC, 4,  1);
	PINS_DRV_WritePin(PTH, 13, 1);
	PINS_DRV_WritePin(PTA, 7,  1);
	PINS_DRV_WritePin(PTA, 10, 1);
}

void processCommand()
{
    uint8_t data[64];
	uint32_t msgId;

	switch(cmd_received)
	{
		case ECU_CMD_REQ_ALL: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;
			CAN_sendFrame(&can_pal1_instance, TX_MB, user_rxBuff.id, &ecu, sizeof(ecu));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_ID: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_ID;

			uint8_t ecu_id = ecu.id;
			memcpy(&data[1], &ecu_id, sizeof(ecu_id));

			CAN_sendFrame(&can_pal1_instance, TX_MB, msgId, data , sizeof(data));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_FWVER:	{
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_FWVER;
			uint8_t fw_ver = ecu.fw_version;
			memcpy(&data[1], &fw_ver, sizeof(fw_ver));

			CAN_sendFrame(&can_pal1_instance, TX_MB, msgId, data, sizeof(data));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_STATUS: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_STATUS;
			uint8_t status = ecu.status;

			memcpy(&data[1], &status, sizeof(status));

			CAN_sendFrame(&can_pal1_instance, TX_MB, msgId, data, sizeof(data));
			cmd_received = 0;
			break;
		}

		case ECU_CMD_RESET:	/* Perform reset */ break;
		case ECU_CMD_SLEEP: /* Perform sleep */ break;
	}
}

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP C55 series of microcontrollers.
**
** ###################################################################
*/
