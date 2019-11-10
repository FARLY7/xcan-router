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
				.instance 	= 0, 			/* CAN_0 Instance */
				.fw_version	= 1
		},
		{
				/* 1: Drive Control ECU */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.instance 	= 1,			/* CAN_1 Instance */
				.fw_version	= 1
		},
		{
				/* 2: Cluster ECU */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.instance 	= 2, 			/* CAN_2 Instance */
				.fw_version	= 1
		},
		{
				/* 3: Diagnostics ECU  */
				.protocol	= PROTOCOL_CAN,	/* CAN Protocol */
				.instance 	= 3, 			/* CAN_3 Instance */
				.fw_version	= 1
		}
};

