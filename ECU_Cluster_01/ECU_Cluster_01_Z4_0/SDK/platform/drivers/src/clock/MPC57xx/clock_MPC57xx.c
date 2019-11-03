/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

/*! @file clock_MPC5748G.c */

/*!
 * @ingroup clock_manager
 * @defgroup clock_MPC57xx
 * @{
 */

 /**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, conversion between a pointer and integer
 * type.
 * The cast is needed to get the addresses of MC_ME and CGM hardware modules.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, cast from pointer to unsigned long
 * The cast is needed to get the addresses of MC_ME and CGM hardware modules.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 17.2, functions shall not call themselves, either
 * directly or indirectly. The maximum depth of recursion is 2.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block scope
 * it its identifier only appears in a single function. interfaceClocks must be visible
 * like clockNameMappings
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 16.1, All switch statements shall be well-formed.
 * Source code must address all devices from C55 family.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 16.6, Every switch statement shall have at least two switch-clauses.
 * Source code must address all devices from C55 family.
 */

#include "device_registers.h"
#include "mc_me_hw_access.h"
#include "cgm_hw_access.h"
#include "cgmcs_hw_access.h"
#include "clock_manager.h"
#include <stddef.h>
/* This header is included for bool type */
/*
 * README:
 * This file provides these APIs:
 * APIs to get the frequency of output clocks in Reference Manual ->
 * Chapter Clocking -> Figure Clock Generation diagram.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Clock name mappings
 *         Constant array storing the mappings between clock names and peripheral clock control indexes.
 *         If there is no peripheral clock control index for a clock name, then the corresponding value is
 *         MC_ME_INVALID_INDEX.
 */
const uint16_t clockNameMappings[] = MC_ME_CLOCK_NAME_MAPPINGS;

/*! @brief Interface clocks
 *         Constant array storing the mappings between clock names and interface clocks.
 *         If a clock name is not a module clock name, then the corresponding value is
 *         CLOCK_NAME_COUNT.
 */
static const clock_names_t interfaceClocks[] = INTERFACE_CLOCKS;

#if (FEATURE_HAS_SXOSC_CLK != 0U) || (FEATURE_HAS_XOSC_CLK != 0U)
uint32_t g_xtal0ClkFreq;                         /* EXTAL0 clock    */
#endif
#if FEATURE_HAS_FXOSC_CLK
uint32_t g_xtal1ClkFreq;                         /* EXTAL1 clock    */
#endif


#define MC_ME_COMPLETE_TRANSITION_TIMEOUT 10000000U

/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/

static void CLOCK_SYS_SetIrc(cgmcs_irc_config_t const* config, uint32_t instance);
static void CLOCK_SYS_SetXosc(cgmcs_xosc_config_t const* config, uint32_t instance);
static void CLOCK_SYS_SetPll(cgmcs_plldig_config_t const* config, cgm_config_t const* cgmConfig, uint32_t instance);
static void CLOCK_SYS_ConfigureSystemClockDividers(cgm_config_t const* config);
static void CLOCK_SYS_ConfigureAuxiliarySelectorsAndDividers(cgm_config_t const* config);
static status_t CLOCK_SYS_ResetMcMeConfiguration(void);
static status_t CLOCK_SYS_ConfigureCgmcs(clock_manager_user_config_t const* config);
static status_t CLOCK_SYS_ConfigureMcMe(clock_manager_user_config_t const* config);
static status_t CLOCK_SYS_ConfigureCgm(clock_manager_user_config_t const* config);
static status_t CLOCK_SYS_GetClockSourceFreq(clock_names_t clockName, uint32_t *frequency);
static status_t CLOCK_SYS_GetSystemClockFreq(clock_names_t clockName,uint32_t *frequency);
static status_t CLOCK_SYS_GetModuleClockFreq(clock_names_t clockName,uint32_t *frequency);
static void CLOCK_SYS_ConfigureClockouts(cgm_config_t const* config);
#if FEATURE_HAS_FLEXCAN_CLKS
static status_t CLOCK_SYS_GetFlexcanClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_ADC_CLKS
static status_t CLOCK_SYS_GetAdcClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_ENET_CLKS
static status_t CLOCK_SYS_GetEnetClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_ENET_TIME_CLKS
static status_t CLOCK_SYS_GetEnetTimeClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_LFAST_CLKS
static status_t CLOCK_SYS_GetLfastClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_SENT_CLKS
static status_t CLOCK_SYS_GetSentClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_SGEN_CLKS
static status_t CLOCK_SYS_GetSgenClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_SPI_CLKS
static status_t CLOCK_SYS_GetSpiClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif
#if FEATURE_HAS_FLEXRAY_CLKS
static status_t CLOCK_SYS_GetFlexrayClockFreq(clock_names_t clockName, uint32_t *frequency);
#endif





/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_SetConfiguration
 * Description   : This function sets the system to target configuration, it
 * only sets the clock modules registers for clock mode change, but not send
 * notifications to drivers.
 *
 * Implements CLOCK_SYS_SetConfiguration_Activity
 *END**************************************************************************/
status_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const* config)
{
    status_t result = STATUS_SUCCESS;
    DEV_ASSERT(config != NULL);

	/* Clears peripheral clock gating and all clock sources in all power modes.
	 * Only an IRC type clock source is enabled and set as system clock source. */
    result = CLOCK_SYS_ResetMcMeConfiguration();

	if (STATUS_SUCCESS == result)
	{
		/* Configure clock sources. */
		result = CLOCK_SYS_ConfigureCgmcs(config);

		if (STATUS_SUCCESS == result)
		{
			/* Configure peripheral clock gating, system clock source
			 * and clock sources in all power modes. */
			result = CLOCK_SYS_ConfigureMcMe(config);

			if (STATUS_SUCCESS == result)
			{
				/* Configure peripheral clocks. */
				result = CLOCK_SYS_ConfigureCgm(config);
			}
		}
    }

    return result;
}

#if FEATURE_HAS_SIRC_CLK
static status_t CLOCK_SYS_GetSircFreq(uint32_t *frequency)
{
    uint32_t divider;

    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetSircStatus(MC_ME))
    {
        *frequency = FEATURE_IRCOSC0_FREQ;

        divider = CGMCS_GetSircDividerRatio(SIRC) + 1U;

        *frequency /= divider;

        status = STATUS_SUCCESS;
    }
    return status;
}

static status_t CLOCK_SYS_GetUndividedSircFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetSircStatus(MC_ME))
    {
        *frequency = FEATURE_IRCOSC0_FREQ;

        status = STATUS_SUCCESS;
    }
    return status;
}

#endif

#if FEATURE_HAS_FIRC_CLK
static status_t CLOCK_SYS_GetFircFreq(uint32_t *frequency)
{
    uint32_t divider;

    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetFircStatus(MC_ME))
    {
        *frequency = FEATURE_IRCOSC1_FREQ;

        divider = CGMCS_GetFircDividerRatio(FIRC) + 1U;

        *frequency /= divider;

        status = STATUS_SUCCESS;
    }
    return status;
}

static status_t CLOCK_SYS_GetUndividedFircFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetFircStatus(MC_ME))
    {
        *frequency = FEATURE_IRCOSC1_FREQ;

        status = STATUS_SUCCESS;
    }
    return status;
}

#endif

#if FEATURE_HAS_IRCOSC_CLK
static status_t CLOCK_SYS_GetIrcoscFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetIrcStatus(MC_ME))
    {
        *frequency = FEATURE_IRCOSC0_FREQ;

        status = STATUS_SUCCESS;
    }
    return status;
}
#endif


#if FEATURE_HAS_SXOSC_CLK
static status_t CLOCK_SYS_GetSxoscFreq(uint32_t *frequency)
{
    uint32_t divider;

    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetSxoscStatus(MC_ME))
    {
        *frequency = g_xtal0ClkFreq;

        divider = CGMCS_GetSxoscDividerRatio(SXOSC) + 1U;

        *frequency /= divider;

        status = STATUS_SUCCESS;
    }
    return status;
}

