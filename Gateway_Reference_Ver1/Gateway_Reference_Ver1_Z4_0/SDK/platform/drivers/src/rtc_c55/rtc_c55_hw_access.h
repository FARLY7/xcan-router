/*
 * Copyright 2017-2018 NXP
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

#ifndef RTC_C55_HW_ACCESS_H
#define RTC_C55_HW_ACCESS_H

/*! @file rtc_c55_hw_access.h */


#include "rtc_c55_driver.h"
/*!
 * rtc_c55_hw_access RTC C55 Hardware Access
 * @details This section describes the programming interface of the RTC C55  Hardware Access.
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name RTC C55 Hardware Access
 * @{
 */

/*!
 * @brief Configure the RTC module
 * @param[in] base               Base address of the module
 * @param[in] freezeEnable       Selects if the counter freezes when the chip is in debug
 * @param[in] clockSelect        Selects the clock input @see rtc_clk_select_t
 * @param[in] divBy512           Enables input clock divide-by-512
 * @param[in] divBy32            Enables input clock divide-by-32
 * @param[in] rolloverIntEnable  Enables counter rollover interrupt
 * @param[in] globalIntEnable    Enables global RTC interrupts
 * @return None
 */
void RTC_Configure(RTC_Type * const base,
                   bool freezeEnable,
                   rtc_clk_select_t clockSelect,
                   bool divBy512,
                   bool divBy32,
                   bool rolloverIntEnable,
                   bool globalIntEnable
                   );

/*!
 * @brief Configures the API function of the RTC module. Before the configuration
 * it waits for the synchronization of the register to be completed.
 * @param[in] base            Base address of the module
 * @param[in] value           Compare value
 * @param[in] enable          Selects if the functionality is enabled
 * @param[in] interruptEnable Selects if the API interrupt is enabled
 * @return Operation status:
 *         - STATUS_SUCCESS - if the operation was completed
 *         - STATUS_ERROR   - if the synchronization has timed out
 */
status_t RTC_ConfigureAPI(RTC_Type * const base,
                          uint32_t value,
                          bool enable,
                          bool interruptEnable);

/*!
 * @brief Configures the Analog Comparator trigger feature of the RTC module.
 * Before the configuration is made it waits for the synchronization of the
 * register to be completed.
 * @param[in] base            Base address of the module
 * @param[in] value           Compare value
 * @param[in] enable          Selects if the functionality is enabled
 * @return Operation status:
 *         - STATUS_SUCCESS - if the operation was completed
 *         - STATUS_ERROR   - if the synchronization has timed out
 */
status_t RTC_ConfigureCMPTrigger(RTC_Type * const base,
                                 uint32_t value,
                                 bool enable);

/*!
 * @brief Configures the Compare function of the RTC module.
 * Before the configuration reports the synchronization of the register
 * with the timer is completed or not.
 * @param[in] base            Base address of the module
 * @param[in] value           Compare value
 * @return Operation status:
 *         - STATUS_SUCCESS - if the operation was completed
 *         - STATUS_BUSY    - if the timer was set but not synchronized
 *         - STATUS_ERROR   - if fails to set the new value
 */
status_t RTC_SetCompareRegister(RTC_Type * const base,
                                uint32_t value);

/*!
 * @brief Returns the status of the RTC counter
 * @param[in] base Base address of the module
 * @return Value of RTC Counter enabled:
 *         - true - if the counter is enabled
 *         - false - if the counter is not enabled
 */
static inline bool RTC_IsCounterEnabled(RTC_Type const * const base)
{
    return ((base->RTCC & RTC_RTCC_CNTEN_MASK) != 0U) ? true : false;
}

/*!
 * @brief Enables the RTC counter
 * @note When the counter is disabled the RTC logic and registers are reset,
 * so an initialization sequence must be done.
 * @param[in] base   Base address of the module
 * @param[in] enable Enable/disable the counter
 * @return None
 */
static inline void RTC_EnableCounter(RTC_Type * const base,
                                     bool enable)
{
    if(enable)
    {
        base->RTCC |= (uint32_t)(1UL << RTC_RTCC_CNTEN_SHIFT);
    }
    else
    {
        base->RTCC &= ~(uint32_t)(1UL << RTC_RTCC_CNTEN_SHIFT);
    }
}

/*!
 * @brief Returns the value of the Control register
 * @param[in] base Base address of the module
 * @return Value of the Counter register
 */
static inline uint32_t RTC_GetControlRegister(RTC_Type const * const base)
{
    return base->RTCC;
}

/*!
 * @brief Set Control Register
 * @param[in] base  Base address of the module
 * @param[in] value Value to be set in Control register
 * @return None
 */
static inline void RTC_SetControlRegister(RTC_Type * const base,
                                          uint32_t value)
{
    base->RTCC = value;
}

/*!
 * @brief Returns the value of the Status register
 * @param[in] base Base address of the module
 * @return Value of the Status register
 */
static inline uint32_t RTC_GetStatusRegister(RTC_Type const * const base)
{
    return base->RTCS;
}

/*!
 * @brief Set Status Register
 * @param[in] base  Base address of the module
 * @param[in] value Value to be set in Status register
 * @return None
 */
static inline void RTC_SetStatusRegister(RTC_Type * const base,
                                          uint32_t value)
{
    base->RTCS = value;
}

/*!
 * @brief Clear flags from Status register
 * @param[in] base Base address of the module
 * @param[in] mask Mask of the flags to be cleared
 * @return None
 */
static inline void RTC_ClearStatusFlags(RTC_Type * const base,
                                        uint32_t mask)
{
    base->RTCS |= mask;
}

/*!
 * @brief Retrieve selected flag status from the Status register
 * @param[in] base Base address of the module
 * @param[in] mask Mask of the flags to be retrieved
 * @return Flag test:
 *         - true  - if at least one of the requested flags are set
 *         - false - if none of the flags are set
 */
static inline bool RTC_GetStatusFlags(RTC_Type const * const base,
                                      uint32_t mask)
{
    return ((base->RTCS & mask) != 0U) ? true : false;
}

/*!
 * @brief Returns the value of the RTC Counter
 * @param[in] base Base address of the module
 * @return Value of the RTC counter
 */
static inline uint32_t RTC_GetCounterRegister(RTC_Type const * const base)
{
    return base->RTCCNT;
}

/*!
 * @brief Returns the value of the Compare register
 * @param[in] base Base address of the module
 * @return Value of the Compare register
 */
static inline uint32_t RTC_GetCompareRegister(RTC_Type const * const base)
{
    return base->RTCVAL;
}

/*!
 * @brief Returns the value of the API Compare register
 * @param[in] base Base address of the module
 * @return Value of the API Compare register
 */
static inline uint32_t RTC_GetAPICompareRegister(RTC_Type const * const base)
{
    return base->APIVAL;
}

/*!
 * @brief Configures Non Supervisor access to the RTC registers
 * @param[in] base   Base address of the module
 * @param[in] enable Enable/Disable access to the registers in User mode
 * @return None
 */
static inline void RTC_ConfigureNonSupervisorAccess(RTC_Type * const base,
                                                    bool enable)
{
    base->RTCSUPV &= ~(RTC_RTCSUPV_SUPV(1));
    base->RTCSUPV |= RTC_RTCSUPV_SUPV((enable == true) ? 0UL : 1UL);
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* RTC_C55_HW_ACCESS_H */
