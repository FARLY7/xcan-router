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
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. Also, the called functions
 * do not store the address into variables with lifetime longer then its own call.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 */

#include <stddef.h>
#include <stdbool.h>

#include "bctu_driver.h"
#include "device_registers.h"
#include "status.h"
#include "osif.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BCTU_HW_REG_SIZE            (32u)

#define BCTU_LIST_NUM_ELEMS         (BCTU_LISTCHR__COUNT * 2u) /* Each LISTCHR reg contains 2 list elements */

#define BCTU_PROTEC_CODE_PERMANENT  (10u) /* 0b1010u */
#define BCTU_PROTEC_CODE_TEMP       (9u)  /* 0b1001u */

#define BCTU_ADC_INSTANCE_BITMASK   (BCTU_GetBitmask(FEATURE_BCTU_NUM_ADC) - 1u)


/*******************************************************************************
 * Local function prototypes
 ******************************************************************************/
static inline uint32_t BCTU_GetBitmask(const uint8_t pos);
static status_t BCTU_PrepareReconfigBlocking(BCTU_Type * const bctu, const uint8_t trigIdx, const uint32_t timeout);
static status_t BCTU_PrepareReconfigNonBlocking(BCTU_Type * const bctu, const uint8_t trigIdx);


/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Table of base addresses for BCTU instances. */
static BCTU_Type * const s_bctuBase[BCTU_INSTANCE_COUNT] = BCTU_BASE_PTRS;


/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_Config
* Description   : This function configures the global functionalities of
* a BCTU instance.
*
* Implements    : BCTU_DRV_Config_Activity
* END**************************************************************************/
void BCTU_DRV_Config(const uint32_t instance, const bctu_config_t * const config)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(config != NULL);
    DEV_ASSERT(config->dmaEnMask <= BCTU_ADC_INSTANCE_BITMASK);
    DEV_ASSERT(config->newDataIntEnMask <= BCTU_ADC_INSTANCE_BITMASK);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t mcr = 0u;

    mcr |= (config->lowPowerModeEn) ? BCTU_MCR_MDIS(1u) : 0u;
    mcr |= (config->globalHwTriggersEn) ? 0u : BCTU_MCR_FRZ(1u);
    mcr |= (config->globalTriggersEn) ? BCTU_MCR_GTRGEN(1u) : 0u;

    mcr |= ((config->dmaEnMask & BCTU_GetBitmask(0u)) != 0u) ? BCTU_MCR_DMA0(1u) : 0u;
    mcr |= ((config->dmaEnMask & BCTU_GetBitmask(1u)) != 0u) ? BCTU_MCR_DMA1(1u) : 0u;

    mcr |= (config->triggerIntEn) ? BCTU_MCR_TRGEN(1u) : 0u;
    mcr |= (config->listIntEn) ? BCTU_MCR_LIST_IEN(1u) : 0u;

    mcr |= ((config->newDataIntEnMask & BCTU_GetBitmask(0u)) != 0u) ? BCTU_MCR_IEN0(1u) : 0u;
    mcr |= ((config->newDataIntEnMask & BCTU_GetBitmask(1u)) != 0u) ? BCTU_MCR_IEN1(1u) : 0u;

    bctu->MCR = mcr;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_GetDefaultConfig
