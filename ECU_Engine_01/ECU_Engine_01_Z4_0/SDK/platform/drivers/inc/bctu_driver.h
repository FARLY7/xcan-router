/*
 * Copyright 2017 NXP
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

#ifndef BCTU_DRIVER_H
#define BCTU_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "status.h"

/*! @file */

/*!
 * @addtogroup bctu_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Structure for configuring global BCTU functionalities
 *
 * This structure is used for configuring global BCTU functionalities
 *
 * Implements : bctu_config_t_Class
 */
typedef struct {
    bool lowPowerModeEn;            /*!< Low power operating mode: true - low power; false - normal operating mode. */
    bool globalHwTriggersEn;        /*!< HW triggers globally enabled/disabled: true - enabled; false - all hw triggers disabled (BCTU in freeze mode). */
    bool globalTriggersEn;          /*!< Triggers globally enabled/disabled: true - enabled; false - disabled (BCTU in configuration mode). */
    uint8_t dmaEnMask;              /*!< Bitmask for DMA transfer enable per ADC instance: (1 << n) corresponds to n-th ADC instance. */
    bool triggerIntEn;              /*!< Enable interrupt request when at least on ADC has been triggered (MSR.TRGF flag is set). */
    bool listIntEn;                 /*!< Enable interrupt request when a LIST last conversion is executed. */
    uint8_t newDataIntEnMask;       /*!< Bitmask for enabling interrupt when new data is available, per ADC instance: (1 << n) corresponds to n-th ADC instance. */
} bctu_config_t;

/*!
 * @brief Structure for configuring a trigger configuration register.
 *
 * This structure is used for configuring a trigger configuration register (TRGCFG)
 *
 * Implements : bctu_trig_config_t_Class
 */
typedef struct {
    bool loopEn;                  /*!< Select if a trigger (hw or sw) starts a loop of conversions. */
    uint8_t adcTargetMask;        /*!< Bitmask to select target ADC instance(s): (1 << n) corresponds to n-th ADC instance. */
    bool hwTriggersEn;            /*!< Enable/disable initiation of ADC conversions by hw input trigger: true - enabled; false - disabled. */
} bctu_trig_config_t;

/*!
 * @brief Structure for storing full information of the conversion result
 *
 * This structure is used for storing full information of the conversion result
 *
 * Implements : bctu_conv_result_t_Class
 */
typedef struct {
    uint8_t triggerIdx;    /*!< Index of the trigger which started the conversion. */
    uint8_t chanIdx;       /*!< Index of the ADC channel which executed the conversion. */
    uint16_t adcData;      /*!< Conversion result data. */
    bool listFlag;         /*!< Conversion initiated from a LIST: true / false. */
    bool lastFlag;         /*!< Last conversion in a LIST: true / false */
} bctu_conv_result_t;

/*!
 * @brief Enumeration for selecting between BCTU status flags
 *
 * This enumeration is used for selecting between BCTU status flags
 *
 * Implements : bctu_status_flag_t_Class
 */
typedef enum {
    BCTU_FLAG_ANY_ADC_TRIGGERED   = 0x0,  /*!< Trigger flag: indicates that at least one ADC was triggered (single flag for all ADC instances). */
    BCTU_FLAG_LIST_LAST_CONV      = 0x1,  /*!< Flag for execution of last conversion in a LIST (dedicated flag for each ADC instance). */
    BCTU_FLAG_DATA_OVERRUN        = 0x2,  /*!< Data Overrun flag (dedicated flag for each ADC instance). */
    BCTU_FLAG_NEW_DATA_AVAILABLE  = 0x3   /*!< New Data Available flag (dedicated flag for each ADC instance). */
} bctu_status_flag_t;

/*!
 * @brief Enumeration for selecting duration of write protection disable
 *
 * This enumeration selects the duration for which write protect is disabled
 *
 * Implements : bctu_wr_prot_duration_t_Class
 */
typedef enum {
    BCTU_WR_PROT_TEMPORARY = 0x0u,
    BCTU_WR_PROT_PERMANENT = 0x1u
} bctu_wr_prot_duration_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif


