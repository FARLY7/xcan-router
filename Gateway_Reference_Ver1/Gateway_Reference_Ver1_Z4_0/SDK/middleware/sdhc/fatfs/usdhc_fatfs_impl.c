/*****************************************************************************/
/* FILE NAME: usdhc_fatfs_impl.c   COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS diskio implementation                                  */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#include "Cpu.h"

#include <stdbool.h>
#include <stdlib.h>

#include "usdhc/usdhc_impl.h"
#include "sd/common.h"
#include "sd/card.h"
#include "fatfs/usdhc_fatfs_impl.h"
#include "common/fatfs_impl.h"
#include "ff.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.3, All declarations of an object or function shall use the same names and type qualifiers.
 * This is needed for stdio routines.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be visible when an object or function with external linkage is defined.
 * This is needed for stdio routines.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer
 * to object type and a pointer to a different object type.
 * This cast is needed for memcpy() routine.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.6, typedefs that indicate size and signedness shall be used in place of the basic numeric type.
 * This cast is needed for reentrant routines.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Constants and macros
*******************************************************************************/

fatfs_callback_setup_t g_fatfsCallbacks[FATFS_DISKIO_CALLS];

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

/*******************************************************************************
* Local variables
*******************************************************************************/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

static ALIGNED(4096)
uint8_t g_bufferedData[FF_MAX_SS * 8] = { 0U };

/*******************************************************************************
* Local functions
*******************************************************************************/

/*******************************************************************************
* Global functions
*******************************************************************************/

