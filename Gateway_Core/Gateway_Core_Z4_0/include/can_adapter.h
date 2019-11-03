/*
 * can_adapter.h
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef CAN_ADAPTER_H_
#define CAN_ADAPTER_H_



/*!
 * @brief uSDHC fatfs callbacks initialization routine. Must be called before FATFS use.
 *
 */
void can_adapter_initialise(void);

void CAN_intialise(uint8_t instance);

void CAN_ioctl(uint8_t instance, uint8_t enable);

uint8_t CAN_status(uint8_t instance);

void CAN_send(uint8_t instance,	uint16_t frameId, uint8_t *data);


#endif /* CAN_ADAPTER_H_ */
