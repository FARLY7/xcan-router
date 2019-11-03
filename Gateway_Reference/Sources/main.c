/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "Cpu.h"
#include "task.h"

/* User includes */
#include "config.h"
#include "ecu.h"
#include "hsm_she.h"
#include "sdhc.h"
#include "can.h"
#include "lin.h"
#include "pdu_router.h"
#include "gateway.h"
#include "console.h"

volatile int exit_code = 0;

//  Enable when CUT3.0 arrives:
//	#define FEATURE_CAN_HAS_ISOCANFDEN_ENABLE 1
#define	TASK_PRIORITY_APP 	(tskIDLE_PRIORITY + 1)

/* GPIO User Buttons & LEDs */
#define SW1_PORT PTA
#define SW1_PIN	 3
#define SW2_PORT PTE
#define SW2_PIN  10

/* FreeRTOS Thread Declarations */
void App_task(void* pvParameters);

/* Function Prototypes */
void initClocks();
void initPeripherals();
void keyOff();

/* Application demo task. When button A is pressed, GW will request updated information from all ECUs on network.
 * When button B is pressed, GW will update FW of all ECUs that have new FW in SD card
 */
void App_Task(void* pvParameters)
{
	//	uint32_t i;
	//	bool SW1, SW2;

	/* Wait for notify from other tasks ready before initialising peripherals? */

	/* Initialise all peripherals: CAN, LIN, HSM, ETH, .. */
	initPeripherals();

	while(1)
	{
		//		SW1 = (PINS_DRV_ReadPins(SW1_PORT) >> SW1_PIN) & 1;
		//		SW2 = (PINS_DRV_ReadPins(SW2_PORT) >> SW2_PIN) & 1;

		//		if(SW2)	/* Request latest ECU information from all nodes (FW version, health) */
		//		{
		//			for(i = 0 ; i < NUM_OF_ECUS ; i++)
		//			{
		//				ECU_sendRequest(&ecu_list[i], ECU_REQ_ALL); /* Request all information from ECU */
		//			}
		//		}

		//		if(SW2) /* FW update trigger. Update FW of all ECUs on network */
		//		{
		//			keyOff();
		//			while(1);
		//		}

		vTaskDelay(1000); /* Delay for 2 seconds? */
	}
}


int main(void)
{
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/

	/* Initialise device clocking */
	initClocks();

	/* Initialise GW layer & create FreeRTOS task */
	GW_initialise();
	/* Initialise PDU layer & create FreeRTOS task */
	PDU_initialise();

	taskENABLE_INTERRUPTS();

	/* Define FreeRTOS tasks */
	xTaskCreate(App_Task, 		"Application", 	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_APP, NULL);
	//	xTaskCreate(LWIP_Task, 		"LWIP", 	   	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_LWIP, NULL);
	/* lower layers are Interrupt driven */


	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following
	  line will never be reached.  If the following line does execute, then
	  there was insufficient FreeRTOS heap memory available for the idle and/or
	  timer tasks	to be created.  See the memory management section on the
	  FreeRTOS web site for more details. */
	for (;; );

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

/* END main */


void initClocks()
{
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
	CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
}
void initPeripherals()
{
	/* Initialisation of some of these peripherals must be done within a
	 * FreeRTOS task when the FreeRTOS component is used in S32 DS project.
	 * This is because all interrupts are disabled until FreeRTOS begins
	 * the scheduler */

	/***** Initialise all pins: ENET, CAN, UART, SDHC, LEDs *****/
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

	/***** Initialise UART (LinFlex2) *****/
	//LIN_initialise(INST_LIN_0, &lin_0_State, &lin_0_InitConfig0);

	/***** Initialise CAN Interfaces ****/
	CAN_initialise(INST_CAN0, &CAN0_Config0);
	CAN_initialise(INST_CAN1, &CAN1_Config0);
	CAN_initialise(INST_CAN2, &CAN2_Config0);
	CAN_initialise(INST_CAN3, &CAN3_Config0);

	/***** Initialise DEBUG Terminal *****/
	//CONSOLE_initialise(INST_UART_CONSOLE,  &uart_console_Config0);

	/***** Initialise Ethernet *****/
	// This is performed within LWIP thread

	/***** Initialise SD Card ****/
	SD_initialise();

	/***** Initialise HSM/SHE ****/
	HSM_initialise();

	/***** Switch on peripheral voltage regulators ****/
	PINS_DRV_WritePin(PTD, 12, 1); // ON-OFF-CAN
	PINS_DRV_WritePin(PTD, 13, 1); // ON-OFF-ETH
}

/* Trigger event for updating FW of all ECUs requiring update */
void keyOff()
{
	FIL fw;	/* FW update of ECU */
	FRESULT res;
	status_t status;
	uint16_t i;

	/* Loop through all ECUs to check which require to be updated */
	for(i = 0 ; i < NUM_OF_ECUS ; i++)
	{
		res = SD_openLatestFW(i, &fw);

		if(FR_OK == res)
		{
			status = ECU_updateFirmware(i, &fw, true);

			if(status == STATUS_SUCCESS)
			{
				ECU_sendRequest(i, ECU_REQ_FWVER); /* Update FW version of ECU */

				SD_storeECUInfo(i); /* Update ECU information on SD card */
			}
			else {
				printf("Failed to UPDATE firmware of ECU %d", i);
			}
		}
		else if(FR_NO_FILE == res)
		{
			printf("ECU %d : new firmware not found", i);
		}
		f_close(&fw);
	}
}
