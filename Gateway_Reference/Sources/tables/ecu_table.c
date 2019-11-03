/*
 * ecu_table.c
 *
 *  Created on: 28 Mar 2018
 *      Author: B49855
 */
#include "config.h"
#include "Cpu.h"

/* @brief:  This is the global list of ECUs within the vehicle. It is maintained in RAM
 * 			and is backed up to SD card on key-off signal.
 */
/*********************** ECU DATABASE **********************/
/***********************************************************/
ECU ecu_list[NUM_OF_ECUS] =
{
		{
				/* 0: Engine ECU */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.interface 	= INST_CAN0, 	/* CAN_0 (Onboard) Interface */
				.fw_version	= 1,
		},
		{
				/* 1: Drive Control ECU */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.interface 	= INST_CAN1, 	/* CAN_5 (CAN_A) Interface */
				.fw_version	= 1,
		},
		{
				/* 2: Cluster ECU */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.interface 	= INST_CAN2, 	/* CAN_2 (CAN_C) Interface */
				.fw_version	= 1,
		},
		{
				/* 3: Diagnostics ECU  */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.interface 	= INST_CAN3, 	/* CAN_2 (CAN_C) Interface */
		}
};