/*!
 * @name Global configuration
 */
/*! @{*/

/*!
 * @brief Configure global functionalities of a BCTU instance.
 *
 * This function configures the global functionalities of a BCTU instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] config - pointer to the input configuration structure
 * @return void
 */
void BCTU_DRV_Config(const uint32_t instance, const bctu_config_t * const config);


/*!
 * @brief Get the default configuration for BCTU.
 *
 * This function populates the configuration structure with the
 * default configuration for BCTU.
 *
 * @param[out] config - pointer to the output configuration structure
 * @return void
 */
void BCTU_DRV_GetDefaultConfig(bctu_config_t * const config);


/*!
 * @brief Reset the BCTU registers to default values.
 *
 * This function attempts to reset the BCTU registers to default values,
 * in the given timeout interval.
 * Trigger config registers cannot be modified while there is an ongoing conversion,
 * so the function waits for the conversion(s) to finish and returns an error code
 * if timeout occurres.
 * It globally disables all triggers, leaving the BCTU in configuration mode.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] timeout - timeout interval in milliseconds
 * @return status:
 * \n - STATUS_SUCCESS: reset succeeded
 * \n - STATUS_TIMEOUT: reset did not complete because an active conversion
 * did not finish execution within the provided timeout interval
 */
status_t BCTU_DRV_Reset(const uint32_t instance, const uint32_t timeout);


/*!
 * @brief Set the state of the Global Trigger Enable flag.
 *
 * This function sets the state of the Global Trigger Enable flag.
 * When triggers are disabled globally, the BCTU is in configuration mode, which allows configuring
 * the trigger config registers freely, without generating any spurious ADC conversion triggers.
 * Enabling the flag will move BCTU out of configuration mode and will enable all trigger inputs which are
 * already individually enabled from the trigger config registers.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] state - true: globally enable triggers \n
 *                    false: globally disable triggers - enter configuration mode
 * @return void
 */
void BCTU_DRV_WriteGlobalTriggerEn(const uint32_t instance, const bool state);


/*!
 * @brief Set the BCTU operating mode: normal or low power.
 *
 * This function sets the BCTU operating mode: normal or low power.
 * In low power mode all trigger inputs are ignored and any pending triggers remain active.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] state - true: enable low power mode \n
 *                    false: enable normal mode
 * @return void
 */
void BCTU_DRV_SetLowPowerMode(const uint32_t instance, const bool state);

/*! @}*/

/*!
 * @name Status flags
 */
/*! @{*/

/*!
 * @brief Get the value of a selected BCTU status flag, for a selected ADC instance.
 *
 * This function returns the value of a selected BCTU status flag, for a selected ADC instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] adcIdx - index of the ADC module
 * @param[in] flag - selects the status flag for which to return the value
 * @return state of the selected flag
 */
bool BCTU_DRV_GetStatusFlag(const uint32_t instance, const uint8_t adcIdx, const bctu_status_flag_t flag);


/*!
 * @brief Clear a selected BCTU status flag, for a selected ADC instance.
 *
 * This function clears a selected BCTU status flag, for a selected ADC instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] adcIdx - index of the ADC module
 * @param[in] flag - selects the status flag to be cleared
 * @return void
 */
void BCTU_DRV_ClearStatusFlag(const uint32_t instance, const uint8_t adcIdx, const bctu_status_flag_t flag);


/*!
 * @brief Clears all the BCTU status flags, for a selected ADC instance.
 * Also clears all common flags, i.e. TRGF.
 *
 * This function clears all the BCTU status flags, for a selected ADC instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] adcIdx - index of the ADC module
 * @return void
 */
void BCTU_DRV_ClearAllStatusFlags(const uint32_t instance, const uint8_t adcIdx);

/*! @}*/

/*!
 * @name Individual triggers and trigger list configuration
 */
/*! @{*/

