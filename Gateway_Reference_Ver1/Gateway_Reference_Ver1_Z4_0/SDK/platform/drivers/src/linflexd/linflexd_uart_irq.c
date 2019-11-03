/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
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
 * @file linflexd_uart_irq.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * The interrupt handlers defined here are installed at runtime and called when the
 * interrupt events occur.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope.
 * The arrays defined here are used in driver c file (linflexd_uart_driver.c).
 */

#include "linflexd_uart_irq.h"

#if defined (FEATURE_LINFLEXD_ORED_INT_LINES)

#if (LINFlexD_INSTANCE_COUNT > 0U)
/* Implementation of LINFlexD0 handler named in startup code. */
void LINFLEXD0_UART_IRQHandler(void)
{
    LINFLEXD_UART_DRV_IRQHandler(0U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 1U)
/* Implementation of LINFlexD1 handler named in startup code. */
void LINFLEXD1_UART_IRQHandler(void)
{
    LINFLEXD_UART_DRV_IRQHandler(1U);
}
#endif

/* ISR array for LINFlexD UART driver */
const isr_t g_uartLINFlexDIsr[LINFlexD_INSTANCE_COUNT] = {
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    &LINFLEXD0_UART_IRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 1U)
    &LINFLEXD1_UART_IRQHandler
    #endif
};

#elif defined (FEATURE_LINFLEXD_RX_TX_ERR_INT_LINES)

#if (LINFlexD_INSTANCE_COUNT > 0U)
/* Implementation of LINFlexD0 rx handler named in startup code. */
void LINFLEXD0_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(0U);
}

/* Implementation of LINFlexD0 tx handler named in startup code. */
void LINFLEXD0_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(0U);
}

/* Implementation of LINFlexD0 error handler named in startup code. */
void LINFLEXD0_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(0U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 1U)
/* Implementation of LINFlexD1 rx handler named in startup code. */
void LINFLEXD1_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(1U);
}

/* Implementation of LINFlexD1 tx handler named in startup code. */
void LINFLEXD1_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(1U);
}

/* Implementation of LINFlexD1 error handler named in startup code. */
void LINFLEXD1_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(1U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 2U)
/* Implementation of LINFlexD2 rx handler named in startup code. */
void LINFLEXD2_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(2U);
}

/* Implementation of LINFlexD2 tx handler named in startup code. */
void LINFLEXD2_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(2U);
}

/* Implementation of LINFlexD2 error handler named in startup code. */
void LINFLEXD2_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(2U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 3U)
/* Implementation of LINFlexD3 rx handler named in startup code. */
void LINFLEXD3_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(3U);
}

/* Implementation of LINFlexD3 tx handler named in startup code. */
void LINFLEXD3_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(3U);
}

/* Implementation of LINFlexD3 error handler named in startup code. */
void LINFLEXD3_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(3U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 4U)
/* Implementation of LINFlexD4 rx handler named in startup code. */
void LINFLEXD4_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(4U);
}

/* Implementation of LINFlexD4 tx handler named in startup code. */
void LINFLEXD4_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(4U);
}

/* Implementation of LINFlexD4 error handler named in startup code. */
void LINFLEXD4_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(4U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 5U)
/* Implementation of LINFlexD5 rx handler named in startup code. */
void LINFLEXD5_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(5U);
}

/* Implementation of LINFlexD5 tx handler named in startup code. */
void LINFLEXD5_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(5U);
}

/* Implementation of LINFlexD5 error handler named in startup code. */
void LINFLEXD5_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(5U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 6U)
/* Implementation of LINFlexD6 rx handler named in startup code. */
void LINFLEXD6_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(6U);
}

/* Implementation of LINFlexD6 tx handler named in startup code. */
void LINFLEXD6_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(6U);
}

/* Implementation of LINFlexD6 error handler named in startup code. */
void LINFLEXD6_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(6U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 7U)
/* Implementation of LINFlexD7 rx handler named in startup code. */
void LINFLEXD7_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(7U);
}

/* Implementation of LINFlexD7 tx handler named in startup code. */
void LINFLEXD7_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(7U);
}

/* Implementation of LINFlexD7 error handler named in startup code. */
void LINFLEXD7_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(7U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 8U)
/* Implementation of LINFlexD8 rx handler named in startup code. */
void LINFLEXD8_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(8U);
}

/* Implementation of LINFlexD8 tx handler named in startup code. */
void LINFLEXD8_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(8U);
}

/* Implementation of LINFlexD8 error handler named in startup code. */
void LINFLEXD8_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(8U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 9U)
/* Implementation of LINFlexD9 rx handler named in startup code. */
void LINFLEXD9_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(9U);
}

/* Implementation of LINFlexD9 tx handler named in startup code. */
void LINFLEXD9_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(9U);
}

/* Implementation of LINFlexD9 error handler named in startup code. */
void LINFLEXD9_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(9U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 10U)
/* Implementation of LINFlexD10 rx handler named in startup code. */
void LINFLEXD10_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(10U);
}

/* Implementation of LINFlexD10 tx handler named in startup code. */
void LINFLEXD10_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(10U);
}

/* Implementation of LINFlexD10 error handler named in startup code. */
void LINFLEXD10_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(10U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 11U)
/* Implementation of LINFlexD11 rx handler named in startup code. */
void LINFLEXD11_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(11U);
}

/* Implementation of LINFlexD11 tx handler named in startup code. */
void LINFLEXD11_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(11U);
}