static status_t CLOCK_SYS_GetUndividedSxoscFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetSxoscStatus(MC_ME))
    {
        *frequency = g_xtal0ClkFreq;

        status = STATUS_SUCCESS;
    }
    return status;
}
#endif

#if FEATURE_HAS_FXOSC_CLK
static status_t CLOCK_SYS_GetFxoscFreq(uint32_t *frequency)
{
    uint32_t divider;

    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetFxoscStatus(MC_ME))
    {
        *frequency = g_xtal1ClkFreq;

        divider = CGMCS_GetFxoscDividerRatio(FXOSC) + 1U;

        *frequency /= divider;

        status = STATUS_SUCCESS;
    }
    return status;
}

static status_t CLOCK_SYS_GetUndividedFxoscFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetFxoscStatus(MC_ME))
    {
        *frequency = g_xtal1ClkFreq;

        status = STATUS_SUCCESS;
    }
    return status;
}

#endif

#if FEATURE_HAS_XOSC_CLK
static status_t CLOCK_SYS_GetXoscFreq(uint32_t *frequency)
{
    /* This clock source is not enabled in current mode. */
    status_t status = STATUS_MCU_GATED_OFF;

    if (MC_ME_GetXoscStatus(MC_ME))
    {
        *frequency = g_xtal0ClkFreq;

        status = STATUS_SUCCESS;
    }
    return status;
}
#endif

#if FEATURE_HAS_PLL_PHI0_CLK
static status_t CLOCK_SYS_GetPllPhiXFreq(uint32_t *frequency, uint32_t outputChannel)
{
    uint32_t multiplyFactor = 0U, denominatorFracLoopDivider = 0U, numeratorFracLoopDivider = 0U, freq1 = 0U, freq2 = 0U;
    status_t status = STATUS_SUCCESS;

    if (MC_ME_GetPllStatus(MC_ME))
    {
        /* Check the pll input reference */
        if (((uint32_t)CGM_PLL_REFERENCE_FIRC) == CGM_GetAC5_SelValue(MC_CGM))
        {
            /* Gets input frequency - FIRC */
            status = CLOCK_SYS_GetFircFreq(frequency);
        }
        else if (((uint32_t)CGM_PLL_REFERENCE_FXOSC) == CGM_GetAC5_SelValue(MC_CGM))
        {
            /* Gets input frequency - FXOSC */
            status = CLOCK_SYS_GetFxoscFreq(frequency);
        }
        else
        {
            /* Invalid input reference */
            DEV_ASSERT(false);
        }

        if (STATUS_SUCCESS == status)
        {
            (*frequency) /= CGMCS_GetPllPreDividerRatio(PLLDIG,0U);  /* Pre-divider. */

            multiplyFactor              = CGMCS_GetPLLMultiplyFactor(PLLDIG,0U);
            numeratorFracLoopDivider    = CGMCS_GetPLLNumeratorFractionalLoopDivider(PLLDIG);
            denominatorFracLoopDivider  = CGMCS_GetPLLDenominatorFractionalLoopDivider(PLLDIG);

            /* VCO = PLL_REF / PREDIV * multiplierFactor when numerator is zero. */
            freq1 = *frequency * multiplyFactor;

            /* VCO = PLL_REF / PREDIV * numeratorFracLoopDivider / (1 + denominatorFracLoopDivider)  when multiplyFactor is zero (multiplyFactor can't be zero)  */
            freq2 = (*frequency / (1U + denominatorFracLoopDivider));
            freq2 = (freq2 * numeratorFracLoopDivider) + (((*frequency - (freq2 * (1U + denominatorFracLoopDivider))) * numeratorFracLoopDivider) / (1U + denominatorFracLoopDivider));

            /* VCO = PLL_REF / PREDIV * (multiplierFactor + numeratorFracLoopDivider / (1 + denominatorFracLoopDivider)) */
            *frequency = freq1 + freq2;

            switch(outputChannel)
            {
                case 0U:
                    (*frequency) = (*frequency) >> (1U + CGMCS_GetPllFirstOutputDividerRatio(PLLDIG,0U));
                    break;

                case 1U:
                    (*frequency) = (*frequency) >> (1U + CGMCS_GetPllSecondOutputDividerRatio(PLLDIG,0U));
                    break;

                default:
                    /* Invalid output channel index */
                    DEV_ASSERT(false);
                    break;
            }

        }
    }
    else
    {
        /* This clock source is not enabled in current mode. */
        status = STATUS_MCU_GATED_OFF;
    }

    return status;

}
#endif


#if (FEATURE_HAS_PLL0_PHI0_CLK != 0U) || (FEATURE_HAS_PLL0_PHI1_CLK != 0U)
static status_t CLOCK_SYS_GetPll0PhiXFreq(uint32_t *frequency, uint32_t outputChannel)
{
    uint32_t multiplier = 0U;
    status_t status = STATUS_SUCCESS;

    if (MC_ME_GetPll0Status(MC_ME))
    {
        /* Check the pll input reference */
        if (((uint32_t)CGM_PLL_REFERENCE_IRCOSC) == CGM_GetAC3_SelValue(MC_CGM))
        {
            /* Gets input frequency - IRCCOSC */
            status = CLOCK_SYS_GetIrcoscFreq(frequency);
        }
        else if (((uint32_t)CGM_PLL_REFERENCE_FXOSC) == CGM_GetAC3_SelValue(MC_CGM))
        {
            /* Gets input frequency - XOSC */
            status = CLOCK_SYS_GetXoscFreq(frequency);
        }
        else
        {
            /* Invalid input reference */
            DEV_ASSERT(false);
        }

        if (STATUS_SUCCESS == status)
        {
            (*frequency) /= CGMCS_GetPllPreDividerRatio(PLLDIG, 0U);  /* Pre-divider. */

            multiplier    = CGMCS_GetPLLMultiplyFactor(PLLDIG, 0U);
            (*frequency) *= multiplier;

            switch(outputChannel)
            {
                case 0U:
                    (*frequency) /= (CGMCS_GetPllFirstOutputDividerRatio(PLLDIG,0U));
                    break;

                case 1U:
                    (*frequency) /= (CGMCS_GetPllSecondOutputDividerRatio(PLLDIG,0U));
                    break;

                default:
                    /* Invalid output channel index */
                    DEV_ASSERT(false);
                    break;
            }
        }
    }
    else
    {
        /* This clock source is not enabled in current mode. */
        status = STATUS_MCU_GATED_OFF;
    }

    return status;

}
#endif

#if (FEATURE_HAS_PLL1_PHI0_CLK != 0U) || (FEATURE_HAS_PLL1_PHI1_CLK != 0U)
static status_t CLOCK_SYS_GetPll1PhiXFreq(uint32_t *frequency, uint32_t outputChannel)
{
    uint32_t multiplyFactor = 0U, fractionalDivider = 0U, outputDivider = 0U;
    uint32_t freq1, freq2;
    status_t status = STATUS_SUCCESS;

    if (MC_ME_GetPll1Status(MC_ME))
    {
        /* Check the pll input reference */
        if (((uint32_t)CGM_PLL_REFERENCE_XOSC) == CGM_GetAC4_SelValue(MC_CGM))
        {
            /* Gets input frequency - XOSC */
            status = CLOCK_SYS_GetXoscFreq(frequency);
        }
        else if (((uint32_t)CGM_PLL_REFERENCE_PLL0_PHI1) == CGM_GetAC4_SelValue(MC_CGM))
        {
            status = CLOCK_SYS_GetPll0PhiXFreq(frequency,1U);
        }
        else
        {
            /* Invalid input reference */
            DEV_ASSERT(false);
        }

        if (STATUS_SUCCESS == status)
        {
            multiplyFactor      = CGMCS_GetPLLMultiplyFactor(PLLDIG,1U);
            fractionalDivider   = (CGMCS_GetPLLFractionalDivider(PLLDIG) << 1U) + 1U;
            outputDivider       = CGMCS_GetPllFirstOutputDividerRatio(PLLDIG,1U) << 1U;

            /* Freq1 = PLL1_REF * PLL1DV[MFD] / (2 * PLL1DV[RFDPHI]) */
            freq1 = ( ((((*frequency) >> 6U) / outputDivider) * multiplyFactor) + (((((*frequency) >> 6U) % outputDivider) * multiplyFactor) / outputDivider) ) << 6U;

            /* Freq2 = PLL1_REF * (2 * PLL1FD[FRCDIV] + 1) / ((1 << 14) * PLL1DV[RFDPHI]) */
            freq2 = ((((*frequency) >> 13U) / outputDivider) * fractionalDivider) + (((((*frequency) >> 13U) % outputDivider) * fractionalDivider) / outputDivider);

            (*frequency) = freq1 + freq2;

            (void)outputChannel;
        }
    }
    else
    {
        /* This clock source is not enabled in current mode. */
        status = STATUS_MCU_GATED_OFF;
    }

    return status;

}
#endif


