/*
 * pdu_table.c
 *
 *  Created on: 28 Mar 2018
 *      Author: B49855
 */
#include "config.h"

/* # The PDUs IDs are implicitly defined by their index within the array.
 * # PDUs MUST be arranged in an ascending order with no missing values.
 * # PDUs for resquest/respond command protocol are derived from ECU ID to
 *   to save RAM in PDU_Table.
 */
/******************** PDU ROUTING TABLE ********************/
/***********************************************************/
PDU_Desc PDU_Descriptors[NUM_OF_PDU_TYPES] =
{
		{
			.label 	   = "Engine RPM",
			.destCount = 2,
			.dest 	   = (uint8_t[]) {3, 2} // ...
			//.destCount = 1,
			//.dest 	   = (uint8_t[]) {2} // ...
		},
		{
			.label 	   = "Engine Temp",
			.destCount = 2,
			.dest 	   = (uint8_t[]) {3, 2} // ...
		},
		{
			.label 	   = "Ignition Switch",
			.destCount = 2,
			.dest 	   = (uint8_t[]) {3, 0} // ...
		},
		{
			.label 	   = "Acceleration Pedal",
			.destCount = 2,
			.dest 	   = (uint8_t[]) {3, 0} // ...
		},
		{
			.label 	   = "Handbrake",
			.destCount = 2,
			.dest 	   = (uint8_t[]) {3, 2} // ...
		}
};
/***********************************************************/



//tRxPDUDesc PDU_Desc[1] =
//{
//		{
//				.id 	= 0x5A5A5A5A,
//				.length = 15,
//				.num_of_signals = 2,
//				.signalDesc = (tSignalDescr[]) {  }
//
//		}
//};