/*!
 * @brief Resets the trigger register associated with a trigger index.
 *
 * This function attempts to reset the trigger register associated with a trigger index,
 * within the given timeout interval.
 * According to BCTU on-the-fly reconfig sequence, trigger config registers cannot be modified
 * while there is an ongoing conversion, so the function waits for the conversion to finish
 * and returns an error code if timeout occurs. \n
 * If succeeds, the function also drops all pending hw input triggers. \n
 *
 * Note: when executing a list, the conversion is considered active until the conversion
 * marked as "last" completes.
 *
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @param[in] timeout - timeout interval in milliseconds
 * @return status:
 * \n - STATUS_SUCCESS: reset succeeded
 * \n - STATUS_TIMEOUT: reset did not complete because an active conversion
 * did not finish execution within the provided timeout interval
 */
status_t BCTU_DRV_ResetTrigger(const uint32_t instance, const uint8_t trigIdx, const uint32_t timeout);


/*!
 * @brief Configures a trigger configuration register for an individual (non-list) conversion.
 *
 * This function attempts to configure a trigger configuration register for an individual (non-list) conversion.
 * If there is an on-going conversion started via target trigger, it only disables the trigger
 * (will ignore any new triggers) and returns error code. \n
 * If succeeds, implicitly clears any pending previous input trigger event. \n
 *
 * Note: when executing a list, the conversion is considered active until the conversion
 * marked as "last" completes.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @param[in] trigConfig - pointer to the configuration structure
 * @param[in] adcChanIdx - index of the ADC channel to be triggered
 * @return status:
 * \n - STATUS_SUCCESS: configured successfully
 * \n - STATUS_BUSY: configuration was not written because there is a triggered active conversion
 */
status_t BCTU_DRV_ConfigTriggerSingle(const uint32_t instance, const uint8_t trigIdx, const bctu_trig_config_t * const trigConfig, const uint8_t adcChanIdx);


/*!
 * @brief Check if there is an active conversion triggered by the selected trigger index.
 *
 * This function checks if there is an active conversion triggered by the selected trigger index.\n
 * Note: when executing a list, the conversion is considered active until the conversion
 * marked as "last" completes.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @return true - currently there is an active conversion triggered by the selected trigger \n
 *         false - currently there isn't an active conversion triggered by the selected trigger
 *
 */
bool BCTU_DRV_IsConvRunning(const uint32_t instance, const uint8_t trigIdx);


/*!
 * @brief Clears any eventual pending hw input signals already
 * occurred on the selected trigger and enables new occurrences of the hw input signals
 * to start new conversions.
 *
 * This function clears any eventual pending hw input signals already
 * occurred on the selected trigger and enables new occurrences of the hw input signals
 * to start new conversions.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @return void
 *
 */
void BCTU_DRV_EnableHwTrigger(const uint32_t instance, const uint8_t trigIdx);

/*!
 * @brief Prevent the selected BCTU trigger to start
 * new conversions on the occurrence of an external input signal or new conversions
 * in an on-going conversion loop (loop flag enabled).
 *
 * This function prevents the selected BCTU trigger to start
 * new conversions on the occurrence of an hw input signal or new conversions
 * in an on-going conversion loop (loop flag enabled).
 * After calling this function, new conversions may still be started by BCTU software trigger.
 * Note: the function does NOT stop an on-going conversion.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @return void
 *
 */
void BCTU_DRV_DisableHwTrigger(const uint32_t instance, const uint8_t trigIdx);

/*!
 * @brief Stop a series of loop conversions initiated by
 * the selected BCTU trigger.
 *
 * This function stops a series of loop conversions initiated by
 * the selected BCTU trigger.
 * Note0: the function does NOT stop an on-going conversion.
 * Note1: the function assumes that no HW trigger occurs on the selected trigger
 * during its execution - the reset & restore operation on the LOOP flag is not atomic!
 * Occurrence of a HW trigger may start a new conversion with an inconsistent value
 * for the LOOP flag.
 * For stopping HW triggered loops of conversions consider also using BCTU_DRV_DisableHwTrigger.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @return void
 *
 */