* Description   : This function populates the configuration structure with the
* default configuration for BCTU.
*
* Implements    : BCTU_DRV_GetDefaultConfig_Activity
* END**************************************************************************/
void BCTU_DRV_GetDefaultConfig(bctu_config_t * const config)
{
    DEV_ASSERT(config != NULL);

    config->lowPowerModeEn      = false;
    config->globalHwTriggersEn  = true;
    config->globalTriggersEn    = false;
    config->dmaEnMask           = 0u;
    config->triggerIntEn        = false;
    config->listIntEn           = false;
    config->newDataIntEnMask    = 0u;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_Reset
* Description   : This function resets the BCTU registers to default values.
* It globally disables all triggers, leaving the BCTU in configuration mode.
*
* Implements    : BCTU_DRV_Reset_Activity
* END**************************************************************************/
status_t BCTU_DRV_Reset(const uint32_t instance, const uint32_t timeout)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];
    bctu_config_t defaultConfig;
    uint8_t trigIdx, adcIdx;
    status_t status = STATUS_SUCCESS;

    /* First step: globally disable the triggers. */
    bctu->MCR &= (~ BCTU_MCR_GTRGEN_MASK);

    /* Reset global config register. */
    BCTU_DRV_GetDefaultConfig(&defaultConfig);
    BCTU_DRV_Config(instance, &defaultConfig);

    /* First, attempt to reset all TRGCFG registers in provided timeout interval.
     * If any current active conversion completes in time,
     * status and result registers may get updated - they must be reset later. */
    for(trigIdx = 0u; trigIdx < BCTU_TRGCFG__COUNT; trigIdx++)
    {
        if(BCTU_DRV_ResetTrigger(instance, trigIdx, timeout) != STATUS_SUCCESS)
        {
            status = STATUS_TIMEOUT;
            break;
        }
    }

    if(status == STATUS_SUCCESS)
    {
        /* Reset status register */
        for(adcIdx = 0u; adcIdx < FEATURE_BCTU_NUM_ADC; adcIdx++)
        {
            BCTU_DRV_ClearAllStatusFlags(instance, adcIdx);
        }

        /* Write protection and result registers cannot be reset */
    }


    return status;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_WriteGlobalTriggerEn
