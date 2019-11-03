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
 * @file linflexd_uart_irq.h
 */

#include "linflexd_hw_access.h"

/*!
 * @brief Or'ed interrupt handler, implemented in driver c file.
 */
void LINFLEXD_UART_DRV_IRQHandler(uint32_t instance);

/*!
 * @brief Rx interrupt handler, implemented in driver c file.
 */
void LINFLEXD_UART_DRV_RxIRQHandler(uint32_t instance);

/*!
 * @brief Tx interrupt handler, implemented in driver c file.
 */
void LINFLEXD_UART_DRV_TxIRQHandler(uint32_t instance);

/*!
 * @brief Error interrupt handler, implemented in driver c file.
 */
void LINFLEXD_UART_DRV_ErrIRQHandler(uint32_t instance);

/*!
 * @brief ISR declarations - implemented in linflexd_uart_irq.c.
 */
#if defined (FEATURE_LINFLEXD_ORED_INT_LINES)

    /* ISR declarations for rx/tx/err interrupts or'ed together */
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    void LINFLEXD0_UART_IRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 1U)
    void LINFLEXD1_UART_IRQHandler(void);
    #endif

    /* ISR array for LINFlexD UART driver */
    extern const isr_t g_uartLINFlexDIsr[LINFlexD_INSTANCE_COUNT];

#elif defined (FEATURE_LINFLEXD_RX_TX_ERR_INT_LINES)

    /* ISR declarations for separate rx/tx/err interrupts */
    #if (LINFlexD_INSTANCE_COUNT > 0U)
    void LINFLEXD0_UART_RxIRQHandler(void);
    void LINFLEXD0_UART_TxIRQHandler(void);
    void LINFLEXD0_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 1U)
    void LINFLEXD1_UART_RxIRQHandler(void);
    void LINFLEXD1_UART_TxIRQHandler(void);
    void LINFLEXD1_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 2U)
    void LINFLEXD2_UART_RxIRQHandler(void);
    void LINFLEXD2_UART_TxIRQHandler(void);
    void LINFLEXD2_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 3U)
    void LINFLEXD3_UART_RxIRQHandler(void);
    void LINFLEXD3_UART_TxIRQHandler(void);
    void LINFLEXD3_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 4U)
    void LINFLEXD4_UART_RxIRQHandler(void);
    void LINFLEXD4_UART_TxIRQHandler(void);
    void LINFLEXD4_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 5U)
    void LINFLEXD5_UART_RxIRQHandler(void);
    void LINFLEXD5_UART_TxIRQHandler(void);
    void LINFLEXD5_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 6U)
    void LINFLEXD6_UART_RxIRQHandler(void);
    void LINFLEXD6_UART_TxIRQHandler(void);
    void LINFLEXD6_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 7U)
    void LINFLEXD7_UART_RxIRQHandler(void);
    void LINFLEXD7_UART_TxIRQHandler(void);
    void LINFLEXD7_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 8U)
    void LINFLEXD8_UART_RxIRQHandler(void);
    void LINFLEXD8_UART_TxIRQHandler(void);
    void LINFLEXD8_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 9U)
    void LINFLEXD9_UART_RxIRQHandler(void);
    void LINFLEXD9_UART_TxIRQHandler(void);
    void LINFLEXD9_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 10U)
    void LINFLEXD10_UART_RxIRQHandler(void);
    void LINFLEXD10_UART_TxIRQHandler(void);
    void LINFLEXD10_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 11U)
    void LINFLEXD11_UART_RxIRQHandler(void);
    void LINFLEXD11_UART_TxIRQHandler(void);
    void LINFLEXD11_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 12U)
    void LINFLEXD12_UART_RxIRQHandler(void);
    void LINFLEXD12_UART_TxIRQHandler(void);
    void LINFLEXD12_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 13U)
    void LINFLEXD13_UART_RxIRQHandler(void);
    void LINFLEXD13_UART_TxIRQHandler(void);
    void LINFLEXD13_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 14U)
    void LINFLEXD14_UART_RxIRQHandler(void);
    void LINFLEXD14_UART_TxIRQHandler(void);
    void LINFLEXD14_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 15U)
    void LINFLEXD15_UART_RxIRQHandler(void);
    void LINFLEXD15_UART_TxIRQHandler(void);
    void LINFLEXD15_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 16U)
    void LINFLEXD16_UART_RxIRQHandler(void);
    void LINFLEXD16_UART_TxIRQHandler(void);
    void LINFLEXD16_UART_ErrIRQHandler(void);
    #endif

    #if (LINFlexD_INSTANCE_COUNT > 17U)
    void LINFLEXD17_UART_RxIRQHandler(void);
    void LINFLEXD17_UART_TxIRQHandler(void);
    void LINFLEXD17_UART_ErrIRQHandler(void);
    #endif

    /* Rx ISR array for LINFlexD UART driver */
    extern const isr_t g_uartLINFlexDRxIsr[LINFlexD_INSTANCE_COUNT];

    /* Tx ISR array for LINFlexD UART driver */
    extern const isr_t g_uartLINFlexDTxIsr[LINFlexD_INSTANCE_COUNT];

    /* Err ISR array for LINFlexD UART driver */
    extern const isr_t g_uartLINFlexDErrIsr[LINFlexD_INSTANCE_COUNT];

#endif

/*******************************************************************************
 * EOF
 ******************************************************************************/
