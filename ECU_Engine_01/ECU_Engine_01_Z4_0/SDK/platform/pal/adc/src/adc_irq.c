/*
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
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

/*!
 * @file adc_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * Default IRQHandler symbols are weak symbols declared in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Default IRQHandler symbols are weak symbols declared in platform startup files (.s).
 *
 */

#include "adc_pal_cfg.h"
#include "adc_irq.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Define default interrupt handlers for ADC instances.
 * Default interrupt handlers already declared in startup files. */

#if defined(ADC_PAL_S32K1xx)

#if (ADC_INSTANCE_COUNT >= 1u)
void ADC0_IRQHandler(void)
{
    ADC_S32K1xx_IrqHandler(0u);
}
#endif /* (ADC_INSTANCE_COUNT >= 1u) */

#if (ADC_INSTANCE_COUNT >= 2u)
void ADC1_IRQHandler(void)
{
    ADC_S32K1xx_IrqHandler(1u);
}
#endif /* (ADC_INSTANCE_COUNT >= 2u) */

#endif /* defined(ADC_PAL_S32K1xx) */


#if defined(ADC_PAL_MPC574xC_G_R)

/* For HW triggered groups with a single conversion */
void BCTU_ConvUpdate_IRQHandler(void)
{
    uint8_t adcIdx = 0u;
    const uint32_t bctuIdx = 0u; /* For ADC_PAL_MPC574xC_G_R, ADC PAL instances are mapped 1:1 with BCTU instances. */

    for(adcIdx = 0u; adcIdx < FEATURE_BCTU_NUM_ADC; adcIdx++)
    {
        /* ADC PAL doesn't trigger conversions in parallel on multiple ADCs,
         * so only one ADC should have new data available. */
        if(BCTU_DRV_GetStatusFlag(bctuIdx, adcIdx, BCTU_FLAG_NEW_DATA_AVAILABLE))
        {
            ADC_MPC574xC_G_R_HwTrigIrqHandler(bctuIdx, adcIdx);

            break;
        }
    }
}

/* For HW triggered groups with multiple conversions */
void BCTU_ListLast_IRQHandler(void)
{
    uint8_t adcIdx = 0u;
    const uint32_t bctuIdx = 0u; /* For ADC_PAL_MPC574xC_G_R, ADC PAL instances are mapped 1:1 with BCTU instances. */

    for(adcIdx = 0u; adcIdx < FEATURE_BCTU_NUM_ADC; adcIdx++)
    {
        /* ADC PAL doesn't trigger conversions in parallel on multiple ADCs,
         * so only one ADC should have new data available. */
        if(BCTU_DRV_GetStatusFlag(bctuIdx, adcIdx, BCTU_FLAG_LIST_LAST_CONV))
        {
            ADC_MPC574xC_G_R_HwTrigIrqHandler(bctuIdx, adcIdx);

            break;
        }
    }
}

void ADC0_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xC_G_R, ADC PAL instances are mapped 1:1 with BCTU instances.
     * ADC0 is connected to BCTU 0. */
    const uint32_t palIdx = 0u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 0u);
}

void ADC1_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xC_G_R, ADC PAL instances are mapped 1:1 with BCTU instances.
     * ADC1 is connected to BCTU 0. */
    const uint32_t palIdx = 0u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 1u);
}

#endif /* defined(ADC_PAL_MPC574xC_G_R) */


#if defined(ADC_PAL_MPC574xP)

void CTU0_FIFO0_IRQHandler(void)
{
    const uint32_t palIdx   = 0u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 0u);
}

void CTU0_FIFO1_IRQHandler(void)
{
    const uint32_t palIdx   = 0u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 1u);
}

void CTU0_FIFO2_IRQHandler(void)
{
    const uint32_t palIdx   = 0u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 2u);
}

void CTU0_FIFO3_IRQHandler(void)
{
    const uint32_t palIdx   = 0u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 3u);
}

void CTU1_FIFO0_IRQHandler(void)
{
    const uint32_t palIdx   = 1u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 0u);
}

void CTU1_FIFO1_IRQHandler(void)
{
    const uint32_t palIdx   = 1u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 1u);
}

void CTU1_FIFO2_IRQHandler(void)
{
    const uint32_t palIdx   = 1u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 2u);
}

void CTU1_FIFO3_IRQHandler(void)
{
    const uint32_t palIdx   = 1u;
	ADC_MPC574xP_HwTrigIrqHandler(palIdx, 3u);
}

void ADC0_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xP, ADC PAL instances are mapped 1:1 with CTU instances.
     * ADC0 is connected to CTU 0. */
    const uint32_t palIdx = 0u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 0u);
}

void ADC1_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xP, ADC PAL instances are mapped 1:1 with CTU instances.
     * ADC1 is connected to CTU 0. */
    const uint32_t palIdx = 0u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 1u);
}

void ADC2_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xP, ADC PAL instances are mapped 1:1 with CTU instances.
     * ADC2 is connected to CTU 1. */
    const uint32_t palIdx = 1u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 2u);
}

void ADC3_EOC_IRQHandler(void)
{
    /* For ADC_PAL_MPC574xP, ADC PAL instances are mapped 1:1 with CTU instances.
     * ADC3 is connected to CTU 1. */
    const uint32_t palIdx = 1u;
    ADC_MPC574x_SwTrigIrqHandler(palIdx, 3u);
}

#endif /* defined(ADC_PAL_MPC574xP) */

/*******************************************************************************
 * EOF
 ******************************************************************************/