* Description   : This function sets the state of the Global Trigger Enable flag.
*
* Implements    : BCTU_DRV_WriteGlobalTriggerEn_Activity
* END**************************************************************************/
void BCTU_DRV_WriteGlobalTriggerEn(const uint32_t instance, const bool state)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];

    if(state == true)
    {
        bctu->MCR |= BCTU_MCR_GTRGEN_MASK;
    }
    else
    {
        bctu->MCR &= (~ BCTU_MCR_GTRGEN_MASK);
    }
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_SetLowPowerMode
* Description   : This function sets the BCTU operating mode: normal or low power.
* In low power mode all trigger inputs are ignored and any pending triggers remain active.
*
* Implements    : BCTU_DRV_SetLowPowerMode_Activity
* END**************************************************************************/
void BCTU_DRV_SetLowPowerMode(const uint32_t instance, const bool state)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];

    if(state == true)
    {
        /* Enable Low Power Mode */
        bctu->MCR |= BCTU_MCR_MDIS_MASK;
    }
    else
    {
        /* Disable Low Power Mode */
        bctu->MCR &= (~ BCTU_MCR_MDIS_MASK);
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_GetStatusFlag
* Description   : This function returns the value of a selected BCTU status flag,
* for a selected ADC instance.
*
* Implements    : BCTU_DRV_GetStatusFlag_Activity
* END**************************************************************************/
bool BCTU_DRV_GetStatusFlag(const uint32_t instance, const uint8_t adcIdx, const bctu_status_flag_t flag)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(adcIdx < FEATURE_BCTU_NUM_ADC);

    const BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t val = 0u;
    bool ret;

    switch(flag)
    {
    case BCTU_FLAG_ANY_ADC_TRIGGERED:
        val = (bctu->MSR & BCTU_MSR_TRGF_MASK);
        break;
    case BCTU_FLAG_LIST_LAST_CONV:
        val = bctu->MSR & ((uint32_t)BCTU_MSR_LIST0_Last_MASK << adcIdx);
        break;
    case BCTU_FLAG_DATA_OVERRUN:
        val = bctu->MSR & ((uint32_t)BCTU_MSR_DATAOVR0_MASK << adcIdx);
        break;
    case BCTU_FLAG_NEW_DATA_AVAILABLE:
        val = bctu->MSR & ((uint32_t)BCTU_MSR_NDATA0_MASK << adcIdx);
        break;
    default:
        /* no-op */
        break;
    }

    ret = (val == 0u) ? false : true;

    return ret;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_ClearStatusFlag
* Description   : This function clears a selected BCTU status flag,
* for a selected ADC instance.
*
* Implements    : BCTU_DRV_ClearStatusFlag_Activity
* END**************************************************************************/
void BCTU_DRV_ClearStatusFlag(const uint32_t instance, const uint8_t adcIdx, const bctu_status_flag_t flag)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(adcIdx < FEATURE_BCTU_NUM_ADC);

    BCTU_Type * const bctu = s_bctuBase[instance];

    switch(flag)
    {
    case BCTU_FLAG_ANY_ADC_TRIGGERED:
        bctu->MSR |= BCTU_MSR_TRGF_CLR_MASK;
        break;

    case BCTU_FLAG_LIST_LAST_CONV:
        bctu->MSR |= (BCTU_MSR_LIST0_Last_CLR_MASK << adcIdx);
        break;

    case BCTU_FLAG_DATA_OVERRUN:
        bctu->MSR |= (BCTU_MSR_DATAOVR0_CLR_MASK << adcIdx);
        break;

    case BCTU_FLAG_NEW_DATA_AVAILABLE:
        bctu->MSR |= (BCTU_MSR_NDATA0_CLR_MASK  << adcIdx);
        break;

    default:
        /* no-op */
        break;
    }

    /* Dummy read to guarantee write is complete, when memory write buffering is enabled. */
    (void) bctu->MSR;

}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_ClearAllStatusFlags
* Description   : Clears all the BCTU status flags, for a selected ADC instance.
* Also clears all common flags, i.e. TRGF.
*
* Implements    : BCTU_DRV_ClearAllStatusFlags_Activity
* END**************************************************************************/
void BCTU_DRV_ClearAllStatusFlags(const uint32_t instance, const uint8_t adcIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(adcIdx < FEATURE_BCTU_NUM_ADC);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t mask = 0u;

    mask |= BCTU_MSR_TRGF_CLR_MASK;
    mask |= (BCTU_MSR_LIST0_Last_CLR_MASK | BCTU_MSR_NDATA0_CLR_MASK | BCTU_MSR_DATAOVR0_CLR_MASK) << adcIdx;

    bctu->MSR = mask;

    /* Dummy read to guarantee write is complete, when memory write buffering is enabled. */
    (void) bctu->MSR;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_ResetTrigger
* Description   : This function resets the trigger register associated with a trigger index.
* The function also drops all pending hw input triggers.
* If there is an on-going conversion already started via current trigger, it will continue to execute.
* In accordance with BCTU on-the-fly reconfiguration sequence, the function will wait until any active
* conversion finishes execution.
*
* Implements    : BCTU_DRV_ResetTrigger_Activity
* END**************************************************************************/
status_t BCTU_DRV_ResetTrigger(const uint32_t instance, const uint8_t trigIdx, const uint32_t timeout)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];
    status_t status;

    if(BCTU_PrepareReconfigBlocking(bctu, trigIdx, timeout) == STATUS_SUCCESS)
    {
        /* After this point the TRGCFG register can be modified freely. */

        /* Reserved bits are written with '0'.
         * Any bits w1c are cleared when preparing on the fly reconfiguration. */
        bctu->TRGCFG_[trigIdx] = 0u;

        status = STATUS_SUCCESS;
    }
    else
    {
        status = STATUS_TIMEOUT;
    }

    return status;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_ConfigTriggerSingle
