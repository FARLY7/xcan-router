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
/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially Boolean'
 * to 'essentially unsigned'. This is required by the conversion of a bool into a bit.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3,  Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macro defines a bitmask used to access status flags.
 *
 */

#include <stddef.h>
#include "adc_c55_driver.h"
#include "device_registers.h"
#include "adc_c55_hw_access.h"

#if defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)
/* Clock Manager is a dependency only when DEV_ASSERT is enabled, used in:
  - ADC_DRV_DoCalibration
  - ADC_DRV_ConfigConverter
*/
#include "clock_manager.h"

/* local definitions on the limits of the clock frequency */
#define ADC_CLOCK_FREQ_MAX_RUNTIME     (80000000u)
#define ADC_CLOCK_FREQ_MAX_CALIBRATION (40000000u)

#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for ADC instances. */
static ADC_Type * const s_adcBase[ADC_INSTANCE_COUNT] = ADC_BASE_PTRS;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_GetDefaultConfigConverter
* Description   : This function initializes the members of the adc_conv_config_t
*  structure to default values (Reference Manual resets).
*
* Implements    : ADC_DRV_GetDefaultConfigConverter_Activity
* END**************************************************************************/
void ADC_DRV_GetDefaultConfigConverter(adc_conv_config_t * const config)
{
    DEV_ASSERT(config != NULL);
    config->convMode = ADC_CONV_MODE_SCAN;
    config->clkSelect = ADC_CLK_HALF_BUS;
    config->refSelect = ADC_REF_VREFH;
    config->ctuMode = ADC_CTU_MODE_DISABLED;
    config->injectedEdge = ADC_INJECTED_EDGE_DISABLED;
    config->sampleTimeInternal = 0u;
    config->sampleTimePrecision = 0u;
    config->sampleTimeExternal = 0u;
    config->autoClockOff = false;
    config->overwriteEnable = false;
    config->dataAlign = ADC_DATA_ALIGNED_RIGHT;
    config->decodeDelay = 0u;
    config->powerDownDelay = 0u;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_ConfigConverter
* Description   : This function configures the ADC converter with the options
*  provided in the configuration structure.
*
* Implements    : ADC_DRV_ConfigConverter_Activity
* END**************************************************************************/
void ADC_DRV_ConfigConverter(const uint32_t instance,
                             const adc_conv_config_t * const config)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(config != NULL);
#if (FEATURE_ADC_HAS_CTU_TRIGGER_MODE == 0)
    /* test that the feature is enabled only on platforms that have it */
    DEV_ASSERT(config->ctuMode != ADC_CTU_MODE_TRIGGER);
#endif

#if defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)
    clock_names_t adc_clocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    uint32_t adc_freq = 0u;
    status_t clk_status = CLOCK_SYS_GetFreq(adc_clocks[instance], &adc_freq);
    DEV_ASSERT(clk_status == STATUS_SUCCESS);
    if (config->clkSelect == ADC_CLK_HALF_BUS) {
        adc_freq /= (uint32_t) 2u; /* half bus speed during normal operation */
    }
    DEV_ASSERT(adc_freq > 0u);
    DEV_ASSERT(adc_freq <= ADC_CLOCK_FREQ_MAX_RUNTIME);
#endif

    ADC_Type * const base = s_adcBase[instance];

    uint32_t mcr = 0u;
    mcr |= ADC_MCR_MODE(config->convMode);
    mcr |= ADC_MCR_ADCLKSEL(config->clkSelect);
    mcr |= ADC_MCR_REFSEL(config->refSelect);
    mcr |= ADC_MCR_ACKO((uint32_t)config->autoClockOff);
    mcr |= ADC_MCR_OWREN((uint32_t)config->overwriteEnable);
    mcr |= ADC_MCR_WLSIDE(config->dataAlign);

    switch(config->ctuMode)
    {
        case ADC_CTU_MODE_CONTROL:
            /* Already in CTU Control Mode CTU_MODE = 0 */
            mcr |= ADC_MCR_CTUEN(1u); /* Enable CTU */
            break;
#if FEATURE_ADC_HAS_CTU_TRIGGER_MODE
        case ADC_CTU_MODE_TRIGGER:
            mcr |= ADC_MCR_CTU_MODE(1u); /* Set CTU to Trigger Mode CTU_MODE = 1 */
            mcr |= ADC_MCR_CTUEN(1u); /* Enable CTU */
            break;
#endif
        case ADC_CTU_MODE_DISABLED:
            /* CTU is disabled (CTUEN = 0 and CTU_MODE = 0) */
            /* Pass through */
        default:
            /* no-op */
            break;
    }

    switch(config->injectedEdge)
    {
        case ADC_INJECTED_EDGE_FALLING:
            /* Already on falling edge JEDGE = 0 */
            mcr |= ADC_MCR_JTRGEN(1u); /* enable Injected trigger */
            break;
        case ADC_INJECTED_EDGE_RISING:
            mcr |= ADC_MCR_JEDGE(1u); /* set to rising edge JEDGE = 1 */
            mcr |= ADC_MCR_JTRGEN(1u); /* enable Injected trigger */
            break;
        case ADC_INJECTED_EDGE_DISABLED:
            /* Injected trigger disabled (JTRGEN = 0 and JEDGE = 0) */
            /* Pass through */
        default:
            /* no-op */
            break;

    }

    ADC_Powerdown(base);
    base->MCR = mcr;
    ADC_Powerup(base);

    base->CTR[ADC_GROUP_PRECISION] = ADC_CTR_INPSAMP(config->sampleTimePrecision);
    base->CTR[ADC_GROUP_INTERNAL] = ADC_CTR_INPSAMP(config->sampleTimeInternal);
#if (ADC_CTR_COUNT > 2u)
    base->CTR[ADC_GROUP_EXTERNAL] = ADC_CTR_INPSAMP(config->sampleTimeExternal);
#endif

    base->PDEDR = ADC_PDEDR_PDED(config->powerDownDelay);
#ifdef ADC_DSDR_DSD
    base->DSDR = ADC_DSDR_DSD(config->decodeDelay);
#endif
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_ChainConfig
* Description   : This function configures the ADC chain feature with the options
*  provided in the configuration structure.
*
* Implements    : ADC_DRV_ChainConfig_Activity
* END**************************************************************************/
void ADC_DRV_ChainConfig(const uint32_t instance,
                         const adc_chain_config_t * const config)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(config != NULL);

    ADC_Type * const base = s_adcBase[instance];
    (void)config;

    uint8_t i;
    if (config->numChansNormal > 0u)
    {
        DEV_ASSERT(config->chanArrayNormal != NULL);

        /* reset Normal Conversion registers */
        uint8_t k;
        for (k = 0u; k < ADC_NCMR_COUNT; k++)
        {
            base->NCMR[k] = 0x00u;
        }

        for (i = 0u; i < config->numChansNormal; i++)
        {
            DEV_ASSERT(config->chanArrayNormal[i] < ADC_CDR_COUNT);
            uint32_t vectAdr = CHAN_2_VECT((uint32_t)config->chanArrayNormal[i]);
            uint32_t vectBit = CHAN_2_BIT((uint32_t)config->chanArrayNormal[i]);
            REG_BIT_SET32(&(base->NCMR[vectAdr]), (uint32_t)((uint32_t)1u << vectBit));
        }
    }

    if (config->numChansInjected > 0u)
    {
        DEV_ASSERT(config->chanArrayInjected != NULL);

        /* reset Injected Conversion registers */
        uint8_t k;
        for (k = 0u; k < ADC_JCMR_COUNT; k++)
        {
            base->JCMR[k] = 0x00u;
        }

        for (i = 0u; i < config->numChansInjected; i++)
        {
            DEV_ASSERT(config->chanArrayInjected[i] < ADC_CDR_COUNT);
            uint32_t vectAdr = CHAN_2_VECT((uint32_t)config->chanArrayInjected[i]);
            uint32_t vectBit = CHAN_2_BIT((uint32_t)config->chanArrayInjected[i]);
            REG_BIT_SET32(&(base->JCMR[vectAdr]), (uint32_t)((uint32_t)1u << vectBit));
        }
    }

    base->CIMR[ADC_GROUP_PRECISION] = config->interruptMaskPrecision;
#if (ADC_CIMR_COUNT > 1u)
    base->CIMR[ADC_GROUP_INTERNAL] = config->interruptMaskInternal;
    base->CIMR[ADC_GROUP_EXTERNAL] = config->interruptMaskExternal;
#endif
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_Reset
* Description   : This function writes all the internal ADC registers with
*  their Reference Manual reset values.
*
* Implements    : ADC_DRV_Reset_Activity
* END**************************************************************************/
void ADC_DRV_Reset(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    adc_conv_config_t default_config;
    ADC_DRV_GetDefaultConfigConverter(&default_config);
    ADC_DRV_ConfigConverter(instance, &default_config);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableChannel
* Description   : This function enables a channel in a conversion chain (
*  ADC_CONV_CHAIN_NORMAL or ADC_CONV_CHAIN_INJECTED).
*
* Implements    : ADC_DRV_EnableChannel_Activity
* END**************************************************************************/
void ADC_DRV_EnableChannel(const uint32_t instance,
                           const adc_conv_chain_t convChain,
                           const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    switch (convChain)
    {
        case ADC_CONV_CHAIN_NORMAL:
            REG_BIT_SET32(&(base->NCMR[vectAdr]), ((uint32_t)1u << vectBit));
            break;
        case ADC_CONV_CHAIN_INJECTED:
            REG_BIT_SET32(&(base->JCMR[vectAdr]), ((uint32_t)1u << vectBit));
            break;
        default:
            DEV_ASSERT(false);
            break;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableChannel
* Description   : This function disables a channel from a conversion chain (ADC_CONV_CHAIN_NORMAL
*  or ADC_CONV_CHAIN_INJECTED).
*
* Implements    : ADC_DRV_DisableChannel_Activity
* END**************************************************************************/
void ADC_DRV_DisableChannel(const uint32_t instance,
                            const adc_conv_chain_t convChain,
                            const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    switch (convChain)
    {
        case ADC_CONV_CHAIN_NORMAL:
            REG_BIT_CLEAR32(&(base->NCMR[vectAdr]), ((uint32_t)1u << vectBit));
            break;
        case ADC_CONV_CHAIN_INJECTED:
            REG_BIT_CLEAR32(&(base->JCMR[vectAdr]), ((uint32_t)1u << vectBit));
            break;
        default:
            DEV_ASSERT(false);
            break;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_StartConversion
* Description   : This function starts a conversion chain (ADC_CONV_CHAIN_NORMAL
*  or ADC_CONV_CHAIN_INJECTED).
*
* Implements    : ADC_DRV_StartConversion_Activity
* END**************************************************************************/
void ADC_DRV_StartConversion(const uint32_t instance,
                             const adc_conv_chain_t convChain)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    switch (convChain)
    {
        case ADC_CONV_CHAIN_NORMAL:
            REG_BIT_SET32(&(base->MCR), ADC_MCR_NSTART(1u));
            break;
        case ADC_CONV_CHAIN_INJECTED:
            REG_BIT_SET32(&(base->MCR), ADC_MCR_JSTART(1u));
            break;
        default:
            DEV_ASSERT(false);
            break;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_GetStatusFlags
* Description   : This function returns the status flags of the ADC.
*
* Implements    : ADC_DRV_GetStatusFlags_Activity
* END**************************************************************************/
uint32_t ADC_DRV_GetStatusFlags(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    const ADC_Type * const base = s_adcBase[instance];
    uint32_t msr = base->MSR;
    uint32_t isr = base->ISR;
    uint32_t flags = 0u;

    flags |= ((msr & ADC_MSR_CALIBRTD_MASK) != 0u) ? ADC_FLAG_CALIBRATED : (uint32_t)0u;
    flags |= ((msr & ADC_MSR_NSTART_MASK) != 0u) ? ADC_FLAG_NORMAL_STARTED : (uint32_t)0u;
    flags |= ((msr & ADC_MSR_JABORT_MASK) != 0u) ? ADC_FLAG_INJECTED_ABORTED : (uint32_t)0u;
    flags |= ((msr & ADC_MSR_JSTART_MASK) != 0u) ? ADC_FLAG_INJECTED_STARTED : (uint32_t)0u;
    flags |= ((msr & ADC_MSR_CTUSTART_MASK) != 0u) ? ADC_FLAG_CTU_STARTED : (uint32_t)0u;
    flags |= ((msr & ADC_MSR_ACKO_MASK) != 0u) ? ADC_FLAG_AUTOCLOCKOFF : (uint32_t)0u;
    flags |= ((isr & ADC_ISR_EOC_MASK) != 0u) ? ADC_FLAG_NORMAL_EOC : (uint32_t)0u;
    flags |= ((isr & ADC_ISR_ECH_MASK) != 0u) ? ADC_FLAG_NORMAL_ENDCHAIN : (uint32_t)0u;
    flags |= ((isr & ADC_ISR_JEOC_MASK) != 0u) ? ADC_FLAG_INJECTED_EOC : (uint32_t)0u;
    flags |= ((isr & ADC_ISR_JECH_MASK) != 0u) ? ADC_FLAG_INJECTED_ENDCHAIN : (uint32_t)0u;
    flags |= ((isr & ADC_ISR_EOCTU_MASK) != 0u) ? ADC_FLAG_CTU_EOC : (uint32_t)0u;

    return flags;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_ClearStatusFlags
* Description   : This function clears the status flags that are set to '1' in
*  the mask.
*
* Implements    : ADC_DRV_ClearStatusFlags_Activity
* END**************************************************************************/
void ADC_DRV_ClearStatusFlags(const uint32_t instance,
                              const uint32_t mask)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    uint32_t isr_flags = 0u;
    isr_flags |= ((mask & ADC_FLAG_NORMAL_EOC) != 0u) ? ADC_ISR_EOC(1u) : (uint32_t)0u;
    isr_flags |= ((mask & ADC_FLAG_NORMAL_ENDCHAIN) != 0u) ? ADC_ISR_ECH(1u) : (uint32_t)0u;
    isr_flags |= ((mask & ADC_FLAG_INJECTED_EOC) != 0u) ? ADC_ISR_JEOC(1u) : (uint32_t)0u;
    isr_flags |= ((mask & ADC_FLAG_INJECTED_ENDCHAIN) != 0u) ? ADC_ISR_JECH(1u) : (uint32_t)0u;
    isr_flags |= ((mask & ADC_FLAG_CTU_EOC) != 0u) ? ADC_ISR_EOCTU(1u) : (uint32_t)0u;

    /* Write-1-to-clear bits in ISR register */
    base->ISR = isr_flags;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_GetConvResultsToArray
* Description   : This function reads the conversion result values for a conversion chain
*  (ADC_CONV_CHAIN_NORMAL, ADC_CONV_CHAIN_INJECTED or ADC_CONV_CHAIN_CTU).
*
* Implements    : ADC_DRV_GetConvResultsToArray_Activity
* END**************************************************************************/
uint32_t ADC_DRV_GetConvResultsToArray(const uint32_t instance,
                                       const adc_conv_chain_t convChain,
                                       uint16_t * const results,
                                       const uint32_t length)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    DEV_ASSERT(results != NULL);

    /* go through each element of the EOC in CEOCFR registers */
    /* if EOC is '1', copy result to array, increment index */
    /* assert on index and length */
    /* return index */

    uint32_t index = 0;
    uint32_t chnIdx;
    for (chnIdx = 0u; chnIdx < ADC_CDR_COUNT; chnIdx++)
    {
        uint32_t vectAdr = CHAN_2_VECT(chnIdx);
        uint32_t vectBit = CHAN_2_BIT(chnIdx);
        if ((base->CEOCFR[vectAdr] & ((uint32_t)1u << vectBit)) != (uint32_t)0u)
        {
            /* the conversion is complete, check if the chain is correct */
            uint32_t cdr = base->CDR[chnIdx];
            if (ADC_CDR_RESULT((uint32_t)convChain) == (base->CDR[chnIdx] & ADC_CDR_RESULT_MASK))
            {
                /* chain is correct, store the result in results and clear the flag */
                results[index] = (uint16_t)(cdr & ADC_CDR_CDATA_MASK) >> ADC_CDR_CDATA_SHIFT;
                base->CEOCFR[vectAdr] = ((uint32_t)1u) << vectBit; /* w1c bit */
                index++;
                if (index > length)
                {
                    /* the current length value of the index exceeds the buffer length.
                     * Exit the loop to not corrupt other memory
                     */
                    DEV_ASSERT(false);
                    break;
                }
            }
        }
    }

    return index;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DoCalibration
* Description   : This functions executes a calibration sequence. It is recommended
*  to run this sequence before using the ADC converter. The maximum clock frequency
*  for the calibration is 40 MHz, this function sets the ADCLKSEL bit resulting in
*  a maximum input clock frequency of 80 MHz.
*
* Implements    : ADC_DRV_DoCalibration_Activity
* END**************************************************************************/
status_t ADC_DRV_DoCalibration(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
#if defined(DEV_ERROR_DETECT) || defined(CUSTOM_DEVASSERT)
    clock_names_t adc_clocks[ADC_INSTANCE_COUNT] = ADC_CLOCKS;
    uint32_t adc_freq = 0u;
    status_t clk_status = CLOCK_SYS_GetFreq(adc_clocks[instance], &adc_freq);
    DEV_ASSERT(clk_status == STATUS_SUCCESS);
    adc_freq /= (uint32_t) 2u; /* half bus speed will be used during calibration */
    DEV_ASSERT(adc_freq > 0u);
    DEV_ASSERT(adc_freq <= ADC_CLOCK_FREQ_MAX_CALIBRATION);
#endif

    status_t ret = STATUS_SUCCESS;
    ADC_Type * const base = s_adcBase[instance];

    ADC_Powerdown(base);
    /* Reset CLKSEL to 0x00 (will set to half bus speed) */
    uint32_t mcr = base->MCR; /* save the current state of MCR to restore ADCLKSEL */
    REG_BIT_CLEAR32(&(base->MCR), ADC_MCR_ADCLKSEL_MASK);
    ADC_Powerup(base);
    uint32_t calbistreg = base->CALBISTREG;
    /* clear the bits and set to calibration values */
    calbistreg &= ~(ADC_CALBISTREG_TSAMP_MASK | ADC_CALBISTREG_NR_SMPL_MASK | ADC_CALBISTREG_AVG_EN_MASK | ADC_CALBISTREG_TEST_EN_MASK);
    calbistreg |= ADC_CALBISTREG_NR_SMPL(0x03u) | ADC_CALBISTREG_AVG_EN(1u);
    base->CALBISTREG = calbistreg;
    /* clear the calibration failed before a new calibration */
    REG_BIT_SET32(&(base->CALBISTREG), ADC_CALBISTREG_TEST_FAIL_MASK);
    /* start calibration */
    REG_BIT_SET32(&(base->CALBISTREG), ADC_CALBISTREG_TEST_EN(1u));
    while ((base->CALBISTREG & ADC_CALBISTREG_C_T_BUSY_MASK) != 0u)
    {
        /* Wait for calibration to finish */
    }

    /* If the calibration failed, return error */
    if ((base->CALBISTREG & ADC_CALBISTREG_TEST_FAIL_MASK) != 0u)
    {
        ret = STATUS_ERROR;
    }

    /* restore the state of ADCLKSEL */
    REG_BIT_SET32(&(base->MCR), (uint32_t) (mcr & ADC_MCR_ADCLKSEL_MASK));

    return ret;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_Powerup
* Description   : This function enables the ADC module (by clearing the Powerdown
*  bit).
*
* Implements    : ADC_DRV_Powerup_Activity
* END**************************************************************************/
void ADC_DRV_Powerup(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    ADC_Powerup(base);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_Powerdown
* Description   : This function disables the ADC module (by setting the Powerdown
*  bit).
*
* Implements    : ADC_DRV_Powerdown_Activity
* END**************************************************************************/
void ADC_DRV_Powerdown(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    ADC_Powerdown(base);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableInterrupts
* Description   : This function enables the ADC interrupts set to '1' in the
*  mask parameter.
*
* Implements    : ADC_DRV_EnableInterrupts_Activity
* END**************************************************************************/
void ADC_DRV_EnableInterrupts(const uint32_t instance,
                              const uint32_t interruptMask)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t imr_flags = 0u;
    imr_flags |= ((interruptMask & ADC_FLAG_NORMAL_EOC) != 0u) ? ADC_IMR_MSKEOC(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_NORMAL_ENDCHAIN) != 0u) ? ADC_IMR_MSKECH(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_INJECTED_EOC) != 0u) ? ADC_IMR_MSKJEOC(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_INJECTED_ENDCHAIN) != 0u) ? ADC_IMR_MSKJECH(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_CTU_EOC) != 0u) ? ADC_IMR_MSKEOCTU(1u) : (uint32_t)0u;
    REG_BIT_SET32(&(base->IMR), imr_flags);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableInterrupts
* Description   : This function disables the ADC interrupts set to '1' in the
*  mask parameter.
*
* Implements    : ADC_DRV_DisableInterrupts_Activity
* END**************************************************************************/
void ADC_DRV_DisableInterrupts(const uint32_t instance,
                               const uint32_t interruptMask)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t imr_flags = 0u;
    imr_flags |= ((interruptMask & ADC_FLAG_NORMAL_EOC) != 0u) ? ADC_IMR_MSKEOC(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_NORMAL_ENDCHAIN) != 0u) ? ADC_IMR_MSKECH(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_INJECTED_EOC) != 0u) ? ADC_IMR_MSKJEOC(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_INJECTED_ENDCHAIN) != 0u) ? ADC_IMR_MSKJECH(1u) : (uint32_t)0u;
    imr_flags |= ((interruptMask & ADC_FLAG_CTU_EOC) != 0u) ? ADC_IMR_MSKEOCTU(1u) : (uint32_t)0u;
    REG_BIT_CLEAR32(&(base->IMR), imr_flags);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableChannelInterrupt
* Description   : This function enables End-of-Conversion interrupt generation for
*  a single channel.
*
* Implements    : ADC_DRV_EnableChannelInterrupt_Activity
* END**************************************************************************/
void ADC_DRV_EnableChannelInterrupt(const uint32_t instance,
                                    const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->CIMR[vectAdr] |= (uint32_t)1u << vectBit;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableChannelInterrupt
* Description   : This function disables End-of-Conversion interrupt generation for
*  a single channel.
*
* Implements    : ADC_DRV_DisableChannelInterrupt_Activity
* END**************************************************************************/
void ADC_DRV_DisableChannelInterrupt(const uint32_t instance,
                                     const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->CIMR[vectAdr] &= ~((uint32_t)1u << vectBit);
}


/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_SetPresamplingSource
* Description   : This function configures the Presampling Source for a channel group (Internal,
*  External or Precision)
*
* Implements    : ADC_DRV_SetPresamplingSource_Activity
* END**************************************************************************/
void ADC_DRV_SetPresamplingSource(const uint32_t instance,
                                  const adc_chan_group_t chanGroup,
                                  const adc_presampling_source_t presampleSource)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t pscr = base->PSCR;
    switch (chanGroup){
#ifdef ADC_PSCR_PREVAL0
        case ADC_CHAN_GROUP_PRECISION:
            pscr &= ~(ADC_PSCR_PREVAL0_MASK);
            pscr |= ADC_PSCR_PREVAL0((uint32_t)presampleSource);
            break;
#endif
#ifdef ADC_PSCR_PREVAL1
        case ADC_CHAN_GROUP_INTERNAL:
            pscr &= ~(ADC_PSCR_PREVAL1_MASK);
            pscr |= ADC_PSCR_PREVAL1((uint32_t)presampleSource);
            break;
#endif
#ifdef ADC_PSCR_PREVAL2
        case ADC_CHAN_GROUP_EXTERNAL:
            pscr &= ~(ADC_PSCR_PREVAL2_MASK);
            pscr |= ADC_PSCR_PREVAL2((uint32_t)presampleSource);
            break;
#endif
        default:
            /* Not supported */
            DEV_ASSERT(false);
            break;
    }
    base->PSCR = pscr;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableChannelPresampling
* Description   : This function enables the Presampling on one channel of the ADC.
*
* Implements    : ADC_DRV_EnableChannelPresampling_Activity
* END**************************************************************************/
void ADC_DRV_EnableChannelPresampling(const uint32_t instance,
                                      const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->PSR[vectAdr] |= (uint32_t)1u << vectBit;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableChannelPresampling
* Description   : This function disables the Presampling on one channel of the ADC.
*
* Implements    : ADC_DRV_DisableChannelPresampling_Activity
* END**************************************************************************/
void ADC_DRV_DisableChannelPresampling(const uint32_t instance,
                                       const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->PSR[vectAdr] &= ~((uint32_t)1u << vectBit);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnablePresampleConversion
* Description   : This function enables bypass of the Sampling Phase, resulting in a conversion
* of the presampled data. This is available only for channels that have presampling
* enabled.
*
* Implements    : ADC_DRV_EnablePresampleConversion_Activity
* END**************************************************************************/
void ADC_DRV_EnablePresampleConversion(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    REG_BIT_SET32(&(base->PSCR), ADC_PSCR_PRECONV(1u));
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisablePresampleConversion
* Description   : This function disables Sampling Phase bypass.
*
* Implements    : ADC_DRV_DisablePresampleConversion_Activity
* END**************************************************************************/
void ADC_DRV_DisablePresampleConversion(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    REG_BIT_CLEAR32(&(base->PSCR), ADC_PSCR_PRECONV(1u));
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableDma
* Description   : This function enables requests to DMA from ADC
*
* Implements    : ADC_DRV_EnableDma_Activity
* END**************************************************************************/
void ADC_DRV_EnableDma(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    REG_BIT_SET32(&(base->DMAE), ADC_DMAE_DMAEN(1u));
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableDma
* Description   : This function disables requests to DMA from ADC
*
* Implements    : ADC_DRV_DisableDma_Activity
* END**************************************************************************/
void ADC_DRV_DisableDma(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    REG_BIT_CLEAR32(&(base->DMAE), ADC_DMAE_DMAEN(1u));
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_EnableChannelDma
* Description   : This function enables DMA requests triggered by End of Conversion event from
*  a selected channel.
*
* Implements    : ADC_DRV_EnableChannelDma_Activity
* END**************************************************************************/
void ADC_DRV_EnableChannelDma(const uint32_t instance,
                              const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->DMAR[vectAdr] |= (uint32_t)1u << vectBit;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_DisableChannelDma
* Description   : This function disables DMA requests triggered by End of Conversion event from
*  a selected channel.
*
* Implements    : ADC_DRV_DisableChannelDma_Activity
* END**************************************************************************/
void ADC_DRV_DisableChannelDma(const uint32_t instance,
                               const uint32_t chnIdx)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    DEV_ASSERT(chnIdx < ADC_CDR_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t vectAdr = CHAN_2_VECT(chnIdx);
    uint32_t vectBit = CHAN_2_BIT(chnIdx);

    base->DMAR[vectAdr] &= ~((uint32_t)1u << vectBit);
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_SetDmaClearSource
* Description   : This function selects the DMA Request Flag Clear Source.
*
* Implements    : ADC_DRV_SetDmaClearSource_Activity
* END**************************************************************************/
void ADC_DRV_SetDmaClearSource(const uint32_t instance,
                               const adc_dma_clear_source_t dmaClear)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];

    uint32_t dmae = base->DMAE;
    dmae &= ~(ADC_DMAE_DCLR_MASK);
    dmae |= ADC_DMAE_DCLR((uint32_t)dmaClear);
    base->DMAE = dmae;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_GetConvInfoToArray
* Description   : This function gets the conversion results for the selected
*  Conversion Chain. It follows the same algorithm as ADC_DRV_GetConvResultsToArray,
*  but will copy some extra information to the output.
*
* Implements    : ADC_DRV_GetConvInfoToArray_Activity
* END**************************************************************************/
uint32_t ADC_DRV_GetConvInfoToArray(const uint32_t instance,
                                    const adc_conv_chain_t convChain,
                                    adc_chan_result_t* const results,
                                    const uint32_t length)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);

    ADC_Type * const base = s_adcBase[instance];
    DEV_ASSERT(results != NULL);

    /* go through each element of the EOC in CEOCFR registers */
    /* if EOC is '1', copy result to array, increment index */
    /* assert on index and length */
    /* return index */

    uint32_t index = 0;
    uint8_t chnIdx;
    for (chnIdx = 0u; chnIdx < ADC_CDR_COUNT; chnIdx++)
    {
        uint32_t vectAdr = CHAN_2_VECT((uint32_t)chnIdx);
        uint32_t vectBit = CHAN_2_BIT((uint32_t)chnIdx);
        if ((base->CEOCFR[vectAdr] & ((uint32_t)1u << vectBit)) != (uint32_t)0u)
        {
            /* the conversion is complete, check if the chain is correct */
            uint32_t cdr = base->CDR[chnIdx];
            if (ADC_CDR_RESULT((uint32_t)convChain) == (base->CDR[chnIdx] & ADC_CDR_RESULT_MASK))
            {
                /* chain is correct, store the result in results and clear the flag */
                results[index].cdata = (uint16_t)(cdr & ADC_CDR_CDATA_MASK) >> ADC_CDR_CDATA_SHIFT;
                results[index].chnIdx = chnIdx;
                results[index].valid = ((cdr & ADC_CDR_VALID_MASK) != 0u) ? true : false;
                results[index].overWritten = ((cdr & ADC_CDR_OVERW_MASK) != 0u) ? true : false;
                base->CEOCFR[vectAdr] = ((uint32_t)1u) << vectBit; /* w1c bit */
                index++;
                if (index > length)
                {
                    /* the current length value of the index exceeds the buffer length.
                     * Exit the loop to not corrupt other memory
                     */
                    DEV_ASSERT(false);
                    break;
                }
            }
        }
    }

    return index;
}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_AbortConversion
* Description   : This function aborts an ongoing conversion.
*
* Implements    : ADC_DRV_AbortConversion_Activity
* END**************************************************************************/
void ADC_DRV_AbortConversion(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    ADC_Type * const base = s_adcBase[instance];
    base->MCR |= ADC_MCR_ABORT(1u);

}

/*FUNCTION**********************************************************************
*
* Function Name : ADC_DRV_AbortChain
* Description   : This function aborts an ongoing chain of conversions.
*
* Implements    : ADC_DRV_AbortChain_Activity
* END**************************************************************************/
void ADC_DRV_AbortChain(const uint32_t instance)
{
    DEV_ASSERT(instance < ADC_INSTANCE_COUNT);
    ADC_Type * const base = s_adcBase[instance];

    base->MCR |= ADC_MCR_ABORT_CHAIN(1u);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
