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

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block scope
 * if its identifier only appears in a single function.
 * __VECTOR_RAM variable is not an object with static storage duration, it needs to be
 * declared as extern.
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or
 * different essential type.
 * This is required by the conversion of a bit-field of a register into a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially unsigned' type to 'essentially enum<i>'.
 * This is required by the conversion of a bit-field (uint16_t) of a register
 * into a enum type. Enum type is strictly defined as unsigned integer.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 * (different essential type categories).
 * This is required by the conversion of a bit/bitfield of a register into boolean or a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.1, Conversions shall not be performed
 * between a pointer to a function and any other type.
 * This is required in order to read/write from vector table memory.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, A conversion should not be performed
 * between a pointer to object and an integer type.
 * The address of hardware modules is provided as integer so
 * a conversion between a pointer and an integer has to be performed.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, A cast shall not be performed
 * between pointer to void and an arithmetic type.
 * The address of hardware modules is provided as integer so
 * it needs to be cast to pointer.
 *
 * @section [global]
 * Violates MISRA 2012 Mandatory Rule 17.3, Symbol 'MFSPR' undeclared, assumed
 * to return int.
 * This is an e200 Power Architecture Assembly instruction used to retrieve
 * the core number.
 *
 */

 /*! @file interrupt_manager.c */

#include <stddef.h>
#include "interrupt_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Counter to manage the nested callings of global disable/enable interrupt.
 */
static int32_t g_interruptDisableCount = 0;

/*!
 * @brief Declaration of vector table.
 * FEATURE_INTERRUPT_IRQ_MAX is the highest interrupt request number.
 */
extern uint32_t __VECTOR_RAM[(uint32_t)(FEATURE_INTERRUPT_IRQ_MAX)];

/*******************************************************************************
 * PROTOTYPES
 ******************************************************************************/
static void INT_SYS_EnableIRQOnCore(int32_t vectorNumber, uint8_t coreId);
static void INT_SYS_DisableIRQOnCore(int32_t vectorNumber, uint8_t coreId);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_InstallHandler
 * Description   : Install an interrupt handler routine for a given IRQ number
 * This function will let application register/replace the interrupt
 * handler for specified IRQ number. Refer to startup_<CPU>.s
 * file for each chip family to find out the default interrupt handler for each device.
 *
 * Implements INT_SYS_InstallHandler_Activity
 *
 *END**************************************************************************/
void INT_SYS_InstallHandler(IRQn_Type irqNumber,
                            const isr_t newHandler,
                            isr_t* const oldHandler)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    /* Save the former handler pointer */
    if (oldHandler != (isr_t *) 0)
    {
        *oldHandler = (isr_t)__VECTOR_RAM[((int32_t)irqNumber)];
    }

    /* Set handler into vector table */
    __VECTOR_RAM[((int32_t)irqNumber)] = (uint32_t)newHandler;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQOnCore
 * Description   : Enables an interrupt for a given vector number and core number.
 * This function will enable an interrupt corresponding to the vector number
 * provided as parameter, on the given core number.
 *
 *END**************************************************************************/
static void INT_SYS_EnableIRQOnCore(int32_t vectorNumber, uint8_t coreId)
{
    /* Enable interrupt for the given core */
    switch (coreId) {
        case 0U:
            INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRC_SELN0_MASK) | (uint16_t)INTC_PSR_PRC_SELN0(1U);
            break;
#if NUMBER_OF_CORES >= 2u
        case 1U:
            INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRC_SELN1_MASK) | (uint16_t)INTC_PSR_PRC_SELN1(1U);
            break;
#endif
#if NUMBER_OF_CORES >= 3u
        case 2U:
            INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRC_SELN2_MASK) | (uint16_t)INTC_PSR_PRC_SELN2(1U);
            break;
#endif
        default:
            /* invalid core number */
            DEV_ASSERT(false);
            break;
      }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisbaleIRQOnCore
 * Description   : Disable individual interrupt for a given vector number and
 * core number.
 * This function will disable an interrupt corresponding to the vector number
 * provided as parameter, on the given core number.
 *
 *END**************************************************************************/
