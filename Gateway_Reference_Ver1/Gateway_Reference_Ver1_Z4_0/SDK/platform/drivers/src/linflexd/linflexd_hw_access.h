/*
 * Copyright (c) 2016 , Freescale Semiconductor, Inc.
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
 * @file LINFLEXD.h
 */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Unusual pointer cast (incompatible indirect types)
 * The cast is needed due to memory map differences between instances.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower
 * or different essential type
 * This is required by the conversion of a unsigned value of a bitfield/bit into a enum value.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially
 * unsigned' to 'essentially enum<i>' / 'essentially Boolean' or vice-versa
 * This is required by the conversions of bitfields of registers to/from an enum, bool
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, cast performed between a pointer to
 * object type and a pointer to a different object type
 * The cast is needed due to memory map differences between instances.
 *
 */

#ifndef LINFLEXD_H
#define LINFLEXD_H

#include "device_registers.h"
#include "linflexd_uart_driver.h"
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEFAULT_OSR              16U
#define BAUDRATE_FRACTION_WIDTH  LINFlexD_LINFBRR_FBR_WIDTH

typedef float float32_t;

/*! @brief LINFlexD mode: UART/LIN. */
typedef enum
{
    LINFlexD_LIN_MODE = 0U,
    LINFlexD_UART_MODE = 1U
} linflexd_mode_t;

/*! @brief UART Mode: FIFO/BUFFER. */
typedef enum
{
    LINFLEXD_UART_RXTX_BUFFER_MODE = 0U,
    LINFLEXD_UART_RXTX_FIFO_MODE = 1U
} linflexd_uart_rxtx_mode_t;

/*! @brief LINFlexD state. */
typedef enum
{
    LINFlexD_STATE_SLEEP_MODE = 0U,
    LINFlexD_STATE_INIT_MODE = 1U,
    LINFlexD_STATE_IDLE_MODE = 2U,
    LINFlexD_STATE_SYNC_BREAK = 3U,
    LINFlexD_STATE_SYNC_DEL = 4U,
    LINFlexD_STATE_SYNC_FIELD = 5U,
    LINFlexD_STATE_IDENTIFIER_FIELD = 6U,
    LINFlexD_STATE_HEADER_RX_TX = 7U,
    LINFlexD_STATE_DATA_RX_TX = 8U,
    LINFlexD_STATE_CHECKSUM = 9U
} linflexd_state_t;

/*! @brief UART Status flags.
 *  These identifiers provide both mask and shift for status bits in UARTSR register.
 */
typedef enum
{
    LINFlexD_UART_NOISE_FLAG                   = LINFlexD_UARTSR_NF_MASK,
    LINFlexD_UART_DATA_TRANSMITTED_FLAG        = LINFlexD_UARTSR_DTFTFF_MASK,
    LINFlexD_UART_DATA_RECEPTION_COMPLETE_FLAG = LINFlexD_UARTSR_DRFRFE_MASK,
    LINFlexD_UART_TIMEOUT_FLAG                 = LINFlexD_UARTSR_TO_MASK,
    LINFlexD_UART_WAKEUP_FLAG                  = LINFlexD_UARTSR_WUF_MASK,
    LINFlexD_UART_RECEIVER_INPUT_STATE_FLAG    = LINFlexD_UARTSR_RDI_MASK,
    LINFlexD_UART_BUFFER_OVERRUN_FLAG          = LINFlexD_UARTSR_BOF_MASK,
    LINFlexD_UART_FRAME_ERROR_FLAG             = LINFlexD_UARTSR_FEF_MASK,
    LINFlexD_UART_MESSAGE_BUFFER_FULL_FLAG     = LINFlexD_UARTSR_RMB_MASK,
    LINFlexD_UART_PARITY_ERROR_FLAG            = LINFlexD_UARTSR_PE_MASK,
    LINFlexD_UART_OUTPUT_COMPARE_FLAG          = LINFlexD_UARTSR_OCF_MASK,
    LINFlexD_UART_STUCK_AT_ZERO_FLAG           = LINFlexD_UARTSR_SZF_MASK
} linflexd_uart_status_flag_t;

/*! @brief LINFlexD interrupts.
 *  These identifiers provide both mask and shift for the interrupt enable bits in LINIER register.
 */
