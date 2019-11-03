/*****************************************************************************/
/* FILE NAME: fatfs_impl.c         COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS diskio implementation                                  */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#include "Cpu.h"
#include "rtcTimer1.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "osif.h"
#if defined(USING_OS_FREERTOS)
#include "FreeRTOS.h"
#endif

#include "common/fatfs_impl.h"
#include "rtc/rtc_timer.h"
#include "diskio.h"
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
 * @section [global]
 * Violates MISRA 2012 Required Directive 4.12, Dynamic memory allocation shall not be used.
 * This is needed for reentrancy-related routines.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 21.3, The memory allocation and deallocation functions of <stdlib.h> shall not be used.
 * This is needed for reentrancy-related routines.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

/*!
 * @brief fatfs definition as per "ff.h".
 *
 */
PARTITION VolToPart[] =
{
        {0, 0},    /* "0:" ==> Physical drive 0, auto detection */
        {0, 2},    /* "1:" ==> Physical drive 0, 2st partition */
        {1, 1}     /* "2:" ==> Physical drive 1, 1st partition */
};

/*******************************************************************************
* Constants and macros
*******************************************************************************/

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/

/*******************************************************************************
* Global functions
*******************************************************************************/

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 * Implements disk_initialize_Activity
 *
 */
DSTATUS disk_initialize(
		BYTE pdrv		/* Physical drive number (0) */
)
{
	status_t status = STATUS_SUCCESS;

	DSTATUS Stat = STA_NOINIT;	/* Disk status */
	if((g_fatfsCallbacks[FATFS_DISKIO_INITIALIZE].event == FATFS_DISKIO_INITIALIZE) &&
		(g_fatfsCallbacks[FATFS_DISKIO_INITIALIZE].disk_initialize_function != NULL))
	{
		Stat = g_fatfsCallbacks[FATFS_DISKIO_INITIALIZE].disk_initialize_function(pdrv);
	}
#if ((0 == FF_FS_NORTC) && (0 == FF_FS_READONLY))
	status = rtcTimer_Init();

	if(STATUS_SUCCESS != status)
	{
		rtcTimer_DeInit();
	}
#endif
	return Stat;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 * Implements disk_status_Activity
 *
 */
DSTATUS disk_status(
		BYTE pdrv		/* Physical drive number (0) */
)
{
	DSTATUS Stat = STA_NOINIT;	/* Disk status */
	if((g_fatfsCallbacks[FATFS_DISKIO_STATUS].event == FATFS_DISKIO_STATUS) &&
		(g_fatfsCallbacks[FATFS_DISKIO_STATUS].disk_status_function != NULL))
	{
		Stat = g_fatfsCallbacks[FATFS_DISKIO_STATUS].disk_status_function(pdrv);
	}
	return Stat;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 * Implements disk_ioctl_Activity
 *
 */
DRESULT disk_ioctl(
		BYTE pdrv,		/* Physical drive number (0) */
		BYTE cmd,		/* Command code */
		void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	if((g_fatfsCallbacks[FATFS_DISKIO_IOCTL].event == FATFS_DISKIO_IOCTL) &&
		(g_fatfsCallbacks[FATFS_DISKIO_IOCTL].disk_ioctl_function != NULL))
	{
		res = g_fatfsCallbacks[FATFS_DISKIO_IOCTL].disk_ioctl_function(pdrv, cmd, buff);
	}
	return res;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 * Implements disk_read_Activity
 *
 */
DRESULT disk_read(
		BYTE pdrv,			/* Physical drive number (0) */
		BYTE *buff,			/* Pointer to the data buffer to store read data */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
)
{
	DRESULT res = RES_PARERR;
	if((g_fatfsCallbacks[FATFS_DISKIO_READ].event == FATFS_DISKIO_READ) &&
		(g_fatfsCallbacks[FATFS_DISKIO_READ].disk_read_function != NULL))
	{
		res = g_fatfsCallbacks[FATFS_DISKIO_READ].disk_read_function(pdrv, buff, sector, count);
	}
	return res;
}

/*!
 * @brief fatfs diskio callback as per "diskio.h".
 * Implements disk_write_Activity
 *
 */
DRESULT disk_write(
		BYTE pdrv,			/* Physical drive number (0) */
		const BYTE *buff,	/* Pointer to the data to be written */
		DWORD sector,		/* Start sector number (LBA) */
		UINT count			/* Sector count (1..255) */
)
{
	DRESULT res = RES_PARERR;
	if((g_fatfsCallbacks[FATFS_DISKIO_WRITE].event == FATFS_DISKIO_WRITE) &&
		(g_fatfsCallbacks[FATFS_DISKIO_WRITE].disk_write_function != NULL))
	{
		res = g_fatfsCallbacks[FATFS_DISKIO_WRITE].disk_write_function(pdrv, buff, sector, count);
	}
	return res;
}

void convertFFTimeToRTCTimeDate(DWORD timeDateValue, rtc_timedate_t *timeDate)
{
	/* Proceed as per get_fattime() documentation. */
	timeDate->year = (uint16_t)(((timeDateValue >> 25U) & 0x7FU) + 1980U); /* bit[31:25] */
	timeDate->month = (uint16_t)((timeDateValue >> 21U) & 0x0FU); /* bit[24:21] */
	timeDate->day = (uint16_t)((timeDateValue >> 16U) & 0x1FU); /* bit[20:16] */
	timeDate->hour = (uint16_t)((timeDateValue >> 11U) & 0x1FU); /* bit[15:11] */
	timeDate->minutes = (uint16_t)((timeDateValue >> 5U) & 0x3FU); /* bit[10:5] */
	timeDate->seconds = (uint8_t)((timeDateValue >> 1U) & 0x1FU); /* bit[4:0] */
}

void convertRTCTimeDateToFFTime(rtc_timedate_t *timeDate, DWORD *timeDateValue)
{
	(void)timeDate;
	/* rtc_timedate_t members have WORD type and so we can safely convert it into DWORD. */
	/* Proceed as per get_fattime() documentation. */
	*timeDateValue = (DWORD)(((uint32_t)timeDate->year - 1980U) << 25U) |
			((DWORD)timeDate->month << 21) |
			((DWORD)timeDate->day << 16) |
			((DWORD)timeDate->hour << 11) |
			((DWORD)timeDate->minutes << 5) |
			((DWORD)timeDate->seconds >> 1);
}

#if ((0 == FF_FS_NORTC) && (0 == FF_FS_READONLY))
/*!
 * @brief fatfs callback as per "ffconf.h".
 * Implements get_fattime_Activity
 *
 */
DWORD get_fattime(void)
{
	/* Current time */
	static rtc_timedate_t timeDate;
	static DWORD timeDateValue = 0U;

	/* Get current time */
	rtcTimer_UpdateTimeDate(&timeDate);
	convertRTCTimeDateToFFTime(&timeDate, &timeDateValue);
	return timeDateValue;
}
#endif

#if (1 == FF_FS_REENTRANT)
volatile uint8_t g_initSyncObjs[FF_VOLUMES] = { 0U };
static mutex_t g_volSem[FF_VOLUMES];
/*!
 * @brief fatfs create a sync object as per "ff.h".
 * Implements ff_cre_syncobj_Activity
 *
 */
int ff_cre_syncobj(BYTE vol, FF_SYNC_t *sobj)
{
	(void)vol;
	int res = 0;
	status_t status = STATUS_SUCCESS;
	DEV_ASSERT(vol < (BYTE)FF_VOLUMES);
	if(0U == g_initSyncObjs[vol])
	{
		res = 1;
		status = OSIF_MutexCreate(&g_volSem[vol]);
	}
	if(STATUS_SUCCESS == status)
	{
		if(STATUS_SUCCESS == OSIF_MutexLock(&g_volSem[vol], OSIF_WAIT_FOREVER))
		{
			g_initSyncObjs[vol]++;
			(void)OSIF_MutexUnlock(&g_volSem[vol]);
			res = 1;
			*sobj = vol;
		}
	}
	return res;
}

/*!
 * @brief fatfs lock a sync object as per "ff.h".
 * Implements ff_req_grant_Activity
 *
 */
int ff_req_grant(FF_SYNC_t sobj)
{
	int res = 0;
	if(STATUS_SUCCESS == OSIF_MutexLock(&g_volSem[sobj], OSIF_WAIT_FOREVER))
	{
		res = 1;
	}
	return res;
}

/*!
 * @brief fatfs unlock a sync object as per "ff.h".
 * Implements ff_rel_grant_Activity
 *
 */
void ff_rel_grant(FF_SYNC_t sobj)
{
	(void)OSIF_MutexUnlock(&g_volSem[sobj]);
	return;
}

/*!
 * @brief fatfs delete a sync object as per "ff.h".
 * Implements ff_del_syncobj_Activity
 *
 */
int ff_del_syncobj(FF_SYNC_t sobj)
{
	if(STATUS_SUCCESS == OSIF_MutexLock(&g_volSem[sobj], OSIF_WAIT_FOREVER))
	{
		if(0U < g_initSyncObjs[sobj])
		{
			g_initSyncObjs[sobj]--;
			(void)OSIF_MutexUnlock(&g_volSem[sobj]);
			if(0U == g_initSyncObjs[sobj])
			{
				(void)OSIF_MutexDestroy(&g_volSem[sobj]);
			}
		}
	}
	/* Always reports success */
	return 1;
}
#endif

#if ((3 == FF_USE_LFN) || defined(USING_OS_FREERTOS))
/*!
 * @brief fatfs allocates memory block as per "ff.h".
 * Implements ff_memalloc_Activity
 *
 */
void* ff_memalloc(	/* Returns pointer to the allocated memory block (null on not enough core) */
	UINT msize		/* Number of bytes to allocate */
)
{
#if defined(USING_OS_FREERTOS)
	void *p = pvPortMalloc(msize);
#else
	void *p = malloc(msize);
#endif
	return p;
}

/*!
 * @brief fatfs frees memory block as per "ff.h".
 * Please note alloc/free memory scheme (heap_2 or higher) must be configured in FreeRTOS component in order to work.
 * Implements ff_memfree_Activity
 *
 */
void ff_memfree(
	void* mblock	/* Pointer to the memory block to free */
)
{
#if defined(USING_OS_FREERTOS)
	vPortFree(mblock);	/* Free the memory block with POSIX API */
#else
	free(mblock);
#endif
}
#endif
