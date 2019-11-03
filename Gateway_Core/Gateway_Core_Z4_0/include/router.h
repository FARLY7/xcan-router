/*
 * router.h
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef ROUTER_H_
#define ROUTER_H_




void ROUTER_Initialise(void *routing_table, uint32_t routingTableSize);

void ROUTER_CAN_InputFrame(uint8_t channelID, uint16_t frameID, uint8_t *payload);



#endif /* ROUTER_H_ */
