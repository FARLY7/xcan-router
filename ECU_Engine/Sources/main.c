/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "pin_mux.h"
#include "clockMan1.h"
#include "dmaController1.h"
#include "can_pal1.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "ecu.h"
#include "can.h"

#define LED_PORT        PTA
#define LED0             0U
#define LED1             4U

volatile int exit_code = 0;

//  Enable when CUT3.0 arrives:
//	#define FEATURE_CAN_HAS_ISOCANFDEN_ENABLE 1

ECU ecu = {
		.id = 0,
		.fw_version = 1,
		.status = ECU_STAT_NORMAL
};

int main(void)
{
  /* Write your local variable definition here */
  uint32_t i;
  status_t status;

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

    /* Initialize LED1 and FlexCAN RX and TX pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* Set Output value LEDs */
    PINS_DRV_SetPins(LED_PORT, (1 << LED0) | (1 << LED1));

    status = ADC_Init(&adc_pal1_instance, &adc_pal1_InitConfig0);
    DEV_ASSERT(status == STATUS_SUCCESS);


	uint32_t msgId;
    uint8_t rpm = 0;
    cmd_received = 0;

    CAN_initialise(INST_CAN_PAL1, &can_pal1_Config0);

    while(1)
    {
		/* Start the selected SW triggered group of conversions */
		status = ADC_StartGroupConversion(&adc_pal1_instance, 0);

		memcpy(&rpm, adc_pal1_Results00[0], sizeof(rpm));


		msgId = ENGINE_RPM | CAN_ID_MASK_TYPE_DATA;
		CAN_sendFrame(INST_CAN_PAL1, TX_MB, msgId, &rpm, sizeof(rpm));

    	for(i = 0 ; i < 1000 ; i++)
    	{
    		processCommand();
    	}

    	PINS_DRV_TogglePins(LED_PORT, (1 << LED0));
    	PINS_DRV_TogglePins(LED_PORT, (1 << LED1));
    }

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


void processCommand()
{
    uint8_t data[2];
	uint32_t msgId;

	switch(cmd_received)
	{
		case ECU_CMD_REQ_ALL: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;
			CAN_sendFrame(INST_CAN_PAL1, TX_MB, user_rxBuff.id, &ecu, sizeof(ecu));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_ID: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_ID;

			uint8_t ecu_id = ecu.id;
			memcpy(&data[1], &ecu_id, sizeof(ecu_id));

			CAN_sendFrame(INST_CAN_PAL1, TX_MB, msgId, &data , sizeof(data));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_FWVER:	{
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_FWVER;
			uint8_t fw_ver = ecu.fw_version;
			memcpy(&data[1], &fw_ver, sizeof(fw_ver));

			CAN_sendFrame(INST_CAN_PAL1, TX_MB, msgId, &data, sizeof(data));
			cmd_received = 0;
			break;
		}
		case ECU_CMD_REQ_STATUS: {
			msgId = CAN_ID_MASK_TYPE_CMD | ecu.id;

			data[0] = ECU_CMD_REQ_STATUS;
			uint8_t status = ecu.status;

			memcpy(&data[1], &status, sizeof(status));

			CAN_sendFrame(INST_CAN_PAL1, TX_MB, msgId, &data, sizeof(data));
			cmd_received = 0;
			break;
		}

		case ECU_CMD_RESET:	/* Perform reset */ break;
		case ECU_CMD_SLEEP: /* Perform sleep */ break;
	}
}



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
