/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RTC_C55_DRIVER_H
#define RTC_C55_DRIVER_H

/*! @file rtc_driver.h */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "device_registers.h"
#include "status.h"

/*!
 * @defgroup rtc_c55_driver Real Time Clock Driver (RTC-API)
 * @brief Real Time Clock Driver for RTC-API Documentation
 * @ingroup rtc_c55
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SECONDS_IN_A_DAY     (86400UL)
#define SECONDS_IN_A_HOUR    (3600U)
#define SECONDS_IN_A_MIN     (60U)
#define MINS_IN_A_HOUR       (60U)
#define HOURS_IN_A_DAY       (24U)
#define DAYS_IN_A_YEAR       (365U)
#define DAYS_IN_A_LEAP_YEAR  (366U)
#define YEAR_RANGE_START     (1970U)
#define YEAR_RANGE_END       (2099U)

/*!
 * @brief Enum containing the RTC module clock sources
 * Implements : rtc_clk_select_t_Class
 */
typedef enum
{
    RTC_CLOCK_SOURCE_XOSC = 0x00U, /*!< RTC_CLOCK_SOURCE_XOSC */
    RTC_CLOCK_SOURCE_SIRC = 0x01U, /*!< RTC_CLOCK_SOURCE_SIRC */
    RTC_CLOCK_SOURCE_FIRC = 0x02U, /*!< RTC_CLOCK_SOURCE_FIRC */
    RTC_CLOCK_SOURCE_FOSC = 0x03U  /*!< RTC_CLOCK_SOURCE_FOSC */
} rtc_clk_select_t;

/*!
 * @brief Enum containing RTC interrupt flags
 * Implements : rtc_interrupt_flag_t_Class
 */
typedef enum
{
    RTC_COUNTER_INTERRUPT  = 0x00U, /*!< RTC_COUNTER_INTERRUPT_FLAG  */
    RTC_API_INTERRUPT      = 0x01U, /*!< RTC_API_INTERRUPT_FLAG      */
    RTC_ROLLOVER_INTERRUPT = 0x02U  /*!< RTC_ROLLOVER_INTERRUPT_FLAG */
} rtc_interrupt_flag_t;

/*!
 * @brief Enum containing RTC status flags
 * Implements : rtc_status_flag_t_Class
 */
typedef enum
{
    RTC_INVALID_RTCVAL_WRITE = 0x00U, /*!< RTC_INVALID_RTCVAL_WRITE */
    RTC_INVALID_API_WRITE    = 0x01U, /*!< RTC_INVALID_API_WRITE    */
    RTC_INVALID_ANL_WRITE    = 0x02U  /*!< RTC_INVALID_ANL_WRITE    */
} rtc_status_flag_t;

/*!
 * @brief RTC alarm callback typedef
 * Implements : rtc_callback_t_Class
 */
typedef void (*rtc_callback_t)(void * callbackParams) ;

/*!
 * @brief RTC Initialization structure
 * Implements : rtc_init_config_t_Class
 */
typedef struct
{
    bool                    freezeEnable;               /*!< Enable freezing the counter in debug mode             */
    bool                    divideBy32;                 /*!< Enable clock divide by 32                             */
    bool                    divideBy512;                /*!< Enable clock divide by 512                            */
    bool                    nonSupervisorAccessEnable;  /*!< Enable writes to the registers in non Supervisor Mode */
    rtc_clk_select_t        clockSelect;                /*!< RTC Clock Select                                      */
} rtc_init_config_t;

/*!
 * @brief RTC Time Date structure
 * Implements : rtc_timedate_t_Class
 */
typedef struct
{
    uint16_t year;      /*!< Year       */
    uint16_t month;     /*!< Month      */
    uint16_t day;       /*!< Day        */
    uint16_t hour;      /*!< Hour       */
    uint16_t minutes;   /*!< Minutes    */
    uint8_t seconds;    /*!< Seconds    */
} rtc_timedate_t;

/*!
 * @brief RTC alarm configuration
 * Implements : rtc_alarm_config_t_Class
 */
typedef struct
{
    rtc_timedate_t  alarmTime;          /*!< Alarm time                                       */
    uint32_t        repetitionInterval; /*!< Interval of repetition in seconds                */
    uint32_t        numberOfRepeats;    /*!< Number of alarm repeats                          */
    bool            repeatForever;      /*!< Repeat forever if set, discard number of repeats */
    bool            alarmIntEnable;     /*!< Enable alarm interrupt                           */
    rtc_callback_t  alarmCallback;      /*!< Pointer to the user callback method.             */
    void          * callbackParams;     /*!< Pointer to the callback parameters.              */
} rtc_alarm_config_t;

/*!
 * @brief static RTC runtime structure, it is designed only for internal
 * purposes such as storing interrupt configuration for each instance.
 * Implements : rtc_state_t_Class
 */
