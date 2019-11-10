/*
 * can_filter.c
 *
 *  Created on: 28 Mar 2018
 *      Author: B49855
 */
#include "config.h"

/* The CAN MSG ID filter table performs one function:
 * 1. Acept frames with a MSG ID found within the filter table.
 */
/********************* CAN FILTER TABLE ********************/
/***********************************************************/
uint32_t CAN_ID_FilterTable[NUM_OF_CANID_FILTERS] = {0, 1, 2, 3, 4, 5, 7, 8, 9};
/***********************************************************/