* Description   : Attempts to configure a trigger configuration register for
* an individual (non-list) conversion. If there is an on-going conversion started
* via target trigger, it only disables the trigger (will ignore any new triggers)
* and returns error code.
*
* Implements    : BCTU_DRV_ConfigTriggerSingle_Activity
* END**************************************************************************/
status_t BCTU_DRV_ConfigTriggerSingle(const uint32_t instance, const uint8_t trigIdx, const bctu_trig_config_t * const trigConfig, const uint8_t adcChanIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);
    DEV_ASSERT(trigConfig != NULL);
    DEV_ASSERT(adcChanIdx < ADC_CDR_COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t trgcfg;
    status_t status;

    status = BCTU_PrepareReconfigNonBlocking(bctu, trigIdx);

    if(status == STATUS_SUCCESS)
    {
        /* After this point the TRGCFG register can be modified freely. */

        /* TRS field is implicitly set to '0' - because configuration is for single conversion.
           Reserved bits are written with '0'. */
        trgcfg = 0u;
        if(trigConfig->loopEn == true)
        {
            trgcfg |= BCTU_TRGCFG__LOOP_MASK;
        }
        if(trigConfig->hwTriggersEn == true)
        {
            trgcfg |= BCTU_TRGCFG__TRIGEN_MASK;
        }

        switch(trigConfig->adcTargetMask)
        {
        case 1u:
            trgcfg |= BCTU_TRGCFG__ADC_SEL0_MASK;
            break;
        case 2u:
            trgcfg |= BCTU_TRGCFG__ADC_SEL1_MASK;
            break;
        default:
            DEV_ASSERT(false); /* A single (non-list) conversion can be triggered only on a single ADC. */
            break;
        }

        trgcfg |= ((uint32_t)adcChanIdx & BCTU_TRGCFG__CHANNEL_VALUE_OR_LADDR_MASK);

        bctu->TRGCFG_[trigIdx] = trgcfg;
    }

    return status;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_IsConvRunning
* Description   : This function checks if there is an active conversion triggered
* by the selected trigger index.
*
* Implements    : BCTU_DRV_IsConvRunning_Activity
* END**************************************************************************/
bool BCTU_DRV_IsConvRunning(const uint32_t instance, const uint8_t trigIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    const BCTU_Type * const bctu = s_bctuBase[instance];
    bool state;

    state = ((bctu->TRGCFG_[trigIdx] & BCTU_TRGCFG__TRG_FLAG_MASK) != 0u) ? true : false;

    return state;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_EnableHwTrigger
* Description   : This function clears any eventual pending hw input signals already
* occurred on the selected trigger and enables new occurrences of the hw input signals
* to start new conversions.
*
* Implements    : BCTU_DRV_EnableHwTrigger_Activity
* END**************************************************************************/
void BCTU_DRV_EnableHwTrigger(const uint32_t instance, const uint8_t trigIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];

    /* Clear any pending hw input signals already occurred on the selected trigger. */
    bctu->TRGCFG_[trigIdx] |= BCTU_TRGCFG__TRG_FLAG_MASK;
    /* Enable new occurrences of the hw input signals to start new conversions. */
    bctu->TRGCFG_[trigIdx] |= BCTU_TRGCFG__TRIGEN_MASK;
}

/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_DisableHwTrigger
* Description   : This function prevents the selected BCTU trigger to start
* new conversions on the occurrence of a hw input signal and also new conversions
* in an on-going conversion loop (loop flag enabled).
* After calling this function, new conversions may still be started by BCTU software trigger.
* Note: the function does NOT stop an on-going conversion.
*
* Implements    : BCTU_DRV_DisableHwTrigger_Activity
* END**************************************************************************/
void BCTU_DRV_DisableHwTrigger(const uint32_t instance, const uint8_t trigIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];

    /* Prevent any new conversions from starting when external input signal occurs.
     * This also stops new conversions from starting when loop flag is enabled. */
    bctu->TRGCFG_[trigIdx] &= (~BCTU_TRGCFG__TRIGEN_MASK);
}

