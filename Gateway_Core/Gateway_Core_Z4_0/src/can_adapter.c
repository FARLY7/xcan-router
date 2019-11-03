/*
 * can_adapter.c
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */
#include "can_adapter.h"
#include "gw_interface.h"


can_callback_setup_t g_canCallbacks[GW_CANIO_CALLS];


void can_adapter_initialize(void)
{
	g_canCallbacks[CAN_INITIALISE]
		.event = CAN_INITIALISE;
	g_canCallbacks[CAN_INITIALISE]
		.disk_initialize_function = CAN_intialise;

	g_canCallbacks[CAN_STATUS]
		.event = CAN_STATUS;
	g_canCallbacks[CAN_STATUS]
		.disk_status_function = CAN_status;

	g_canCallbacks[CAN_IOCTL]
		.event = CAN_IOCTL;
	g_canCallbacks[CAN_IOCTL]
		.disk_ioctl_function = CAN_ioctl;

	g_canCallbacks[CAN_SEND]
		.event = CAN_SEND;
	g_canCallbacks[CAN_SEND]
		.disk_read_function = CAN_send;
}


void CAN_initialise(uint8_t instance)
{
	/* CAN_DRV_Initialise() */
}

