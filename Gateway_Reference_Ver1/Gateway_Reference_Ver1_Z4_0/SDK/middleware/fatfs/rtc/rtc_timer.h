/*****************************************************************************/
/* FILE NAME: rtc_timer.h         COPYRIGHT (c) NXP Semiconductors 2017      */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: FATFS RTC timer                                              */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#ifndef FATFS_RTC_TIMER_H
#define FATFS_RTC_TIMER_H

#include "rtcTimer1.h"

/*!
 * @addtogroup FATFS_RTC_TIMER
 * @ingroup fatfs_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

extern volatile rtc_timedate_t g_rtcTimer1CurrentTimeDate;
extern volatile uint32_t g_rtcTimer1Initialized;

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize & start RTC timer if it isn't initialized yet
 *
 * @param void.
 * @retval void.
 */
status_t rtcTimer_Init(void);

/*!
 * @brief Stop & undo RTC timer initialization
 *
 * @param void.
 * @retval void.
 */
void rtcTimer_DeInit(void);

/*!
 * @brief RTC_DRV_GetTimeDate wrapper
 *
 * Used in order to store current time & date in g_rtcTimer1CurrentTimeDate.
 * It should be called by rtcTimer1_AlarmCallback0 routine
 * installed by PEX in Alarm configuration of RTC.
 *
 * @param void.
 * @retval void.
 */
void rtcTimer_GetTimeDate(void);

/*!
 * @brief Synchronize rtc_timedate_t structure with g_rtcTimer1CurrentTimeDate value
 *
 * @param void * pointer to rtc_timedate_t current time & date.
 * @retval void.
 */
void rtcTimer_UpdateTimeDate(rtc_timedate_t *newTimeDate);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* FATFS_RTC_TIMER_H */
