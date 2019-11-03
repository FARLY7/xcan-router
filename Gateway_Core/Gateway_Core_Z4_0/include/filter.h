/*
 * Filter.h
 *
 *  Created on: 18 Sep 2018
 *      Author: nxa20140
 */

#ifndef FILTER_H_
#define FILTER_H_


enum {
	FILTER_IDLE,
	FILTER_BUSY,
} FSTATUS;


void FILTER_Initialise(void *filterTable, uint32_t filterTableSize);
void FILTER_Deinitialise(void);
bool FILTER_FilterFrame(uint8_t channelID, uint16_t frameID);
FSTATUS FILTER_GetStatus(void);


#endif /* FILTER_H_ */
