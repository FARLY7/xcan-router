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

#ifndef ADC_C55_DRIVER_H
#define ADC_C55_DRIVER_H

/*! @file */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macro defines a bitmask used to access status flags.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#include "status.h"

/*!
 * @addtogroup adc_c55_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @brief Macros for status flags
 *
 * These flags map to internal hardware flags in different registers, but are
 * grouped together for convenience.
 *
 */
#define ADC_FLAG_CALIBRATED        ((uint32_t)1u << 0u)
#define ADC_FLAG_NORMAL_STARTED    ((uint32_t)1u << 1u)
#define ADC_FLAG_NORMAL_EOC        ((uint32_t)1u << 2u)
#define ADC_FLAG_NORMAL_ENDCHAIN   ((uint32_t)1u << 3u)
#define ADC_FLAG_INJECTED_STARTED  ((uint32_t)1u << 4u)
#define ADC_FLAG_INJECTED_EOC      ((uint32_t)1u << 5u)
#define ADC_FLAG_INJECTED_ENDCHAIN ((uint32_t)1u << 6u)
#define ADC_FLAG_INJECTED_ABORTED  ((uint32_t)1u << 7u)
#define ADC_FLAG_CTU_STARTED       ((uint32_t)1u << 8u)
#define ADC_FLAG_CTU_EOC           ((uint32_t)1u << 9u)
#define ADC_FLAG_AUTOCLOCKOFF      ((uint32_t)1u << 10u)

#define ADC_FLAG_ALL               (ADC_FLAG_CALIBRATED | \
                                    ADC_FLAG_NORMAL_STARTED | \
                                    ADC_FLAG_NORMAL_EOC | \
                                    ADC_FLAG_NORMAL_ENDCHAIN | \
                                    ADC_FLAG_INJECTED_STARTED | \
                                    ADC_FLAG_INJECTED_EOC | \
                                    ADC_FLAG_INJECTED_ENDCHAIN | \
                                    ADC_FLAG_INJECTED_ABORTED | \
                                    ADC_FLAG_CTU_STARTED | \
                                    ADC_FLAG_CTU_EOC | \
                                    ADC_FLAG_AUTOCLOCKOFF \
                                    )

/*! @brief Conversion mode selection (One-shot or Scan) */
typedef enum
{
    ADC_CONV_MODE_ONESHOT = 0x00u,  /*!< One-shot conversion mode */
    ADC_CONV_MODE_SCAN = 0x01u      /*!< Scan conversion mode */
} adc_conv_mode_t;

/*! @brief Converter input clock */
typedef enum
{
    ADC_CLK_HALF_BUS = 0x00u,       /*!< Input clock is Bus clock / 2*/
    ADC_CLK_FULL_BUS = 0x01u        /*!< Input clock is Bus clock */
} adc_clk_sel_t;

/*! @brief Reference selection */
typedef enum
{
    ADC_REF_VREFH = 0x00u           /*!< Reference is VREFH */
} adc_ref_sel_t;

/*! @brief CTU Mode selection */
typedef enum
{
    ADC_CTU_MODE_DISABLED = 0x00u,  /*!< CTU Mode disabled */
    ADC_CTU_MODE_CONTROL = 0x02,    /*!< CTU is in Control Mode */
    ADC_CTU_MODE_TRIGGER = 0x03u    /*!< CTU is in Trigger Mode */
} adc_ctu_mode_t;

/*! @brief Injected Trigger selection */
typedef enum
{
    ADC_INJECTED_EDGE_DISABLED = 0x00u, /*!< Injected trigger disabled */
    ADC_INJECTED_EDGE_FALLING = 0x02u,  /*!< Injected trigger on Falling Edge */
    ADC_INJECTED_EDGE_RISING = 0x03u,   /*!< Injected trigger on Rising Edge */
} adc_injected_edge_t;

/*! @brief Conversion chain selection */
typedef enum
{
    ADC_CONV_CHAIN_NORMAL = 0x00u,      /*!< Selects the "Normal" Conversion Chain */
    ADC_CONV_CHAIN_INJECTED = 0x01u,    /*!< Selects the "Injected" Conversion Chain */
    ADC_CONV_CHAIN_CTU = 0x02u          /*!< Selects the "CTU" Conversion Chain */
} adc_conv_chain_t;