/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_StopLoopConversions
* Description   : This function stops a series of loop conversions initiated by
* the selected BCTU trigger.
* Note0: the function does NOT stop an on-going conversion.
* Note1: the function assumes that no HW trigger occurs on the selected trigger
* during its execution - the reset & restore operation on the LOOP flag is not atomic!
* Occurrence of a HW trigger may start a new conversion with an inconsistent value
* for the LOOP flag.
* For stopping HW triggered loops of conversions consider also using BCTU_DRV_DisableHwTrigger.
*
* Implements    : BCTU_DRV_StopLoopConversions_Activity
* END**************************************************************************/
void BCTU_DRV_StopLoopConversions(const uint32_t instance, const uint8_t trigIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t loopBitmask = 0u;

    loopBitmask = bctu->TRGCFG_[trigIdx] & BCTU_TRGCFG__LOOP_MASK;

    /* Reset loop flag to stop a series of loop conversions */
    bctu->TRGCFG_[trigIdx] &= (~BCTU_TRGCFG__LOOP_MASK);

    /* Restore loop flag */
    bctu->TRGCFG_[trigIdx] |= (loopBitmask & BCTU_TRGCFG__LOOP_MASK);
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_ConfigTriggerList
* Description   : Attempts to configure a trigger configuration register for a
* list of conversions. If there is an on-going conversion started via target trigger,
* it only disables the trigger (will ignore any new triggers) and returns error code.
*
* Implements    : BCTU_DRV_ConfigTriggerList_Activity
* END**************************************************************************/
status_t BCTU_DRV_ConfigTriggerList(const uint32_t instance, const uint8_t trigIdx, const bctu_trig_config_t * const trigConfig, const uint8_t listAddress)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);
    DEV_ASSERT(trigConfig != NULL);
    DEV_ASSERT(listAddress < BCTU_LIST_NUM_ELEMS);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t trgcfg;
    status_t status;

    status = BCTU_PrepareReconfigNonBlocking(bctu, trigIdx);

    if(status == STATUS_SUCCESS)
    {
        /* After this point the TRGCFG register can be modified freely. */

        /* TRS field is implicitly set to '1' - because configuration is for list conversion.
           Reserved bits are written with '0'. */
        trgcfg = BCTU_TRGCFG__TRS_MASK;
        if(trigConfig->loopEn == true)
        {
            trgcfg |= BCTU_TRGCFG__LOOP_MASK;
        }
        if(trigConfig->hwTriggersEn == true)
        {
            trgcfg |= BCTU_TRGCFG__TRIGEN_MASK;
        }

        /* More than one ADC instance can be selected for a LIST. */
        DEV_ASSERT((trigConfig->adcTargetMask <= BCTU_ADC_INSTANCE_BITMASK) && \
                   (trigConfig->adcTargetMask != 0u));

        trgcfg |= ((trigConfig->adcTargetMask & BCTU_GetBitmask(0u)) != 0u) ? (uint32_t)BCTU_TRGCFG__ADC_SEL0_MASK : (uint32_t)0u;
        trgcfg |= ((trigConfig->adcTargetMask & BCTU_GetBitmask(1u)) != 0u) ? (uint32_t)BCTU_TRGCFG__ADC_SEL1_MASK : (uint32_t)0u;

        trgcfg |= (listAddress & (BCTU_LIST_NUM_ELEMS - 1u));

        bctu->TRGCFG_[trigIdx] = trgcfg;
    }

    return status;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_SetConvListArray