static void INT_SYS_DisableIRQOnCore(int32_t vectorNumber, uint8_t coreId)
{
    /* Clear processor where the interrupt was sent */
    switch (coreId) {
        case 0U:
            INTC->PSR[vectorNumber] &= ~INTC_PSR_PRC_SELN0_MASK;
            break;
#if NUMBER_OF_CORES >= 2u
        case 1U:
            INTC->PSR[vectorNumber] &= ~INTC_PSR_PRC_SELN1_MASK;
            break;
#endif
#if NUMBER_OF_CORES >= 3u
        case 2U:
            INTC->PSR[vectorNumber] &= ~INTC_PSR_PRC_SELN2_MASK;
            break;
#endif
        default:
            /* invalid core number */
            DEV_ASSERT(false);
            break;
      }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQ
 * Description   : Enables an interrupt for a given IRQ number.
 * This function will enable an interrupt corresponding to the interrupt number
 * provided as parameter. Interrupt request will be sent to current core,
 * with priority defined in FEATURE_INTERRUPT_DEFAULT_PRIO.
 *
 * Implements INT_SYS_EnableIRQ_Activity
 *END**************************************************************************/
void INT_SYS_EnableIRQ(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    int32_t vectorNumber = (int32_t)irqNumber;

    /* Set interrupt priority to what it is defined in FEATURE_INTERRUPT_DEFAULT_PRIO */
    INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRIN_MASK) | (uint16_t)INTC_PSR_PRIN(FEATURE_INTERRUPT_DEFAULT_PRIO);

    /* Enable interrupt for the current core */
    INT_SYS_EnableIRQOnCore(vectorNumber, (uint8_t)GET_CORE_ID());
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQ_MC
 * Description   : Enables an interrupt for a given IRQ number and the given
 * cores.
 * This function will enable an interrupt corresponding to the interrupt number
 * provided as parameter. Interrupt request will be sent to the array of cores
 * sent as parameter. Interrupt will be enabled with priority defined in
 * FEATURE_INTERRUPT_DEFAULT_PRIO.
 *
 * Implements INT_SYS_EnableIRQ_MC_Activity
 *END**************************************************************************/
void INT_SYS_EnableIRQ_MC(IRQn_Type irqNumber, const uint8_t *coresIds, uint8_t coresCnt)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    uint8_t i;
    int32_t vectorNumber = (int32_t)irqNumber;

    /* Set interrupt priority to what it is defined in FEATURE_INTERRUPT_DEFAULT_PRIO */
    INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRIN_MASK) | (uint16_t)INTC_PSR_PRIN(FEATURE_INTERRUPT_DEFAULT_PRIO);

    /* Enable interrupt for each requested core */
    for (i = 0U; i  < coresCnt; i++) {
        INT_SYS_EnableIRQOnCore(vectorNumber, coresIds[i]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_GetCoresForIRQ
 * Description   : Gets the cores on which an interrupt for the specified IRQ
 * number is enabled.
 * This function expects cores array given as parameter to be an array which has
 * the array index the core number. The function will populate this array as follows:
 * - INTERRUPT_CORE_ENABLED if interrupt is enabled on that core
 * - INTERRUPT_CORE_DISABLED if interrupt is not enabled on that core.
 *
 * Implements INT_SYS_GetCoresForIRQ_Activity
 *
 *END**************************************************************************/
void INT_SYS_GetCoresForIRQ(IRQn_Type irqNumber, interrupt_core_enable_t *cores) {

     /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);
    DEV_ASSERT(cores != NULL);

    uint8_t coreId;
    int32_t vectorNumber = (int32_t)irqNumber;

    for (coreId = 0; coreId < NUMBER_OF_CORES; coreId++) {
        switch (coreId) {
            case 0U:
                cores[coreId] = (interrupt_core_enable_t)((INTC->PSR[vectorNumber] & INTC_PSR_PRC_SELN0_MASK) >> INTC_PSR_PRC_SELN0_SHIFT);
                break;
#if NUMBER_OF_CORES >= 2u
            case 1U:
                cores[coreId] = (interrupt_core_enable_t)((INTC->PSR[vectorNumber] & INTC_PSR_PRC_SELN1_MASK) >> INTC_PSR_PRC_SELN1_SHIFT);
                break;
#endif
#if NUMBER_OF_CORES >= 3u
            case 2U:
                cores[coreId] = (interrupt_core_enable_t)((INTC->PSR[vectorNumber] & INTC_PSR_PRC_SELN2_MASK) >> INTC_PSR_PRC_SELN2_SHIFT);
                break;
#endif
            default:
                /* invalid core number */
                DEV_ASSERT(false);
                break;
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQ
 * Description   : Disable individual interrupt for a specified IRQ
 * This function will disable an interrupt corresponding to the interrupt number
 * provided as parameter. It is considered interrupt request was sent to
 * current core.
 *
 * Implements INT_SYS_DisableIRQ_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQ(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    int32_t vectorNumber = (int32_t)irqNumber;

    /* Clear interrupt priority */
    INTC->PSR[vectorNumber] &= ~INTC_PSR_PRIN_MASK;

    /* Clear processor where the interrupt was sent(current core) */
    INT_SYS_DisableIRQOnCore(vectorNumber, (uint8_t)GET_CORE_ID());
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQ_MC
 * Description   : Disable individual interrupt for a specified IRQ, on the given
 * cores.
 * This function will disable an interrupt corresponding to the interrupt number
 * provided as parameter. It will disable interrupt only on the cores given as
 * parameter.
 *
 * Implements INT_SYS_DisableIRQ_MC_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQ_MC(IRQn_Type irqNumber, const uint8_t *coresIds, uint8_t coresCnt)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    uint8_t i;
    int32_t vectorNumber = (int32_t)irqNumber;

    /* Clear processors where the interrupt was sent */
    for (i = 0U; i  < coresCnt; i++) {
        INT_SYS_DisableIRQOnCore(vectorNumber, coresIds[i]);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQ_MC
 * Description   : Disable individual interrupt for a specified IRQ, on all cores.
 * This function will disable an interrupt corresponding to the interrupt number
 * provided as parameter. It clears the priority and then all the processors for
 * which it was enabled.
 *
 * Implements INT_SYS_DisableIRQ_MC_All_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQ_MC_All(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    int32_t vectorNumber = (int32_t)irqNumber;

    /* Clear interrupt priority */
    INTC->PSR[vectorNumber] &= ~INTC_PSR_PRIN_MASK;

    /* Clear processors */
    INTC->PSR[vectorNumber] &= ~FEATURE_INTERRUPT_ENABLE_ON_ALL_CORES_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_EnableIRQGlobal
 * Description   : Enable system interrupt
 * This function will enable the global interrupt by calling the core API
 *
 * Implements INT_SYS_EnableIRQGlobal_Activity
 *
 *END**************************************************************************/
void INT_SYS_EnableIRQGlobal(void)
{
    /* Check and update */
    if (g_interruptDisableCount > 0)
    {
        g_interruptDisableCount--;

        if (g_interruptDisableCount <= 0)
        {
            /* Enable the global interrupt*/
            ENABLE_INTERRUPTS();
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_DisableIRQGlobal
 * Description   : Disable system interrupt
 * This function will disable the global interrupt by calling the core API
 *
 * Implements INT_SYS_DisableIRQGlobal_Activity
 *
 *END**************************************************************************/
void INT_SYS_DisableIRQGlobal(void)
{
    /* Disable the global interrupt */
    DISABLE_INTERRUPTS();

    /* Update counter*/
    g_interruptDisableCount++;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_SetPriority
 * Description   : Set the priority of an interrupt
 * This function will set the priority of an interrupt.
 *
 * Implements INT_SYS_SetPriority_Activity
 *
 *END**************************************************************************/
void INT_SYS_SetPriority(IRQn_Type irqNumber, uint8_t priority)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);
    /* Check IRQ priority */
    DEV_ASSERT(priority <= FEATURE_INTERRUPT_PRIO_MAX);

    /* Set IRQ priority */
    int32_t vectorNumber = (int32_t)irqNumber;

    INTC->PSR[vectorNumber] = (INTC->PSR[vectorNumber] & ~INTC_PSR_PRIN_MASK) | INTC_PSR_PRIN(priority);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_GetPriority
 * Description   : Get the priority of an interrupt
 * This function will get the priority of an interrupt.
 *
 * Implements INT_SYS_GetPriority_Activity
 *
 *END**************************************************************************/
uint8_t INT_SYS_GetPriority(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_IRQ_MAX);

    int32_t vectorNumber = (int32_t)irqNumber;

    uint8_t priority = (uint8_t)((INTC->PSR[vectorNumber] & INTC_PSR_PRIN_MASK) >> INTC_PSR_PRIN_SHIFT);
    return priority;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_SetSoftwareIRQRequest
 * Description   : Set a software settable interrupt request
 * This function will set a software settable interrupt request.
 *
 * Implements INT_SYS_SetSoftwareIRQRequest_Activity
 *
 *END**************************************************************************/
void INT_SYS_SetSoftwareIRQRequest(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_SOFTWARE_IRQ_MAX);

    /* Set a software settable interrupt request */
    int32_t vectorNumber = (int32_t)irqNumber;

    INTC->SSCIR[vectorNumber] = (INTC->SSCIR[vectorNumber] & ~INTC_SSCIR_SET_MASK) | (uint8_t)INTC_SSCIR_SET(1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : INT_SYS_ClearSoftwareIRQRequest
 * Description   : Clear a software settable interrupt request
 * This function will clear a software settable interrupt request.
 *
 * Implements INT_SYS_ClearSoftwareIRQRequest_Activity
 *
 *END**************************************************************************/
void INT_SYS_ClearSoftwareIRQRequest(IRQn_Type irqNumber)
{
    /* Check IRQ number */
    DEV_ASSERT(FEATURE_INTERRUPT_IRQ_MIN <= irqNumber);
    DEV_ASSERT(irqNumber <= FEATURE_INTERRUPT_SOFTWARE_IRQ_MAX);

    /* Clear a software settable interrupt request */
    int32_t vectorNumber = (int32_t)irqNumber;

    INTC->SSCIR[vectorNumber] = (INTC->SSCIR[vectorNumber] & ~INTC_SSCIR_CLR_MASK) | (uint8_t)INTC_SSCIR_CLR(1U);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