typedef struct
{
    rtc_alarm_config_t          * alarmConfig;      /*!< Alarm configuration                  */
    volatile bool                 isAlarmTimeNew;   /*!< Check if there is a new alarm        */
    uint32_t                      timeOffset;       /*!< Time offset saved when the time
                                                     *   and date were configured             */
    uint32_t                      startTime;        /*!< Time stamp with the start time,
                                                     *   used for calculating the current
                                                     *   time                                 */
    volatile uint32_t             rolloverCount;    /*!< Overflow count, it is incremented
                                                     *   when the counter has overflowed to
                                                     *   keep the accuracy                    */
    uint32_t                      ticksPerSecond;   /*!< Number of ticks per second           */
} rtc_state_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initialize RTC driver
 * This function initializes the RTC driver based on the configuration given by user.
 *
 * @param[in] instance      Instance number
 * @param[in] driverState   Pointer to the internal state structure, used for storing
 *                          data by the driver.
 * @param[in] initConfig    Pointer to the user configuration structure
 * @return None
 */
void RTC_DRV_Init(uint32_t instance,
                  rtc_state_t * const driverState,
                  const rtc_init_config_t * const initConfig);

/*!
 * @brief Deinitializes RTC driver
 * This function will stop the RTC counter and will disable the interrupts.
 *
 * @param[in] instance Instance number
 * @return None
 */
void RTC_DRV_Deinit(uint32_t instance);

/*!
 * @brief Gets the default configuration for the RTC
 * This function will save the default RTC configuration in the structure passed
 * by the user.
 *
 * @param[out] config Pointer to the configuration structure
 * @return None
 */
void RTC_DRV_GetDefaultConfig(rtc_init_config_t * const config);

/*!
 * @brief Start the RTC counter
 * This function will start the counter if it was disabled. It will return
 * STATUS_ERROR if the counter was already enabled. Otherwise it will return STATUS_SUCCESS.
 *
 * @param[in] instance Instance number
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful
 *         - STATUS_ERROR if the counter was already enabled
 */
status_t RTC_DRV_StartCounter(uint32_t instance);

/*!
 * @brief Stop the RTC counter
 * This function will stop the counter if it was enabled. It will return
 * STATUS_ERROR if the counter is already disabled. Otherwise it will return STATUS_SUCCESS.
 *
 * @param[in] instance Instance number
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful
 *         - STATUS_ERROR if the counter was already disabled
 */
status_t RTC_DRV_StopCounter(uint32_t instance);

/*!
 * @brief Set the time and date
 * This function will set the date passed by the user. It will return STATUS_ERROR
 * if the time and date are invalid. Otherwise it will return STATUS_SUCCESS.
 *
 * @param[in] instance Instance number
 * @param[in] timeDate Pointer to the structure containing the time and date
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful
 *         - STATUS_ERROR if the date and time are invalid
 */
status_t RTC_DRV_SetTimeDate(uint32_t instance,
                             const rtc_timedate_t * const timeDate);

/*!
 * @brief Get the time and date
 * This function will get the current time and date and it will store in the structure
 * passed by the user.
 *
 * @param[in]  instance Instance number
 * @param[out] timeDate Pointer to the structure where to store the retrieved time and date
 * @return None
 */
void RTC_DRV_GetTimeDate(uint32_t instance,
                         rtc_timedate_t * const timeDate);

/*!
 * @brief Update internal state frequency from the one in hardware
 * This function will update the ticks number that are passed in a second based on the frequency
 * retrieved from Clock Manager and module configuration.
 * This function needs to be called when the system clocks or power mode are changed.
 * After calling this function both alarm and periodic interrupts need to be reconfigured
 * @param[in] instance Instance number
 * @return None
 */
void RTC_DRV_UpdateModuleFrequency(uint32_t instance);

/*!
 * @brief Get interrupt flag
 * This function will get the passed flag status.
 * @param[in] instance Instance number
 * @param[in] flag     Interrupt flag
 * @return Flag status:
 *         - True if the flag is set
 *         - False otherwise
 */
bool RTC_DRV_GetInterruptFlag(uint32_t instance,
                              rtc_interrupt_flag_t flag);
/*!
 * @brief Clear interrupt flag
 * This function will clear the selected interrupt flag
 * @param[in] instance Instance number
 * @param[in] flag     Interrupt flag
 * @return None
 */
void RTC_DRV_ClearInterruptFlag(uint32_t instance,
                                rtc_interrupt_flag_t flag);
/*!
 * @brief Get status flag
 * This function will get the passed flag status.
 * @param[in] instance Instance number
 * @param[in] flag     Status flag
 * @return Flag status:
 *         - True if the flag is set
 *         - False otherwise
 */