static status_t CLOCK_SYS_GetClockSourceFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;

    *frequency = 0U;

    switch(clockName)
    {
#if FEATURE_HAS_SIRC_CLK
        case SIRC_CLK:
            status = CLOCK_SYS_GetSircFreq(frequency);
            break;

        case SIRC_UNDIVIDED_CLK:
            status = CLOCK_SYS_GetUndividedSircFreq(frequency);
            break;
#endif

#if FEATURE_HAS_FIRC_CLK
        case FIRC_CLK:
            status = CLOCK_SYS_GetFircFreq(frequency);
            break;

        case FIRC_UNDIVIDED_CLK:
            status = CLOCK_SYS_GetUndividedFircFreq(frequency);
            break;
#endif

#if FEATURE_HAS_IRCOSC_CLK
        case IRCOSC_CLK:
            status = CLOCK_SYS_GetIrcoscFreq(frequency);
            break;
#endif

#if FEATURE_HAS_SXOSC_CLK
        case SXOSC_CLK:
            status = CLOCK_SYS_GetSxoscFreq(frequency);
            break;

        case SXOSC_UNDIVIDED_CLK:
            status = CLOCK_SYS_GetUndividedSxoscFreq(frequency);
            break;
#endif

#if FEATURE_HAS_FXOSC_CLK
        case FXOSC_CLK:
            status = CLOCK_SYS_GetFxoscFreq(frequency);
            break;

        case FXOSC_UNDIVIDED_CLK:
            status = CLOCK_SYS_GetUndividedFxoscFreq(frequency);
            break;
#endif

#if FEATURE_HAS_XOSC_CLK
        case XOSC_CLK:
            status = CLOCK_SYS_GetXoscFreq(frequency);
            break;
#endif

#if FEATURE_HAS_PLL_PHI0_CLK
        case PLL_PHI0_CLK:
            status = CLOCK_SYS_GetPllPhiXFreq(frequency,0U);
            break;
#endif

#if FEATURE_HAS_PLL_PHI1_CLK
        case PLL_PHI1_CLK:
            status = CLOCK_SYS_GetPllPhiXFreq(frequency,1U);
            break;
#endif


#if FEATURE_HAS_PLL0_PHI0_CLK
        case PLL0_PHI0_CLK:
            status = CLOCK_SYS_GetPll0PhiXFreq(frequency,0U);
            break;
#endif

#if FEATURE_HAS_PLL0_PHI1_CLK
        case PLL0_PHI1_CLK:
            status = CLOCK_SYS_GetPll0PhiXFreq(frequency,1U);
            break;
#endif

#if FEATURE_HAS_PLL1_PHI0_CLK
        case PLL1_PHI0_CLK:
            status = CLOCK_SYS_GetPll1PhiXFreq(frequency,0U);
            break;
#endif

        default:
            /* This clock source is not supported. */
            status = STATUS_UNSUPPORTED;
            break;
    }
    return status;
}


static status_t CLOCK_SYS_GetSystemClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t currentSysClk, sysClkFreq = 0U, divider;
    *frequency = 0U;

    /* Get System Clock from CGM_0 */
    currentSysClk = MC_ME_GetCurrentSystemClock(MC_ME);

    switch(currentSysClk)
    {
#if FEATURE_HAS_FIRC_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_FIRC):
            status = CLOCK_SYS_GetClockSourceFreq(FIRC_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_FXOSC_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_FXOSC):
            status = CLOCK_SYS_GetClockSourceFreq(FXOSC_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_PLL_PHI0_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_PLL_PHI0):
            status = CLOCK_SYS_GetClockSourceFreq(PLL_PHI0_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_IRCOSC_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_IRCOSC):
            status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_XOSC_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_XOSC):
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_PLL0_PHI0):
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,&sysClkFreq);
            break;
#endif
#if FEATURE_HAS_PLL1_PHI0_CLK
        case ((uint32_t)CGM_SYSTEM_CLOCK_SRC_PLL1_PHI0):
            status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,&sysClkFreq);
            break;
