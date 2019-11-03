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

#if !defined(CGMCS_HW_ACCESS_H)
#define CGMCS_HW_ACCESS_H

#include "device_registers.h"
#include <stdbool.h>
#include <stddef.h>

/*!
 * @file cgmcs_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 */

/*!
 * @ingroup cgmcs_hw_access
 * @defgroup cgmcs_hw_access
 * @{
 */


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/


#ifdef FIRC
/*!
 * @brief Gets the FIRC clock source divider ratio.
 *
 * This function gets the FIRC clock source divider ratio.
 *
 * @param[in] base Register base address for the FIRC instance.
 * @return FIRC divider ratio
 */
static inline uint32_t CGMCS_GetFircDividerRatio(const FIRC_Type * base)
{
    return ((base->CTL & FIRC_CTL_FIRCDIV_MASK) >> FIRC_CTL_FIRCDIV_SHIFT);
}
#endif

#ifdef FXOSC
/*!
 * @brief Gets the FXOSC clock source divider ratio.
 *
 * This function gets the FXOSC clock source divider ratio.
 *
 * @param[in] base Register base address for the FXOSC instance.
 * @return FXOSC divider ratio
 */
static inline uint32_t CGMCS_GetFxoscDividerRatio(const FXOSC_Type * base)
{
    return ((base->CTL & FXOSC_CTL_OSCDIV_MASK) >> FXOSC_CTL_OSCDIV_SHIFT);
}
#endif

#ifdef SIRC
/*!
 * @brief Gets the SIRC clock source divider ratio.
 *
 * This function gets the SIRC clock source divider ratio.
 *
 * @param[in] base Register base address for the SIRC instance.
 * @return SIRC divider ratio
 */
static inline uint32_t CGMCS_GetSircDividerRatio(const SIRC_Type * base)
{
    return ((base->CTL & SIRC_CTL_SIRCDIV_MASK) >> SIRC_CTL_SIRCDIV_SHIFT);
}
#endif

#ifdef SXOSC
/*!
 * @brief Gets the SXOSC clock source divider ratio.
 *
 * This function gets the SXOSC clock source divider ratio.
 *
 * @param[in] base Register base address for the SXOSC instance.
 * @return SXOSC divider ratio
 */
static inline uint32_t CGMCS_GetSxoscDividerRatio(const SXOSC_Type * base)
{
    return ((base->CTL & SXOSC_CTL_OSCDIV_MASK) >> SXOSC_CTL_OSCDIV_SHIFT);
}
#endif

#ifdef SIRC
/*!
 * @brief Sets IRC divider
 *
 * This function sets IRC divider
 *
 * @param[in] base             Register base address for the SIRC instance.
 * @param[in] sircDivider      SIRC divider value
 */
static inline void CGMCS_SetSircDivider(SIRC_Type * base, uint32_t sircDivider)
{
#if defined(SIRC_CTL_SIRCDIV)
    uint32_t value = (uint32_t)SIRC_CTL_SIRCDIV(sircDivider);
    base->CTL = value;
#endif
}
#endif

#ifdef FIRC
/*!
 * @brief Sets FIRC clock source
 *
 * This function sets FIRC clock source
 *
 * @param[in] base             Register base address for the FIRC instance.
 * @param[in] fircDivider      FIRC divider value
 */
static inline void CGMCS_SetFircDivider(FIRC_Type * base, uint32_t fircDivider)
{
#if defined(SIRC_CTL_SIRCDIV)
    uint32_t value = (uint32_t)FIRC_CTL_FIRCDIV(fircDivider);
    base->CTL = value;
#endif
}
#endif

#ifdef IRCOSC
/*!
 * @brief Sets FIRC clock source
 *
 * This function sets FIRC clock source
 *
 * @param[in] base             Register base address for the FIRC instance.
 * @param[in] fircDivider      FIRC divider value
 */