* Description   : This function sets an array of adc channel numbers in the conversion list.
* Also marks the element corresponding to the last channel in the array, as last element in the list.
* Implicitly wrap-around of the array onto the list, if (listSize - startElemIdx) < numArrayElems.
*
* Implements    : BCTU_DRV_SetConvListArray_Activity
* END**************************************************************************/
void BCTU_DRV_SetConvListArray(const uint32_t instance, const uint8_t startElemIdx, const uint8_t * const adcChanNumbers, const uint8_t numArrayElems)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(startElemIdx < BCTU_LIST_NUM_ELEMS);
    DEV_ASSERT(adcChanNumbers != NULL);
    DEV_ASSERT(numArrayElems > 0u);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint8_t i, regIdx = 0u, startRegIdx;
    uint8_t numElemsPreInserted, chanIdx;

    startRegIdx = startElemIdx / 2u;
    if((startElemIdx % 2u) == 0u)
    {
        /* Position of first element is even. No special processing required. */
        numElemsPreInserted = 0u;
    }
    else
    {
        /* Position of first element is odd.
         * First element of the input array needs to be inserted separately. */
        /* Clear the bits in LIST reg, corresponding to odd position. */
        bctu->LISTCHR_[startRegIdx] &= ~(BCTU_LISTCHR__ADC_CHL_y_plus_1_MASK | BCTU_LISTCHR__LAST_y_plus_1_MASK);
        /* Write the adc channel in the LIST reg, corresponding to odd position. */
        bctu->LISTCHR_[startRegIdx] |= BCTU_LISTCHR__ADC_CHL_y_plus_1(adcChanNumbers[0u]);
        /* Move to next LIST reg. */
        startRegIdx++;
        numElemsPreInserted = 1u;
    }
    for(i = 0u; i < ((numArrayElems - numElemsPreInserted) / 2u); i++)
    {
    	chanIdx = (2u * i) + numElemsPreInserted;
        regIdx = (startRegIdx + i) % BCTU_LISTCHR__COUNT;
        bctu->LISTCHR_[regIdx] = BCTU_LISTCHR__ADC_CH_y(adcChanNumbers[chanIdx]) | BCTU_LISTCHR__ADC_CHL_y_plus_1(adcChanNumbers[chanIdx + 1u]);
    }

    if(((numArrayElems - numElemsPreInserted) % 2u) == 1u) /* If last element in the array corresponds to an even position. */
    {
        regIdx = (regIdx + 1u) % BCTU_LISTCHR__COUNT;

        /* Clear LIST reg bits, corresponding to even position. */
        bctu->LISTCHR_[regIdx] &= ~(BCTU_LISTCHR__ADC_CH_y_MASK | BCTU_LISTCHR__LAST_y_MASK);

        /* Write the adc channel in the LIST reg, corresponding to odd position. */
        bctu->LISTCHR_[regIdx] |= BCTU_LISTCHR__ADC_CH_y(adcChanNumbers[numArrayElems - 1u]);

        /* Mark the element as last. */
        bctu->LISTCHR_[regIdx] |= BCTU_LISTCHR__LAST_y_MASK;
    }
    else /* If last element in the array corresponds to an odd position. */
    {
        /* Last element has already been written to LIST reg. */
        /* Just mark the element as last. */
        bctu->LISTCHR_[regIdx] |= BCTU_LISTCHR__LAST_y_plus_1_MASK;
    }
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_SwTriggerConv
* Description   : This function triggers a conversion or list of conversions associated
* with a trigger index, if write protection is disabled.
* If write protection is enabled, the function will not trigger the conversion.
* It is recommended that this function is used with HW triggers disabled for the target trigger index,
* to avoid ambiguity regarding source of trigger (hw or sw) for conversion results.
*
* The BCTU HW will ignore attempts to trigger a new conversion or list,
* while there is a pending conversion or, in case of an active conversion list,
* before the last conversion is started.
*
* Implements    : BCTU_DRV_SwTriggerConv_Activity
* END**************************************************************************/
void BCTU_DRV_SwTriggerConv(const uint32_t instance, const uint8_t trigIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(trigIdx < BCTU_TRGCFG__COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t bitmask;

    /* Write protection must be temporarily or permanently disabled. */
#if defined(CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT)
    {
        uint32_t regVal = (bctu->WRPROT & BCTU_WRPROT_PROTEC_CODE_MASK) >> BCTU_WRPROT_PROTEC_CODE_SHIFT;
        DEV_ASSERT((regVal == BCTU_PROTEC_CODE_PERMANENT) || (regVal == BCTU_PROTEC_CODE_TEMP));
    }
#endif /* defined(CUSTOM_DEVASSERT) || defined (DEV_ERROR_DETECT) */

    /* Set the bit corresponding to the trigIdx position in the 32 bit register. */
    bitmask = BCTU_GetBitmask(trigIdx % BCTU_HW_REG_SIZE);
    switch(trigIdx / BCTU_HW_REG_SIZE)
    {
    case 0u:
        bctu->SFTRGR1 |= bitmask;
        break;
    case 1u:
        bctu->SFTRGR2 |= bitmask;
        break;
#if (BCTU_TRGCFG__COUNT > (2u * BCTU_HW_REG_SIZE))
    case 2u:
        bctu->SFTRGR3 |= bitmask;
        break;
#endif /* (BCTU_TRGCFG__COUNT > (2u * BCTU_HW_REG_SIZE)) */
    default:
        DEV_ASSERT(false); /* Invalid index of SWTRG register. */
        break;
    }
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_DisableWriteProtect
* Description   : This function temporarily/permanently disables write protection.
* When write protection is disabled, triggers can be activated via software,
* using BCTU_DRV_SwTriggerConv().
* Temporarily disabling the write protection allows only a single write operation
* on designated registers, thus a single use of BCTU_DRV_SwTriggerConv().
*
* Implements    : BCTU_DRV_DisableWriteProtect_Activity
* END**************************************************************************/
void BCTU_DRV_DisableWriteProtect(const uint32_t instance, const bctu_wr_prot_duration_t temp)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);

    BCTU_Type * const bctu = s_bctuBase[instance];

    DEV_ASSERT((bctu->MCR & BCTU_MCR_MDIS_MASK) == 0u); /* WRPROT is writable only if not in low power mode */

    switch(temp)
    {
    case BCTU_WR_PROT_TEMPORARY:
        bctu->WRPROT = BCTU_WRPROT_PROTEC_CODE(BCTU_PROTEC_CODE_TEMP);
        break;
    case BCTU_WR_PROT_PERMANENT:
        bctu->WRPROT = BCTU_WRPROT_PROTEC_CODE(BCTU_PROTEC_CODE_PERMANENT);
        break;
    default:
        DEV_ASSERT(false);
        break;
    }
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_GetConvData
* Description   : This function returns the current conversion result data
* corresponding to an adc instance.
*
* Implements    : BCTU_DRV_GetConvData_Activity
* END**************************************************************************/
uint16_t BCTU_DRV_GetConvData(const uint32_t instance, const uint8_t adcIdx)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(adcIdx < FEATURE_BCTU_NUM_ADC);

    const BCTU_Type * const bctu = s_bctuBase[instance];
    uint32_t data = 0u;

    data = (adcIdx == 0u) ? ((bctu->ADC0DR & BCTU_ADC0DR_ADC_DATA_MASK) >> BCTU_ADC0DR_ADC_DATA_SHIFT) : \
                            ((bctu->ADC1DR & BCTU_ADC1DR_ADC_DATA_MASK) >> BCTU_ADC1DR_ADC_DATA_SHIFT);

    return (uint16_t) data;
}


