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
/*!
 * @file pins_siul2_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, local macro not referenced
 * The SoC header defines macros for all modules and registers.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
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

#ifndef PINS_SIUL2_HW_ACCESS_H
#define PINS_SIUL2_HW_ACCESS_H

#include "pins_driver.h"
/*!
 * siul2_hal System Integration Unit Lite2 (SIUL2)
 * @brief This module covers the functionality of the SIUL2 peripheral.
 * <p>
 *  SIUL2 HAL provides the API for reading and writing register bit-fields belonging to the SIUL2 module.
 * </p>
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef SIUL2_IMCR_DAISY_MASK
#define SIUL2_INPUT_SOURCE_SELECT_MASK  SIUL2_IMCR_DAISY_MASK
#define SIUL2_INPUT_SOURCE_SELECT       SIUL2_IMCR_DAISY
#elif defined SIUL2_IMCR_SSS_MASK
#define SIUL2_INPUT_SOURCE_SELECT_MASK  SIUL2_IMCR_SSS_MASK
#define SIUL2_INPUT_SOURCE_SELECT       SIUL2_IMCR_SSS
#endif

#ifdef SIUL2_MSCR_MUX_MODE_MASK
#define SIUL2_OUTPUT_SOURCE_SELECT_MASK SIUL2_MSCR_MUX_MODE_MASK
#define SIUL2_OUTPUT_SOURCE_SELECT      SIUL2_MSCR_MUX_MODE
#elif defined SIUL2_MSCR_SSS_MASK
#define SIUL2_OUTPUT_SOURCE_SELECT_MASK SIUL2_MSCR_SSS_MASK
#define SIUL2_OUTPUT_SOURCE_SELECT      SIUL2_MSCR_SSS
#endif

#define SIUL2_INTERRUPT_COUNT           32u

extern uint8_t g_filterClkPrescaler;     /* Prescaled Filter Clock Period = T(IRC) x (IFCP + 1)
                                          * T(IRC) is the internal oscillator period.
                                          * IFCP can be 0 to 15                             */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Initializes the pins with the given configuration structure
 *
 * This function configures the pins with the options provided in the
 * given structure.
 *
 * @param[in] pinNumber the number of configured pins in structure
 * @param[in] config the configuration structure
 * @return the status of the operation
 */
status_t PINS_Init(const pin_settings_config_t * config);

#if FEATURE_PINS_HAS_PULL_SELECTION
/*!
 * @brief Configures the internal resistor.
 *
 * This function configures the internal resistor.
 *
 * @param[in] base Port base pointer
 * @param[in] pin Port pin number
 * @param[in] pullConfig The pull configuration
 */
static inline void PINS_SetPullSel(PORT_Type * const base,
                                   uint16_t pin,
                                   port_pull_config_t pullConfig)
{
    if (pullConfig == PORT_INTERNAL_PULL_UP_ENABLED)
    {
        base->MSCR[pin] |= SIUL2_MSCR_PUE(1);
        base->MSCR[pin] |= SIUL2_MSCR_PUS(1);
    }
    else if (pullConfig == PORT_INTERNAL_PULL_DOWN_ENABLED)
    {
        base->MSCR[pin] |= SIUL2_MSCR_PUE(1);
        base->MSCR[pin] &= ~SIUL2_MSCR_PUS(1);
    }
    else
    {
        base->MSCR[pin] &= ~SIUL2_MSCR_PUE(1);
    }
}

#endif /* FEATURE_PINS_HAS_PULL_SELECTION */

/*!
 * @brief Configures the output buffer.
 *
 * This function configures the output buffer.
 *
 * @param[in] base Port base pointer
 * @param[in] pin Port pin number
 * @param[in] enable Enable output buffer
 * @param[in] mux pin muxing slot selection
 */
static inline void PINS_SetOutputBuffer(PORT_Type * const base,
                                        uint16_t pin,
                                        bool enable,
                                        port_mux_t mux)
{
    base->MSCR[pin] &= ~SIUL2_MSCR_OBE_MASK;
    base->MSCR[pin] |= SIUL2_MSCR_OBE(enable ? 1UL : 0UL);
    base->MSCR[pin] &= ~SIUL2_OUTPUT_SOURCE_SELECT_MASK;
    base->MSCR[pin] |= SIUL2_OUTPUT_SOURCE_SELECT(mux);
}

/*!
 * @brief Configures the input buffer.
 *
 * This function configures the input buffer.
 *
 * @param[in] base Port base pointer
 * @param[in] pin Port pin number
 * @param[in] enable Enable input buffer
 * @param[in] inputMuxReg pin muxing register slot selection
 * @param[in] inputMux pin muxing slot selection
 */
static inline void PINS_SetInputBuffer(PORT_Type * const base,
                                       uint16_t pin,
                                       bool enable,
                                       uint32_t inputMuxReg,
                                       port_input_mux_t inputMux)
{
    base->MSCR[pin] &= ~SIUL2_MSCR_IBE_MASK;
    base->MSCR[pin] |= SIUL2_MSCR_IBE(enable ? 1UL : 0UL);

    if (inputMux != PORT_INPUT_MUX_NO_INIT)
    {
        SIUL2_IMCR_BASE[inputMuxReg] = SIUL2_INPUT_SOURCE_SELECT(inputMux);
    }
}

/*!
 * @brief Configures the interrupt filter.
 *
 * This function configures the interrupt filter clock prescaler.
 *
 * @param[in] prescaler The clock prescaler value
 */