static inline void CGMCS_SetIrcTrimmingValue(IRCOSC_Type * base, uint32_t trimmingValue)
{
#if defined(IRCOSC_CTL_USER_TRIM)
    uint32_t value = (uint32_t)IRCOSC_CTL_USER_TRIM(trimmingValue);
    base->CTL = value;
#endif
}
#endif

#ifdef SXOSC
/*!
 * @brief Sets SXOSC clock source
 *
 * This function sets SXOSC clock source
 *
 * @param[in] base             Register base address for the SXOSC instance.
 * @param[in] autoLevelControl Auto level control
 * @param[in] startupDelay     Startup delay
 * @param[in] sxoscDivider     Sxosc divider value
 */
static inline void CGMCS_SetSxosc(SXOSC_Type * base, uint32_t autoLevelControl, uint32_t startupDelay, uint32_t xoscDivider)
{
#if defined(SXOSC_CTL_ALC) && defined(SXOSC_CTL_EOCV) && defined(SXOSC_CTL_OSCDIV)
    uint32_t value = (uint32_t)(SXOSC_CTL_ALC(autoLevelControl)     |
                                SXOSC_CTL_EOCV(startupDelay)        |
                                SXOSC_CTL_OSCDIV(xoscDivider) );
    base->CTL = value;
#endif
}
#endif

#ifdef FXOSC
/*!
 * @brief Sets FXOSC clock source
 *
 * This function sets FXOSC clock source
 *
 * @param[in] base             Register base address for the FXOSC instance.
 * @param[in] autoLevelControl Auto level control
 * @param[in] startupDelay     Startup delay
 * @param[in] fxoscDivider     Fxosc divider value
 */
static inline void CGMCS_SetFxosc(FXOSC_Type * base, uint32_t bypassOption, uint32_t startupDelay, uint32_t mode, uint32_t xoscDivider)
{
#if defined(FXOSC_CTL_OSCBYP) && defined(FXOSC_CTL_EOCV) && defined(FXOSC_CTL_OSCM) && defined(FXOSC_CTL_OSCDIV)
    uint32_t value = (uint32_t)(FXOSC_CTL_OSCBYP(bypassOption)            |
                                FXOSC_CTL_EOCV(startupDelay)              |
                                FXOSC_CTL_OSCM(mode)                      |
                                FXOSC_CTL_OSCDIV(xoscDivider));
    base->CTL = value;
#endif
}
#endif

#ifdef XOSC
/*!
 * @brief Sets XOSC clock source
 *
 * This function sets XOSC clock source
 *
 * @param[in] base             Register base address for the FXOSC instance.
 * @param[in] autoLevelControl Auto level control
 * @param[in] startupDelay     Startup delay
 * @param[in] fxoscDivider     Fxosc divider value
 */
static inline void CGMCS_SetXosc(XOSC_Type * base, uint32_t bypassOption, uint32_t startupDelay, uint32_t mode, uint32_t monitor)
{
#if defined(XOSC_CTL_OSCBYP) && defined(XOSC_CTL_EOCV) && defined(XOSC_CTL_OSCM) && defined(XOSC_CTL_MON)
    uint32_t value = (uint32_t)(XOSC_CTL_OSCBYP(bypassOption)            |
                                XOSC_CTL_EOCV(startupDelay)              |
                                XOSC_CTL_MON(monitor)                    |
                                XOSC_CTL_OSCM(mode));
    base->CTL = value;
#endif
}
#endif

/*!
 * @brief Sets PLL Divider Register
 *
 * This function sets PLL Divider Register
 *
 * @param[in] base             Register base address for the PLLDIG instance.
 * @param[in] instance         Instance number
 * @param[in] phi0Divider      first output divider
 * @param[in] phi1Divider      second output divider
 * @param[in] predivider       Predivider
 * @param[in] mulFactorDiv     Multiplier factor divider
 */
