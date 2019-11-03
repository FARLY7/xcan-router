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

#ifndef ADC_IRQ_H
#define ADC_IRQ_H

#include "device_registers.h"
#include "adc_pal_mapping.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 #if defined(ADC_PAL_S32K1xx)

void ADC_S32K1xx_IrqHandler(const uint32_t instIdx);

#endif /* defined(ADC_PAL_S32K1xx) */


#if defined(ADC_PAL_MPC574xC_G_R)

void ADC_MPC574xC_G_R_HwTrigIrqHandler(const uint32_t instIdx, const uint8_t adcIdx);

#endif /* defined(ADC_PAL_MPC574xC_G_R) */

#if defined(ADC_PAL_MPC574xP)

void ADC_MPC574xP_HwTrigIrqHandler(const uint32_t instIdx, const uint8_t fifoIdx);

#endif /* defined(ADC_PAL_MPC574xP) */

#if (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_MPC574xP))

void ADC_MPC574x_SwTrigIrqHandler(const uint32_t instIdx, const uint8_t adcIdx);

#endif /* (defined(ADC_PAL_MPC574xC_G_R) || defined(ADC_PAL_MPC574xP)) */

#endif /* ADC_IRQ_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
