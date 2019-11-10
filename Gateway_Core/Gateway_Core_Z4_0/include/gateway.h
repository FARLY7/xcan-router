/*
 * Gateway.h*
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef GATEWAY_H_
#define GATEWAY_H_

/*!
 * @brief The event for uSDHC. This enumeration is used to register the callback function.
 * Implements : usdhc_event_t_Class
 */
typedef enum
{
	/* Gateway events */
    GW_EVENT_FRAME_RECEIVED,     /* Transfer completion event */
	GW_EVENT_FRAME_TRANSMITTED,
    GW_EVENT_FRAME_ACCEPTED,     /* Card insertion event */
    GW_EVENT_FRAME_REJECTED,     /* Card removal event */
	/* Gateway CAN events */
	GW_EVENT_CAN_TIMEOUT,
} gw_event_t;

typedef enum
{
	GW_STATUS_IDLE,					/* Gateway is idle */
	GW_STATUS_RECEIVING_FRAME,		/* Gateway is currently receiving a frame */
	GW_STATUS_TRANSMITING_FRAME,	/* Gateway is currently transmitting a frame */
	GW_STATUS_FILTERING_FRAME,		/* Gateway is currently filtering a frame */
	GW_STATUS_ROUTING_FRAME			/* Gateway is currently routing a frame */
} gw_status_t;



typedef void (*gw_event_callback_t)(gw_state_t *gwState,
									gw_event_t eventType);

typedef bool (*gw_filter_callback_t)(gw_state_t *gwState,
									 gw_event_t eventType,
									 uint32_t channelID,
									 uint32_t frameID);

typedef uint32_t (*gw_routing_callback_t)(gw_state_t *gwState,
										  gw_event_t eventType,
										  uint32_t channelID,
										  uint32_t frameID);


typedef struct
{
	volatile gw_status_t	status;			/* Current status of gateway */
	gw_event_callback_t	 	eventCallback;	/* Callback function for events */
	gw_filter_callback_t	filterCallback;	/* Callback function for filtering operation */
	gw_routing_callback_t	lookupCallback;	/* Callback function for lookup operation */
} gw_state_t;


/*! @brief Data structure to initialise the uSDHC
 * Implements : usdhc_config_t_Class
 */
typedef struct
{
    void *		routingTable;          /*!< ADMA table for transferring with ADMA1 and ADMA2 mode. */
    uint32_t	routingTableSize;      /*!< The size of ADMA table in bytes */
    void *		canConfigTable;
    uint32_t	canConfigTableSize;
} gw_config_t;

void GW_Initialise(gw_config_t *config);	/* Initialise the Gateway core */
void GW_Deinitialise(void);					/* De-initialise the Gateway core */
void GW_EnableInput(uint8_t canInstance);	/* Enable CAN channel's input */
void GW_EnableOutput(uint8_t canInstance);	/* Enable CAN channel's output */
void GW_DisableInput(uint8_t canInstance);	/* Disable CAN channel's input */
void GW_DisableOutput(uint8_t canInstance);	/* Disable CAN channel's output */
//void GW_EnableTimer(void);
//void GW_Tick(void);

/*!
 * @brief Installs a callback function for the IRQ handler.
 * @param callback The callback function.
 */
void GW_InstallEventCallback(gw_event_callback_t callback);
/*!
 * @brief Installs an error callback function for the IRQ handler and enables error interrupts.
 * @param callback The error callback function.
 */
void GW_InstallRoutingCallback(gw_routing_callback_t callback);
/*!
 * @brief Installs an error callback function for the IRQ handler and enables error interrupts.
 * @param callback The error callback function.
 */
void GW_InstallFilterCallback(gw_filter_callback_t callback);


#endif /* GATEWAY_H_ */