static inline void CGMCS_PLLDivider(PLLDIG_Type * base, uint32_t instance, uint32_t phi0Divider, uint32_t phi1Divider, uint32_t predivider, uint32_t mulFactorDiv)
{
    uint32_t value = 0U;

    /* Configure divider */
    switch(instance)
    {
        case 0U:
        {
#if defined(PLLDIG_PLLDV_RFDPHI1) && defined(PLLDIG_PLLDV_RFDPHI) && defined(PLLDIG_PLLDV_PREDIV) && defined(PLLDIG_PLLDV_MFD)
            value = (uint32_t) (PLLDIG_PLLDV_RFDPHI1(phi1Divider) |
                                PLLDIG_PLLDV_RFDPHI(phi0Divider)  |
                                PLLDIG_PLLDV_PREDIV(predivider)   |
                                PLLDIG_PLLDV_MFD(mulFactorDiv));
            base->PLLDV = value;
#endif

#if defined(PLLDIG_PLL0DV_RFDPHI1) && defined(PLLDIG_PLL0DV_RFDPHI) && defined(PLLDIG_PLL0DV_PREDIV) && defined(PLLDIG_PLL0DV_MFD)
            value = (uint32_t) (PLLDIG_PLL0DV_RFDPHI1(phi1Divider) |
                                PLLDIG_PLL0DV_RFDPHI(phi0Divider)  |
                                PLLDIG_PLL0DV_PREDIV(predivider)   |
                                PLLDIG_PLL0DV_MFD(mulFactorDiv));
            base->PLL0DV = value;
#endif
        }
        break;
        case 1U:
        {
#if defined(PLLDIG_PLL1DV_RFDPHI) && defined(PLLDIG_PLL1DV_MFD)
            value = (uint32_t) (PLLDIG_PLL1DV_RFDPHI(phi0Divider)  |
                                PLLDIG_PLL1DV_MFD(mulFactorDiv));
            base->PLL1DV = value;
#endif
        }
        break;

        default:
        {
            /* 2 Invalid instance number. */
            DEV_ASSERT(false);
        }
        break;
    }
}

/*!
 * @brief Gets the PLLDIG clock source predivider ratio.
 *
 * This function gets the PLLDIG clock source predivider ratio.
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG predivider ratio
 */
static inline uint32_t CGMCS_GetPllPreDividerRatio(const PLLDIG_Type * base, uint32_t instance)
{
    uint32_t retVal;
    (void)base;
    (void)instance;

#if defined(PLLDIG_PLLDV_PREDIV_MASK) && defined(PLLDIG_PLLDV_PREDIV_SHIFT)
    retVal = ((base->PLLDV & PLLDIG_PLLDV_PREDIV_MASK) >> PLLDIG_PLLDV_PREDIV_SHIFT);
#elif defined(PLLDIG_PLL0DV_PREDIV_MASK) && defined(PLLDIG_PLL0DV_PREDIV_SHIFT)
    if (instance == 0U)
    {
        retVal = ((base->PLL0DV & PLLDIG_PLL0DV_PREDIV_MASK) >> PLLDIG_PLL0DV_PREDIV_SHIFT);
    }
    else
    {
        retVal = 0U;
    }
#else
    retVal = 0U;
#endif
    return retVal;
}

/*!
 * @brief Gets the PLLDIG clock source multiply factor.
 *
 * This function gets the PLLDIG clock source multiply factor.
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source multiply factor
 */
