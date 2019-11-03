/*
 * gw_can_interface.h
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef GW_CAN_INTERFACE_H_
#define GW_CAN_INTERFACE_H_





void GW_CAN_intialise(uint8_t instance);

void GW_CAN_setOutput(uint8_t instance, bool output);

void GW_CAN_setInput(uint8_t instance, bool input);

void GW_CAN_getStatus(uint8_t instance);

void GW_CAN_sendFrame(uint8_t instance,	uint16_t frameId, uint8_t *data);





#endif /* GW_CAN_INTERFACE_H_ */
