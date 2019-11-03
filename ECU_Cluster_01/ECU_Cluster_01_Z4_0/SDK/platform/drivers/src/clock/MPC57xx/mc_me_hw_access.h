/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#if !defined(MC_ME_HW_ACCESS_H)
#define MC_ME_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file mc_me_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 */

/*!
 * @ingroup mc_me_hw_access
 * @defgroup mc_me_hw_access
 * @{
 */

/*! @brief Clock name mappings
 *         Constant array storing the mappings between clock names and peripheral clock control indexes.
 *         If there is no peripheral clock control index for a clock name, then the corresponding value is
 *         MC_ME_INVALID_INDEX.
 */
extern const uint16_t clockNameMappings[CLOCK_NAME_COUNT];

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#ifdef MC_ME_ME_TEST
#define TEST_MODE_VALUE 0x01
#endif

#ifdef MC_ME_ME_SAFE
#define SAFE_MODE_VALUE 0x02
#endif

#ifdef MC_ME_ME_DRUN
#define DRUN_MODE_VALUE 0x03
#endif

#ifdef MC_ME_ME_RUN0
#define RUN0_MODE_VALUE 0x04
#endif

#ifdef MC_ME_ME_RUN1
#define RUN1_MODE_VALUE 0x05
#endif

#ifdef MC_ME_ME_RUN2
#define RUN2_MODE_VALUE 0x06
#endif

#ifdef MC_ME_ME_RUN3
#define RUN3_MODE_VALUE 0x07
#endif

/*!
 * @brief MC_ME mode change.
 *
 * This function is changing mode in MC_ME module
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @param[in] mode Destination mode.
 */
static inline void MC_ME_ChangeMode(MC_ME_Type * base)
{
    uint32_t currentMode = (base->MCTL & MC_ME_MCTL_TARGET_MODE_MASK) >> MC_ME_MCTL_TARGET_MODE_SHIFT;

    /* Change takes effect after mode transition. */
    base->MCTL = MC_ME_MCTL_TARGET_MODE(currentMode) | MC_ME_MCTL_KEY(FEATURE_MC_ME_KEY);
    base->MCTL = MC_ME_MCTL_TARGET_MODE(currentMode) | MC_ME_MCTL_KEY(FEATURE_MC_ME_KEY_INV);
}

/*!
 * @brief MC_ME transition mode status.
 *
 * This function is returning the transition mode status (transition completed or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return transition status
 */