bool RTC_DRV_GetStatusFlag(uint32_t instance,
                           rtc_status_flag_t flag);

/*!
 * @brief Configure the alarm
 * This function will configure the alarm based on the configuration structure passed by the user.
 * It will return STATUS_ERROR if the alarm time configured is invalid, if the time and date
 * are not in future. If the time date meets synchronization requirements
 * and timer is not synchronized will report STATUS_BUSY. Otherwise it will return STATUS_SUCCESS.
 * @note  When using alarm that are configured to be repetitive, enable the interrupt. Otherwise the
 * repeat function will not work.
 *
 * @param[in] instance    Instance number
 * @param[in] alarmConfig Pointer to the alarm configuration
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful and timer syncronized
 *         - STATUS_BUSY if the timer is not syncronized and alarm value succesfully set
 *         - STATUS_ERROR if the date and time are invalid, or if they are not in the future
 *         				  or meet the timer syncronization requirements.
 */
status_t RTC_DRV_ConfigureAlarm(uint32_t instance,
                                rtc_alarm_config_t * const alarmConfig);

/*!
 * @brief Configure the periodic interrupt
 * This function will configure a periodic interrupt(using the RTC's API functionality) with the
 * period configured by the user. It will calculate the necessary compare value using periodInMs
 * parameter and the clock source of the module. If the calculated value is computed correctly
 * it will return STATUS_SUCCESS. If the time date does not meet synchronization requirements
 * and timer is not synchronized will report STATUS_BUSY otherwise it will return STATUS_ERROR
 * and the functionality will not be enabled.
 * @note If the interrupt is enabled, then the user must clear the APIF flag in the ISR.
 *
 * @param[in] instance        Instance number
 * @param[in] periodInMs      Period duration in milliseconds
 * @param[in] interruptEnable Interrupt enable flag
 * @param[in] enable          Enable API
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful
 *         - STATUS_ERROR if the interrupt period could not be achieved
 */
status_t RTC_DRV_ConfigurePeriodicInterrupt(uint32_t instance,
                                            uint32_t periodInMs,
                                            bool interruptEnable,
                                            bool enable);

/*!
 * @brief Configure the analog comparator trigger period
 * This function will configure the analog comparator trigger period with the
 * period configured by the user. It will calculate the necessary compare value using periodInMs
 * parameter and the clock source of the module. If the calculated value is computed correctly
 * it will return STATUS_SUCCESS. If the time date does not meet synchronization requirements
 * and timer is not synchronized will report STATUS_BUSY otherwise it will return STATUS_ERROR
 * and the functionality will not be enabled.
 *
 * @param[in] instance        Instance number
 * @param[in] periodInMs      Period duration in milliseconds
 * @param[in] enable          Trigger enable / disable
 * @return Operation status:
 *         - STATUS_SUCCESS if the operation was successful
 *         - STATUS_ERROR if the period could not be achieved
 */
status_t RTC_DRV_ConfigureComparatorTrigger(uint32_t instance,
                                            uint32_t periodInMs,
                                            bool enable);

/*!
 * @brief Convert seconds to rtc_timedate_t structure
 *
 * @param[in]  seconds Pointer to the seconds
 * @param[out] timeDate Pointer to the structure in which to store the result
 * @return     None
 */
void RTC_DRV_ConvertSecondsToTimeDate(const uint32_t * seconds,
                                      rtc_timedate_t * const timeDate);

/*!
 * @brief Convert rtc_timedate_t structure to seconds
 *
 * @param[in]  timeDate Pointer to the source struct
 * @param[out] seconds Pointer to the variable in which to store the result
 * @return     None
 */
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t * const timeDate,
                                      uint32_t * const seconds);

/*!
 * @brief Check if the current year is leap
 *
 * @param[in] year Year to check
 * @return    True if the year is leap, false if not
 */
bool RTC_DRV_IsYearLeap(uint16_t year);

/*!
 * @brief Check if the date time struct is configured properly
 *
 * @param[in] timeDate Structure to check to check
 * @return    True if the time date is in the correct format, false if not
 */
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t * const timeDate);

/*!
 * @brief This method is the API's Interrupt handler for generic and alarm IRQ.
 * It will handle the alarm processing and calls the user callback if they
 * are not NULL.
 *
 * @param[in] instance RTC instance used
 * @return    None
 */
void RTC_DRV_IRQHandler(uint32_t instance);

/*!
 * @brief Check if the RTCVAL value is syncronized with timer.
 * This function is used to feature a wait mechanisme to complete
 * timer syncronization
 *
 * @param[in] instance        Instance number
 * @return    True if the timer is syncronized, false if not
 */
bool RTC_DRV_IsSyncRTCVAL(uint32_t instance);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* RTC_C55_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