static inline uint32_t CGMCS_GetPLLMultiplyFactor(const PLLDIG_Type * base, uint32_t instance)
{
    uint32_t retVal;
    (void)base;
    (void)instance;
#if defined(PLLDIG_PLLDV_MFD_MASK) && defined(PLLDIG_PLLDV_MFD_SHIFT)
    retVal = ((base->PLLDV & PLLDIG_PLLDV_MFD_MASK) >> PLLDIG_PLLDV_MFD_SHIFT);
#elif defined(PLLDIG_PLL0DV_MFD_MASK) && defined(PLLDIG_PLL0DV_MFD_SHIFT) && defined(PLLDIG_PLL1DV_MFD_MASK) && defined(PLLDIG_PLL1DV_MFD_SHIFT)
    if (instance == 0U)
    {
        retVal = ((base->PLL0DV & PLLDIG_PLL0DV_MFD_MASK) >> PLLDIG_PLL0DV_MFD_SHIFT);
    }
    else
    {
        retVal = ((base->PLL1DV & PLLDIG_PLL1DV_MFD_MASK) >> PLLDIG_PLL1DV_MFD_SHIFT);
    }
#else
    retValue = 0U;
#endif
    return retVal;
}

/*!
 * @brief Gets the PLLDIG clock source first output divider ratio.
 *
 * This function gets the PLLDIG clock source first output divider ratio.
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source first output divider ratio.
 */
static inline uint32_t CGMCS_GetPllFirstOutputDividerRatio(const PLLDIG_Type * base, uint32_t instance)
{
    uint32_t retVal;
    (void)base;
    (void)instance;
#if defined(PLLDIG_PLLDV_RFDPHI_MASK) && defined(PLLDIG_PLLDV_RFDPHI_SHIFT)
    retVal = ((base->PLLDV & PLLDIG_PLLDV_RFDPHI_MASK) >> PLLDIG_PLLDV_RFDPHI_SHIFT);
#elif defined(PLLDIG_PLL0DV_RFDPHI_MASK) && defined(PLLDIG_PLL0DV_RFDPHI_SHIFT) && defined(PLLDIG_PLL1DV_RFDPHI_MASK) && defined(PLLDIG_PLL1DV_RFDPHI_SHIFT)
    if (instance == 0U)
    {
        retVal = ((base->PLL0DV & PLLDIG_PLL0DV_RFDPHI_MASK) >> PLLDIG_PLL0DV_RFDPHI_SHIFT);
    }
    else
    {
        retVal = ((base->PLL1DV & PLLDIG_PLL1DV_RFDPHI_MASK) >> PLLDIG_PLL1DV_RFDPHI_SHIFT);
    }
#else
    retVal = 0U;
#endif
    return retVal;
}

/*!
 * @brief Gets the PLLDIG clock source second output divider ratio.
 *
 * This function gets the PLLDIG clock source second output divider ratio.
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source second output divider ratio.
 */
static inline uint32_t CGMCS_GetPllSecondOutputDividerRatio(const PLLDIG_Type * base, uint32_t instance)
{
    uint32_t retVal;
    (void)base;
    (void)instance;
#if defined(PLLDIG_PLLDV_RFDPHI1_MASK) && defined(PLLDIG_PLLDV_RFDPHI1_SHIFT)
    retVal = ((base->PLLDV & PLLDIG_PLLDV_RFDPHI1_MASK) >> PLLDIG_PLLDV_RFDPHI1_SHIFT);
#elif defined(PLLDIG_PLL0DV_RFDPHI1_MASK) && defined(PLLDIG_PLL0DV_RFDPHI1_SHIFT)
    if (instance == 0U)
    {
        retVal = ((base->PLL0DV & PLLDIG_PLL0DV_RFDPHI1_MASK) >> PLLDIG_PLL0DV_RFDPHI1_SHIFT);
    }
    else
    {
        retVal = 0U;
    }
#else
    retVal = 0U;
#endif
    return retVal;
}

/*!
 * @brief Sets PLL Denominator Fractional Loop Divider
 *
 * This function sets PLL Denominator Fractional Loop Divider
 *
 * @param[in] base             Register base address for the PLLDIG instance.
 * @param[in] mfden            Denominator fractional loop divider
 */
static inline void CGMCS_SetPLLDenominatorFractionalLoopDivider(PLLDIG_Type * base, uint32_t mfden)
{
      (void)base;
      (void)mfden;
#if defined(PLLDIG_PLLCAL3_MFDEN)
      /* Write denominator fractional loop divider */
      base->PLLCAL3 = PLLDIG_PLLCAL3_MFDEN(mfden);
#endif
}