void BCTU_DRV_StopLoopConversions(const uint32_t instance, const uint8_t trigIdx);

/*!
 * @brief Configures a trigger configuration register for a list of conversions.
 *
 * This function attempts to configure a trigger configuration register for a list of conversions.
 * If there is an on-going conversion started via target trigger, it only disables the trigger
 * (will ignore any new triggers) and returns error code. \n
 * If succeeds, implicitly clears any pending previous input trigger event.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @param[in] trigConfig - pointer to the configuration structure
 * @param[in] listAddress - index of the starting element in the conversion list
 * @return status:
 * \n - STATUS_SUCCESS: configured successfully
 * \n - STATUS_BUSY: configuration was not written because there is a triggered active conversion
 */
status_t BCTU_DRV_ConfigTriggerList(const uint32_t instance, const uint8_t trigIdx, const bctu_trig_config_t * const trigConfig, const uint8_t listAddress);


/*!
 * @brief Set an array of adc channel numbers in the conversion list.
 *
 * This function sets an array of adc channel numbers in the conversion list.
 * Also marks the element corresponding to the last channel in the array, as last element in the list.
 * Implicitly wrap-around of the array onto the list, if (listSize - startElemIdx) < numArrayElems.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] startElemIdx - index in the list corresponding to the first element in the array
 * @param[in] adcChanNumbers - pointer to the first element in the adc channel numbers input array
 * @param[in] numArrayElems - number of elements in the adc channel numbers input array (required to be >0)
 * @return void
 */
void BCTU_DRV_SetConvListArray(const uint32_t instance, const uint8_t startElemIdx, const uint8_t * const adcChanNumbers, const uint8_t numArrayElems);

/*! @}*/

/*!
 * @name Software triggering and conversion results
 */
/*! @{*/

/*!
 * @brief Triggers a conversion or list of conversions associated with a trigger index,
 * if write protection is disabled.
 *
 * This function triggers a conversion or list of conversions associated with a trigger index, if write protection is disabled.
 * If write protection is enabled, the function will not trigger the conversion.\n
 * It is recommended that this function is used with HW triggers disabled for the target trigger index,
 * to avoid ambiguity regarding source of trigger (hw or sw) for conversion results.\n
 *
 * The BCTU HW will ignore attempts to trigger a new conversion or list, while there is a pending conversion or,
 * in case of an active conversion list, before the last conversion is started.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] trigIdx - index of the trigger register
 * @return void
 */
void BCTU_DRV_SwTriggerConv(const uint32_t instance, const uint8_t trigIdx);

/*!
 * @brief Disable write protection temporarily or permanently.
 *
 * This function temporarily/permanently disables write protection.
 * When write protection is disabled, triggers can be activated via software,
 * using BCTU_DRV_SwTriggerConv().
 * Temporarily disabling the write protection allows only a single write operation on designated registers,
 * thus a single use of BCTU_DRV_SwTriggerConv().
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] temp - select between temporary and permanent disabling of write protection
 * @return void
 */
void BCTU_DRV_DisableWriteProtect(const uint32_t instance, const bctu_wr_prot_duration_t temp);

/*!
 * @brief Return the current conversion result data corresponding to an adc instance.
 *
 * This function returns the current conversion result data corresponding to an adc instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] adcIdx - index of the target ADC module
 * @return uint16_t - conversion result
 */
uint16_t BCTU_DRV_GetConvData(const uint32_t instance, const uint8_t adcIdx);


/*!
 * @brief Return the full conversion result information corresponding to an adc instance.
 *
 * This function returns the full conversion result information corresponding to an adc instance.
 *
 * @param[in] instance - the BCTU instance number
 * @param[in] adcIdx - index of the target ADC module
 * @param[out] result - pointer to the structure to be populated with the conversion result info
 * @return void
 */
void BCTU_DRV_GetConvResult(const uint32_t instance, const uint8_t adcIdx, bctu_conv_result_t * const result);

/*! @}*/

#if defined (__cplusplus)
}
#endif

/*! @}*/


#endif /* BCTU_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
