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

#include "rtc_c55_hw_access.h"
#include "clock_manager.h"
#include "interrupt_manager.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The function is defined for use by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

#define MAX_32BIT (0xffffffffUL)
/* Value used for approximation when is made the conversion from ticks to seconds */
#define RTC_PPM_TOLERANCE (100UL)
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!< Table of RTC base pointers */
static RTC_Type * const g_rtcBase[RTC_INSTANCE_COUNT] = RTC_BASE_PTRS;

/*!< Table of RTC state struct pointers */
static rtc_state_t * g_rtcStateStruct[RTC_INSTANCE_COUNT] = {NULL, };

/*!< @brief Table used to store the RTC IRQ names */
static const IRQn_Type       g_rtcIrqNumbers[]   = RTC_IRQS;
/*!< @brief Table used to store the RTC API IRQ names */

static const IRQn_Type       g_rtcApiIrqNb[]     = RTC_API_IRQS;

/*!< Table of month length (in days) for the Un-leap-year*/
static const uint8_t ULY[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/*!< Table of month length (in days) for the Leap-year*/
static const uint8_t LY[] = {0U, 31U, 29U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

/*!< Number of days from begin of the non Leap-year*/
static const uint16_t MONTH_DAYS[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

/*******************************************************************************
 * Private Functions
 ******************************************************************************/
static uint32_t RTC_DRV_TicksToSeconds(uint32_t instance, uint32_t ticks);
static uint32_t RTC_DRV_SecondsToTicks(uint32_t instance, uint32_t seconds);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Init
 * Description   : This function initializes the RTC driver based on the
 *                 configuration given by user.
 * Return        : None
 * Implements    : RTC_DRV_Init_Activity
 *END**************************************************************************/
void RTC_DRV_Init(uint32_t instance,
                  rtc_state_t * const driverState,
                  const rtc_init_config_t * const initConfig)
{
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(driverState != NULL);
    DEV_ASSERT(initConfig != NULL);

    /* Check if the driver is already initialized */
    DEV_ASSERT(g_rtcStateStruct[instance] == NULL);

    base = g_rtcBase[instance];

    /* Initialize the state structure */
    driverState->rolloverCount  = 0UL;
    driverState->startTime      = 0UL;
    driverState->timeOffset     = 0UL;
    driverState->alarmConfig    = NULL;
    driverState->isAlarmTimeNew = false;
    g_rtcStateStruct[instance]  = driverState;

    /* Configure the RTC based on the user configuration and enable global and rollover interrupt */
    RTC_Configure(base,
                  initConfig->freezeEnable,
                  initConfig->clockSelect,
                  initConfig->divideBy512,
                  initConfig->divideBy32,
                  true,
                  true);

    /* Clear status flags */
    RTC_ClearStatusFlags(base, (RTC_RTCS_RTCF_MASK | RTC_RTCS_APIF_MASK | RTC_RTCS_ROVRF_MASK));
    /* Configure supervisor access mode */
    RTC_ConfigureNonSupervisorAccess(base, initConfig->nonSupervisorAccessEnable);
    /* Enable RTC IRQ */
    INT_SYS_EnableIRQ(g_rtcIrqNumbers[instance]);
    /* Update module frequency */
    RTC_DRV_UpdateModuleFrequency(instance);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetDefaultConfig
 * Description   : This function will save the default RTC configuration in the
 *                 structure passed by the user.
 * Return        : None
 * Implements    : RTC_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void RTC_DRV_GetDefaultConfig(rtc_init_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->clockSelect  = RTC_CLOCK_SOURCE_XOSC;
    config->divideBy32   = false;
    config->divideBy512  = false;
    config->freezeEnable = true;
    config->nonSupervisorAccessEnable = false;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_Deinit
 * Description   : This function will stop the RTC counter and will disable the
 *                 interrupts.
 * Return        : None
 * Implements    : RTC_DRV_Deinit_Activity
 *END**************************************************************************/
void RTC_DRV_Deinit(uint32_t instance)
{
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];

    /* Clear status register */
    RTC_ClearStatusFlags(base, (RTC_RTCS_RTCF_MASK | RTC_RTCS_APIF_MASK | RTC_RTCS_ROVRF_MASK));
    /* Clear the Control Register, this will trigger a software reset of the module */
    RTC_SetControlRegister(base, 0UL);
    /* Deallocate the state structure to avoid errors on reinitialization */
    g_rtcStateStruct[instance] = NULL;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_StartCounter
 * Description   : This function will start the counter if it was disabled.
 *                 Otherwise it will return
 * Return        : STATUS_SUCCESS if the operation was successful
 *                 STATUS_ERROR if the counter was already enabled
 * Implements    : RTC_DRV_StartCounter_Activity
 *END**************************************************************************/
status_t RTC_DRV_StartCounter(uint32_t instance)
{
    status_t statusCode;
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];

    /* Check if the counter is already enabled */
    if(RTC_IsCounterEnabled(base))
    {
        /* Set the status code to error */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Enable the counter */
        RTC_EnableCounter(base, true);
        statusCode = STATUS_SUCCESS;
    }

    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_StopCounter
 * Description   : This function will stop the counter if it was enabled.
 * WARNING       : When the counter is disabled it resets all the RTC logic.
 *                 In this case the module will need to be reinitialized.
 * Return        : STATUS_SUCCESS if the operation was successful
 *                 STATUS_ERROR if the counter was already disabled
 * Implements    : RTC_DRV_StopCounter_Activity
 *END**************************************************************************/
status_t RTC_DRV_StopCounter(uint32_t instance)
{
    status_t statusCode;
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];

    /* Check if the counter is already disabled */
    if(!RTC_IsCounterEnabled(base))
    {
        /* Set the status code to error */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Disable the counter */
        RTC_EnableCounter(base, false);
        /* Deallocate the state structure to avoid errors on reinitialization */
        g_rtcStateStruct[instance] = NULL;
        statusCode = STATUS_SUCCESS;
    }

    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_SetTimeDate
 * Description   : This function will set the date passed by the user.
 * Return        : STATUS_SUCCESS if the operation was successful
 *                 STATUS_ERROR if the date and time are invalid
 * Implements    : RTC_DRV_SetTimeDate_Activity
 *END**************************************************************************/
status_t RTC_DRV_SetTimeDate(uint32_t instance,
                             const rtc_timedate_t * const timeDate)
{
    status_t statusCode;
    const RTC_Type * base;
    rtc_state_t * driverState;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(timeDate != NULL);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];

    /* Check if the date/time format is correct */
    if(!RTC_DRV_IsTimeDateCorrectFormat(timeDate))
    {
        /* Set the status code to error */
        statusCode = STATUS_ERROR;
    }
    else
    {
        /* Save the value of the RTC counter in the state structure */
        driverState->timeOffset = RTC_GetCounterRegister(base);
        /* Reset the number of overflows */
        driverState->rolloverCount = 0UL;
        /* Convert the time and date to seconds and save it in the state structure */
        RTC_DRV_ConvertTimeDateToSeconds(timeDate, &driverState->startTime);
        statusCode = STATUS_SUCCESS;
    }

    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetTimeDate
 * Description   : This function will get the current time and date and it will
 *                 store in the structure passed by the user.
 * Return        : None
 * Implements    : RTC_DRV_GetTimeDate_Activity
 *END**************************************************************************/
void RTC_DRV_GetTimeDate(uint32_t instance,
                         rtc_timedate_t * const timeDate)
{
    const RTC_Type * base;
    const rtc_state_t * driverState;
    uint32_t counterValue;
    uint32_t secondsPassed;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(timeDate != NULL);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];

    /* Get the current counter value in ticks */
    counterValue = RTC_GetCounterRegister(base);
    /* Compute the number of the seconds passed since setting the time and date */
    secondsPassed = RTC_DRV_TicksToSeconds(instance, counterValue);
    /* Add the start time to the calculated value */
    secondsPassed += driverState->startTime;
    /* Convert the seconds into time/date format */
    RTC_DRV_ConvertSecondsToTimeDate(&secondsPassed, timeDate);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetInterruptFlag
 * Description   : This function will check whether the selected flag is set.
 * Return        : True if flag is set, false otherwise
 * Implements    : RTC_DRV_GetInterruptFlag_Activity
 *END**************************************************************************/
bool RTC_DRV_GetInterruptFlag(uint32_t instance,
                              rtc_interrupt_flag_t flag)
{
    bool isFlagSet;
    uint32_t statusReg;
    const RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];
    statusReg = RTC_GetStatusRegister(base);

    switch(flag)
    {
    case RTC_ROLLOVER_INTERRUPT:
        isFlagSet = ((statusReg & RTC_RTCS_ROVRF_MASK) != 0U) ? true : false;
        break;
    case RTC_API_INTERRUPT:
        isFlagSet = ((statusReg & RTC_RTCS_APIF_MASK) != 0U) ? true : false;
        break;
    case RTC_COUNTER_INTERRUPT:
        isFlagSet = ((statusReg & RTC_RTCS_RTCF_MASK) != 0U) ? true : false;
        break;
    default:
        isFlagSet = false;
        break;
    }

    return isFlagSet;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ClearInterruptFlag
 * Description   : This function will clear the selected flag.
 * Return        : None
 * Implements    : RTC_DRV_ClearInterruptFlag_Activity
 *END**************************************************************************/
void RTC_DRV_ClearInterruptFlag(uint32_t instance,
                                rtc_interrupt_flag_t flag)
{
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];

    switch(flag)
    {
    case RTC_ROLLOVER_INTERRUPT:
        RTC_ClearStatusFlags(base, RTC_RTCS_ROVRF_MASK);
        break;
    case RTC_API_INTERRUPT:
        RTC_ClearStatusFlags(base, RTC_RTCS_APIF_MASK);
        break;
    case RTC_COUNTER_INTERRUPT:
        RTC_ClearStatusFlags(base, RTC_RTCS_RTCF_MASK);
        break;
    default:
        /* Nothing to do here */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_GetStatusFlag
 * Description   : This function will check whether the selected flag is set.
 * Return        : True if flag is set, false otherwise
 * Implements    : RTC_DRV_GetStatusFlag_Activity
 *END**************************************************************************/
bool RTC_DRV_GetStatusFlag(uint32_t instance,
                           rtc_status_flag_t flag)
{
    bool isFlagSet;
    const RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];

    switch(flag)
    {
    case RTC_INVALID_ANL_WRITE:
        isFlagSet = RTC_GetStatusFlags(base, RTC_RTCS_INV_ANL_MASK);
        break;
    case RTC_INVALID_API_WRITE:
        isFlagSet = RTC_GetStatusFlags(base, RTC_RTCS_INV_API_MASK);
        break;
    case RTC_INVALID_RTCVAL_WRITE:
        isFlagSet = RTC_GetStatusFlags(base, RTC_RTCS_INV_RTC_MASK);
        break;
    default:
        isFlagSet = false;
        break;
    }

    return isFlagSet;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_UpdateModuleFrequency
 * Description   : This function will update the ticks number that are
 *                 passed in a second based on the frequency retrieved
 *                 from Clock Manager and module configuration.
 *                 This function needs to be called when the system
 *                 clocks or power mode are changed.
 *                 After calling this function both alarm and periodic
 *                 interrupts need to be reconfigured.
 * Return        : None
 * Implements    : RTC_DRV_UpdateModuleFrequency_Activity
 *END**************************************************************************/
void RTC_DRV_UpdateModuleFrequency(uint32_t instance)
{
    const RTC_Type * base;
    rtc_state_t * driverState;
    status_t clockManStatus;
    uint32_t div32;
    uint32_t div512;
    uint32_t temp;
    clock_names_t clockName;
    uint32_t frequency;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    /* Initilalize variables */
    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];
    clockName = SXOSC_CLK;
    /* Load the Control register */
    temp = RTC_GetControlRegister(base);
    /* Get enabled dividers */
    div32 =  ((temp & RTC_RTCC_DIV32EN_MASK) != 0UL) ?  32UL  : 1UL;
    div512 = ((temp & RTC_RTCC_DIV512EN_MASK) != 0UL) ? 512UL : 1UL;
    /* Get the configured clock source */
    temp = ((temp & RTC_RTCC_CLKSEL_MASK) >> RTC_RTCC_CLKSEL_SHIFT);
    switch(temp)
    {
    case 0x00UL:
        /* RTC_CLOCK_SOURCE_XOSC */
        clockName = SXOSC_CLK;
        break;
    case 0x01UL:
        /* RTC_CLOCK_SOURCE_SIRC */
        clockName = SIRC_CLK;
        break;
    case 0x02UL:
        /* RTC_CLOCK_SOURCE_FIRC */
        clockName = FIRC_CLK;
        break;
    case 0x03UL:
        /* RTC_CLOCK_SOURCE_FOSC */
        clockName = FXOSC_CLK;
        break;
    default:
        /* Should not enter this case */
        break;
    }
    /* Get clock frequency */
    clockManStatus = CLOCK_SYS_GetFreq(clockName, &frequency);
    DEV_ASSERT(clockManStatus == STATUS_SUCCESS);
    (void)clockManStatus;
    /* Calculate the number of ticks per second */
    driverState->ticksPerSecond = (frequency / (uint32_t)(div32 * div512));
    /* Verify whether the calculate RTC clock frequency is an integer and increment
     * ticksPerSecond variable if the non integer part of the frequency is
     * larger than 0.5 * ticksPerSecond
     */
    if((frequency % (uint32_t)(div32 * div512)) > ((uint32_t)(div32 * div512) / 2UL))
    {
    	driverState->ticksPerSecond++;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_TicksToSeconds
 * Description   : Static function that will return the number of seconds passed
 *                 based on the number of ticks
 * Return        : Number of seconds passed
 * Implements    : RTC_DRV_TicksToSeconds_Activity
 *END**************************************************************************/
static uint32_t RTC_DRV_TicksToSeconds(uint32_t instance,
                                       uint32_t ticks)
{
    rtc_state_t const * driverState;
    uint64_t seconds;
    uint64_t totalTicks;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    seconds = 0UL;

    driverState = g_rtcStateStruct[instance];
    /* Calculate the number of seconds passed since setting the time and date */
    totalTicks = (uint64_t)( ((uint64_t)driverState->rolloverCount * (uint64_t)MAX_32BIT) + (uint64_t)ticks - (uint64_t)driverState->timeOffset);
    seconds = totalTicks / (uint64_t)driverState->ticksPerSecond;
    /* Approximate if conversion of seconds from ticks is less than tolerance to not lose precision */
    if( (totalTicks % (uint64_t)driverState->ticksPerSecond) >= (((uint64_t)driverState->ticksPerSecond - (uint64_t)RTC_PPM_TOLERANCE )) )
    {
    	/* Increment seconds because value was truncated by div operation */
    	seconds++;
    }

    return (uint32_t)(seconds & MAX_32BIT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_SecondsToTicks
 * Description   : Static function that will return the number of RTC Counter
 *                 ticks for the specified number of seconds.
 * Return        : Number of ticks per second
 * Implements    : RTC_DRV_SecondsToTicks_Activity
 *END**************************************************************************/
static uint32_t RTC_DRV_SecondsToTicks(uint32_t instance,
                                       uint32_t seconds)
{
    const rtc_state_t * driverState;
    uint64_t ticks;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    ticks = 0UL;

    driverState = g_rtcStateStruct[instance];
    /* Calculate the number of ticks passed since setting the time and date */
    ticks = (uint64_t)(((uint64_t)seconds * (uint64_t)driverState->ticksPerSecond) + \
                       (uint64_t)driverState->timeOffset - ((uint64_t)driverState->rolloverCount * MAX_32BIT));

    return (uint32_t)(ticks & MAX_32BIT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigurePeriodicInterrupt
 * Description   : This function will configure a periodic interrupt(using the
 *                 RTC's API functionality) with the period configured by the user.
 *                 It will calculate the necessary compare value using periodInMs
 *                 parameter and the clock source of the module.
 * Return        : STATUS_SUCCESS if the operation was successful
 *                 STATUS_BUSY if the timer is not synchronized and the new value is set
 *                 STATUS_ERROR if the interrupt period could not be achieved
 *                 or if the register was not synchronized.
 * Implements    : RTC_DRV_ConfigurePeriodicInterrupt_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigurePeriodicInterrupt(uint32_t instance,
                                            uint32_t periodInMs,
                                            bool interruptEnable,
                                            bool enable)
{
    status_t statusCode;
    RTC_Type * base;
    const rtc_state_t * driverState;
    uint64_t configuredPeriod;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];
    statusCode = STATUS_SUCCESS;

    /* Disable API Interrupt request */
    INT_SYS_DisableIRQ(g_rtcApiIrqNb[instance]);

    if(enable)
    {
        /* Calculate the period */
        configuredPeriod = (((uint64_t)driverState->ticksPerSecond * periodInMs) / 1000UL) - 1UL;

        /* Check if the calculated period is valid */
        if((configuredPeriod > FEATURE_RTC_MINIMUM_API_VALUE) && (configuredPeriod < MAX_32BIT))
        {
            /* Configure the API with new values */
            statusCode = RTC_ConfigureAPI(base, (uint32_t)configuredPeriod, true, interruptEnable);
            /* Enable IRQ if requested */
            if(interruptEnable)
            {
                INT_SYS_EnableIRQ(g_rtcApiIrqNb[instance]);
            }
        }
        else
        {
            /* Set the return code to error */
            statusCode = STATUS_ERROR;
        }
    }
    else
    {
        /* Disable the API */
        base->RTCC &= ~(RTC_RTCC_APIEN_MASK | RTC_RTCC_APIIE_MASK);
    }

    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureComparatorTrigger
 * Description   : This function will configure the analog comparator trigger
 *                 period with the period configured by the user. It will
 *                 calculate the necessary compare value using periodInMs
 *                 parameter and the clock source of the module.
 * Return        : STATUS_SUCCESS if the operation was successful
 *                 STATUS_BUSY if the timer is not synchronized and the new value is set
 *                 STATUS_ERROR if the period could not be achieved
 *                 or if the register was not synchronized.
 * Implements    : RTC_DRV_ConfigureComparatorTrigger_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigureComparatorTrigger(uint32_t instance,
                                            uint32_t periodInMs,
                                            bool enable)
{
    status_t statusCode;
    RTC_Type * base;
    const rtc_state_t * driverState;
    uint64_t configuredPeriod;
    uint32_t apival;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];
    statusCode = STATUS_SUCCESS;
    apival = RTC_GetAPICompareRegister(base);

    if(enable)
    {
        /* Calculate the period */
        configuredPeriod = ((((uint64_t)driverState->ticksPerSecond * periodInMs) / 1000UL) / apival);

        /* Check if the calculated period is valid */
        if(configuredPeriod < MAX_32BIT)
        {
            statusCode = RTC_ConfigureCMPTrigger(base, (uint32_t)configuredPeriod, true);
        }
        else
        {
            /* Set the status code to error */
            statusCode = STATUS_ERROR;
        }
    }
    else
    {
        base->RTCC &= ~(RTC_RTCC_TRIG_EN_MASK);
    }

    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConfigureAlarm
 * Description   : This function will configure the alarm based on the
 *                 configuration structure passed by the user.
 *                 When using alarm that are configured to be repetitive,
 *                 enable the interrupt. Otherwise the repeat function will
 *                 not work.
 * Return        : STATUS_SUCCESS if the configuration is successful
 * 				   STATUS_BUSY if the timer is not synchronized and the new value is set
 *                 STATUS_ERROR if the alarm time is invalid, if the value of alarm don't
 *                 respect the minimum ticks requirement to allow timer to synchronize
 * Implements    : RTC_DRV_ConfigureAlarm_Activity
 *END**************************************************************************/
status_t RTC_DRV_ConfigureAlarm(uint32_t instance,
                                rtc_alarm_config_t * const alarmConfig)
{
    status_t statusCode;
    RTC_Type * base;
    rtc_state_t * driverState;
    uint32_t alarmTime;
    uint32_t counterValue;
    uint32_t currentTime;
    uint32_t newAlarmTime;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
    DEV_ASSERT(alarmConfig != NULL);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];

    /* Check if the alarm time is in a correct format */
    if (RTC_DRV_IsTimeDateCorrectFormat(&(alarmConfig->alarmTime)) == true)
    {
        driverState->alarmConfig = alarmConfig;
        /* Convert the time date to seconds */
        RTC_DRV_ConvertTimeDateToSeconds(&(alarmConfig->alarmTime), &alarmTime);
        /* Get current time in seconds */
        counterValue = RTC_GetCounterRegister(base);
        currentTime = RTC_DRV_TicksToSeconds(instance, counterValue);
        currentTime += driverState->startTime;

        /* Check if the alarm time is greater than current time */
        if(alarmTime > currentTime)
        {
            /* Write alarm time into Time Alarm Register */
            newAlarmTime = RTC_DRV_SecondsToTicks(instance, alarmTime - driverState->startTime);
            if(newAlarmTime >= (uint32_t)(counterValue + FEATURE_RTC_SYNC_TICKS_VALUE))
            {
            		statusCode = RTC_SetCompareRegister(base, newAlarmTime);
            		if(statusCode == STATUS_SUCCESS )
            		{
            			driverState->isAlarmTimeNew = true;
            		}
            		else
            		{
            			driverState->isAlarmTimeNew = false;
            		}
            }
            else
            {
            	statusCode = STATUS_ERROR;
            }
        }
        else
        {
            statusCode = STATUS_ERROR;
        }
    }
    else
    {
        statusCode = STATUS_ERROR;
    }

    /* Return the exit code */
    return statusCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IRQHandler
 * Description   : This method is the API's Interrupt handler for generic and
 *                 alarm IRQ. It will handle the alarm processing and calls the
 *                 user callbacks if they are not NULL.
 * Return        : None
 * Implements    : RTC_DRV_IRQHandler_Activity
 *END**************************************************************************/
void RTC_DRV_IRQHandler(uint32_t instance)
{
    uint32_t counterValue;
    uint32_t newAlarmTime;
    rtc_state_t * driverState;
    rtc_alarm_config_t * alarmConfig;
    RTC_Type * base;

    DEV_ASSERT(instance < RTC_INSTANCE_COUNT);

    base = g_rtcBase[instance];
    driverState = g_rtcStateStruct[instance];
    alarmConfig = driverState->alarmConfig;

    /* Check if an alarm has occurred */
    if (RTC_DRV_GetInterruptFlag(instance, RTC_COUNTER_INTERRUPT))
    {
        /* If the alarm interrupt configuration has been defined process the
         * alarm IRQ and the alarmIRQ has been enabled.
         */
        if ((alarmConfig != NULL) && (alarmConfig->alarmIntEnable == true))
        {
            /* If recurrence is enabled modify the alarm register to the next
             * alarm.
             */
            if ((alarmConfig->numberOfRepeats > 0UL) || (alarmConfig->repeatForever == true))
            {
                counterValue = RTC_GetCounterRegister(base);
                newAlarmTime = counterValue + RTC_DRV_SecondsToTicks(instance, alarmConfig->repetitionInterval);
                /* Check the value of newAlrm to be more than 6 ticks to have time to synchronize */
                if(newAlarmTime >= (uint32_t)(counterValue + FEATURE_RTC_SYNC_TICKS_VALUE))
                {
                	if(RTC_SetCompareRegister(base, newAlarmTime) == STATUS_SUCCESS )
                	{
                		driverState->isAlarmTimeNew = true;
                	}
                	else
                	{
                		driverState->isAlarmTimeNew = false;
                	}
                }
                /* If the alarm repeats forever, set number of repeats to 0
                 * to avoid an accidental trigger of the core overflow flag
                 */
                alarmConfig->numberOfRepeats = (alarmConfig->repeatForever == false) ? (alarmConfig->numberOfRepeats - 1UL) : 0UL;
            }
            else
            {
                /* Set the internal variable which indicates that a new alarm is enabled to false */
                g_rtcStateStruct[instance]->isAlarmTimeNew = false;
            }
            /* If the user has defined a callback, call it */
            if (alarmConfig->alarmCallback != NULL)
            {
                alarmConfig->alarmCallback(alarmConfig->callbackParams);
            }
        }
        /* Clear alarm interrupt flag */
        RTC_DRV_ClearInterruptFlag(instance, RTC_COUNTER_INTERRUPT);
    }
    /* If the IRQ is not caused by the alarm then call the user callback if
     * defined.
     */
    else if (RTC_DRV_GetInterruptFlag(instance, RTC_ROLLOVER_INTERRUPT))
    {
        driverState->rolloverCount++;
        RTC_DRV_ClearInterruptFlag(instance, RTC_ROLLOVER_INTERRUPT);
    }
    else
    {
        /* Do nothing*/
        ;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConvertSecondsToTimeDate
 * Description   : This method converts seconds into time-date format.
 * Return        : None
 * Implements    : RTC_DRV_ConvertSecondsToTimeDate_Activity
 *END**************************************************************************/
void RTC_DRV_ConvertSecondsToTimeDate(const uint32_t * const seconds,
                                      rtc_timedate_t * const timeDate)
{
    DEV_ASSERT(seconds != NULL);
    DEV_ASSERT(timeDate != NULL);

    /* Declare the variables needed */
    uint8_t i;
    bool yearLeap;
    uint32_t numberOfDays;
    uint32_t tempSeconds;
    uint16_t daysInYear;

    /* Because the starting year(1970) is not leap, set the daysInYear
     * variable with the number of the days in a normal year
     */
    daysInYear = DAYS_IN_A_YEAR;

    /* Set the year to the beginning of the range */
    timeDate->year = YEAR_RANGE_START;

    /* Get the number of days */
    numberOfDays = (*seconds) / SECONDS_IN_A_DAY;
    /* Get the number of seconds remaining */
    tempSeconds = (*seconds) % SECONDS_IN_A_DAY;

    /* Get the current hour */
    timeDate->hour        = (uint16_t)(tempSeconds / SECONDS_IN_A_HOUR);
    /* Get the remaining seconds */
    tempSeconds           = tempSeconds % SECONDS_IN_A_HOUR;
    /* Get the minutes */
    timeDate->minutes     = (uint16_t)(tempSeconds / SECONDS_IN_A_MIN);
    /* Get seconds */
    timeDate->seconds     = (uint8_t)(tempSeconds % SECONDS_IN_A_MIN);

    /* Get the current year */
    while (numberOfDays >= daysInYear)
    {
        /* Increment year if the number of days is greater than the ones in
         * one year
         */
        timeDate->year++;
        /* Subtract the number of the days */
        numberOfDays -= daysInYear;

        /* Check if the year is leap or unleap */
        if (!RTC_DRV_IsYearLeap(timeDate->year))
        {
            /* Set the number of non leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_YEAR;
        }
        else
        {
            /* Set the number of leap year to the current year number
             * of days.
             */
            daysInYear = DAYS_IN_A_LEAP_YEAR;
        }
    }

    /* Add the current day */
    numberOfDays += 1U;

    /* Check if the current year is leap */
    yearLeap = RTC_DRV_IsYearLeap(timeDate->year);

    /* Get the month */
    for (i = 1U; i <= 12U; i++)
    {
        uint32_t daysInCurrentMonth = ((yearLeap == true) ? (uint32_t)LY[i] : (uint32_t)ULY[i]);
        if (numberOfDays <= daysInCurrentMonth)
        {
            timeDate->month = (uint16_t)i;
            break;
        }
        else
        {
            numberOfDays -= daysInCurrentMonth;
        }

    }

    /* Set the current day */
    timeDate->day = (uint16_t)numberOfDays;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_ConvertTimeDateToSeconds
 * Description   : This method converts time-date into seconds.
 * Return        : None
 * Implements    : RTC_DRV_ConvertTimeDateToSeconds_Activity
 *END**************************************************************************/
void RTC_DRV_ConvertTimeDateToSeconds(const rtc_timedate_t * const timeDate,
                                      uint32_t * const seconds)
{
    DEV_ASSERT(seconds != NULL);
    DEV_ASSERT(timeDate != NULL);

    /* Declare local variables */
    uint16_t year;

    /* Convert years to seconds */
    (*seconds) = (uint32_t)(DAYS_IN_A_YEAR * (uint32_t)(SECONDS_IN_A_DAY));
    (*seconds) *= ((uint32_t)timeDate->year - YEAR_RANGE_START);

    /* Add the seconds from the leap years */
    for (year = YEAR_RANGE_START; year < timeDate->year; year++)
    {
        if (RTC_DRV_IsYearLeap(year))
        {
            (*seconds) += SECONDS_IN_A_DAY;
        }
    }

    /* If the current year is leap and 29th of February has passed, add
     * another day to seconds passed.
     */
    if ((RTC_DRV_IsYearLeap(year)) && (timeDate->month > 2U))
    {
        (*seconds) += SECONDS_IN_A_DAY;
    }

    /* Add the rest of the seconds from the current month */
    (*seconds) += MONTH_DAYS[timeDate->month] * SECONDS_IN_A_DAY;
    /* Add the rest of the seconds from the current day */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->day - 1U) * (uint32_t)SECONDS_IN_A_DAY);
    /* Add the rest of the seconds from the current time */
    (*seconds) += (uint32_t)(((uint32_t)timeDate->hour * SECONDS_IN_A_HOUR)   + \
                             ((uint32_t)timeDate->minutes * SECONDS_IN_A_MIN) + \
                             (uint32_t)timeDate->seconds);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsTimeDateCorrectFormat
 * Description   : This method checks if date-time structure is in a correct
 *                 format
 * Return        : True if the following conditions are met:
 *                  - is a valid year, month and date
 *                  - is a valid time format
 *                 False otherwise
 * Implements    : RTC_DRV_IsTimeDateCorrectFormat_Activity
 *END**************************************************************************/
bool RTC_DRV_IsTimeDateCorrectFormat(const rtc_timedate_t * const timeDate)
{
    DEV_ASSERT(timeDate != NULL);

    /* Declare local variables */
    bool returnCode;
    const uint8_t * pDays;

    /* Set the days-in-month table for the corresponding year */
    pDays = RTC_DRV_IsYearLeap(timeDate->year) ? (LY) : (ULY);

    /* Check if the time and date are in the correct ranges */
    if ((timeDate->year < YEAR_RANGE_START) || (timeDate->year > YEAR_RANGE_END)
            ||  (timeDate->month < 1U) || (timeDate->month > 12U)
            ||  (timeDate->day < 1U) || (timeDate->day > 31U)
            ||  (timeDate->hour >= HOURS_IN_A_DAY)
            ||  (timeDate->minutes >= MINS_IN_A_HOUR) || (timeDate->seconds >= SECONDS_IN_A_MIN))
    {
        returnCode = false;
    }
    /* Check if the day is a valid day from the corresponding month */
    else if (timeDate->day > pDays[timeDate->month])
    {
        returnCode = false;
    }
    else
    {
        returnCode = true;
    }

    /* Return the exit code */
    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsYearLeap
 * Description   : This method checks if the year passed as a parameter is a leap
 *                 one.
 * Return        : True if the year is leap, false if otherwise.
 * Implements    : RTC_DRV_IsYearLeap_Activity
 *END**************************************************************************/
bool RTC_DRV_IsYearLeap(uint16_t year)
{
    bool isYearLeap;

    if ((year % 4U) > 0U)
    {
        isYearLeap = false;
    }
    else if ((year % 100U) > 0U)
    {
        isYearLeap = true;
    }
    else if ((year % 400U) > 0U)
    {
        isYearLeap = false;
    }
    else
    {
        isYearLeap = true;
    }

    /* Return the exit code */
    return isYearLeap;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_DRV_IsSyncRTCVAL
 * Description   : This method checks if the RTCVAL Value is synchronized with the
 * 				   RTC Counter
 * Return        : True if the new RTCVAL is accepted, false if otherwise.
 * Implements    : RTC_DRV_IsSyncRTCVAL_Activity
 *END**************************************************************************/
bool RTC_DRV_IsSyncRTCVAL(uint32_t instance)
{
	 bool status;
     RTC_Type const * base;

     DEV_ASSERT(instance < RTC_INSTANCE_COUNT);
     base = g_rtcBase[instance];

     if((RTC_GetStatusRegister(base) & RTC_RTCS_INV_RTC_MASK) == 0U)
     {
    	status=true;
     }
     else
     {
    	status=false;
     }
     return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
