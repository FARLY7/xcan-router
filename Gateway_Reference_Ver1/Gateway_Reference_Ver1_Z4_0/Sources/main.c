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
#include "framework.h" /* Gateway Framework */

volatile int exit_code = 0;

//  Enable when CUT3.0 arrives:
//	#define FEATURE_CAN_HAS_ISOCANFDEN_ENABLE 1

#define	TASK_PRIORITY_APP 	(tskIDLE_PRIORITY + 7)

/* GPIO User Buttons & LEDs */
#define SW1_PORT PTA
#define SW1_PIN	 3
#define SW2_PORT PTE
#define SW2_PIN  10

/* Function Prototypes */
void App_task(void* pvParameters);
void initClocks();
void initPeripherals();
void keyOff();

/* Application demo task. When button A is pressed, GW will request updated information from all ECUs on network.
 * When button B is pressed, GW will update FW of all ECUs that have new FW in SD card
 */
void App_Task(void* pvParameters)
{
	/* Initialise all peripherals: CAN, LIN, HSM, ETH, .. */
	initPeripherals();

	/* Add Gateway Heartbeat task to the FreeRTOS scheduler */
	//xTaskCreate(Heartbeat_Task, "GW_Heartbeat", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_HEART, NULL);

	while(1)
	{
		vTaskDelay(1000); /* Delay for 10 seconds */
	}
}


int main(void)
{
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
	PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
	/*** End of Processor Expert internal initialization.                    ***/

	/* 1. Initialise device clocking */
	initClocks();

	/* 2. Initialise the SW layers of the gateway framework */
	//GW_initialise();  /* Initialise GW layer & FreeRTOS task */
	PDU_initialise(); /* Initialise PDU layer & FreeRTOS task */

	/* Start using DEV ASSERTS */
    //DEV_ASSERT(status == STATUS_SUCCESS);

	/* Main user application task */
	xTaskCreate(App_Task, 		"Application", 	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_APP, NULL);
	//	xTaskCreate(LWIP_Task, 		"LWIP", 	   	configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_LWIP, NULL);


	/* The user must lastly initialise all lower level peripherals & interfaces
	 * within their application task, for two reasons:
	 *
	 * 1. Interrupts (which are required by some modules) are not enabled until
	 * 	  the FreeRTOS scheduler has begun.
	 * 2. It is important that all upper SW layers are in a prepared state to
	 *    receive and process packets.
	 */

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
	//PINS_DRV_SetOutputBuffer(PTH, 8, 0, 0);
	SIUL2->MSCR[120] = 0x300B0005;

	vTaskDelay(100); /* Small delay to allow pins to stabilise */

#ifdef DEBUG_ENABLE
	/***** Initialise UART DEBUG Terminal *****/
	CONSOLE_initialise(&CONSOLE_instance,  &CONSOLE_Config0);
#endif

	/***** Initialise Ethernet *****/
	// This is performed within LWIP thread

	PINS_DRV_WritePin(PTD, 12, 1); // ON-OFF-CAN, 5V Vreg ON
	PINS_DRV_WritePin(PTD, 13, 1); // ON-OFF-ETH, 12V Vreg ON

	/***** Initialise SD Card ****/
	SD_initialise();

	/***** Initialise HSM/SHE ****/
	HSM_initialise();


	/***** Initialise LIN Interfaces *****/
	//LIN_initialise(INST_LIN_0, &lin_1_State, &lin_1_InitConfig0);

	/***** Initialise CAN Interfaces ****/
	CAN_initialise(&CAN0_instance, &CAN0_Config0);
	CAN_initialise(&CAN1_instance, &CAN1_Config0);
	CAN_initialise(&CAN2_instance, &CAN2_Config0);
	CAN_initialise(&CAN3_instance, &CAN3_Config0);

	/***** Switch on peripheral PHYs & Vregs ****/
	PINS_DRV_WritePin(PTD, 12, 1); // ON-OFF-CAN, 5V Vreg ON
	PINS_DRV_WritePin(PTD, 13, 1); // ON-OFF-ETH, 12V Vreg ON
	//PINS_DRV_WritePin(PTF, 11, 1); // LIN1 SLP_N, Initialise low
	//PINS_DRV_WritePin(PTF, 13, 1); // LIN6 SLP_N, Initialise low

}

///* Trigger event for updating FW of all ECUs requiring update */
//void keyOff()
//{
//	FIL fw;	/* FW update of ECU */
//	FRESULT res;
//	status_t status;
//	uint16_t i;
//
//	/* Loop through all ECUs to check which require to be updated */
//	for(i = 0 ; i < NUM_OF_ECUS ; i++)
//	{
//		res = SD_openLatestFW(i, &fw);
//
//		if(FR_OK == res)
//		{
//			status = ECU_updateFirmware(i, &fw, true);
//
//			if(status == STATUS_SUCCESS)
//			{
//				ECU_sendCommand(i, ECU_CMD_REQ_FWVER); /* Update FW version of ECU */
//
//				SD_storeECUInfo(i); /* Update ECU information on SD card */
//			}
//			else {
//				printf("Failed to UPDATE firmware of ECU %d", i);
//			}
//		}
//		else if(FR_NO_FILE == res)
//		{
//			printf("ECU %d : new firmware not found", i);
//		}
//		f_close(&fw);
//	}
//}