/*!
 * @brief Gets the PLLDIG Denominator Fractional Loop Divider
 *
 * This function gets the PLLDIG Denominator Fractional Loop Divider
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source Fractional Divider Register.
 */
static inline uint32_t CGMCS_GetPLLDenominatorFractionalLoopDivider(const PLLDIG_Type * base)
{
      (void)base;
#if defined(PLLDIG_PLLCAL3_MFDEN_MASK) && defined(PLLDIG_PLLCAL3_MFDEN_SHIFT)
    return ((base->PLLCAL3 & PLLDIG_PLLCAL3_MFDEN_MASK) >> PLLDIG_PLLCAL3_MFDEN_SHIFT);
#else
    return 0U;
#endif
}

/*!
 * @brief Sets PLL Numerator Fractional Loop Divider
 *
 * This function sets PLL Numerator Fractional Loop Divider
 *
 * @param[in] base             Register base address for the PLLDIG instance.
 * @param[in] mfn              Numerator Fractional Loop Divider
 */
static inline void CGMCS_SetPLLNumeratorFractionalLoopDivider(PLLDIG_Type * base, uint32_t mfn)
{
    (void)base;
    (void)mfn;
      /* Sigma delta modulation is disabled. Write MFN. */
#if defined(PLLDIG_PLLFD_MFN)
        base->PLLFD |= PLLDIG_PLLFD_MFN(mfn);
#endif
}



static inline void CGMCS_SetPLLSigmaDeltaModulation(PLLDIG_Type * base, bool sigmaDeltaEnable, bool secondOrderSigmaDeltaEnable, bool thirdOrderSigmaDeltaEnable)
{
    (void)base;
    (void)sigmaDeltaEnable;
	(void)secondOrderSigmaDeltaEnable;
	(void)thirdOrderSigmaDeltaEnable;

     /* Configure sigma modulation. */
#if defined(PLLDIG_PLLFD_SMDEN) && defined(PLLDIG_PLLFD_SDM2) && defined(PLLDIG_PLLFD_SDM3)
     uint32_t value = 0U;
	 value |= PLLDIG_PLLFD_SMDEN(sigmaDeltaEnable ? 1UL : 0UL);
     value |= PLLDIG_PLLFD_SDM2(secondOrderSigmaDeltaEnable ? 1UL : 0UL);
	 value |= PLLDIG_PLLFD_SDM3(thirdOrderSigmaDeltaEnable ? 1UL : 0UL);
	 base->PLLFD |= value;
#endif
}


static inline void CGMCS_SetPLLDitherControl(PLLDIG_Type * base, bool ditherControlEnable, uint32_t ditherControlValue)
{
    (void)base;
    (void)ditherControlEnable;
	(void)ditherControlValue;

     /* Configure DITHER CONTROL */
#if defined(PLLDIG_PLLFD_DTHDIS) && defined(PLLDIG_PLLFD_DTHRCTL)
     uint32_t value = 0U;
     value |= PLLDIG_PLLFD_DTHDIS(ditherControlEnable ? 0UL : 1UL);
     value |= PLLDIG_PLLFD_DTHRCTL(ditherControlValue);
	 base->PLLFD |= value;
#endif
}



/*!
 * @brief Gets the PLLDIG Numerator Fractional Loop Divider
 *
 * This function gets the PLLDIG Numerator Fractional Loop Divider
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source Frequency Modulation.
 */
static inline uint32_t CGMCS_GetPLLNumeratorFractionalLoopDivider(const PLLDIG_Type * base)
{
#if defined(PLLDIG_PLLFD_MFN_MASK) && defined(PLLDIG_PLLFD_MFN_SHIFT)
    return ((base->PLLFD & PLLDIG_PLLFD_MFN_MASK) >> PLLDIG_PLLFD_MFN_SHIFT);
#else
      (void)base;
    return 0U;
#endif
}

