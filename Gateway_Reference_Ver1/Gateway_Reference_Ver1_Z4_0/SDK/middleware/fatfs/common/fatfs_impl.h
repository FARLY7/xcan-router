/*****************************************************************************/
/* FILE NAME: fatfs_impl.h         COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS diskio implementation                                  */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#ifndef FATFS_FATFS_IMPL_H
#define FATFS_FATFS_IMPL_H

#include "Cpu.h"
#include "rtcTimer1.h"

#include "osif.h"

#include <stdint.h>

#include "diskio.h"
#include "ff.h"

/*!
 * @addtogroup FATFS_IMPL
 * @ingroup fatfs_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Events for fatfs diskio
 *
 */
typedef enum
{
	FATFS_DISKIO_INITIALIZE = 0,
	FATFS_DISKIO_STATUS,
	FATFS_DISKIO_IOCTL,
	FATFS_DISKIO_READ,
	FATFS_DISKIO_WRITE
} fatfs_event_t;

/*!
 * @brief Callback function for fatfs diskio
 *
 * @param status    uint32_t this parameter is used for callback function. It indicates the status of operation on lower layers.
 *                  In case of success, value of status is status_t' STATUS_SUCCESS otherwise error value
 * @param param     void user data
 */
typedef void (*fatfs_callback_t)(uint32_t status, void *param);

typedef DSTATUS (*disk_initialize_proto_t)(
		BYTE pdrv		/* Physical drive number (0) */
);
typedef DSTATUS (*disk_status_proto_t)(
		BYTE pdrv		/* Physical drive number (0) */
);
typedef DRESULT (*disk_ioctl_proto_t)(
		BYTE pdrv,		/* Physical drive number (0) */
		BYTE cmd,		/* Command code */
		void *buff		/* Buffer to send/receive control data */
);
typedef DRESULT (*disk_read_proto_t)(
		BYTE pdrv,			/* Physical drive number (0) */
		BYTE *buff,			/* Pointer to the data buffer to store read data */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
);
typedef DRESULT (*disk_write_proto_t)(
		BYTE pdrv,			/* Physical drive number (0) */
		const BYTE *buff,	/* Pointer to the data to be written */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
);

/*!
 * @brief Callback setup for fatfs
 *
 * @param event           fatfs_event_t this parameter is used for callback to diskio function.
 *
 * @param preFunction     fatfs_callback_t function executed before diskio event handling
 *
 * @param postFunction    fatfs_callback_t function executed after diskio event handling
 */
typedef struct
{
	fatfs_event_t event;
	fatfs_callback_t preFunction;
	fatfs_callback_t postFunction;
	disk_initialize_proto_t disk_initialize_function;
	disk_status_proto_t disk_status_function;
	disk_ioctl_proto_t disk_ioctl_function;
	disk_read_proto_t disk_read_function;
	disk_write_proto_t disk_write_function;
} fatfs_callback_setup_t;

/*!
 * @brief Total number of DISKIO functions
 *
 */
#define FATFS_DISKIO_CALLS (5U)
/*!
 * @brief Callback functions for fatfs
 *
 * This array must be implemented in user application.
 * Before-initialization and post-initialization functions are considered.
 *
 */
extern fatfs_callback_setup_t g_fatfsCallbacks[FATFS_DISKIO_CALLS];

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief convert timestamp into S32SDK rtc driver' rtc_timedate_t structure.
 *
 * @param DWORD timeDateValue input timestamp.
 * @param rtc_timedate_t *timeDate pointer to rtc_timedate_t structure.
 * @retval void.
 */
void convertFFTimeToRTCTimeDate(DWORD timeDateValue, rtc_timedate_t *timeDate);

/*!
 * @brief convert timestamp into S32SDK rtc driver' rtc_timedate_t structure.
 *
 * @param rtc_timedate_t *timeDate pointer to rtc_timedate_t structure.
 * @param DWORD *timeDateValue pointer to input timestamp variable.
 * @retval void.
 */
void convertRTCTimeDateToFFTime(rtc_timedate_t *timeDate, DWORD *timeDateValue);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* FATFS_FATFS_IMPL_H */
