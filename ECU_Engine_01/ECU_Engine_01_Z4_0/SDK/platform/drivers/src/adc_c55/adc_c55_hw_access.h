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
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * Function-like macros are used to calculate bit positions based on channel index
 *
 */

#ifndef ADC_C55_HW_ACCESS_H
#define ADC_C55_HW_ACCESS_H

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

#include "device_registers.h"

/*!
 * @defgroup adc_c55_group ADC C55 Hardware Access
 * @brief ADC C55 Group
 * @{
 */

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Define the hardware register size when calculating bit positions */
#define ADC_HW_REG_SIZE (32u)

/* Definitions to compute bit positions from channel index */
#define CHAN_2_VECT(CHNIDX) ((CHNIDX) / ADC_HW_REG_SIZE)
#define CHAN_2_BIT(CHNIDX)  ((CHNIDX) % ADC_HW_REG_SIZE)

/* Define ADC Channel Groups */
#define ADC_GROUP_PRECISION (0u)
#define ADC_GROUP_INTERNAL  (1u)
#if ((ADC_CIMR_COUNT > 2u) || (ADC_CTR_COUNT > 2u))
/* Define the group only if it exists in hardware */
#define ADC_GROUP_EXTERNAL  (2u)
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

static inline void ADC_Powerup(ADC_Type * const base)
{
    REG_BIT_CLEAR32(&(base->MCR), ADC_MCR_PWDN(1u));
}


static inline void ADC_Powerdown(ADC_Type * const base)
{
    REG_BIT_SET32(&(base->MCR), ADC_MCR_PWDN(1u));
}

#if defined (__cplusplus)
extern "C" {
#endif

#if defined (__cplusplus)
}
#endif

/*! @} */

#endif /* ADC_C55_HW_ACCESS_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