/*!
 * @brief Sets PLL Fractional Divider
 *
 * This function sets PLL Fractional Divider
 *
 * @param[in] base              Register base address for the PLLDIG instance.
 * @param[in] enable            Enable fractional loop divider
 * @param[in] fracDivider       Fractional loop divider value
 */
static inline void CGMCS_SetPLLFractionalDivider(PLLDIG_Type * base, bool enable, uint32_t fracDivider)
{
#if defined(PLLDIG_PLL1FD_FDEN) && defined(PLLDIG_PLL1FD_FRCDIV)
    uint32_t value = (uint32_t) (PLLDIG_PLL1FD_FDEN(enable ? 1UL : 0UL)  |
                                 PLLDIG_PLL1FD_FRCDIV(fracDivider));
    base->PLL1FD = value;
#else
      (void)base;
      (void)enable;
      (void)fracDivider;
#endif
}

/*!
 * @brief Gets the PLLDIG Fractional Divider.
 *
 * This function gets the PLLDIG Fractional Divider
 *
 * @param[in] base Register base address for the PLLDIG_Type instance.
 * @return PLLDIG clock source Fractional Divider Register.
 */
static inline uint32_t CGMCS_GetPLLFractionalDivider(const PLLDIG_Type * base)
{
    uint32_t retValue;
    (void)base;
#if defined(PLLDIG_PLL1FD_FDEN) && defined(PLLDIG_PLL1FD_FRCDIV)
    if (((base->PLL1FD & PLLDIG_PLL1FD_FDEN_MASK) >> PLLDIG_PLL1FD_FDEN_SHIFT) != 0U)
    {
        retValue = (base->PLL1FD & PLLDIG_PLL1FD_FRCDIV_MASK) >> PLLDIG_PLL1FD_FRCDIV_SHIFT;
    }
    else
    {
        retValue = 0U;
    }
#else
    retValue = 0U;
#endif
    return retValue;
}



/*!
 * @brief Configures PLL Modulation
 *
 * This function sets PLL Modulation
 *
 * @param[in] base              Register base address for the PLLDIG instance.
 * @param[in] enable            Enable fractional loop divider
 * @param[in] fracDivider       Fractional loop divider value
 */
static inline void CGMCS_ConfigurePllModulation(PLLDIG_Type * base, bool enable, uint32_t modulationType, uint32_t modulationPeriod, uint32_t incrementStep)
{
    uint32_t value;
#if defined(PLLDIG_PLLFM_SSCGBYP) && defined(PLLDIG_PLLFM_STEPNO) && defined(PLLDIG_PLLFM_STEPSIZE)
    (void)modulationType;
    value = (uint32_t) (PLLDIG_PLLFM_SSCGBYP(enable ? 1UL : 0UL)  |
                        PLLDIG_PLLFM_STEPNO(incrementStep)        |
                        PLLDIG_PLLFM_STEPSIZE(modulationPeriod));
    base->PLLFM = value;
#endif

/*  TODO Remove macros from header files and define macros in features.h or different PLLs */
#if defined(PLLDIG_PLL1FM_MODEN) && defined(PLLDIG_PLL1FM_MODSEL) && defined(PLLDIG_PLL1FM_INCSTP) && defined(PLLDIG_PLL1FM_MODPRD)
    value = (uint32_t) (PLLDIG_PLL1FM_MODEN(enable ? 1UL : 0UL)    |
                        PLLDIG_PLL1FM_MODSEL(modulationType)       |
                        PLLDIG_PLL1FM_INCSTP(incrementStep)        |
                        PLLDIG_PLL1FM_MODPRD(modulationPeriod));
    base->PLL1FM = value;
#endif
}


#if defined(__cplusplus)
}
#endif /* __cplusplus*/


/*! @}*/

#endif /* CGMCS_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