typedef enum
{
    LINFlexD_HEADER_RECEIVED_INT         = LINFlexD_LINIER_HRIE_MASK,
    LINFlexD_DATA_TRANSMITTED_INT        = LINFlexD_LINIER_DTIE_MASK,
    LINFlexD_DATA_RECEPTION_COMPLETE_INT = LINFlexD_LINIER_DRIE_MASK,
    LINFlexD_UART_TIMEOUT_ERROR_INT      = LINFlexD_LINIER_TOIE_MASK,
    LINFlexD_WAKEUP_INT                  = LINFlexD_LINIER_WUIE_MASK,
    LINFlexD_LIN_STATE_INT               = LINFlexD_LINIER_LSIE_MASK,
    LINFlexD_BUFFER_OVERRUN_INT          = LINFlexD_LINIER_BOIE_MASK,
    LINFlexD_FRAME_ERROR_INT             = LINFlexD_LINIER_FEIE_MASK,
    LINFlexD_HEADER_ERROR_INT            = LINFlexD_LINIER_HEIE_MASK,
    LINFlexD_CHECKSUM_ERROR_INT          = LINFlexD_LINIER_CEIE_MASK,
    LINFlexD_BIT_ERROR_INT               = LINFlexD_LINIER_BEIE_MASK,
    LINFlexD_OUTPUT_COMPARE_INT          = LINFlexD_LINIER_OCIE_MASK,
    LINFlexD_STUCK_AT_ZERO_INT           = LINFlexD_LINIER_SZIE_MASK
} linflexd_interrupt_t;


#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Requests LINFlexD module to enter init mode.
 *
 * This function enters initialization mode.
 *
 * @param base LINFlexD base pointer.
 */
static inline void LINFLEXD_EnterInitMode(LINFlexD_Type *base)
{
    uint32_t regValTemp;

    regValTemp = base->LINCR1;
    regValTemp &= ~(LINFlexD_LINCR1_SLEEP_MASK);
    regValTemp |= LINFlexD_LINCR1_INIT(1UL);
    base->LINCR1 = regValTemp;
}

/*!
 * @brief Requests LINFlexD module to enter normal mode.
 *
 * This function enters normal mode. *
 *
 * @param base LINFlexD base pointer.
 */
static inline void LINFLEXD_EnterNormalMode(LINFlexD_Type *base)
{
    uint32_t regValTemp;

    regValTemp = base->LINCR1;
    regValTemp &= ~(LINFlexD_LINCR1_SLEEP_MASK);
    regValTemp &= ~(LINFlexD_LINCR1_INIT_MASK);
    base->LINCR1 = regValTemp;
}

/*!
 * @brief Requests LINFlexD module to enter sleep mode.
 *
 * This function enters sleep mode. *
 *
 * @param base LINFlexD base pointer.
 */
static inline void LINFLEXD_EnterSleepMode(LINFlexD_Type *base)
{
    uint32_t regValTemp;

    regValTemp = base->LINCR1;
    regValTemp |= LINFlexD_LINCR1_SLEEP(1UL);
    base->LINCR1 = regValTemp;
}

/*!
 * @brief Sets the LINFlexD mode.
 *
 * This function sets LIN or UART mode, based on the parameter received.
 *
 * @param base LINFlexD base pointer.
 * @param mode LINFlexD mode - UART/LIN.
 */
static inline void LINFLEXD_SetMode(LINFlexD_Type *base, linflexd_mode_t mode)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_UART_MASK);
    regValTemp |= LINFlexD_UARTCR_UART((uint32_t)mode);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Returns the current LINFlexD state.
 *
 * This function returns the state of the LINFlexD instance. The possible states are:
 *    - Sleep mode
 *    - Init mode
 *    - Idle mode
 *    - Sync Break Reception/Transmission
 *    - Sync Delimiter Reception/Transmission
 *    - Sync Field Reception/Transmission
 *    - Identifier Field Reception/Transmission
 *    - Header Reception/Transmission
 *    - Data Reception/Transmission
 *    - Checksum Reception/Transmission
 *
 * @param base LINFlexD base pointer.
 * @return LINFlexD mode - UART/LIN.
 */
static inline linflexd_state_t LINFLEXD_GetLinState(const LINFlexD_Type * base)
{
    uint8_t state = (uint8_t)((base->LINSR & LINFlexD_LINSR_LINS_MASK) >> LINFlexD_LINSR_LINS_SHIFT);
    return (linflexd_state_t)state;
}