/*! @brief Data alignment selection */
typedef enum
{
    ADC_DATA_ALIGNED_RIGHT = 0x00u,     /*!< Measured data is right-aligned */
    ADC_DATA_ALIGNED_LEFT = 0x01u       /*!< Measured data is left-aligned */
} adc_data_aligned_t;

/*! @brief Data alignment selection */
typedef enum {
    ADC_DMA_REQ_CLEAR_ON_ACK = 0u,      /*!< Clear DMA Request on Ack from DMA Controller */
    ADC_DMA_REQ_CLEAR_ON_READ = 1u,     /*!< Clear DMA Request on read of Data Registers */
} adc_dma_clear_source_t;

/*! @brief Presampling Voltage selection */
typedef enum {
    ADC_PRESAMPLE_SRC0 = 0x00u,         /*!< Presampling from */
    ADC_PRESAMPLE_SRC1 = 0x01u,         /*!< Presampling from */
    ADC_PRESAMPLE_SRC2 = 0x02u,         /*!< Presampling from */
    ADC_PRESAMPLE_SRC3 = 0x03u          /*!< Presampling from */
} adc_presampling_source_t;

/*! @brief Channel Group selection */
typedef enum {
    ADC_CHAN_GROUP_PRECISION = 0u,          /*!< Precision Channels Group (0-31) */
    ADC_CHAN_GROUP_INTERNAL = 1u,           /*!< Precision Channels Group (32-63) */
    ADC_CHAN_GROUP_EXTERNAL = 2u,           /*!< Precision Channels Group (64-95) */
} adc_chan_group_t;

/*!
 * @brief Defines the converter configuration
 *
 * This structure is used to configure the ADC converter
 *
 * Implements : adc_conv_config_t_Class
 */
typedef struct
{
    adc_conv_mode_t convMode;   /*!< Conversion Mode (One-shot or Scan) */
    adc_clk_sel_t clkSelect;    /*!< Clock input */
    adc_ref_sel_t refSelect;    /*!< Reference selection  */
    adc_ctu_mode_t ctuMode;     /*!< CTU mode */
    adc_injected_edge_t injectedEdge;   /*!< Injected Trigger selection */
    uint8_t sampleTimeInternal;  /*!< Sample time for Internal channels (32-63) */
    uint8_t sampleTimePrecision; /*!< Sample time for Precision channels (0-31) */
    uint8_t sampleTimeExternal;  /*!< Sample time for External channels (64-95) */
    bool autoClockOff; /*!< Enable Auto Clock Off */
    bool overwriteEnable; /*!< Overwrite new conversion data over old data */
    adc_data_aligned_t dataAlign; /*!< Data alignment in conversion result register */
    uint8_t decodeDelay; /*!< Delay for decoding Input MUX channels */
    uint8_t powerDownDelay; /*!< Delay before entering Power Down */
} adc_conv_config_t;

/*!
 * @brief Defines the chain configuration
 *
 * This structure is used to configure the ADC Normal and Injected Chains
 *
 * Implements : adc_chain_config_t_Class
 */
typedef struct
{
    uint8_t numChansNormal;          /*!< Number of channels configured for Normal Chain */
    uint8_t * chanArrayNormal;       /*!< Array of channel indexes to enable for Normal Chain */
    uint8_t numChansInjected;        /*!< Number of channels configured for Injected Chain */
    uint8_t * chanArrayInjected;     /*!< Array of channel indexes to enable for Injected Chain */
    uint32_t interruptMaskInternal;  /*!< Bit-mask for internal channels interrupts */
    uint32_t interruptMaskPrecision; /*!< Bit-mask for precision channels interrupts */
    uint32_t interruptMaskExternal;  /*!< Bit-mask for external channels interrupts */
} adc_chain_config_t;


/*!
 * @brief Defines the data regarding a conversion, beyond the conversion data.
 *
 * This structure is used to return information about conversions beyond just conversion data
 *
 * Implements : adc_chan_result_t_Class
 */
