/*****************************************************************************/
/* FILE NAME: usdhc_fatfs_impl.h   COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS diskio implementation                                  */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#ifndef USDHC_FATFS_IMPL_H
#define USDHC_FATFS_IMPL_H

#include "Cpu.h"

#include <stdint.h>

#include "common/fatfs_impl.h"
#include "ff.h"

/*!
 * @addtogroup USDHC_FATFS_IMPL
 * @ingroup sdhc_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief uSDHC fatfs callbacks initialization routine. Must be called before FATFS use.
 *
 */
void uSDHC_fatfs_initialize(void);

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DSTATUS uSDHC_disk_initialize(
		BYTE pdrv		/* Physical drive number (0) */
);

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DSTATUS uSDHC_disk_status(
		BYTE pdrv		/* Physical drive number (0) */
);

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_ioctl(
		BYTE pdrv,		/* Physical drive number (0) */
		BYTE cmd,		/* Command code */
		void *buff		/* Buffer to send/receive control data */
);

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_read(
		BYTE pdrv,			/* Physical drive number (0) */
		BYTE *buff,			/* Pointer to the data buffer to store read data */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
);

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_write(
		BYTE pdrv,			/* Physical drive number (0) */
		const BYTE *buff,	/* Pointer to the data to be written */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* USDHC_FATFS_IMPL_H */