/*!
 * @brief Sets the word length.
 *
 * This function sets the word length in UART mode.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param length UART mode word length.
 */
static inline void LINFLEXD_SetUartWordLength(LINFlexD_Type *base, linflexd_uart_word_length_t length)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_WL0_MASK);
    regValTemp |= LINFlexD_UARTCR_WL0((uint32_t)length & 1U);
    regValTemp &= ~(LINFlexD_UARTCR_WL1_MASK);
    regValTemp |= LINFlexD_UARTCR_WL1(((uint32_t)length >> 1U) & 1U);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Enables/disables parity bit.
 *
 * This function enables or disables the parity bit from UART bytes.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param enable true - enable parity queuing/checking, false - disable parity queuing/checking.
 */
static inline void LINFLEXD_SetParityControl(LINFlexD_Type *base, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_PCE_MASK);
    regValTemp |= LINFlexD_UARTCR_PCE(enable ? 1UL : 0UL);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets parity type.
 *
 * This function sets the type of parity to be used for UART bytes. Available options are:
 *    - odd
 *    - even
 *    - always 0
 *    - always 1
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param parityType the type of parity.
 */
static inline void LINFLEXD_SetParityType(LINFlexD_Type *base, linflexd_uart_parity_type_t parityType)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_PC0_MASK);
    regValTemp |= LINFlexD_UARTCR_PC0((uint32_t)parityType & 1U);
    regValTemp &= ~(LINFlexD_UARTCR_PC1_MASK);
    regValTemp |= LINFlexD_UARTCR_PC1(((uint32_t)parityType >> 1U) & 1U);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Enables/disables transmitter.
 *
 * This function enables or disables the UART transmitter, based on the parameter received.
 * Should be called only when the module is in UART mode.
 *
 * @param base LINFlexD base pointer.
 * @param enable enable/disable transmitter.
 */
static inline void LINFLEXD_SetTransmitterState(LINFlexD_Type *base, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_TxEn_MASK);
    regValTemp |= LINFlexD_UARTCR_TxEn(enable ? 1UL : 0UL);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Enables/disables receiver.
 *
 * This function enables or disables the UART receiver, based on the parameter received.
 * Should be called only when the module is in UART mode.
 *
 * @param base LINFlexD base pointer.
 * @param enable enable/disable receiver.
 */
static inline void LINFLEXD_SetReceiverState(LINFlexD_Type *base, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_RxEn_MASK);
    regValTemp |= LINFlexD_UARTCR_RxEn(enable ? 1UL : 0UL);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets the transmission mode (FIFO/Buffer).
 *
 * This function sets the transmission mode (FIFO/Buffer).
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param mode tx mode: FIFO/Buffer.
 */
static inline void LINFLEXD_SetTxMode(LINFlexD_Type *base, linflexd_uart_rxtx_mode_t mode)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_TFBM_MASK);
    regValTemp |= LINFlexD_UARTCR_TFBM((uint32_t)mode);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets the reception mode (FIFO/Buffer).
 *
 * This function sets the reception mode (FIFO/Buffer).
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param mode rx mode: FIFO/Buffer.
 */
static inline void LINFLEXD_SetRxMode(LINFlexD_Type *base, linflexd_uart_rxtx_mode_t mode)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_RFBM_MASK);
    regValTemp |= LINFlexD_UARTCR_RFBM((uint32_t)mode);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets the reception data field length/FIFO depth.
 *
 * This function sets either the reception data field length or the number of bytes in the Rx FIFO, according to
 * the current reception mode.
 *
 * @param base LINFlexD base pointer.
 * @param length data field length/FIFO depth.
 */
static inline void LINFLEXD_SetRxDataFieldLength(LINFlexD_Type *base, uint8_t length)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_RDFL_RFC_MASK);
    regValTemp |= LINFlexD_UARTCR_RDFL_RFC(length);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets the transmission data field length/FIFO depth.
 *
 * This function sets either the transmission data field length or the number of bytes in the Tx FIFO, according to
 * the current transmission mode.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param length data field length/FIFO depth.
 */
static inline void LINFLEXD_SetTxDataFieldLength(LINFlexD_Type *base, uint8_t length)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_TDFL_TFC_MASK);
    regValTemp |= LINFlexD_UARTCR_TDFL_TFC(length);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets the number of stop bits for Rx.
 *
 * This function sets the number of stop bits for received bytes.
 *
 * @param base LINFlexD base pointer.
 * @param stopBitsCount number of stop bits.
 */
