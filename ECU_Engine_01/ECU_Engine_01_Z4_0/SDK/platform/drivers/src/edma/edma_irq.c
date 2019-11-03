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

/*!
 * @file edma_irq.c
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * These are symbols weak symbols defined in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, external could be made static.
 * The functions are called by the interrupt controller when the appropriate event
 * occurs.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * This is required for initializing pointers to the module's memory map, which
 * is located at a fixed address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 */

#include "edma_irq.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef FEATURE_DMA_HWV3

/*! @brief DMA0_Ch0_Ch15_IRQHandler with the same name in the startup code*/
void DMA0_Ch0_Ch15_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 0U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 0U; virtualChannel <= 15U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_INT & DMA_TCD_CH_INT_INT_MASK) == DMA_TCD_CH_INT_INT_MASK)
        {
            EDMA_DRV_IRQHandler(virtualChannel);
        }
        index++;
    }
}

/*! @brief DMA0_Ch16_Ch31_IRQHandler with the same name in the startup code*/
void DMA0_Ch16_Ch31_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 16U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 16U; virtualChannel <= 31U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_INT & DMA_TCD_CH_INT_INT_MASK) == DMA_TCD_CH_INT_INT_MASK)
        {
            EDMA_DRV_IRQHandler(virtualChannel);
        }
        index++;
    }
}

/*! @brief DMA1_Ch0_Ch15_IRQHandler with the same name in the startup code*/
void DMA1_Ch0_Ch15_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 32U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(1U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 32U; virtualChannel <= 47U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_INT & DMA_TCD_CH_INT_INT_MASK) == DMA_TCD_CH_INT_INT_MASK)
        {
            EDMA_DRV_IRQHandler(virtualChannel);
        }
        index++;
    }
}

/*! @brief DMA1_Ch16_Ch31_IRQHandler with the same name in the startup code*/
void DMA1_Ch16_Ch31_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 48U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(1U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 48U; virtualChannel <= 63U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_INT & DMA_TCD_CH_INT_INT_MASK) == DMA_TCD_CH_INT_INT_MASK)
        {
            EDMA_DRV_IRQHandler(virtualChannel);
        }
        index++;
    }
}

#ifdef FEATURE_DMA_HAS_ERROR_IRQ
/*! @brief DMA ERROR IRQ handler with the same name in the startup code*/
void DMA0_Error_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 0U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 0U; virtualChannel <= 31U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_ES & DMA_TCD_CH_ES_ERR_MASK) == DMA_TCD_CH_ES_ERR_MASK)
        {
            EDMA_DRV_ErrorIRQHandler(virtualChannel);
        }
        index++;
    }
}

/*! @brief DMA ERROR IRQ handler with the same name in the startup code*/
void DMA1_Error_IRQHandler(void)
{
    uint8_t index = 0U;
    uint8_t virtualChannel = 32U;
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(1U);
    /* Check all the flags and call the handler for the appropriate channel */
    for(virtualChannel = 32U; virtualChannel <= 63U; virtualChannel++)
    {
        if ((edmaRegBase->TCD[index].CH_ES & DMA_TCD_CH_ES_ERR_MASK) == DMA_TCD_CH_ES_ERR_MASK)
        {
            EDMA_DRV_ErrorIRQHandler(virtualChannel);
        }
        index++;
    }
}
#endif

#else

#ifdef FEATURE_DMA_ORED_IRQ_LINES_16_CHN
/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA0_15_IRQHandler(void)
{
    /* Read the status flags register */
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    uint32_t mask = 0xFFFF;
    uint32_t flags = edmaRegBase->INT;
    uint8_t virtualChannel = 0U;
    flags &= mask;
    /* Check all the flags from 0 to 15 and call the handler for the appropriate channel */
    while (flags > 0U)
    {
       if ((flags & 1U) > 0U)
       {
           EDMA_DRV_IRQHandler(virtualChannel);
       }
       virtualChannel++;
       flags >>= 1U;
    }
}

/*! @brief DMA16_31_IRQn IRQ handler with the same name in the startup code*/
void DMA16_31_IRQHandler(void)
{
    /* Read the status flags register */
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    uint32_t flags = edmaRegBase->INT;
    uint8_t virtualChannel = 16U;
    flags >>= 16U;
    /* Check all the flags from 16 to 31 and call the handler for the appropriate channel */
    while (flags > 0U)
    {
       if ((flags & 1U) > 0U)
       {
           EDMA_DRV_IRQHandler(virtualChannel);
       }
       virtualChannel++;
       flags >>= 1U;
    }
}
#endif

#ifdef FEATURE_DMA_SEPARATE_IRQ_LINES_PER_CHN
/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA0_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(0U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA1_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(1U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA2_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(2U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA3_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(3U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA4_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(4U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA5_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(5U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA6_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(6U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA7_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(7U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA8_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(8U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA9_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(9U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA10_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(10U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA11_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(11U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA12_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(12U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA13_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(13U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA14_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(14U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA15_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(15U);
}
#if (FEATURE_DMA_CHANNELS > 16U)
void DMA16_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(16U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA17_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(17U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA18_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(18U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA19_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(19U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA20_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(20U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA21_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(21U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA22_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(22U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA23_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(23U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA24_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(24U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA25_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(25U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA26_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(26U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA27_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(27U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA28_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(28U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA29_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(29U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA30_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(30U);
}

/*! @brief DMA IRQ handler with the same name in the startup code*/
void DMA31_IRQHandler(void)
{
    EDMA_DRV_IRQHandler(31U);
}
#endif
#endif

#ifdef FEATURE_DMA_HAS_ERROR_IRQ
/*! @brief DMA ERROR IRQ handler with the same name in the startup code*/
void DMA_Error_IRQHandler(void)
{
	const DMA_Type * edmaRegBase = EDMA_DRV_GetDmaRegBaseAddr(0U);
    uint32_t error = EDMA_GetErrorIntStatusFlag(edmaRegBase);
    uint8_t virtualChannel = 0U;

    for (virtualChannel = 0U;
         virtualChannel < FEATURE_DMA_VIRTUAL_CHANNELS;
         virtualChannel++)
    {
        if((error & EDMA_ERR_LSB_MASK) != 0UL)
        {
            EDMA_DRV_ErrorIRQHandler(virtualChannel);
        }
        error = error >> 1U;
    }
}
#endif

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/