/* Implementation of LINFlexD11 error handler named in startup code. */
void LINFLEXD11_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(11U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 12U)
/* Implementation of LINFlexD12 rx handler named in startup code. */
void LINFLEXD12_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(12U);
}

/* Implementation of LINFlexD12 tx handler named in startup code. */
void LINFLEXD12_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(12U);
}

/* Implementation of LINFlexD12 error handler named in startup code. */
void LINFLEXD12_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(12U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 13U)
/* Implementation of LINFlexD13 rx handler named in startup code. */
void LINFLEXD13_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(13U);
}

/* Implementation of LINFlexD13 tx handler named in startup code. */
void LINFLEXD13_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(13U);
}

/* Implementation of LINFlexD13 error handler named in startup code. */
void LINFLEXD13_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(13U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 14U)
/* Implementation of LINFlexD14 rx handler named in startup code. */
void LINFLEXD14_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(14U);
}

/* Implementation of LINFlexD14 tx handler named in startup code. */
void LINFLEXD14_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(14U);
}

/* Implementation of LINFlexD14 error handler named in startup code. */
void LINFLEXD14_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(14U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 15U)
/* Implementation of LINFlexD15 rx handler named in startup code. */
void LINFLEXD15_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(15U);
}

/* Implementation of LINFlexD15 tx handler named in startup code. */
void LINFLEXD15_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(15U);
}

/* Implementation of LINFlexD15 error handler named in startup code. */
void LINFLEXD15_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(15U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 16U)
/* Implementation of LINFlexD16 rx handler named in startup code. */
void LINFLEXD16_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(16U);
}

/* Implementation of LINFlexD16 tx handler named in startup code. */
void LINFLEXD16_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(16U);
}

/* Implementation of LINFlexD16 error handler named in startup code. */
void LINFLEXD16_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(16U);
}
#endif

#if (LINFlexD_INSTANCE_COUNT > 17U)
/* Implementation of LINFlexD17 rx handler named in startup code. */
void LINFLEXD17_UART_RxIRQHandler(void)
{
    LINFLEXD_UART_DRV_RxIRQHandler(17U);
}

/* Implementation of LINFlexD17 tx handler named in startup code. */
void LINFLEXD17_UART_TxIRQHandler(void)
{
    LINFLEXD_UART_DRV_TxIRQHandler(17U);
}

/* Implementation of LINFlexD17 error handler named in startup code. */
void LINFLEXD17_UART_ErrIRQHandler(void)
{
    LINFLEXD_UART_DRV_ErrIRQHandler(17U);
}
#endif

/* Rx ISR array for LINFlexD UART driver */
const isr_t g_uartLINFlexDRxIsr[LINFlexD_INSTANCE_COUNT] = {
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    &LINFLEXD0_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 1U)
    &LINFLEXD1_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 2U)
    &LINFLEXD2_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 3U)
    &LINFLEXD3_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 4U)
    &LINFLEXD4_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 5U)
    &LINFLEXD5_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 6U)
    &LINFLEXD6_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 7U)
    &LINFLEXD7_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 8U)
    &LINFLEXD8_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 9U)
    &LINFLEXD9_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 10U)
    &LINFLEXD10_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 11U)
    &LINFLEXD11_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 12U)
    &LINFLEXD12_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 13U)
    &LINFLEXD13_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 14U)
    &LINFLEXD14_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 15U)
    &LINFLEXD15_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 16U)
    &LINFLEXD16_UART_RxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 17U)
    &LINFLEXD17_UART_RxIRQHandler
    #endif
};

/* Tx ISR array for LINFlexD UART driver */
const isr_t g_uartLINFlexDTxIsr[LINFlexD_INSTANCE_COUNT] = {
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    &LINFLEXD0_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 1U)
    &LINFLEXD1_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 2U)
    &LINFLEXD2_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 3U)
    &LINFLEXD3_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 4U)
    &LINFLEXD4_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 5U)
    &LINFLEXD5_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 6U)
    &LINFLEXD6_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 7U)
    &LINFLEXD7_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 8U)
    &LINFLEXD8_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 9U)
    &LINFLEXD9_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 10U)
    &LINFLEXD10_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 11U)
    &LINFLEXD11_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 12U)
    &LINFLEXD12_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 13U)
    &LINFLEXD13_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 14U)
    &LINFLEXD14_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 15U)
    &LINFLEXD15_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 16U)
    &LINFLEXD16_UART_TxIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 17U)
    &LINFLEXD17_UART_TxIRQHandler
    #endif
};

/* Err ISR array for LINFlexD UART driver */
const isr_t g_uartLINFlexDErrIsr[LINFlexD_INSTANCE_COUNT] = {
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    &LINFLEXD0_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 1U)
    &LINFLEXD1_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 2U)
    &LINFLEXD2_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 3U)
    &LINFLEXD3_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 4U)
    &LINFLEXD4_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 5U)
    &LINFLEXD5_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 6U)
    &LINFLEXD6_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 7U)
    &LINFLEXD7_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 8U)
    &LINFLEXD8_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 9U)
    &LINFLEXD9_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 10U)
    &LINFLEXD10_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 11U)
    &LINFLEXD11_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 12U)
    &LINFLEXD12_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 13U)
    &LINFLEXD13_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 14U)
    &LINFLEXD14_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 15U)
    &LINFLEXD15_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 16U)
    &LINFLEXD16_UART_ErrIRQHandler,
    #endif
    #if (LINFlexD_INSTANCE_COUNT > 17U)
    &LINFLEXD17_UART_ErrIRQHandler
    #endif
};

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
