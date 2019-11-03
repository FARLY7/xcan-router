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

#ifndef ADC_PAL_MAPPING_H
#define ADC_PAL_MAPPING_H

#include "adc_pal_cfg.h"
#include "device_registers.h"

/* Include peripheral drivers */
#if defined(ADC_PAL_S32K1xx)

#include "adc_driver.h"
#include "adc_hw_access.h"
#include "trgmux_driver.h"
#include "pdb_driver.h"

#endif /* defined(ADC_PAL_S32K1xx) */

#if defined(ADC_PAL_MPC574xC_G_R)

#include "adc_c55_driver.h"
#include "bctu_driver.h"

#endif /* defined(ADC_PAL_MPC574xC_G_R) */

#if defined(ADC_PAL_MPC574xP)

#include "adc_c55_driver.h"
#include "ctu_driver.h"

#endif /* defined(ADC_PAL_MPC574xP) */


/*!
 * @brief Enumeration with the types of ADC peripherals supported by ADC PAL
 *
 * This enumeration contains the types of ADC peripherals supported by ADC PAL.
 * Implements : adc_inst_type_t_Class
 */
typedef enum{
#if (defined(PDB_INSTANCE_COUNT) && defined(TRGMUX_INSTANCE_COUNT))
    ADC_INST_TYPE_ADC_S32K1xx      = 0u,
#endif /* (defined(PDB_INSTANCE_COUNT) && defined(TRGMUX_INSTANCE_COUNT)) */

#if defined(BCTU_INSTANCE_COUNT)
    ADC_INST_TYPE_ADC_MPC574xC_G_R = 1u,
#endif /* defined(BCTU_INSTANCE_COUNT) */

#if defined(CTU_INSTANCE_COUNT)
    ADC_INST_TYPE_ADC_MPC574xP     = 2u
#endif /* defined(CTU_INSTANCE_COUNT) */
} adc_inst_type_t;


/*!
 * @brief Structure storing PAL instance information
 *
 * This structure is used for storing PAL instance information.
 * Implements : adc_instance_t_Class
 */
typedef struct{
    adc_inst_type_t instType;    /*!< Peripheral over which the PAL is used */
    uint32_t instIdx;            /*!< Instance index of the peripheral (for ADC PAL the triggering peripheral) over which the PAL is used */
} adc_instance_t;


#endif /* ADC_PAL_MAPPING_H */