static inline void LINFLEXD_SetRxStopBitsCount(LINFlexD_Type *base, linflexd_uart_stop_bits_count_t stopBitsCount)
{
    uint32_t regValTemp;

    regValTemp = base->UARTCR;
    regValTemp &= ~(LINFlexD_UARTCR_SBUR_MASK);
    regValTemp |= LINFlexD_UARTCR_SBUR(stopBitsCount);
    base->UARTCR = regValTemp;
}

/*!
 * @brief Sets fractional baud rate.
 *
 * This function configures the bits that decide the fractional part of the LIN Baud Rate.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param fbr fractional baud rate.
 */
static inline void LINFLEXD_SetFractionalBaudRate(LINFlexD_Type *base, uint8_t fbr)
{
    uint32_t regValTemp;

    regValTemp = base->LINFBRR;
    regValTemp &= ~(LINFlexD_LINFBRR_FBR_MASK);
    regValTemp |= LINFlexD_LINFBRR_FBR(fbr);
    base->LINFBRR = regValTemp;
}

/*!
 * @brief Gets fractional baud rate.
 *
 * This function returns the bits that decide the fractional part of the LIN Baud Rate.
 *
 * @param base LINFlexD base pointer.
 * @return fractional baud rate.
 */
static inline uint8_t LINFLEXD_GetFractionalBaudRate(const LINFlexD_Type * base)
{
    return (uint8_t)((base->LINFBRR & LINFlexD_LINFBRR_FBR_MASK) >> LINFlexD_LINFBRR_FBR_SHIFT);
}

/*!
 * @brief Sets integer baud rate.
 *
 * This function configures the bits that decide the integer part of the LIN Baud Rate.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param fbr integer baud rate.
 */
static inline void LINFLEXD_SetIntegerBaudRate(LINFlexD_Type *base, uint32_t ibr)
{
    uint32_t regValTemp;

    regValTemp = base->LINIBRR;
    regValTemp &= ~(LINFlexD_LINIBRR_IBR_MASK);
    regValTemp |= LINFlexD_LINIBRR_IBR(ibr);
    base->LINIBRR = regValTemp;
}

/*!
 * @brief Gets integer baud rate.
 *
 * This function returns the bits that decide the integer part of the LIN Baud Rate.
 *
 * @param base LINFlexD base pointer.
 * @return integer baud rate.
 */
static inline uint32_t LINFLEXD_GetIntegerBaudRate(const LINFlexD_Type * base)
{
    return (uint32_t)((base->LINIBRR & LINFlexD_LINIBRR_IBR_MASK) >> LINFlexD_LINIBRR_IBR_SHIFT);
}

/*!
 * @brief Sets the number of stop bits for Tx.
 *
 * This function sets the number of stop bits for transmitted bytes.
 * Should be called only when the module is in init mode.
 *
 * @param base LINFlexD base pointer.
 * @param defaultOffset - true - use the memory map defined in the header file to access the register;
 *                      - false - use custom memory map to access the register.
 * @param stopBitsCount number of stop bits.
 */
static inline void LINFLEXD_SetTxStopBitsCount(LINFlexD_Type *base, linflexd_uart_stop_bits_count_t stopBitsCount,
                                               bool defaultOffset)
{
    uint32_t regValTemp;

#if defined(FEATURE_LINFLEXD_HAS_DIFFERENT_MEM_MAP)
    if (!defaultOffset)
    {
        regValTemp = ((LINFlexD_0IFCR_Type *)base)->GCR;
        regValTemp &= ~(LINFlexD_GCR_STOP_MASK);
        regValTemp |= LINFlexD_GCR_STOP(stopBitsCount);
        ((LINFlexD_0IFCR_Type *)base)->GCR = regValTemp;
    }
    else
    {
        regValTemp = base->GCR;
        regValTemp &= ~(LINFlexD_GCR_STOP_MASK);
        regValTemp |= LINFlexD_GCR_STOP(stopBitsCount);
        base->GCR = regValTemp;
    }
#else
    (void)defaultOffset;
    regValTemp = base->GCR;
    regValTemp &= ~(LINFlexD_GCR_STOP_MASK);
    regValTemp |= LINFlexD_GCR_STOP(stopBitsCount);
    base->GCR = regValTemp;
#endif
}