void uSDHC_fatfs_initialize(void)
{
	g_fatfsCallbacks[FATFS_DISKIO_INITIALIZE]
		.event = FATFS_DISKIO_INITIALIZE;
	g_fatfsCallbacks[FATFS_DISKIO_INITIALIZE]
		.disk_initialize_function = uSDHC_disk_initialize;

	g_fatfsCallbacks[FATFS_DISKIO_STATUS]
		.event = FATFS_DISKIO_STATUS;
	g_fatfsCallbacks[FATFS_DISKIO_STATUS]
		.disk_status_function = uSDHC_disk_status;

	g_fatfsCallbacks[FATFS_DISKIO_IOCTL]
		.event = FATFS_DISKIO_IOCTL;
	g_fatfsCallbacks[FATFS_DISKIO_IOCTL]
		.disk_ioctl_function = uSDHC_disk_ioctl;

	g_fatfsCallbacks[FATFS_DISKIO_READ]
		.event = FATFS_DISKIO_READ;
	g_fatfsCallbacks[FATFS_DISKIO_READ]
		.disk_read_function = uSDHC_disk_read;

	g_fatfsCallbacks[FATFS_DISKIO_WRITE]
		.event = FATFS_DISKIO_WRITE;
	g_fatfsCallbacks[FATFS_DISKIO_WRITE]
		.disk_write_function = uSDHC_disk_write;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DSTATUS uSDHC_disk_initialize(
		BYTE pdrv		/* Physical drive number (0) */
)
{
	/* Supports only single drive */
	if(0U != pdrv)
	{
		Stat = STA_NOINIT;
	}
	if((common_status_t)STATUS_SUCCESS == uSDHC_Init(false))
	{
		if(0U != (Stat & (DSTATUS)STA_NOINIT))
		{
			Stat &= ~(DSTATUS)(STA_NOINIT);
		}
		Stat = disk_status(pdrv);
	}
	else
	{
		if(0U == (Stat & (DSTATUS)STA_NODISK))
		{
			Stat |= (DSTATUS)STA_NODISK;
		}
	}
	return Stat;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DSTATUS uSDHC_disk_status(
		BYTE pdrv		/* Physical drive number (0) */
)
{
	sd_card_t *card;
	bool isProtected = false;
	/* Supports only single drive */
	if(0U != pdrv)
	{
		Stat = STA_NOINIT;
	}
	if(0U == (Stat & (DSTATUS)STA_NOINIT))
	{
		card = uSDHC_GetSDCard();
		if(NULL != card)
		{
			/* Test write protection flags. */
			isProtected = SD_CheckReadOnly(card);
			if(false == isProtected)
			{
				isProtected = uSDHC_IsSDCardWriteProtect();
			}
			if(true == isProtected)
			{
				if(0U == (Stat & (DSTATUS)STA_PROTECT))
				{
					Stat |= (DSTATUS)STA_PROTECT;
				}
			}
		}
	}
	return Stat;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_ioctl(
		BYTE pdrv,		/* Physical drive number (0) */
		BYTE cmd,		/* Command code */
		void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	static DWORD blockSizeDW;
	static WORD blockSizeW;
	sd_card_t *card = NULL;
	(void)card;

	if(0U != (disk_status(pdrv) & (DSTATUS)STA_NOINIT))
	{
		res = RES_NOTRDY;
	}
	if(RES_OK == res)
	{
		card = uSDHC_GetSDCard();
		if(NULL == card)
		{
			res = RES_ERROR;
		}
		if(RES_OK == res)
		{
			res = RES_ERROR;
			switch(cmd) {
			case CTRL_SYNC:
				/* Make sure that no pending write process */
                res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				/* Get number of sectors on the disk (DWORD) */
				if(NULL != buff)
				{
					/* Buffer has DWORD type here. */
					(void)memcpy(buff, &(card->blockCount), sizeof(DWORD));
					res = RES_OK;
				}
				break;
			case GET_BLOCK_SIZE:
				if(NULL != buff)
				{
					/* Buffer has DWORD type here. */
					blockSizeDW = card->csd.eraseSectorSize;
					(void)memcpy(buff, &(blockSizeDW), sizeof(DWORD));
					res = RES_OK;
				}
				break;
			case GET_SECTOR_SIZE:
				DEV_ASSERT(card->blockSize == FSL_SDMMC_DEFAULT_BLOCK_SIZE);
				if(NULL != buff)
				{
					/* Buffer has WORD type here. */
					blockSizeW = (WORD)card->blockSize;
					(void)memcpy(buff, &(blockSizeW), sizeof(WORD));
					res = RES_OK;
				}
				break;
			default:
				res = RES_PARERR;
				break;
			}
		}
	}
	return res;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_read(
		BYTE pdrv,			/* Physical drive number (0) */
		BYTE *buff,			/* Pointer to the data buffer to store read data */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
)
{
	DRESULT res = RES_OK;
	UINT i;
	UINT j;
	UINT buffLen;
	bool needSwap;
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	sd_card_t *card;

	if((0U != pdrv) || (0U == count))
	{
		res = RES_PARERR;
	}
	if(0U != (Stat & (DSTATUS)STA_NOINIT))
	{
		res = RES_NOTRDY;
	}
	if(RES_OK == res)
	{
		card = uSDHC_GetSDCard();
		if(NULL == card)
		{
			res = RES_ERROR;
		}
		if(RES_OK == res)
		{
			res = RES_ERROR;
			if((g_fatfsCallbacks[FATFS_DISKIO_READ].event == FATFS_DISKIO_READ) &&
					(g_fatfsCallbacks[FATFS_DISKIO_READ].preFunction != NULL))
			{
				g_fatfsCallbacks[FATFS_DISKIO_READ].preFunction((uint32_t)status, NULL);
			}
			buffLen = (UINT)(card->blockSize * count);
			DEV_ASSERT(buffLen <= sizeof(g_bufferedData));
			status = SD_ReadBlocks(card, g_bufferedData, sector, count);
			if((common_status_t)STATUS_SUCCESS == status)
			{
				/* De-select the card */
				res = RES_OK;
				needSwap = false;
				j = 0U;
				for(i = 0U; i < buffLen; i++)
				{
					buff[i] = g_bufferedData[i];
					if((0U < i) && (0U == (i % 4U)))
					{
						swap_dword((uint32_t *)buff, j * 4U, (uint32_t)(*(uint32_t *)(&buff[j * 4U])));
						j++;
						needSwap = true;
					}
				}
				if(needSwap)
				{
					swap_dword((uint32_t *)buff, j * 4U, (uint32_t)(*(uint32_t *)(&buff[j * 4U])));
				}
			}
			if((g_fatfsCallbacks[FATFS_DISKIO_READ].event == FATFS_DISKIO_READ) &&
					(g_fatfsCallbacks[FATFS_DISKIO_READ].postFunction != NULL))
			{
				g_fatfsCallbacks[FATFS_DISKIO_READ].postFunction((uint32_t)status, NULL);
			}
		}
	}
	return res;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 *
 */
DRESULT uSDHC_disk_write(
		BYTE pdrv,			/* Physical drive number (0) */
		const BYTE *buff,	/* Pointer to the data to be written */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
)
{
	DRESULT res = RES_OK;
	UINT i;
	UINT j;
	UINT buffLen;
	bool needSwap;
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	sd_card_t *card;
	(void)buff;

	if((0U != pdrv) || (0U == count))
	{
		res = RES_PARERR;
	}
	if(0U != (Stat & (DSTATUS)STA_NOINIT))
	{
		res = RES_NOTRDY;
	}
	if(0U != (Stat & (DSTATUS)STA_PROTECT))
	{
		res = RES_WRPRT;
	}
	if(RES_OK == res)
	{
		card = uSDHC_GetSDCard();
		if(NULL == card)
		{
			res = RES_ERROR;
		}
		if(RES_OK == res)
		{
			res = RES_ERROR;
			if((g_fatfsCallbacks[FATFS_DISKIO_WRITE].event == FATFS_DISKIO_WRITE) &&
					(g_fatfsCallbacks[FATFS_DISKIO_WRITE].preFunction != NULL))
			{
				g_fatfsCallbacks[FATFS_DISKIO_WRITE].preFunction((uint32_t)status, NULL);
			}
			buffLen = (UINT)(card->blockSize * count);
			DEV_ASSERT(buffLen <= sizeof(g_bufferedData));
			needSwap = false;
			j = 0U;
			for(i = 0U; i < buffLen; i++)
			{
				g_bufferedData[i] = buff[i];
				if((0U < i) && (0U == (i % 4U)))
				{
					swap_dword((uint32_t *)g_bufferedData, j * 4U, (uint32_t)(*(uint32_t *)(&g_bufferedData[j * 4U])));
					j++;
					needSwap = true;
				}
			}
			if(needSwap)
			{
				swap_dword((uint32_t *)g_bufferedData, j * 4U, (uint32_t)(*(uint32_t *)(&g_bufferedData[j * 4U])));
			}
			status = SD_WriteBlocks(card, g_bufferedData, sector, count);
			if((common_status_t)STATUS_SUCCESS == status)
			{
				/* De-select the card */
				res = RES_OK;
			}
			if((g_fatfsCallbacks[FATFS_DISKIO_WRITE].event == FATFS_DISKIO_WRITE) &&
					(g_fatfsCallbacks[FATFS_DISKIO_WRITE].postFunction != NULL))
			{
				g_fatfsCallbacks[FATFS_DISKIO_WRITE].postFunction((uint32_t)status, NULL);
			}
		}
	}
	return res;
}