#endif
        default:
            /* Invalid clock source for system clock */
            DEV_ASSERT(false);
            status = STATUS_ERROR;
            break;
    }

    if (status == STATUS_SUCCESS)
    {
        /* System clock and core clock are the same. */
        if ((clockName == SYS_CLK) || (clockName == CORE_CLK))
        {
            *frequency = sysClkFreq;
        }
#if FEATURE_HAS_S160_CLKS
        else if (clockName == S160_CLK)
        {
            if (CGM_GetSC_DC0_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC0_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_S80_CLKS
        else if (clockName == S80_CLK)
        {
            if (CGM_GetSC_DC1_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC1_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_S40_CLKS
        else if (clockName == S40_CLK)
        {
            if (CGM_GetSC_DC2_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC2_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_F40_CLKS
        else if (clockName == F40_CLK)
        {
            if (CGM_GetSC_DC3_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC3_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_F80_CLKS
        else if (clockName == F80_CLK)
        {
            if (CGM_GetSC_DC4_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC4_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_FS80_CLKS
        else if (clockName == FS80_CLK)
        {
            if (CGM_GetSC_DC5_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC5_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_F20_CLKS
        else if (clockName == F20_CLK)
        {
            if (CGM_GetSC_DC6_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC6_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_PBRIDGEx_CLKS
        else if (clockName == PBRIDGEx_CLK)
        {
            if (CGM_GetSC_DC0_Status(MC_CGM))
            {
                divider = CGM_GetSC_DC0_DividerValue(MC_CGM) + 1U;
                sysClkFreq /= divider;
                *frequency = sysClkFreq;
            }
        }
#endif
#if FEATURE_HAS_HALFSYS_CLKS
        else if (clockName == HALFSYS_CLK)
        {
            *frequency = sysClkFreq / 2U;
        }
#endif
#if FEATURE_HAS_MOTC_CLKS
        else if (clockName == MOTC_CLK)
        {
            switch(CGM_GetAC0_SelValue(MC_CGM))
            {
                case CGM_MOTC_SOURCE_IRCOSC:
                    status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,frequency);
                    break;
                case CGM_MOTC_SOURCE_XOSC:
                    status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
                    break;
                case CGM_MOTC_SOURCE_PLL0_PHI0:
                    status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
                    break;
                case CGM_MOTC_SOURCE_PLL1_PHI0:
                    status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,frequency);
                    break;
                default:
                    /* Invalid value of AC0 selector */
                    DEV_ASSERT(false);
                    break;
            }

            if (CGM_GetAC0_DC0_Status(MC_CGM))
            {
                divider = CGM_GetAC0_DC0_DividerValue(MC_CGM) + 1U;
                *frequency /= divider;
            }
        }
#endif
        else
        {
            DEV_ASSERT(false);
            status = STATUS_ERROR;
        }
    }
    return status;
}


static status_t CLOCK_SYS_GetModuleClockFreq(clock_names_t clockName,uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t interfaceClock = 0U;

    /* If clock name is not valid, also the interface clock is not valid */
    if (interfaceClocks[clockName] == CLOCK_NAME_COUNT)
    {
        status = STATUS_UNSUPPORTED;
    }
    else
    {
        /* Check interface clock */

#if FEATURE_FLEXCAN0_CLK_CONFIGURABLE_INTERFACE_CLOCK == 1U
        if (clockName == FLEXCAN0_CLK)
        {
            switch(CGM_GetAC9_SelValue(MC_CGM))
            {
                case CGM_FLEXCANx_SOURCE_FS80:
                    status = CLOCK_SYS_GetFreq(FS80_CLK,&interfaceClock);
                    break;

                case CGM_FLEXCANx_SOURCE_FXOSC:
                    status = CLOCK_SYS_GetFreq(FXOSC_CLK,&interfaceClock);
                    break;

                default:
                    /* Invalid clock entry */
                    DEV_ASSERT(false);
                    break;
            }
        }
        else
        {
            status = CLOCK_SYS_GetFreq(interfaceClocks[clockName],&interfaceClock);
        }
#else
            status = CLOCK_SYS_GetFreq(interfaceClocks[clockName],&interfaceClock);
#endif




        if ((!MC_ME_GetModuleStatus(MC_ME,clockName) || (interfaceClock == 0U)))
        {
            status = STATUS_MCU_GATED_OFF;
        }
    }

    if (status == STATUS_SUCCESS)
    {
        /* It will never reach this point because CLOCK_NAME_COUNT is an invalid clock name */
        if (clockName == CLOCK_NAME_COUNT)
        {
            status = STATUS_UNSUPPORTED;
        }
        /* Module clocks  */
    #if FEATURE_HAS_S160_CLKS
        else if (clockName < END_OF_S160_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(S160_CLK,frequency);
        }
    #endif /* FEATURE_HAS_S80_CLKS */
    #if FEATURE_HAS_S80_CLKS
        else if (clockName < END_OF_S80_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(S80_CLK,frequency);
        }
    #endif /* FEATURE_HAS_S40_CLKS */
    #if FEATURE_HAS_S40_CLKS
        else if (clockName < END_OF_S40_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(S40_CLK,frequency);
        }
    #endif /* FEATURE_HAS_F40_CLKS */
    #if FEATURE_HAS_F40_CLKS
        else if (clockName < END_OF_F40_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(F40_CLK,frequency);
        }
    #endif /* FEATURE_HAS_F40_CLKS */
    #if FEATURE_HAS_F80_CLKS
        else if (clockName < END_OF_F80_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(F80_CLK,frequency);
        }
    #endif /* FEATURE_HAS_F80_CLKS */
    #if FEATURE_HAS_FS80_CLKS
        else if (clockName < END_OF_FS80_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(FS80_CLK,frequency);
        }
    #endif /* FEATURE_HAS_FS80_CLKS */
    #if FEATURE_HAS_F20_CLKS
        else if (clockName < END_OF_F20_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(F20_CLK,frequency);
        }
    #endif /* FEATURE_HAS_F20_CLKS */
    #if FEATURE_HAS_PBRIDGEx_CLKS
        else if (clockName < END_OF_PBRIDGEx_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(PBRIDGEx_CLK,frequency);
        }
    #endif /* FEATURE_HAS_PBRIDGEx_CLKS */
    #if FEATURE_HAS_HALFSYS_CLKS
        else if (clockName < END_OF_HALFSYS_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(HALFSYS_CLK,frequency);
        }
    #endif /* FEATURE_HAS_HALFSYS_CLKS */
    #if FEATURE_HAS_MOTC_CLKS
        else if (clockName < END_OF_MOTC_CLKS)
        {
            status = CLOCK_SYS_GetSystemClockFreq(MOTC_CLK,frequency);
        }
    #endif /* END_OF_MOTC_CLKS */
    #if FEATURE_HAS_ADC_CLKS
        else if (clockName < END_OF_ADC_CLKS)
        {
            status = CLOCK_SYS_GetAdcClockFreq(clockName,frequency);
        }
    #endif /* END_OF_ADC_CLKS */
    #if FEATURE_HAS_ENET_CLKS
        else if (clockName < END_OF_ENET_CLKS)
        {
            status = CLOCK_SYS_GetEnetClockFreq(clockName,frequency);
        }
    #endif /* END_OF_ENET_CLKS */
    #if FEATURE_HAS_ENET_TIME_CLKS
        else if (clockName < END_OF_ENET_TIME_CLKS)
        {
            status = CLOCK_SYS_GetEnetTimeClockFreq(clockName,frequency);
        }
    #endif /* END_OF_ENET_TIME_CLKS */
    #if FEATURE_HAS_FLEXCAN_CLKS
        else if (clockName < END_OF_FLEXCAN_CLKS)
        {
            status = CLOCK_SYS_GetFlexcanClockFreq(clockName,frequency);
        }
    #endif /* END_OF_FLEXCAN_CLKS */
    #if FEATURE_HAS_FLEXRAY_CLKS
        else if (clockName < END_OF_FLEXRAY_CLKS)
        {
            status = CLOCK_SYS_GetFlexrayClockFreq(clockName,frequency);
        }
    #endif /* END_OF_FLEXRAY_CLKS */
    #if FEATURE_HAS_LFAST_CLKS
        else if (clockName < END_OF_LFAST_CLKS)
        {
            status = CLOCK_SYS_GetLfastClockFreq(clockName,frequency);
        }
    #endif /* END_OF_LFAST_CLKS */
    #if FEATURE_HAS_SENT_CLKS
        else if (clockName < END_OF_SENT_CLKS)
        {
            status = CLOCK_SYS_GetSentClockFreq(clockName,frequency);
        }
    #endif /* END_OF_SENT_CLKS */
    #if FEATURE_HAS_SGEN_CLKS
        else if (clockName < END_OF_SGEN_CLKS)
        {
            status = CLOCK_SYS_GetSgenClockFreq(clockName,frequency);
        }
    #endif /* END_OF_SGEN_CLKS */
    #if FEATURE_HAS_SPI_CLKS
        else if (clockName < END_OF_SPI_CLKS)
        {
            status = CLOCK_SYS_GetSpiClockFreq(clockName,frequency);
        }
    #endif /* END_OF_SPI_CLKS */
        else if (clockName < END_OF_PERIPHERAL_CLKS)
        {
            /* These modules don't have a protocol clock, output frequency is zero */
            status = STATUS_SUCCESS;
        }
        else
        {
            status = STATUS_UNSUPPORTED;
        }
    }

    return status;
}

#if FEATURE_HAS_FLEXCAN_CLKS
static status_t CLOCK_SYS_GetFlexcanClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_FLEXCAN == AC2__SC
    selector = CGM_GetAC2_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_FLEXCANx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
        default:
            /* There is no support in CGM for FLECAN clock selection */
            *frequency = 0U;
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_FLEXCAN == AC2__DC0
    if (CGM_GetAC2_DC0_Status(MC_CGM))
    {
        divider = CGM_GetAC2_DC0_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_ADC_CLKS
static status_t CLOCK_SYS_GetAdcClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_ADC == AC0__SC
    selector = CGM_GetAC0_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_IRCOSC_CLK
        case CGM_ADCx_SOURCE_IRCOSC:
            status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_XOSC_CLK
        case CGM_ADCx_SOURCE_XOSC:
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_ADCx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL1_PHI0_CLK
        case CGM_ADCx_SOURCE_PLL1_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_ADC == AC0__DC2
    if (CGM_GetAC0_DC2_Status(MC_CGM))
    {
        divider = CGM_GetAC0_DC2_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_ENET_CLKS
static status_t CLOCK_SYS_GetEnetClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;

    (void)divider;
    (void)clockName;

#if FEATURE_PROTOCOL_CLOCK_FOR_ENET == AC2__SC
    selector = CGM_GetAC2_SelValue(MC_CGM);
#elif FEATURE_PROTOCOL_CLOCK_FOR_ENET == AC10__SC
        selector = CGM_GetAC10_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_F40_CLKS
        case CGM_ENETx_SOURCE_F40:
            status = CLOCK_SYS_GetSystemClockFreq(F40_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_FXOSC_CLK
        case CGM_ENETx_SOURCE_FXOSC:
            status = CLOCK_SYS_GetClockSourceFreq(FXOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_IRCOSC_CLK
        case CGM_ENETx_SOURCE_IRCOSC:
            status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_XOSC_CLK
        case CGM_ENETx_SOURCE_XOSC:
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_ENETx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI1_CLK
        case CGM_ENETx_SOURCE_PLL0_PHI1:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI1_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL1_PHI0_CLK
        case CGM_ENETx_SOURCE_PLL1_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_ENET == AC10__DC0
    if (CGM_GetAC10_DC0_Status(MC_CGM))
    {
        divider = CGM_GetAC10_DC0_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_ENET_TIME_CLKS
static status_t CLOCK_SYS_GetEnetTimeClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;

    (void)divider;
    (void)clockName;

#if FEATURE_PROTOCOL_CLOCK_FOR_ENET_TIME == AC11__SC
        selector = CGM_GetAC11_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_F40_CLKS
        case CGM_ENETx_SOURCE_F40:
            status = CLOCK_SYS_GetSystemClockFreq(F40_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_FXOSC_CLK
        case CGM_ENETx_SOURCE_FXOSC:
            status = CLOCK_SYS_GetClockSourceFreq(FXOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_IRCOSC_CLK
        case CGM_ENETx_SOURCE_IRCOSC:
            status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_XOSC_CLK
        case CGM_ENETx_SOURCE_XOSC:
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_ENETx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI1_CLK
        case CGM_ENETx_SOURCE_PLL0_PHI1:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI1_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL1_PHI0_CLK
        case CGM_ENETx_SOURCE_PLL1_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_ENET_TIME == AC11__DC0
    if (CGM_GetAC11_DC0_Status(MC_CGM))
    {
        divider = CGM_GetAC11_DC0_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}


#endif

#if FEATURE_HAS_LFAST_CLKS
static status_t CLOCK_SYS_GetLfastClockFreq(clock_names_t clockName, uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_LFAST == AC5__SC
    selector = CGM_GetAC5_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_XOSC_CLK
        case CGM_LFASTx_SOURCE_XOSC:
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_LFASTx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif

        case CGM_LFASTx_SOURCE_SYS_PIN:
            *frequency = 0U;
        break;
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_LFAST == AC5__DC0
    if (CGM_GetAC5_DC0_Status(MC_CGM))
    {
        divider = CGM_GetAC5_DC0_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;

}
#endif

#if FEATURE_HAS_SENT_CLKS
static status_t CLOCK_SYS_GetSentClockFreq(clock_names_t clockName, uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_SENT == AC1__SC
    selector = CGM_GetAC1_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_SENTx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_SENT == AC1__DC1
    if (CGM_GetAC1_DC1_Status(MC_CGM))
    {
        divider = CGM_GetAC1_DC1_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_FLEXRAY_CLKS
static status_t CLOCK_SYS_GetFlexrayClockFreq(clock_names_t clockName, uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_FLEXRAY == AC1__SC
    selector = CGM_GetAC1_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_SENTx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_FLEXRAY == AC1__DC0
    if (CGM_GetAC1_DC0_Status(MC_CGM))
    {
        divider = CGM_GetAC1_DC0_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_SGEN_CLKS
static status_t CLOCK_SYS_GetSgenClockFreq(clock_names_t clockName, uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_SGEN == AC0__SC
    selector = CGM_GetAC0_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_IRCOSC_CLK
        case CGM_ADCx_SOURCE_IRCOSC:
            status = CLOCK_SYS_GetClockSourceFreq(IRCOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_XOSC_CLK
        case CGM_ADCx_SOURCE_XOSC:
            status = CLOCK_SYS_GetClockSourceFreq(XOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL0_PHI0_CLK
        case CGM_ADCx_SOURCE_PLL0_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL0_PHI0_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_PLL1_PHI0_CLK
        case CGM_ADCx_SOURCE_PLL1_PHI0:
            status = CLOCK_SYS_GetClockSourceFreq(PLL1_PHI0_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

#if FEATURE_FRACTIONAL_DIVIDER_FOR_SGEN == AC0__DC1
    if (CGM_GetAC0_DC1_Status(MC_CGM))
    {
        divider = CGM_GetAC0_DC1_DividerValue(MC_CGM) + 1U;
        *frequency /= divider;
    }
#endif

    return status;
}
#endif

#if FEATURE_HAS_SPI_CLKS
static status_t CLOCK_SYS_GetSpiClockFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    uint32_t selector = 0U, divider = 0U;
    (void) clockName;
    (void)divider;

#if FEATURE_PROTOCOL_CLOCK_FOR_SPI == AC8__SC
    selector = CGM_GetAC8_SelValue(MC_CGM);
#endif

    switch(selector)
    {
#if FEATURE_HAS_FXOSC_CLK
        case CGM_SPI0_SOURCE_FXOSC:
            status = CLOCK_SYS_GetClockSourceFreq(FXOSC_CLK,frequency);
        break;
#endif
#if FEATURE_HAS_F40_CLKS
        case CGM_SPI0_SOURCE_F40:
            status = CLOCK_SYS_GetSystemClockFreq(F40_CLK,frequency);
        break;
#endif
        default:
            DEV_ASSERT(false);
            break;
    }

    return status;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetFreq
 * Description   : This function returns the frequency of a given clock
 *
 * Implements CLOCK_SYS_GetFreq_Activity
 * END**************************************************************************/
status_t CLOCK_SYS_GetFreq(clock_names_t clockName,
                           uint32_t *frequency)
{
    status_t status = STATUS_SUCCESS;
    *frequency = 0U;

    if((clockName == END_OF_CLK_SOURCES) || (clockName == END_OF_SYSTEM_CLKS) || (clockName == END_OF_PERIPHERAL_CLKS))
    {
        status = STATUS_UNSUPPORTED;
    }
    else if(clockName < END_OF_CLK_SOURCES)
    {
        status = CLOCK_SYS_GetClockSourceFreq(clockName,frequency);
    }
    else if (clockName < END_OF_SYSTEM_CLKS)
    {
        status = CLOCK_SYS_GetSystemClockFreq(clockName,frequency);
    }
    else if (clockName < END_OF_PERIPHERAL_CLKS)
    {
        status = CLOCK_SYS_GetModuleClockFreq(clockName,frequency);
    }
    else
    {
        status = STATUS_UNSUPPORTED;
    }
    return status;
}





/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureCgmcs
 * Description   : Configures clock sources
 *END**************************************************************************/
static status_t CLOCK_SYS_ConfigureCgmcs(clock_manager_user_config_t const* config)
{

#if FEATURE_HAS_SIRC_CLK
    CLOCK_SYS_SetIrc(&config->cgmcsConfig.irc0Config, 0U);
#endif

#if FEATURE_HAS_FIRC_CLK
    CLOCK_SYS_SetIrc(&config->cgmcsConfig.irc1Config, 1U);
#endif

#if FEATURE_HAS_IRCOSC_CLK
    CLOCK_SYS_SetIrc(&config->cgmcsConfig.irc0Config, 0U);
#endif

#if FEATURE_HAS_SXOSC_CLK
    CLOCK_SYS_SetXosc(&config->cgmcsConfig.xosc0Config, 0U);
#endif

#if FEATURE_HAS_FXOSC_CLK
    CLOCK_SYS_SetXosc(&config->cgmcsConfig.xosc1Config, 1U);
#endif

#if FEATURE_HAS_XOSC_CLK
    CLOCK_SYS_SetXosc(&config->cgmcsConfig.xosc0Config, 0U);
#endif

    CLOCK_SYS_SetPll(&config->cgmcsConfig.pll0Config, &config->cgmConfig, 0U);

#if (FEATURE_HAS_PLL1_PHI0_CLK != 0U) || (FEATURE_HAS_PLL1_PHI1_CLK != 0U)
    CLOCK_SYS_SetPll(&config->cgmcsConfig.pll1Config, &config->cgmConfig, 1U);
#endif

    return STATUS_SUCCESS;
}


/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ResetMcMeConfiguration
 * Description   : Clears peripheral clock gating and all clock sources. Only an IRC
 * type clock source is enabled and set as system clock source.
 *END**************************************************************************/
static status_t CLOCK_SYS_ResetMcMeConfiguration(void)
{
    status_t retValue = STATUS_TIMEOUT;
    uint32_t i;

    /* Resets configuration of clock sources in MC_ME. */
    MC_ME_ResetClockSourcesConfiguration(MC_ME);

    /* Resets configuration of peripherals in MC_ME. */
    MC_ME_ResetPeripheralsConfiguration(MC_ME,(uint32_t)MC_ME_PERIPH_CONFIG_COUNT);

    /* Clock sources are enabled after mode transition */
    MC_ME_ChangeMode(MC_ME);

    for (i= 0U; i < MC_ME_COMPLETE_TRANSITION_TIMEOUT; i++)
    {
        /* Check whether transition completed */
        if (MC_ME_GetTransitionStatus(MC_ME))
        {
            retValue = STATUS_SUCCESS;
            break;
        }
    }

    return retValue;
}


/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureMcMeClocking
 * Description   : Configures peripheral clock gating, system clock source and
 * clock sources in all power modes.
 *END**************************************************************************/
static status_t CLOCK_SYS_ConfigureMcMe(clock_manager_user_config_t const* config)
{
    status_t retValue = STATUS_TIMEOUT;
    uint32_t i;

    /* Set clock sources in all modes */
    MC_ME_SetClockSourcesInDrunMode(MC_ME, (uint32_t)config->mcmeConfig.drun.sysclk,
            config->mcmeConfig.drun.clocksEnabled.irc0,
            config->mcmeConfig.drun.clocksEnabled.irc1,
            config->mcmeConfig.drun.clocksEnabled.xosc0,
            config->mcmeConfig.drun.clocksEnabled.xosc1,
            config->mcmeConfig.drun.clocksEnabled.pll0,
            config->mcmeConfig.drun.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInRun0Mode(MC_ME, (uint32_t)config->mcmeConfig.run0.sysclk,
            config->mcmeConfig.run0.clocksEnabled.irc0,
            config->mcmeConfig.run0.clocksEnabled.irc1,
            config->mcmeConfig.run0.clocksEnabled.xosc0,
            config->mcmeConfig.run0.clocksEnabled.xosc1,
            config->mcmeConfig.run0.clocksEnabled.pll0,
            config->mcmeConfig.run0.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInRun1Mode(MC_ME, (uint32_t)config->mcmeConfig.run1.sysclk,
            config->mcmeConfig.run1.clocksEnabled.irc0,
            config->mcmeConfig.run1.clocksEnabled.irc1,
            config->mcmeConfig.run1.clocksEnabled.xosc0,
            config->mcmeConfig.run1.clocksEnabled.xosc1,
            config->mcmeConfig.run1.clocksEnabled.pll0,
            config->mcmeConfig.run1.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInRun2Mode(MC_ME, (uint32_t)config->mcmeConfig.run2.sysclk,
            config->mcmeConfig.run2.clocksEnabled.irc0,
            config->mcmeConfig.run2.clocksEnabled.irc1,
            config->mcmeConfig.run2.clocksEnabled.xosc0,
            config->mcmeConfig.run2.clocksEnabled.xosc1,
            config->mcmeConfig.run2.clocksEnabled.pll0,
            config->mcmeConfig.run2.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInRun3Mode(MC_ME, (uint32_t)config->mcmeConfig.run3.sysclk,
            config->mcmeConfig.run3.clocksEnabled.irc0,
            config->mcmeConfig.run3.clocksEnabled.irc1,
            config->mcmeConfig.run3.clocksEnabled.xosc0,
            config->mcmeConfig.run3.clocksEnabled.xosc1,
            config->mcmeConfig.run3.clocksEnabled.pll0,
            config->mcmeConfig.run3.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInSafeMode(MC_ME, (uint32_t)config->mcmeConfig.safe.sysclk,
            config->mcmeConfig.safe.clocksEnabled.irc0,
            config->mcmeConfig.safe.clocksEnabled.irc1,
            config->mcmeConfig.safe.clocksEnabled.xosc0,
            config->mcmeConfig.safe.clocksEnabled.xosc1,
            config->mcmeConfig.safe.clocksEnabled.pll0,
            config->mcmeConfig.safe.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInTestMode(MC_ME, (uint32_t)config->mcmeConfig.test.sysclk,
            config->mcmeConfig.test.clocksEnabled.irc0,
            config->mcmeConfig.test.clocksEnabled.irc1,
            config->mcmeConfig.test.clocksEnabled.xosc0,
            config->mcmeConfig.test.clocksEnabled.xosc1,
            config->mcmeConfig.test.clocksEnabled.pll0,
            config->mcmeConfig.test.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInStopMode(MC_ME, (uint32_t)config->mcmeConfig.stop0.sysclk,
            config->mcmeConfig.stop0.clocksEnabled.irc0,
            config->mcmeConfig.stop0.clocksEnabled.irc1,
            config->mcmeConfig.stop0.clocksEnabled.xosc0,
            config->mcmeConfig.stop0.clocksEnabled.xosc1,
            config->mcmeConfig.stop0.clocksEnabled.pll0,
            config->mcmeConfig.stop0.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInStandbyMode(MC_ME, (uint32_t)config->mcmeConfig.standby0.sysclk,
            config->mcmeConfig.standby0.clocksEnabled.irc0,
            config->mcmeConfig.standby0.clocksEnabled.irc1,
            config->mcmeConfig.standby0.clocksEnabled.xosc0,
            config->mcmeConfig.standby0.clocksEnabled.xosc1,
            config->mcmeConfig.standby0.clocksEnabled.pll0,
            config->mcmeConfig.standby0.clocksEnabled.pll1);
    MC_ME_SetClockSourcesInHaltMode(MC_ME, (uint32_t)config->mcmeConfig.halt0.sysclk,
            config->mcmeConfig.halt0.clocksEnabled.irc0,
            config->mcmeConfig.halt0.clocksEnabled.irc1,
            config->mcmeConfig.halt0.clocksEnabled.xosc0,
            config->mcmeConfig.halt0.clocksEnabled.xosc1,
            config->mcmeConfig.halt0.clocksEnabled.pll0,
            config->mcmeConfig.halt0.clocksEnabled.pll1);
    #ifdef LPU_RUN_CF_SYS_CLK_SEL
    LPU_SetClockSourcesInLpuDrunMode(LPU, (uint32_t)config->mcmeConfig.lpurun.sysclk,
            config->mcmeConfig.lpurun.clocksEnabled.irc0,
            config->mcmeConfig.lpurun.clocksEnabled.irc1,
            config->mcmeConfig.lpurun.clocksEnabled.xosc0,
            config->mcmeConfig.lpurun.clocksEnabled.xosc1);
    #endif
    #if defined(LPU_STOP_CF_FIRC_ON_MASK) || defined(LPU_STOP_CF_SIRC_ON_MASK) || defined(LPU_STOP_CF_FXOSC_ON_MASK) || defined(LPU_STOP_CF_SXOSC_ON_MASK)
    LPU_SetClockSourcesInLpuStopMode(LPU, (uint32_t)config->mcmeConfig.lpustop.sysclk,
            config->mcmeConfig.lpustop.clocksEnabled.irc0,
            config->mcmeConfig.lpustop.clocksEnabled.irc1,
            config->mcmeConfig.lpustop.clocksEnabled.xosc0,
            config->mcmeConfig.lpustop.clocksEnabled.xosc1);
    #endif
    #if defined(LPU_STANDBY_CF_FIRC_ON_MASK) || defined(LPU_STANDBY_CF_SIRC_ON_MASK) || defined(LPU_STANDBY_CF_FXOSC_ON_MASK) || defined(LPU_STANDBY_CF_SXOSC_ON_MASK)
    LPU_SetClockSourcesInLpuStandbyMode(LPU, (uint32_t)config->mcmeConfig.lpustandby.sysclk,
            config->mcmeConfig.lpustandby.clocksEnabled.irc0,
            config->mcmeConfig.lpustandby.clocksEnabled.irc1,
            config->mcmeConfig.lpustandby.clocksEnabled.xosc0,
            config->mcmeConfig.lpustandby.clocksEnabled.xosc1);
    #endif

    /* Clock sources are enabled after mode transition */
    MC_ME_ChangeMode(MC_ME);

    for (i= 0U; i < MC_ME_COMPLETE_TRANSITION_TIMEOUT; i++)
    {
        /* Check whether transition completed */
        if (MC_ME_GetTransitionStatus(MC_ME))
        {
            retValue = STATUS_SUCCESS;
            break;
        }
    }

    if (STATUS_SUCCESS == retValue)
    {
        /* Set run peripheral configurations */
        for (i = 0U; i < ((uint32_t)MC_ME_PERIPH_CONFIG_COUNT); i++)
        {
            MC_ME_SetRunPeripheralConfig(MC_ME, i,
                    config->mcmeConfig.periphRunConfig[i].reset,
                    config->mcmeConfig.periphRunConfig[i].safe,
                    config->mcmeConfig.periphRunConfig[i].test,
                    config->mcmeConfig.periphRunConfig[i].drun,
                    config->mcmeConfig.periphRunConfig[i].run0,
                    config->mcmeConfig.periphRunConfig[i].run1,
                    config->mcmeConfig.periphRunConfig[i].run2,
                    config->mcmeConfig.periphRunConfig[i].run3);
        }

        /* Set low peripheral configurations */
        for (i = 0U; i < ((uint32_t)MC_ME_PERIPH_CONFIG_COUNT); i++)
        {
            MC_ME_SetLowPeripheralConfig(MC_ME, i,
                    config->mcmeConfig.periphLowPowerConfig[i].stop0,
                    config->mcmeConfig.periphLowPowerConfig[i].standby0,
                    config->mcmeConfig.periphLowPowerConfig[i].halt0);
        }

        for (i = 0U; i < config->mcmeConfig.count; i++)
        {
            /* Set peripheral clock control */
            MC_ME_SetPeripheralClockControl(MC_ME,
                    config->mcmeConfig.peripherals[i].clockName,
                    (uint8_t) config->mcmeConfig.peripherals[i].mc_me_RunPeriphConfig,
                    (uint8_t) config->mcmeConfig.peripherals[i].mc_me_LowPowerPeriphConfig);
        }


        retValue = STATUS_TIMEOUT;

        /* Peripherals are clocked after mode transition */
        MC_ME_ChangeMode(MC_ME);


        for (i= 0U; i < MC_ME_COMPLETE_TRANSITION_TIMEOUT; i++)
        {
            /* Check whether transition completed */
            if (MC_ME_GetTransitionStatus(MC_ME))
            {
                retValue = STATUS_SUCCESS;
                break;
            }
        }
    }

    return retValue;
}


/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureCgm
 * Description   : Configures clock generation module
 *END**************************************************************************/
static status_t CLOCK_SYS_ConfigureCgm(clock_manager_user_config_t const* config)
{
    /* Configure system dividers */
    CLOCK_SYS_ConfigureSystemClockDividers(&config->cgmConfig);

    /* Configure auxiliary selector and dividers */
    CLOCK_SYS_ConfigureAuxiliarySelectorsAndDividers(&config->cgmConfig);

    /* Configure clock outputs. */
    CLOCK_SYS_ConfigureClockouts(&config->cgmConfig);

    return STATUS_SUCCESS;
}



/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_SetIrc
 * Description   : Configures IRC clock source
 *END**************************************************************************/
static void CLOCK_SYS_SetIrc(cgmcs_irc_config_t const* config, uint32_t instance)
{
    /* Configure divider */
    switch(instance)
    {
        case 0U:
#if FEATURE_HAS_SIRC_CLK
            CGMCS_SetSircDivider(SIRC,(uint32_t)config->divider);
#endif
            break;
        case 1U:
#if FEATURE_HAS_FIRC_CLK
            CGMCS_SetFircDivider(FIRC,(uint32_t)config->divider);
#endif
            break;
        default:
            /* 2 Invalid instance number. */
            DEV_ASSERT(false);
            break;
    }
#if FEATURE_HAS_IRCOSC_CLK
    CGMCS_SetIrcTrimmingValue(IRCOSC,(uint32_t)config->trimmingValue);
#endif
}


/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_SetXosc
 * Description   : Configures XOSC clock source
 *END**************************************************************************/
static void CLOCK_SYS_SetXosc(cgmcs_xosc_config_t const* config, uint32_t instance)
{
    /* Configure divider */
    switch(instance)
    {
        case 0U:
#if FEATURE_HAS_SXOSC_CLK
            CGMCS_SetSxosc(SXOSC,
                           config->autoLevelControl ? 1UL : 0UL,
                           (uint32_t)config->startupDelay,
                           (uint32_t)config->divider);
#endif
#if FEATURE_HAS_XOSC_CLK
    CGMCS_SetXosc(XOSC,
                   (uint32_t)config->bypassOption,
                   (uint32_t)config->startupDelay,
                   (uint32_t)config->mode,
                   config->monitor ? 1UL : 0UL);
#endif
#if (FEATURE_HAS_SXOSC_CLK != 0U) || (FEATURE_HAS_XOSC_CLK != 0U)
            g_xtal0ClkFreq = config->freq;
#endif
            break;
        case 1U:
#if FEATURE_HAS_FXOSC_CLK
            CGMCS_SetFxosc(FXOSC,
                           (uint32_t)config->bypassOption,
                           (uint32_t)config->startupDelay,
                           (uint32_t)config->mode,
                           (uint32_t)config->divider);
            g_xtal1ClkFreq = config->freq;
#endif
            break;
        default:
            /* 2 Invalid instance number. */
            DEV_ASSERT(false);
            break;
    }
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_SetPll
 * Description   : Configures ARM PLL clock source
 *END**************************************************************************/
static void CLOCK_SYS_SetPll(cgmcs_plldig_config_t const* config, cgm_config_t const* cgmConfig, uint32_t instance)
{
        uint32_t phi0Divider = 0U, phi1Divider = 0U;

        switch(instance)
        {
#if defined    (FEATURE_PLL0_INPUT_REFERENCE)
            case 0U:
#if FEATURE_PLL0_INPUT_REFERENCE == AC5__SC
                CGM_SetAC5(MC_CGM, (uint32_t)cgmConfig->ac5_sc);
#endif
#if FEATURE_PLL0_INPUT_REFERENCE == AC3__SC
                CGM_SetAC3(MC_CGM, (uint32_t)cgmConfig->ac3_sc);
#endif
#endif
            break;

#if defined    (FEATURE_PLL1_INPUT_REFERENCE)
            case 1U:
#if FEATURE_PLL1_INPUT_REFERENCE == AC4__SC
                CGM_SetAC4(MC_CGM, (uint32_t)cgmConfig->ac4_sc);
#endif
#endif
            break;

            default:
                /* Invalid PLL instance */
                DEV_ASSERT(false);
                break;
        }


#if FEATURE_PLL_REDUCED_FREQ_DIV_VERSION == 0U
        phi0Divider = ((uint32_t)config->phi0Divider) + 1U;
        phi1Divider = ((uint32_t)config->phi1Divider) + 1U;
#endif

#if FEATURE_PLL_REDUCED_FREQ_DIV_VERSION == 1U

          switch(config->phi0Divider)
          {
                case PLLDIG_PHI_DIV_BY_2:
                  phi0Divider = 0U;
                  break;

                case PLLDIG_PHI_DIV_BY_4:
                  phi0Divider = 1U;
                  break;

                case PLLDIG_PHI_DIV_BY_8:
                  phi0Divider = 2U;
                  break;

                case PLLDIG_PHI_DIV_BY_16:
                  phi0Divider = 3U;
                  break;

                case PLLDIG_PHI_DIV_BY_32:
                  phi0Divider = 4U;
                  break;

                default:
                    /* Invalid divider value */
                    DEV_ASSERT(false);
                    break;
          }


          switch(config->phi1Divider)
          {
                case PLLDIG_PHI_DIV_BY_2:
                  phi1Divider = 0U;
                  break;

                case PLLDIG_PHI_DIV_BY_4:
                  phi1Divider = 1U;
                  break;

                case PLLDIG_PHI_DIV_BY_8:
                  phi1Divider = 2U;
                  break;

                case PLLDIG_PHI_DIV_BY_16:
                  phi1Divider = 3U;
                  break;

                case PLLDIG_PHI_DIV_BY_32:
                  phi1Divider = 4U;
                  break;

                default:
                    /* Invalid divider value */
                    DEV_ASSERT(false);
                    break;
          }
#endif

      /* Configure PLL */
      CGMCS_PLLDivider(PLLDIG,
                         instance,
                         phi0Divider,
                         phi1Divider,
                         (uint32_t)config->predivider,
                         (uint32_t)config->mulFactorDiv);

      /* Write MFN - numerator for fractional loop divider. */
      CGMCS_SetPLLNumeratorFractionalLoopDivider(PLLDIG, (uint32_t)config->numeratorFracLoopDiv);

      /* Configure sigma delta modulation. */
      CGMCS_SetPLLSigmaDeltaModulation(PLLDIG, config->sigmaDelta, config->secondOrderSigmaDelta, config->thirdOrderSigmaDelta);

      /* Configure dither control. */
      CGMCS_SetPLLDitherControl(PLLDIG, config->ditherControl, (uint32_t)config->ditherControlValue);

      /* Write MFDEN - denominator fractional loop divider */
      CGMCS_SetPLLDenominatorFractionalLoopDivider(PLLDIG, (uint32_t)config->denominatorFracLoopDiv);

      /* Configure modulation */
      CGMCS_ConfigurePllModulation(PLLDIG, config->modulation, (uint32_t)config->modulationType, config->modulationPeriod, config->incrementStep);
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureSystemClockDividers
 * Description   : Configures system clocks
 *END**************************************************************************/
static void CLOCK_SYS_ConfigureSystemClockDividers(cgm_config_t const* config)
{
    /* First system clock divider */
    CGM_SetSC_DC0(MC_CGM,
            config->sc_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc0);

    /* Second system clock divider */
    CGM_SetSC_DC1(MC_CGM,
            config->sc_dc1 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc1);

    /* Third system clock divider */
    CGM_SetSC_DC2(MC_CGM,
            config->sc_dc2 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc2);

    /* 4th system clock divider */
    CGM_SetSC_DC3(MC_CGM,
            config->sc_dc3 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc3);

    /* 5th system clock divider */
    CGM_SetSC_DC4(MC_CGM,
            config->sc_dc4 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc4);

    /* 6th system clock divider */
    CGM_SetSC_DC5(MC_CGM,
            config->sc_dc5 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc5);

    /* 7th system clock divider */
    CGM_SetSC_DC6(MC_CGM,
            config->sc_dc6 != CGM_CLOCK_DISABLE,
            (uint32_t) config->sc_dc6);
}


/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureAuxiliarySelectorsAndDividers
 * Description   : Configures auxiliary clocks
 *END**************************************************************************/
static void CLOCK_SYS_ConfigureAuxiliarySelectorsAndDividers(cgm_config_t const* config)
{
    /* Configure auxiliary clock selector 0 and dividers 0, 1, 2. */
    CGM_SetAC0(MC_CGM, (uint32_t)config->ac0_sc);
    CGM_SetAC0_DC0(MC_CGM,
            config->ac0_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac0_dc0);
    CGM_SetAC0_DC1(MC_CGM,
            config->ac0_dc1 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac0_dc1);
    CGM_SetAC0_DC2(MC_CGM,
            config->ac0_dc2 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac0_dc2);

    /* Configure auxiliary clock selector 1 and dividers 0, 1. */
    CGM_SetAC1(MC_CGM, (uint32_t)config->ac1_sc);
    CGM_SetAC1_DC0(MC_CGM,
            config->ac1_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac1_dc0);
    CGM_SetAC1_DC1(MC_CGM,
            config->ac1_dc1 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac1_dc1);

    /* Configure auxiliary clock selector 2 and dividers 0. */
    CGM_SetAC2(MC_CGM, (uint32_t)config->ac2_sc);
    CGM_SetAC2_DC0(MC_CGM,
            config->ac2_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac2_dc0);

    /* Configure auxiliary clock selector 3 and dividers -. */
    CGM_SetAC3(MC_CGM, (uint32_t)config->ac3_sc);

    /* Configure auxiliary clock selector 4 and dividers -. */
    CGM_SetAC4(MC_CGM, (uint32_t)config->ac4_sc);

    /* Configure auxiliary clock selector 5 and dividers 0. */
    CGM_SetAC5(MC_CGM, (uint32_t)config->ac5_sc);
    CGM_SetAC5_DC0(MC_CGM,
            config->ac5_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac5_dc0);

    /* Configure auxiliary clock selector 6 and dividers 0. */
    CGM_SetAC6(MC_CGM, (uint32_t)config->ac6_sc);
    CGM_SetAC6_DC0(MC_CGM,
            config->ac6_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac6_dc0);

    /* Configure auxiliary clock selector 8 and dividers -. */
    CGM_SetAC8(MC_CGM, (uint32_t)config->ac8_sc);

    /* Configure auxiliary clock selector 9 and dividers -. */
    CGM_SetAC9(MC_CGM, (uint32_t)config->ac9_sc);

    /* Configure auxiliary clock selector 10 and dividers 0. */
    CGM_SetAC10(MC_CGM, (uint32_t)config->ac10_sc);
    CGM_SetAC10_DC0(MC_CGM,
            config->ac10_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac10_dc0);

    /* Configure auxiliary clock selector 11 and dividers 0. */
    CGM_SetAC11(MC_CGM, (uint32_t)config->ac11_sc);
    CGM_SetAC11_DC0(MC_CGM,
            config->ac11_dc0 != CGM_CLOCK_DISABLE,
            (uint32_t) config->ac11_dc0);
}

/*FUNCTION**********************************************************************
 * Function Name : CLOCK_SYS_ConfigureClockouts
 * Description   : Configures clock outs
 *END**************************************************************************/
static void CLOCK_SYS_ConfigureClockouts(cgm_config_t const* config)
{
    /* Configure clock out 0 selector and divider. */
    CGM_SetClkout0(MC_CGM,
            config->clkout0_dc != CGM_CLOCK_DISABLE,
            (uint32_t) config->clkout0_sc,
            (uint32_t) config->clkout0_dc);

    /* Configure clock out 1 selector and divider. */
    CGM_SetClkout1(MC_CGM,
            config->clkout1_dc != CGM_CLOCK_DISABLE,
            (uint32_t) config->clkout1_sc,
            (uint32_t) config->clkout1_dc);

}


/*! @}*/

/*******************************************************************************
 * EOF
 ******************************************************************************/