/*!
 * @brief Clears an UART interrupt flag.
 *
 * This function clears the UART status flag received as parameter.
 *
 * @param base LINFlexD base pointer.
 * @param flag UART status flag.
 */
static inline void LINFLEXD_ClearStatusFlag(LINFlexD_Type * base, linflexd_uart_status_flag_t flag)
{
    base->UARTSR = flag;
}

/*!
 * @brief Returns an UART interrupt flag.
 *
 * This function returns the value of the UART status flag received as parameter.
 *
 * @param base LINFlexD base pointer.
 * @return true - UART status flag set, false - UART status flag reset.
 */
static inline bool LINFLEXD_GetStatusFlag(const LINFlexD_Type * base, linflexd_uart_status_flag_t flag)
{
    return ((base->UARTSR & (uint32_t)flag) != 0U);
}

/*!
 * @brief Enables/disables an UART interrupt.
 *
 * This function configures whether the UART event will trigger an interrupt.
 *
 * @param base LINFlexD base pointer.
 * @param intSrc UART interrupt source.
 * @param enable true - enable interrupt, false - disable interrupt.
 */
static inline void LINFLEXD_SetInterruptMode(LINFlexD_Type * base, linflexd_interrupt_t intSrc, bool enable)
{
    uint32_t regValTemp;

    regValTemp = base->LINIER;
    if (enable)
    {
        regValTemp |= (uint32_t)intSrc;
    }
    else
    {
        regValTemp &= ~(uint32_t)intSrc;
    }
    base->LINIER = regValTemp;
}

/*!
 * @brief Returns the state of an UART interrupt.
 *
 * This function returns whether the UART event will trigger an interrupt.
 *
 * @param base LINFlexD base pointer.
 * @param intSrc UART interrupt source.
 * @return true - interrupt enabled, false - interrupt disabled.
 */
static inline bool LINFLEXD_IsInterruptEnabled(const LINFlexD_Type * base, linflexd_interrupt_t intSrc)
{
    return ((base->LINIER & (uint32_t)intSrc) != 0U);
}

/*!
 * @brief Sets the first byte of the tx data buffer.
 *
 * This function writes one byte to the tx data buffer.
 *
 * @param base LINFlexD base pointer.
 * @param data data byte.
 */
static inline void LINFLEXD_SetTxDataBuffer1Byte(LINFlexD_Type * base, uint8_t data)
{
    uint32_t regValTemp;

    regValTemp = base->BDRL;
    regValTemp &= ~(LINFlexD_BDRL_DATA0_MASK);
    regValTemp |= LINFlexD_BDRL_DATA0(data);
    base->BDRL = regValTemp;
}

/*!
 * @brief Sets the first half-word of the tx data buffer.
 *
 * This function writes two bytes to the tx data buffer.
 *
 * @param base LINFlexD base pointer.
 * @param data data half-word.
 */
static inline void LINFLEXD_SetTxDataBuffer2Bytes(LINFlexD_Type * base, uint16_t data)
{
    uint32_t mask, regValTemp;
    mask = LINFlexD_BDRL_DATA0_MASK | LINFlexD_BDRL_DATA1_MASK;
    regValTemp = base->BDRL;
    regValTemp &= ~(mask);
    regValTemp |= ((uint32_t)data & mask);
    base->BDRL = regValTemp;
}

/*!
 * @brief Gets the first byte of the rx data buffer.
 *
 * This function retrieves one byte from the rx data buffer.
 *
 * @param base LINFlexD base pointer.
 * @return data byte.
 */
static inline uint8_t LINFLEXD_GetRxDataBuffer1Byte(const LINFlexD_Type * base)
{
    return (uint8_t)((base->BDRM & LINFlexD_BDRM_DATA4_MASK) >> LINFlexD_BDRM_DATA4_SHIFT);
}

/*!
 * @brief Gets the first half-word of the rx data buffer.
 *
 * This function retrieves two bytes from the rx data buffer.
 *
 * @param base LINFlexD base pointer.
 * @return data half-word.
 */
static inline uint16_t LINFLEXD_GetRxDataBuffer2Bytes(const LINFlexD_Type * base)
{
    uint32_t mask = LINFlexD_BDRM_DATA4_MASK | LINFlexD_BDRM_DATA5_MASK;
    return (uint16_t)(base->BDRM & mask);
}

#if defined(__cplusplus)
}
#endif

#endif /* LINFLEXD_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
