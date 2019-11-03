/*****************************************************************************/
/* FILE NAME: rtc_timer.c         COPYRIGHT (c) NXP Semiconductors 2017      */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS RTC timer                                              */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#include "rtcTimer1.h"

#include "rtc/rtc_timer.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is needed for DEV_ASSERT macro condition.
 *
 */

/*******************************************************************************
* Global variables
*******************************************************************************/

/*! @brief RTC current timedate structure */
volatile rtc_timedate_t g_rtcTimer1CurrentTimeDate;
/*! @brief RTC initialization flag */
volatile uint32_t g_rtcTimer1Initialized = 0U;

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
* @brief S32 RTC driver callback. PEX: it is installed as User Callback in Alarm Configuration.
*
* @param void * pointer to user callback.
* @retval void.
*/
void rtcTimer1_AlarmCallback0(void *callbackParam)
{
	(void)callbackParam;
	rtcTimer_GetTimeDate();
}

/*
 * Implements rtcTimer_Init_Activity
 */
status_t rtcTimer_Init(void)
{
	status_t status = STATUS_SUCCESS;

	if(0U == g_rtcTimer1Initialized)
	{
		g_rtcTimer1CurrentTimeDate = rtcTimer1_StartTime0;
		/* Initialize the RTC-API module */
		RTC_DRV_Init(RTCTIMER1, &rtcTimer1_State, &rtcTimer1_Config0);
		/* Set the time and date */
		(void)RTC_DRV_SetTimeDate(RTCTIMER1, &rtcTimer1_StartTime0);
		/* Start the counter */
		status = RTC_DRV_StartCounter(RTCTIMER1);
		DEV_ASSERT(STATUS_SUCCESS == status);
#if defined(USING_OS_FREERTOS)
		g_rtcTimer1Initialized = 1U;
#else
		/* Configure a timer alarm to be raised periodically */
		status = RTC_DRV_ConfigureAlarm(RTCTIMER1, &rtcTimer1_AlarmConfig0);
		if(STATUS_SUCCESS == status)
		{
			g_rtcTimer1Initialized = 1U;
		}
#endif
	}

	return status;
}

/*
 * Implements rtcTimer_DeInit_Activity
 */
void rtcTimer_DeInit(void)
{
	(void)RTC_DRV_StopCounter(RTCTIMER1);
	RTC_DRV_Deinit(RTCTIMER1);
	g_rtcTimer1Initialized = 0U;
	g_rtcTimer1CurrentTimeDate = rtcTimer1_StartTime0;

}

/*
 * Implements rtcTimer_GetTimeDate_Activity
 */
void rtcTimer_GetTimeDate(void)
{
	static rtc_timedate_t c_timeDate;
	if(1U == g_rtcTimer1Initialized)
	{
		RTC_DRV_GetTimeDate(RTCTIMER1, &c_timeDate);
		g_rtcTimer1CurrentTimeDate = c_timeDate;
	}
}

/*
 * Implements rtcTimer_UpdateTimeDate_Activity
 */
void rtcTimer_UpdateTimeDate(rtc_timedate_t *newTimeDate)
{
	DEV_ASSERT(NULL != newTimeDate);
	if(1U == g_rtcTimer1Initialized)
	{
		*newTimeDate = g_rtcTimer1CurrentTimeDate;
	}
	else
	{
		*newTimeDate = rtcTimer1_StartTime0;
	}
}