typedef struct {
    uint8_t chnIdx;   /*!< ADC Channel Index */
    bool valid;       /*!< Data Valid Flag */
    bool overWritten; /*!< Data Overwritten Flag */
    uint16_t cdata;   /*!< Conversion Data */
} adc_chan_result_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined (__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the converter configuration structure
 *
 * This function initializes the struct members to default (reset) values.
 *
 * @param[out] config configuration struct pointer
 */
void ADC_DRV_GetDefaultConfigConverter(adc_conv_config_t * const config);

/*!
 * @brief Configures the converter with the given configuration structure
 *
 * This function configures the ADC converter with the options
 * provided in the structure.
 *
 * @param[in] instance the instance number
 * @param[in] config configuration struct pointer
 */
void ADC_DRV_ConfigConverter(const uint32_t instance,
                             const adc_conv_config_t * const config);

/*!
 * @brief Configures the converter chains the given configuration structure
 *
 * This function configures the ADC Normal and Injected Chains with the options
 * provided in the structure.
 *
 * @param[in] instance the instance number
 * @param[in] config configuration struct pointer
 */
void ADC_DRV_ChainConfig(const uint32_t instance,
                         const adc_chain_config_t * const config);

/*!
 * @brief Reset the ADC
 *
 * This function resets the ADC internal registers to default values.
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_Reset(const uint32_t instance);

/*!
 * @brief Enable a channel
 *
 * This function enables a channel in a specified conversion chain
 *
 * @param[in] instance the instance number
 * @param[in] convChain conversion chain (Normal or Injected)
 * @param[in] chnIdx the index of the channel to enable
 */
void ADC_DRV_EnableChannel(const uint32_t instance,
                           const adc_conv_chain_t convChain,
                           const uint32_t chnIdx);

/*!
 * @brief Disable a channel
 *
 * This function disables a channel in a specified conversion chain
 *
 * @param[in] instance the instance number
 * @param[in] convChain conversion chain (Normal or Injected)
 * @param[in] chnIdx the index of the channel to enable
 */
void ADC_DRV_DisableChannel(const uint32_t instance,
                            const adc_conv_chain_t convChain,
                            const uint32_t chnIdx);

/*!
 * @brief Start conversion
 *
 * This function starts a conversion channel (Normal or Injected)
 *
 * @param[in] instance the instance number
 * @param[in] convChain conversion chain (Normal or Injected)
 */
void ADC_DRV_StartConversion(const uint32_t instance,
                             const adc_conv_chain_t convChain);

/*!
 * @brief Get the status flags
 *
 * This function returns the status flags of the ADC.
 *
 * @param[in] instance the instance number
 * @return the status flag bit-mask
 */
uint32_t ADC_DRV_GetStatusFlags(const uint32_t instance);

/*!
 * @brief Clear the status flags
 *
 * This function clears the status flags of the ADC.
 *
 * @param[in] instance the instance number
 * @param[in] mask bit-mask of flags to clear
 */
void ADC_DRV_ClearStatusFlags(const uint32_t instance,
                              const uint32_t mask);

/*!
 * @brief Get conversion results for a conversion chain
 *
 * This function gets the conversion results for the selected Conversion Chain.
 *
 * @param[in] instance the instance number
 * @param[in] convChain conversion chain (Normal, Injected or CTU)
 * @param[out] results the output buffer
 * @param[in] length the length of the buffer
 * @return the number of values written in the buffer (max length)
 */
uint32_t ADC_DRV_GetConvResultsToArray(const uint32_t instance,
                                       const adc_conv_chain_t convChain,
                                       uint16_t * const results,
                                       const uint32_t length);

/*!
 * @brief Perform Calibration of the ADC
 *
 * This function performs a calibration of the ADC. The maximum input clock
 * frequency for the ADC is 80 MHz, checked with assertions if DEV_ASSERT is
 * enabled. After calibration, the ADC is left in Powerup state (PWDN bit is clear).
 *
 * @param[in] instance the instance number
 * @return the calibration result
 *  - STATUS_SUCCESS: calibration successful
 *  - STATUS_ERROR: calibration failed
 */
status_t ADC_DRV_DoCalibration(const uint32_t instance);

/*!
 * @brief Power up the ADC
 *
 * This function enables the ADC (disables the Power Down feature).
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_Powerup(const uint32_t instance);

/*!
 * @brief Power down the ADC
 *
 * This function disables the ADC (enables the Power Down feature).
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_Powerdown(const uint32_t instance);

/*!
 * @brief Enable ADC interrupts
 *
 * This function enables ADC interrupts.
 *
 * @param[in] instance the instance number
 * @param[in] interruptMask mask of interrupts to enable (of status flags)
 */
void ADC_DRV_EnableInterrupts(const uint32_t instance,
                              const uint32_t interruptMask);

/*!
 * @brief Disable ADC interrupts
 *
 * This function disables ADC interrupts.
 *
 * @param[in] instance the instance number
 * @param[in] interruptMask mask of interrupts to disable (of status flags)
 */
void ADC_DRV_DisableInterrupts(const uint32_t instance,
                               const uint32_t interruptMask);


/*!
 * @brief Enable ADC interrupt for a channel
 *
 * This function enables interrupt generation on End of Conversion event for a single channel.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_EnableChannelInterrupt(const uint32_t instance,
                                    const uint32_t chnIdx);

/*!
 * @brief Disable ADC interrupt for a channel
 *
 * This function disables interrupt generation on End of Conversion event for a single channel.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_DisableChannelInterrupt(const uint32_t instance,
                                     const uint32_t chnIdx);


/*!
 * @brief Set the Presampling Source for the channel group
 *
 * This function configures the Presampling Source for a channel group.
 *
 * @param[in] instance the instance number
 * @param[in] chanGroup the channel group
 * @param[in] presampleSource the presampling source
 */
void ADC_DRV_SetPresamplingSource(const uint32_t instance,
                                  const adc_chan_group_t chanGroup,
                                  const adc_presampling_source_t presampleSource);

/*!
 * @brief Enable Presampling on one channel
 *
 * This function enables the Presampling on one channel of the ADC.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_EnableChannelPresampling(const uint32_t instance,
                                      const uint32_t chnIdx);

/*!
 * @brief Disable Presampling on one channel
 *
 * This function disables the Presampling on one channel of the ADC.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_DisableChannelPresampling(const uint32_t instance,
                                       const uint32_t chnIdx);

/*!
 * @brief Enable Conversion Presampled Data
 *
 * This function enables bypass of the Sampling Phase, resulting in a conversion
 * of the presampled data. This is available only for channels that have presampling
 * enabled.
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_EnablePresampleConversion(const uint32_t instance);

/*!
 * @brief Disable Conversion of Presampled Data
 *
 * This function disables Sampling Phase bypass.
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_DisablePresampleConversion(const uint32_t instance);

/*!
 * @brief Enable DMA Requests
 *
 * This function enables requests to DMA from ADC
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_EnableDma(const uint32_t instance);

/*!
 * @brief Disable DMA Requests
 *
 * This function disables requests to DMA from ADC
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_DisableDma(const uint32_t instance);

/*!
 * @brief Enable DMA on one channel
 *
 * This function enables DMA requests triggered by End of Conversion event from
 * a selected channel.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_EnableChannelDma(const uint32_t instance,
                              const uint32_t chnIdx);

/*!
 * @brief Disable DMA on one channel
 *
 * This function disables DMA requests triggered by End of Conversion event from
 * a selected channel.
 *
 * @param[in] instance the instance number
 * @param[in] chnIdx the index of the channel
 */
void ADC_DRV_DisableChannelDma(const uint32_t instance,
                               const uint32_t chnIdx);

/*!
 * @brief Set DMA Request Clear Source
 *
 * This function selects the DMA Request Flag Clear Source.
 *
 * @param[in] instance the instance number
 * @param[in] dmaClear the clear source for DMA Requests (Ack from DMA Controller or
    read of data registers)
 */
void ADC_DRV_SetDmaClearSource(const uint32_t instance,
                               const adc_dma_clear_source_t dmaClear);

/*!
 * @brief Get conversion results for a conversion chain with extended information
 *
 * This function gets the conversion results for the selected Conversion Chain, with
 * extended information about each conversion result (channel index, valid an overwritten
 * properties and conversion data). This function should be used in case of configurations
 * with overlapping channel lists in different chains, resulting in overwrite of conversion
 * data when a higher priority chain is executed before all data was read.
 *
 * @param[in] instance the instance number
 * @param[in] convChain conversion chain (Normal, Injected or CTU)
 * @param[out] results the output buffer
 * @param[in] length the length of the buffer
 * @return the number of values written in the buffer (max length)
 */
uint32_t ADC_DRV_GetConvInfoToArray(const uint32_t instance,
                                    const adc_conv_chain_t convChain,
                                    adc_chan_result_t* const results,
                                    const uint32_t length);

/*!
 * @brief Abort ongoing conversion
 *
 * This function aborts an ongoing conversion.
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_AbortConversion(const uint32_t instance);

/*!
 * @brief Abort ongoing chain conversion
 *
 * This function aborts an ongoing chain of conversions.
 *
 * @param[in] instance the instance number
 */
void ADC_DRV_AbortChain(const uint32_t instance);

#if defined (__cplusplus)
}
#endif

/*! @} */

#endif /* ADC_C55_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