static inline bool MC_ME_GetTransitionStatus(const MC_ME_Type * base)
{
        return (base->GS & MC_ME_GS_S_MTRANS_MASK) == 0U;
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME
 *
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 * @param[in] modeRegister  register to be read/written
 */
static void MC_ME_SetClockSources(uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1, volatile uint32_t *modeRegister)
{
    uint32_t regValue = *modeRegister;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;

    regValue &= ~(MC_ME_GS_S_SYSCLK_MASK);

#ifdef MC_ME_GS_S_FIRCON_MASK
    regValue &= ~(MC_ME_GS_S_FIRCON_MASK);
#endif
#ifdef MC_ME_GS_S_SIRCON_MASK
    regValue &= ~(MC_ME_GS_S_SIRCON_MASK);
#endif
#ifdef MC_ME_GS_S_IRC_MASK
    regValue &= ~(MC_ME_GS_S_IRC_MASK);
#endif

#ifdef MC_ME_GS_S_SXOSC_MASK
    regValue &= ~(MC_ME_GS_S_SXOSC_MASK);
#endif
#ifdef MC_ME_GS_S_FXOSC_MASK
    regValue &= ~(MC_ME_GS_S_FXOSC_MASK);
#endif
#ifdef MC_ME_GS_S_XOSC_MASK
    regValue &= ~(MC_ME_GS_S_XOSC_MASK);
#endif

#ifdef MC_ME_GS_S_PLL0_MASK
    regValue &= ~(MC_ME_GS_S_PLL0_MASK);
#endif
#ifdef MC_ME_GS_S_PLL1_MASK
    regValue &= ~(MC_ME_GS_S_PLL1_MASK);
#endif
#ifdef MC_ME_GS_S_PLLON_MASK
    regValue &= ~(MC_ME_GS_S_PLLON_MASK);
#endif

    regValue |= MC_ME_GS_S_SYSCLK(sysClk);


#ifdef MC_ME_GS_S_SIRCON
    regValue |= MC_ME_GS_S_SIRCON(irc0 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_FIRCON
    regValue |= MC_ME_GS_S_FIRCON(irc1 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_IRC
    regValue |= MC_ME_GS_S_IRC(irc0 ? 1UL : 0UL);
#endif

#ifdef MC_ME_GS_S_SXOSC
    regValue |= MC_ME_GS_S_SXOSC(xosc0 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_FXOSC
    regValue |= MC_ME_GS_S_FXOSC(xosc1 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_XOSC
    regValue |= MC_ME_GS_S_XOSC(xosc0 ? 1UL : 0UL);
#endif

#ifdef MC_ME_GS_S_PLL0
    regValue |= MC_ME_GS_S_PLL0(pll0 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_PLL1
    regValue |= MC_ME_GS_S_PLL1(pll1 ? 1UL : 0UL);
#endif
#ifdef MC_ME_GS_S_PLLON
    regValue |= MC_ME_GS_S_PLLON(pll0 ? 1UL : 0UL);
#endif

    *modeRegister = regValue;
}






/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in drun mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInDrunMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_DRUN
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->DRUN_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in run0 mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInRun0Mode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_RUN0
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->RUN0_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in run1 mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInRun1Mode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_RUN1
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->RUN1_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in run2 mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInRun2Mode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_RUN2
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->RUN2_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in run3 mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInRun3Mode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_RUN3
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->RUN3_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in safe mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInSafeMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_SAFE
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->SAFE_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in safe mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInTestMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#ifdef MC_ME_ME_TEST
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->TEST_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in stop mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */

static inline void MC_ME_SetClockSourcesInStopMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#if defined(MC_ME_STOP_MC_SYSCLK)
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->STOP_MC);
#elif defined(MC_ME_STOP0_MC_SYSCLK)
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->STOP0_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}


/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in standby mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInStandbyMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#if defined(MC_ME_STANDBY_MC_SYSCLK)
	MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->STANDBY_MC);
#elif defined(MC_ME_STANDBY0_MC_SYSCLK)
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->STANDBY0_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

/*!
 * @brief Enables clock sources from MC_ME
 *
 * This function enables/disables all clock sources from MC_ME in standby mode
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void MC_ME_SetClockSourcesInHaltMode(MC_ME_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1, bool pll0, bool pll1)
{
#if defined(MC_ME_HALT_MC_SYSCLK)
	MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->HALT_MC);
#elif defined(MC_ME_HALT0_MC_SYSCLK)
    MC_ME_SetClockSources(sysClk,irc0,irc1,xosc0,xosc1,pll0,pll1,&base->HALT0_MC);
#else
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;
    (void)pll0;
    (void)pll1;
#endif
}

#ifdef LPU_RUN_CF_SYS_CLK_SEL
/*!
 * @brief Enables clock sources from LPU
 *
 * This function enables/disables all clock sources from LPU in lpuDrun mode
 *
 * @param[in] base   Register base address for the LPU instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void LPU_SetClockSourcesInLpuDrunMode(LPU_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1)
{
	uint32_t regValue;
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;

	regValue = base->RUN_CF;
    regValue &= ~(LPU_RUN_CF_SYS_CLK_SEL_MASK);
#ifdef LPU_RUN_CF_FIRC_ON_MASK
    regValue &= ~(LPU_RUN_CF_FIRC_ON_MASK);
#endif
#ifdef LPU_RUN_CF_SIRC_ON_MASK
    regValue &= ~(LPU_RUN_CF_SIRC_ON_MASK);
#endif
#ifdef LPU_RUN_CF_FXOSC_ON_MASK
    regValue &= ~(LPU_RUN_CF_FXOSC_ON_MASK);
#endif
#ifdef LPU_RUN_CF_SXOSC_ON_MASK
    regValue &= ~(LPU_RUN_CF_SXOSC_ON_MASK);
#endif

    regValue |= LPU_RUN_CF_SYS_CLK_SEL(sysClk);

#ifdef LPU_RUN_CF_SIRC_ON
    regValue |= LPU_RUN_CF_SIRC_ON(irc0 ? 1UL : 0UL);
#endif
#ifdef LPU_RUN_CF_FIRC_ON_MASK
    regValue |= LPU_RUN_CF_FIRC_ON(irc1 ? 1UL : 0UL);
#endif
#ifdef LPU_RUN_CF_SXOSC_ON
    regValue |= LPU_RUN_CF_SXOSC_ON(xosc0 ? 1UL : 0UL);
#endif
#ifdef LPU_RUN_CF_FXOSC_ON
    regValue |= LPU_RUN_CF_FXOSC_ON(xosc1 ? 1UL : 0UL);
#endif

    base->RUN_CF = regValue;
}
#endif

#if defined(LPU_STOP_CF_FIRC_ON_MASK) || defined(LPU_STOP_CF_SIRC_ON_MASK) || defined(LPU_STOP_CF_FXOSC_ON_MASK) || defined(LPU_STOP_CF_SXOSC_ON_MASK)
/*!
 * @brief Enables clock sources from LPU
 *
 * This function enables/disables all clock sources from LPU in lpuStop mode
 *
 * @param[in] base   Register base address for the LPU instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void LPU_SetClockSourcesInLpuStopMode(LPU_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1)
{
	uint32_t regValue;
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;

	regValue = base->STOP_CF;
#ifdef LPU_STOP_CF_FIRC_ON_MASK
    regValue &= ~(LPU_STOP_CF_FIRC_ON_MASK);
#endif
#ifdef LPU_STOP_CF_SIRC_ON_MASK
    regValue &= ~(LPU_STOP_CF_SIRC_ON_MASK);
#endif
#ifdef LPU_STOP_CF_FXOSC_ON_MASK
    regValue &= ~(LPU_STOP_CF_FXOSC_ON_MASK);
#endif
#ifdef LPU_STOP_CF_SXOSC_ON_MASK
    regValue &= ~(LPU_STOP_CF_SXOSC_ON_MASK);
#endif

#ifdef LPU_STOP_CF_FIRC_ON
    regValue |= LPU_STOP_CF_FIRC_ON(irc0 ? 1UL : 0UL);
#endif
#ifdef LPU_STOP_CF_SIRC_ON
    regValue |= LPU_STOP_CF_SIRC_ON(irc1 ? 1UL : 0UL);
#endif
#ifdef LPU_STOP_CF_SXOSC_ON
    regValue |= LPU_STOP_CF_SXOSC_ON(xosc0 ? 1UL : 0UL);
#endif
#ifdef LPU_STOP_CF_FXOSC_ON
    regValue |= LPU_STOP_CF_FXOSC_ON(xosc1 ? 1UL : 0UL);
#endif

    base->STOP_CF = regValue;
}
#endif

#if defined(LPU_STANDBY_CF_FIRC_ON_MASK) || defined(LPU_STANDBY_CF_SIRC_ON_MASK) || defined(LPU_STANDBY_CF_FXOSC_ON_MASK) || defined(LPU_STANDBY_CF_SXOSC_ON_MASK)
/*!
 * @brief Enables clock sources from LPU
 *
 * This function enables/disables all clock sources from LPU in lpuStandby mode
 *
 * @param[in] base   Register base address for the LPU instance.
 * @param[in] sysClk Destination mode.
 * @param[in] irc0   First IRCOSC clock source.
 * @param[in] irc1   Second IRCOSC clock source.
 * @param[in] xosc0  First XOSC clock source.
 * @param[in] xosc1  Second XOSC clock source.
 * @param[in] pll0   First PLL clock source.
 * @param[in] pll1   Second PLL clock source.
 */
static inline void LPU_SetClockSourcesInLpuStandbyMode(LPU_Type * base, uint32_t sysClk, bool irc0, bool irc1, bool xosc0, bool xosc1)
{
    uint32_t regValue;
    (void)base;
    (void)sysClk;
    (void)irc0;
    (void)irc1;
    (void)xosc0;
    (void)xosc1;

    regValue = base->STANDBY_CF;
#ifdef LPU_STANDBY_CF_FIRC_ON_MASK
    regValue &= ~(LPU_STANDBY_CF_FIRC_ON_MASK);
#endif
#ifdef LPU_STANDBY_CF_SIRC_ON_MASK
    regValue &= ~(LPU_STANDBY_CF_SIRC_ON_MASK);
#endif
#ifdef LPU_STANDBY_CF_FXOSC_ON_MASK
    regValue &= ~(LPU_STANDBY_CF_FXOSC_ON_MASK);
#endif
#ifdef LPU_STANDBY_CF_SXOSC_ON_MASK
    regValue &= ~(LPU_STANDBY_CF_SXOSC_ON_MASK);
#endif

#ifdef LPU_STANDBY_CF_FIRC_ON
    regValue |= LPU_STANDBY_CF_FIRC_ON(irc0 ? 1UL : 0UL);
#endif
#ifdef LPU_STANDBY_CF_SIRC_ON
    regValue |= LPU_STANDBY_CF_SIRC_ON(irc1 ? 1UL : 0UL);
#endif
#ifdef LPU_STANDBY_CF_SXOSC_ON
    regValue |= LPU_STANDBY_CF_SXOSC_ON(xosc0 ? 1UL : 0UL);
#endif
#ifdef LPU_STANDBY_CF_FXOSC_ON
    regValue |= LPU_STANDBY_CF_FXOSC_ON(xosc1 ? 1UL : 0UL);
#endif

    base->STANDBY_CF = regValue;
}
#endif

/*!
 * @brief Gets current system clock
 *
 * This function gets current system clock
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return system clock
 */
static inline uint32_t MC_ME_GetCurrentSystemClock(const MC_ME_Type * base)
{
     /* Get System Clock from CGM */
    return (base->GS & MC_ME_GS_S_SYSCLK_MASK) >> MC_ME_GS_S_SYSCLK_SHIFT;
}

/*!
 * @brief Gets FIRC status
 *
 * This function gets FIRC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetFircStatus(const MC_ME_Type * base)
{
     /* Get System Clock from CGM */
#ifdef MC_ME_GS_S_FIRCON_MASK
    return (base->GS & MC_ME_GS_S_FIRCON_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets SIRC status
 *
 * This function gets SIRC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetSircStatus(const MC_ME_Type * base)
{
     /* Get System Clock from CGM */
#ifdef MC_ME_GS_S_SIRCON_MASK
    return (base->GS & MC_ME_GS_S_SIRCON_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets IRC status
 *
 * This function gets IRC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetIrcStatus(const MC_ME_Type * base)
{
     /* Get System Clock from CGM */
#ifdef MC_ME_GS_S_IRC_MASK
    return (base->GS & MC_ME_GS_S_IRC_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}


/*!
 * @brief Gets FXOSC status
 *
 * This function gets FXOSC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetFxoscStatus(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_FXOSC_MASK
    return (base->GS & MC_ME_GS_S_FXOSC_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets SXOSC status
 *
 * This function gets SXOSC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetSxoscStatus(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_SXOSC_MASK
    return (base->GS & MC_ME_GS_S_SXOSC_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets XOSC status
 *
 * This function gets XOSC clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetXoscStatus(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_XOSC_MASK
    return (base->GS & MC_ME_GS_S_XOSC_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets PLL status
 *
 * This function gets PLL clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetPllStatus(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_PLLON_MASK
    return (base->GS & MC_ME_GS_S_PLLON_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets PLL0 status
 *
 * This function gets PLL clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetPll0Status(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_PLL0_MASK
    return (base->GS & MC_ME_GS_S_PLL0_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}

/*!
 * @brief Gets PLL1 status
 *
 * This function gets PLL1 clock source status (enable or not)
 *
 * @param[in] base Register base address for the MC_ME instance.
 * @return clock source is enabled or not
 */
static inline bool MC_ME_GetPll1Status(const MC_ME_Type * base)
{
#ifdef MC_ME_GS_S_PLL1_MASK
    return (base->GS & MC_ME_GS_S_PLL1_MASK) != 0U;
#else
    (void)base;
    return false;
#endif
}


/*!
 * @brief Set run periheral configuration
 *
 * This function sets run periheral configuration
 *
 * @param[in] base Register base address for the MC_ME instance.
 */
static inline void MC_ME_SetRunPeripheralConfig(MC_ME_Type * base, uint32_t configIndex, bool reset, bool safe, bool test, bool drun, bool run0, bool run1, bool run2, bool run3)
{
    uint32_t value = 0U;

#ifdef MC_ME_ME_DRUN
    value |= MC_ME_ME_DRUN(drun ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_RUN0
    value |= MC_ME_ME_RUN0(run0 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_RUN1
    value |= MC_ME_ME_RUN1(run1 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_RUN2
    value |= MC_ME_ME_RUN2(run2 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_RUN3
    value |= MC_ME_ME_RUN3(run3 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_SAFE
    value |= MC_ME_ME_SAFE(safe ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_TEST
    value |= MC_ME_ME_TEST(test ? 1UL : 0UL);
#else
    (void)test;
#endif

#ifdef MC_ME_ME_RESET_DEST
    value |= MC_ME_ME_RESET_DEST(reset ? 1UL : 0UL);
#endif

    base->RUN_PC[configIndex] = value;
}

/*!
 * @brief Set low periheral configuration
 *
 * This function sets low periheral configuration
 *
 * @param[in] base Register base address for the MC_ME instance.
 */
static inline void MC_ME_SetLowPeripheralConfig(MC_ME_Type * base, uint32_t configIndex, bool stop0, bool standby0, bool halt0)
{
    uint32_t value = 0U;

#ifdef MC_ME_ME_STOP0
    (void)standby0;
    (void)halt0;
    value |= MC_ME_ME_STOP0(stop0 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_STANDBY0
    (void)stop0;
    (void)halt0;
    value |= MC_ME_ME_STANDBY0(stop0 ? 1UL : 0UL);
#endif

#ifdef MC_ME_ME_HALT0
    (void)stop0;
    (void)standby0;
    value |= MC_ME_ME_HALT0(halt0 ? 1UL : 0UL);
#endif

    base->LP_PC[configIndex] = value;
}

/*!
* @brief Sets Peripheral clock control register
*
* @param[in] base             mc_me base pointer
* @param[in] clockName        clock name
* @param[in] runConfigIndex   running configuration index
* @param[in] lowConfigIndex   low configuration index
*/
static inline void MC_ME_SetPeripheralClockControl(MC_ME_Type* base, clock_names_t clockName, uint8_t runConfigIndex, uint8_t lowConfigIndex)
{
   /* Configure the low and run configurations */
   if (clockNameMappings[clockName] != MC_ME_INVALID_INDEX)
   {
	   base->PCTLn[clockNameMappings[clockName]] = (MC_ME_PCTLn_RUN_CFG(runConfigIndex) | MC_ME_PCTLn_LP_CFG(lowConfigIndex));
   }
}

/*!
* @brief Sets Peripheral clock control register
*
* @param[in] base             mc_me base pointer
* @param[in] clockName        clock name
* @return module is enabled or not
*/
static inline bool MC_ME_GetModuleStatus(const MC_ME_Type* base, clock_names_t clockName)
{
  bool retValue = false;
  uint32_t currentMode = (base->MCTL & MC_ME_MCTL_TARGET_MODE_MASK) >> MC_ME_MCTL_TARGET_MODE_SHIFT;
  uint8_t moduleRunConfigurationIndex = 0U;
  uint32_t moduleRunConfiguration = 0U;

  /* Check whether this module supports clock gating in MC_ME */
  if (clockNameMappings[clockName] == MC_ME_INVALID_INDEX)
  {
      retValue = true;
  }
  else
  {
      moduleRunConfigurationIndex = (base->PCTLn[clockNameMappings[clockName]] & MC_ME_PCTLn_RUN_CFG_MASK) >> MC_ME_PCTLn_RUN_CFG_SHIFT;
      moduleRunConfiguration = base->RUN_PC[moduleRunConfigurationIndex];
  }

  switch(currentMode)
  {
#ifdef MC_ME_ME_TEST
      case TEST_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_TEST_MASK) >> MC_ME_RUN_PC_TEST_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_SAFE
      case SAFE_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_SAFE_MASK) >> MC_ME_RUN_PC_SAFE_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_DRUN
      case DRUN_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_DRUN_MASK) >> MC_ME_RUN_PC_DRUN_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_RUN0
      case RUN0_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_RUN0_MASK) >> MC_ME_RUN_PC_RUN0_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_RUN1
      case RUN1_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_RUN1_MASK) >> MC_ME_RUN_PC_RUN1_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_RUN2
      case RUN2_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_RUN2_MASK) >> MC_ME_RUN_PC_RUN2_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
#ifdef MC_ME_ME_RUN3
      case RUN3_MODE_VALUE:
          if (((moduleRunConfiguration & MC_ME_RUN_PC_RUN3_MASK) >> MC_ME_RUN_PC_RUN3_SHIFT) != 0u)
          {
              retValue = true;
          }
          break;
#endif
      default:
          /* Invalid power mode */
          DEV_ASSERT(false);
          break;
  }

  return retValue;
}


/*!
 * @brief Resets configuration of clock sources in MC_ME.
 *
 * Resets configuration of clock sources in MC_ME. Enables IRC type clock sources and sets an IRC type clock source as system clock in all modes.
 *
 * @param[in] base   Register base address for the MC_ME instance.
 * @param[in] sysClk Destination mode. */
static inline void MC_ME_ResetClockSourcesConfiguration(MC_ME_Type * base)
{
   /* Enables IRC sources. Disables XOSC and PLL sources. Sets IRC source as system clock */
#ifdef MC_ME_ME_DRUN
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->DRUN_MC);
#endif
#ifdef MC_ME_ME_RUN0
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->RUN0_MC);
#endif
#ifdef MC_ME_ME_RUN1
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->RUN1_MC);
#endif
#ifdef MC_ME_ME_RUN2
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->RUN2_MC);
#endif
#ifdef MC_ME_ME_RUN3
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->RUN3_MC);
#endif
#ifdef MC_ME_ME_SAFE
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->SAFE_MC);
#endif
#ifdef MC_ME_ME_TEST
    MC_ME_SetClockSources(0U,true,true,false,false,false,false,&base->TEST_MC);
#endif
}

/*!
 * @brief Resets configuration of peripherals in MC_ME.
 *
 * Resets configuration of peripherals in MC_ME. Disables module clocks in all modes.
 *
 * @param[in] base   Register base address for the MC_ME instance. */
static inline void MC_ME_ResetPeripheralsConfiguration(MC_ME_Type * base, uint32_t configsNo)
{
    uint32_t i;

	/* Reset run peripheral configurations */
    for (i = 0U; i < configsNo; i++)
    {
        MC_ME_SetRunPeripheralConfig(base,i,false,false,false,false,false,false,false,false);
    }

    /* Reset low peripheral configurations */
    for (i = 0U; i < configsNo; i++)
    {
        MC_ME_SetLowPeripheralConfig(base,i,false,false,false);
    }

    /* Reset run and low peripheral indexes */
	#if defined(MC_ME_PCTL_ADC_0_INDEX)
	base->PCTLn[MC_ME_PCTL_ADC_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ADC_1_INDEX)
	base->PCTLn[MC_ME_PCTL_ADC_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ADC_2_INDEX)
	base->PCTLn[MC_ME_PCTL_ADC_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ADC_3_INDEX)
	base->PCTLn[MC_ME_PCTL_ADC_3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_BCTU_INDEX)
	base->PCTLn[MC_ME_PCTL_BCTU_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CAN_0_INDEX)
	base->PCTLn[MC_ME_PCTL_CAN_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CAN_1_INDEX)
	base->PCTLn[MC_ME_PCTL_CAN_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CAN_2_INDEX)
	base->PCTLn[MC_ME_PCTL_CAN_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CMP_0_INDEX)
	base->PCTLn[MC_ME_PCTL_CMP_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CMP_1_INDEX)
	base->PCTLn[MC_ME_PCTL_CMP_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CMP_2_INDEX)
	base->PCTLn[MC_ME_PCTL_CMP_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CRC_0_INDEX)
	base->PCTLn[MC_ME_PCTL_CRC_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CRC_1_INDEX)
	base->PCTLn[MC_ME_PCTL_CRC_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CRC_INDEX)
	base->PCTLn[MC_ME_PCTL_CRC_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CTU_0_INDEX)
	base->PCTLn[MC_ME_PCTL_CTU_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_CTU_1_INDEX)
	base->PCTLn[MC_ME_PCTL_CTU_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DMAMUX_0_INDEX)
	base->PCTLn[MC_ME_PCTL_DMAMUX_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DMAMUX_1_INDEX)
	base->PCTLn[MC_ME_PCTL_DMAMUX_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DMAUX_INDEX)
	base->PCTLn[MC_ME_PCTL_DMAUX_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DSPI_0_INDEX)
	base->PCTLn[MC_ME_PCTL_DSPI_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DSPI_1_INDEX)
	base->PCTLn[MC_ME_PCTL_DSPI_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DSPI_2_INDEX)
	base->PCTLn[MC_ME_PCTL_DSPI_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DSPI_3_INDEX)
	base->PCTLn[MC_ME_PCTL_DSPI_3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_DTS_INDEX)
	base->PCTLn[MC_ME_PCTL_DTS_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_eMIOS_0_INDEX)
	base->PCTLn[MC_ME_PCTL_eMIOS_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_eMIOS_1_INDEX)
	base->PCTLn[MC_ME_PCTL_eMIOS_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_eMIOS_2_INDEX)
	base->PCTLn[MC_ME_PCTL_eMIOS_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ENET_0_INDEX)
	base->PCTLn[MC_ME_PCTL_ENET_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ENET_INDEX)
	base->PCTLn[MC_ME_PCTL_ENET_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ETIMER_0_INDEX)
	base->PCTLn[MC_ME_PCTL_ETIMER_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ETIMER_1_INDEX)
	base->PCTLn[MC_ME_PCTL_ETIMER_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_ETIMER_2_INDEX)
	base->PCTLn[MC_ME_PCTL_ETIMER_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_0_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_1_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_2_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_3_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_4_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_4_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_5_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_5_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_6_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_6_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXCAN_7_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXCAN_7_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_Flexray_INDEX)
	base->PCTLn[MC_ME_PCTL_Flexray_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FLEXRAY_INDEX)
	base->PCTLn[MC_ME_PCTL_FLEXRAY_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_FlexRay_INDEX)
	base->PCTLn[MC_ME_PCTL_FlexRay_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_IIC_0_INDEX)
	base->PCTLn[MC_ME_PCTL_IIC_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_IIC_1_INDEX)
	base->PCTLn[MC_ME_PCTL_IIC_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_IIC_2_INDEX)
	base->PCTLn[MC_ME_PCTL_IIC_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_IIC_3_INDEX)
	base->PCTLn[MC_ME_PCTL_IIC_3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_JDC_INDEX)
	base->PCTLn[MC_ME_PCTL_JDC_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_JTAGM_INDEX)
	base->PCTLn[MC_ME_PCTL_JTAGM_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LFAST_0_INDEX)
	base->PCTLn[MC_ME_PCTL_LFAST_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_0_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_1_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_10_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_10_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_11_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_11_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_12_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_12_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_13_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_13_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_14_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_14_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_15_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_15_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_16_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_16_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_17_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_17_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_2_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_3_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_4_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_4_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_5_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_5_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_6_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_6_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_7_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_7_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_8_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_8_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LIN_9_INDEX)
	base->PCTLn[MC_ME_PCTL_LIN_9_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LINFlex_0_INDEX)
	base->PCTLn[MC_ME_PCTL_LINFlex_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_LINFlex_1_INDEX)
	base->PCTLn[MC_ME_PCTL_LINFlex_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_MEMU_0_INDEX)
	base->PCTLn[MC_ME_PCTL_MEMU_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_MEMU_1_INDEX)
	base->PCTLn[MC_ME_PCTL_MEMU_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_MIPI_INDEX)
	base->PCTLn[MC_ME_PCTL_MIPI_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_MLB_INDEX)
	base->PCTLn[MC_ME_PCTL_MLB_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_PIT_0_INDEX)
	base->PCTLn[MC_ME_PCTL_PIT_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_PIT_1_INDEX)
	base->PCTLn[MC_ME_PCTL_PIT_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_PIT_RTI_0_INDEX)
	base->PCTLn[MC_ME_PCTL_PIT_RTI_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_PWM_0_INDEX)
	base->PCTLn[MC_ME_PCTL_PWM_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_PWM_1_INDEX)
	base->PCTLn[MC_ME_PCTL_PWM_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_RADAR_CTE_INDEX)
	base->PCTLn[MC_ME_PCTL_RADAR_CTE_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_RTC_API_INDEX)
	base->PCTLn[MC_ME_PCTL_RTC_API_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SAI0_INDEX)
	base->PCTLn[MC_ME_PCTL_SAI0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SAI1_INDEX)
	base->PCTLn[MC_ME_PCTL_SAI1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SAI2_INDEX)
	base->PCTLn[MC_ME_PCTL_SAI2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SDHC_INDEX)
	base->PCTLn[MC_ME_PCTL_SDHC_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SENT_0_INDEX)
	base->PCTLn[MC_ME_PCTL_SENT_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SENT_1_INDEX)
	base->PCTLn[MC_ME_PCTL_SENT_1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SGEN_0_INDEX)
	base->PCTLn[MC_ME_PCTL_SGEN_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SIPI_0_INDEX)
	base->PCTLn[MC_ME_PCTL_SIPI_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI0_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI1_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI1_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI2_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI2_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI3_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI3_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI4_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI4_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPI5_INDEX)
	base->PCTLn[MC_ME_PCTL_SPI5_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_SPT_INDEX)
	base->PCTLn[MC_ME_PCTL_SPT_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_USB_OTG_INDEX)
	base->PCTLn[MC_ME_PCTL_USB_OTG_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_USB_SPH_INDEX)
	base->PCTLn[MC_ME_PCTL_USB_SPH_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_WGM_0_INDEX)
	base->PCTLn[MC_ME_PCTL_WGM_0_INDEX] = 0U;
	#endif

	#if defined(MC_ME_PCTL_WKPU_INDEX)
	base->PCTLn[MC_ME_PCTL_WKPU_INDEX] = 0U;
	#endif
}


#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* MC_ME_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