/*FUNCTION**********************************************************************
*
* Function Name : BCTU_DRV_GetConvResult
* Description   : This function returns the full conversion result information
* corresponding to an adc instance.
*
* Implements    : BCTU_DRV_GetConvResult_Activity
* END**************************************************************************/
void BCTU_DRV_GetConvResult(const uint32_t instance, const uint8_t adcIdx, bctu_conv_result_t * const result)
{
    DEV_ASSERT(instance < BCTU_INSTANCE_COUNT);
    DEV_ASSERT(adcIdx < FEATURE_BCTU_NUM_ADC);
    DEV_ASSERT(result != NULL);

    const BCTU_Type * const bctu = s_bctuBase[instance];

    if(adcIdx == 0u)
    {
        result->adcData    = (uint16_t) ((bctu->ADC0DR & BCTU_ADC0DR_ADC_DATA_MASK) >> BCTU_ADC0DR_ADC_DATA_SHIFT);
        result->chanIdx    = (uint8_t)  ((bctu->ADC0DR & BCTU_ADC0DR_CH_MASK)       >> BCTU_ADC0DR_CH_SHIFT);
        result->triggerIdx = (uint8_t)  ((bctu->ADC0DR & BCTU_ADC0DR_TRG_SRC_MASK)  >> BCTU_ADC0DR_TRG_SRC_SHIFT);
        result->lastFlag   = ((bctu->ADC0DR & BCTU_ADC0DR_LAST_MASK) != 0u) ? true : false;
        result->listFlag   = ((bctu->ADC0DR & BCTU_ADC0DR_LIST_MASK) != 0u) ? true : false;
    }
    else
    {
        result->adcData    = (uint16_t) ((bctu->ADC1DR & BCTU_ADC1DR_ADC_DATA_MASK) >> BCTU_ADC1DR_ADC_DATA_SHIFT);
        result->chanIdx    = (uint8_t)  ((bctu->ADC1DR & BCTU_ADC1DR_CH_MASK)       >> BCTU_ADC1DR_CH_SHIFT);
        result->triggerIdx = (uint8_t)  ((bctu->ADC1DR & BCTU_ADC1DR_TRG_SRC_MASK)  >> BCTU_ADC1DR_TRG_SRC_SHIFT);
        result->lastFlag   = ((bctu->ADC1DR & BCTU_ADC1DR_LAST_MASK) != 0u) ? true : false;
        result->listFlag   = ((bctu->ADC1DR & BCTU_ADC1DR_LIST_MASK) != 0u) ? true : false;
    }
}


