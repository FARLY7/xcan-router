/*
 * gw_can_interface.h
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef GW_INTERFACE_H_
#define GW_INTERFACE_H_


/* Cant I not just define functiono headers which are caled by GW, but implemented by adapter? */


typedef enum
{
	CAN_INITIALISE = 0,
	CAN_STATUS,
	CAN_IOCTL,
	CAN_SEND,
} gw_can_event_t;

typedef void (*can_initialise_proto_t)( uint8_t instance );
typedef void (*can_ioctl_proto_t)( uint8_t instance, uint8_t enable );
typedef void (*can_status_proto_t)( uint8_t instance );
typedef void (*can_send_proto_t)( uint8_t instance, uint16_t frameId, uint8_t *data);

typedef struct
{
	gw_can_event_t event;
	can_initialise_proto_t can_initialise_function;
	can_ioctl_proto_t can_ioctl_function;
	can_status_proto_t can_status_function;
	can_send_proto_t can_send_function;
} can_callback_setup_t;

#define GW_CANIO_CALLS (4U)

extern can_callback_setup_t g_canCallbacks[GW_CANIO_CALLS];

#endif /* GW_INTERFACE_H_ */