static inline void PINS_ConfigIntFilterClock(uint8_t prescaler)
{
    SIUL2_Type * base = SIUL2;

    base->IFCPR = (uint32_t)((uint32_t)prescaler & SIUL2_IFCPR_IFCP_MASK);
}

/*!
 * @brief Configures the external interrupt.
 *
 * This function configures the external interrupt.
 *
 * @param[in] intConfig The configuration for interrupt pin
 */
static inline void PINS_SetExInt(siul2_interrupt_config_t intConfig)
{
    uint32_t pinIntValue;
    DEV_ASSERT(intConfig.eirqPinIdx < SIUL2_INTERRUPT_COUNT);
    SIUL2_Type * const base = SIUL2;

    /* Get the appropriate value follow index of pin interrupt */
    pinIntValue = (1UL << intConfig.eirqPinIdx);
    /* External interrupt initialization */
    if (intConfig.intEdgeSel != SIUL2_INT_DISABLE)
    {
        /* Set maximum interrupt filter counter value */
        base->IFMCR[intConfig.eirqPinIdx] = (uint32_t)((uint32_t)intConfig.maxCnt & SIUL2_IFMCR_MAXCNT_MASK);
        /* Setting the appropriate IFEn bits in IFER0 */
        if (intConfig.digitalFilter == true)
        {
            base->IFER0 |= pinIntValue;
        }
        else
        {
            base->IFER0 &= ~pinIntValue;
        }

        /* Setting the appropriate IREEn bits in IREER0 */
        if ((intConfig.intEdgeSel == SIUL2_INT_RISING_EDGE) || (intConfig.intEdgeSel == SIUL2_INT_EITHER_EDGE))
        {
            base->IREER0 |= pinIntValue;
        }
        else
        {
            base->IREER0 &= ~pinIntValue;
        }
        /* Setting the appropriate IREEn bits in IFEER0 */
        if ((intConfig.intEdgeSel == SIUL2_INT_FALLING_EDGE) || (intConfig.intEdgeSel == SIUL2_INT_EITHER_EDGE))
        {
            base->IFEER0 |= pinIntValue;
        }
        else
        {
            base->IFEER0 &= ~pinIntValue;
        }

    #if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
        /* Select the request desired between DMA or Interrupt */
        if (SIUL2_INT_USING_DMA == intConfig.intExeSel)
        {
            base->DIRSR0 |= pinIntValue;
        }
        else
        {
            base->DIRSR0 &= ~pinIntValue;
        }

        /* Write to EIFn bits in DISR0  to clear any flags */
        base->DISR0  |= pinIntValue;
        /* Enable the interrupt pins */
        base->DIRER0 |= pinIntValue;
    #else /* if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA */
        /* Select the request desired Interrupt */
        base->IRSR0 &= ~pinIntValue;
        /* Write to EIFn bits in ISR0  to clear any flags  */
        base->ISR0 = pinIntValue;
        /* Enable the interrupt pins */
        base->IRER0 |= pinIntValue;
    #endif /* if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA */
    }
    else
    {
    #if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
        /* Mask interrupts by clearing the EIREn bits in DIRER0 */
        base->DIRER0 &= ~pinIntValue;
    #else
        /* Mask interrupts by clearing the EIREn bits in IRER0 */
        base->IRER0 &= ~pinIntValue;
    #endif
    }
}

/*!
 * @brief Clears the individual pin external interrupt status flag.
 *
 * This function clears the individual pin external interrupt status flag.
 *
 * @param[in] eirqPinIdx IRQ pin
 */
static inline void PINS_ClearPinExIntFlag(uint32_t eirqPinIdx)
{
    DEV_ASSERT(eirqPinIdx < SIUL2_INTERRUPT_COUNT);
    SIUL2_Type * base = SIUL2;

#if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
    base->DISR0 = 1UL << eirqPinIdx;
#else
    base->ISR0 = 1UL << eirqPinIdx;
#endif
}

/*!
 * @brief Gets the individual pin external interrupt status flag.
 *
 * This function gets the individual pin external interrupt status flag.
 *
 * @param[in] eirqPinIdx IRQ pin
 * @return Pin external status flag
 */
static inline bool PINS_GetPinExIntFlag(uint32_t eirqPinIdx)
{
    DEV_ASSERT(eirqPinIdx < SIUL2_INTERRUPT_COUNT);
    const SIUL2_Type * base = SIUL2;

#if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
    return (base->DISR0 & (1UL << eirqPinIdx)) != 0U;
#else
    return (base->ISR0 & (1UL << eirqPinIdx)) != 0U;
#endif
}

/*!
 * @brief Clears the entire external interrupt status flag.
 *
 * This function clears the entire external interrupt status flag.
 *
 */
static inline void PINS_ClearExIntFlag(void)
{
    SIUL2_Type * base = SIUL2;

#if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
    base->DISR0 = 0xFFFFFFFFUL;
#else
    base->ISR0 = 0xFFFFFFFFUL;
#endif
}

/*!
 * @brief Reads the entire external interrupt status flag
 *
 * This function reads the entire external interrupt status flag.
 *
 * @return All 32 IRQ pins
 */
static inline uint32_t PINS_GetExIntFlag(void)
{
    const SIUL2_Type * base = SIUL2;

#if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
    return base->DISR0;
#else
    return base->ISR0;
#endif
}

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* PINS_SIUL2_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