/*LOCAL FUNCTION**********************************************************************
*
* Function Name : BCTU_GetBitmask
* Description   : Internal helper function to get 32b bitmask with '1' set on input position.
* END**************************************************************************/
static inline uint32_t BCTU_GetBitmask(const uint8_t pos)
{
    return (1u << pos);
}


/*LOCAL FUNCTION**********************************************************************
*
* Function Name : BCTU_PrepareReconfigNonBlocking
* Description   : Attempts to prepare on the fly reconfiguration of a trigger config register:
* - disable new conversions \n
* - if there is no running conversion, clear pending triggers and return STATUS_SUCCESS \n
* - if there is a running conversion, abort and return STATUS_BUSY
* END**************************************************************************/
static status_t BCTU_PrepareReconfigNonBlocking(BCTU_Type * const bctu, const uint8_t trigIdx)
{
    status_t status;

    /* Disable any new conversions from this trigger.
     * This also stops new conversions from starting when loop flag is enabled. */
    bctu->TRGCFG_[trigIdx] &= (~BCTU_TRGCFG__TRIGEN_MASK);

    /* Check if there is any ongoing conversion triggered by current trigger config register.  */
    if((bctu->TRGCFG_[trigIdx] & BCTU_TRGCFG__TRG_FLAG_MASK) != 0u)
    {
        status = STATUS_BUSY;
    }
    else
    {
        /* Clear pending trigger sources */
        bctu->TRGCFG_[trigIdx] |= BCTU_TRGCFG__TRG_FLAG_MASK;

        /* After this point the TRGCFG register can be modified freely. */

        status = STATUS_SUCCESS;
    }

    return status;
}

/*LOCAL FUNCTION**********************************************************************
*
* Function Name : BCTU_PrepareReconfigBlocking
* Description   : Prepares on the fly reconfiguration of a trigger configuration
* register: disables new conversions, clears pending triggers and waits for current conversion
* to finish execution, within a provided timeout interval.
* Returns error, if the current active conversion doesn't finish in the provided interval.
*
* END**************************************************************************/
static status_t BCTU_PrepareReconfigBlocking(BCTU_Type * const bctu, const uint8_t trigIdx, const uint32_t timeout)
{
    uint32_t start, delta;
    status_t status;

    /* TODO: would be nice to have timeout in microseconds */

    /* Make sure OSIF timer is initialized. */
    OSIF_TimeDelay(0u);

    /* Disable any new conversions from this trigger. */
    bctu->TRGCFG_[trigIdx] &= (~BCTU_TRGCFG__TRIGEN_MASK);

    /* Make sure looping is disabled. */
    bctu->TRGCFG_[trigIdx] &= (~ BCTU_TRGCFG__LOOP_MASK);

    /* Wait until current ongoing conversion (if any) finishes execution or timeout occurs.  */
    start = OSIF_GetMilliseconds();
    delta = 0u;
    while(((bctu->TRGCFG_[trigIdx] & BCTU_TRGCFG__TRG_FLAG_MASK) != 0u) && (delta < timeout))
    {
        delta = OSIF_GetMilliseconds() - start;
    }

    if(delta >= timeout)
    {
        status = STATUS_TIMEOUT;
    }
    else
    {
        /* Clear pending trigger sources */
        bctu->TRGCFG_[trigIdx] |= BCTU_TRGCFG__TRG_FLAG_MASK;

        status = STATUS_SUCCESS;

        /* After this point the TRGCFG register can be modified freely. */
    }

    return status;
}

