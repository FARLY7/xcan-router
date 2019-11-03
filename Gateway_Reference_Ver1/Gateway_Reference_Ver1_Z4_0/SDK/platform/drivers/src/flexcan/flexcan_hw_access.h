/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
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
 * @file flexcan_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * There are some global macros used for accessing different fields of CAN frames
 * which might also be useful to the user.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 */

#ifndef FLEXCAN_HW_ACCESS_H
#define FLEXCAN_HW_ACCESS_H

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "flexcan_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief FlexCAN message buffer CODE for Rx buffers*/
enum {
    FLEXCAN_RX_INACTIVE  = 0x0, /*!< MB is not active.*/
    FLEXCAN_RX_FULL      = 0x2, /*!< MB is full.*/
    FLEXCAN_RX_EMPTY     = 0x4, /*!< MB is active and empty.*/
    FLEXCAN_RX_OVERRUN   = 0x6, /*!< MB is overwritten into a full buffer.*/
    FLEXCAN_RX_BUSY      = 0x8, /*!< FlexCAN is updating the contents of the MB.*/
                                /*!  The CPU must not access the MB.*/
    FLEXCAN_RX_RANSWER   = 0xA, /*!< A frame was configured to recognize a Remote Request Frame*/
                                /*!  and transmit a Response Frame in return.*/
    FLEXCAN_RX_NOT_USED   = 0xF /*!< Not used*/
};

/*! @brief FlexCAN message buffer CODE FOR Tx buffers*/
enum {
    FLEXCAN_TX_INACTIVE  = 0x08, /*!< MB is not active.*/
    FLEXCAN_TX_ABORT     = 0x09, /*!< MB is aborted.*/
    FLEXCAN_TX_DATA      = 0x0C, /*!< MB is a TX Data Frame(MB RTR must be 0).*/
    FLEXCAN_TX_REMOTE    = 0x1C, /*!< MB is a TX Remote Request Frame (MB RTR must be 1).*/
    FLEXCAN_TX_TANSWER   = 0x0E, /*!< MB is a TX Response Request Frame from.*/
                                 /*!  an incoming Remote Request Frame.*/
    FLEXCAN_TX_NOT_USED   = 0xF  /*!< Not used*/
};

/*! @brief FlexCAN message buffer transmission types*/
enum {
    FLEXCAN_MB_STATUS_TYPE_TX,          /*!< Transmit MB*/
    FLEXCAN_MB_STATUS_TYPE_TX_REMOTE,   /*!< Transmit remote request MB*/
    FLEXCAN_MB_STATUS_TYPE_RX,          /*!< Receive MB*/
    FLEXCAN_MB_STATUS_TYPE_RX_REMOTE,   /*!< Receive remote request MB*/
    FLEXCAN_MB_STATUS_TYPE_RX_TX_REMOTE /*!< FlexCAN remote frame receives remote request and*/
                                        /*!  transmits MB.*/
};

/*! @brief FlexCAN error interrupt types
 */
typedef enum {
    FLEXCAN_INT_RX_WARNING = CAN_CTRL1_RWRNMSK_MASK,     /*!< RX warning interrupt*/
    FLEXCAN_INT_TX_WARNING = CAN_CTRL1_TWRNMSK_MASK,     /*!< TX warning interrupt*/
    FLEXCAN_INT_ERR        = CAN_CTRL1_ERRMSK_MASK,      /*!< Error interrupt*/
    FLEXCAN_INT_BUSOFF     = CAN_CTRL1_BOFFMSK_MASK,     /*!< Bus off interrupt*/
} flexcan_int_type_t;

/*! @brief FlexCAN Message Buffer code and status for transmit and receive
 */
typedef struct {
    uint32_t code;                        /*!< MB code for TX or RX buffers.*/
                                          /*! Defined by flexcan_mb_code_rx_t and flexcan_mb_code_tx_t */
    flexcan_msgbuff_id_type_t msgIdType;  /*!< Type of message ID (standard or extended)*/
    uint32_t dataLen;                     /*!< Length of Data in Bytes*/
    bool fd_enable;
    uint8_t fd_padding;
    bool enable_brs;                   /* Enable bit rate switch*/
} flexcan_msgbuff_code_status_t;

#define CAN_ID_EXT_MASK                          0x3FFFFu
#define CAN_ID_EXT_SHIFT                         0
#define CAN_ID_EXT_WIDTH                         18

#define CAN_ID_STD_MASK                          0x1FFC0000u
#define CAN_ID_STD_SHIFT                         18
#define CAN_ID_STD_WIDTH                         11

#define CAN_ID_PRIO_MASK                         0xE0000000u
#define CAN_ID_PRIO_SHIFT                        29
#define CAN_ID_PRIO_WIDTH                        3
/* CS Bit Fields */
#define CAN_CS_TIME_STAMP_MASK                   0xFFFFu
#define CAN_CS_TIME_STAMP_SHIFT                  0
#define CAN_CS_TIME_STAMP_WIDTH                  16

#define CAN_CS_DLC_MASK                          0xF0000u
#define CAN_CS_DLC_SHIFT                         16
#define CAN_CS_DLC_WIDTH                         4

#define CAN_CS_RTR_MASK                          0x100000u
#define CAN_CS_RTR_SHIFT                         20
#define CAN_CS_RTR_WIDTH                         1

#define CAN_CS_IDE_MASK                          0x200000u
#define CAN_CS_IDE_SHIFT                         21
#define CAN_CS_IDE_WIDTH                         1

#define CAN_CS_SRR_MASK                          0x400000u
#define CAN_CS_SRR_SHIFT                         22
#define CAN_CS_SRR_WIDTH                         1

#define CAN_CS_CODE_MASK                         0xF000000u
#define CAN_CS_CODE_SHIFT                        24
#define CAN_CS_CODE_WIDTH                        4

#define CAN_MB_EDL_MASK                          0x80000000u
#define CAN_MB_BRS_MASK                          0x40000000u

/*! @brief FlexCAN endianness handling */
#ifdef CORE_BIG_ENDIAN
    #define FlexcanSwapBytesInWordIndex(index) (index)
    #define FlexcanSwapBytesInWord(a, b) (b = a)
#elif defined CORE_LITTLE_ENDIAN
    #define FlexcanSwapBytesInWordIndex(index) (((index) & ~3U) + (3U - ((index) & 3U)))
    #define FlexcanSwapBytesInWord(a, b) REV_BYTES_32(a, b)
#else
    #error "No endianness defined!"
#endif


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Configuration
 * @{
 */

/*!
 * @brief Enables FlexCAN controller.
 *
 * @param   base    The FlexCAN base address
 */
void FLEXCAN_Enable(CAN_Type * base);

/*!
 * @brief Disables FlexCAN controller.
 *
 * @param   base    The FlexCAN base address
 */
void FLEXCAN_Disable(CAN_Type * base);

#if FEATURE_CAN_HAS_PE_CLKSRC_SELECT
/*!
 * @brief Selects the clock source for FlexCAN.
 *
 * @param   base The FlexCAN base address
 * @param   clk         The FlexCAN clock source
 */
static inline void FLEXCAN_SelectClock(CAN_Type * base, flexcan_clk_source_t clk)
{
    base->CTRL1 = (base->CTRL1 & ~CAN_CTRL1_CLKSRC_MASK) | CAN_CTRL1_CLKSRC(clk);
}
#endif

/*!
 * @brief Initializes the FlexCAN controller.
 *
 * @param   base  The FlexCAN base address
 */
void FLEXCAN_Init(CAN_Type * base);

/*!
 * @brief Sets the FlexCAN time segments for setting up bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetTimeSegments(CAN_Type * base, const flexcan_time_segment_t *timeSeg)
{
    DEV_ASSERT(timeSeg != NULL);

    (base->CTRL1) = ((base->CTRL1) & ~((CAN_CTRL1_PROPSEG_MASK | CAN_CTRL1_PSEG2_MASK |
                                        CAN_CTRL1_PSEG1_MASK | CAN_CTRL1_PRESDIV_MASK) |
                                        CAN_CTRL1_RJW_MASK));

    (base->CTRL1) = ((base->CTRL1) | (CAN_CTRL1_PROPSEG(timeSeg->propSeg) |
                                      CAN_CTRL1_PSEG2(timeSeg->phaseSeg2) |
                                      CAN_CTRL1_PSEG1(timeSeg->phaseSeg1) |
                                      CAN_CTRL1_PRESDIV(timeSeg->preDivider) |
                                      CAN_CTRL1_RJW(timeSeg->rJumpwidth)));
}

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Sets the FlexCAN extended time segments for setting up bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetExtendedTimeSegments(CAN_Type * base, const flexcan_time_segment_t *timeSeg)
{
    DEV_ASSERT(timeSeg != NULL);

    /* If extended bit time definitions are enabled, use CBT register */
    (base->CBT) = ((base->CBT) & ~((CAN_CBT_EPROPSEG_MASK | CAN_CBT_EPSEG2_MASK |
                                    CAN_CBT_EPSEG1_MASK | CAN_CBT_EPRESDIV_MASK) |
                                    CAN_CBT_ERJW_MASK));

    (base->CBT) = ((base->CBT) | (CAN_CBT_EPROPSEG(timeSeg->propSeg) |
                                  CAN_CBT_EPSEG2(timeSeg->phaseSeg2) |
                                  CAN_CBT_EPSEG1(timeSeg->phaseSeg1) |
                                  CAN_CBT_EPRESDIV(timeSeg->preDivider) |
                                  CAN_CBT_ERJW(timeSeg->rJumpwidth)));
}

/*!
 * @brief Sets the FlexCAN time segments for setting up bit rate for FD BRS.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments, which need to be set for the bit rate.
 */
static inline void FLEXCAN_SetFDTimeSegments(CAN_Type * base, const flexcan_time_segment_t *timeSeg)
{
    DEV_ASSERT(timeSeg != NULL);

    /* Set FlexCAN time segments*/
    (base->FDCBT) = ((base->FDCBT) & ~((CAN_FDCBT_FPROPSEG_MASK | CAN_FDCBT_FPSEG2_MASK |
                                        CAN_FDCBT_FPSEG1_MASK | CAN_FDCBT_FPRESDIV_MASK) |
                                        CAN_FDCBT_FRJW_MASK));

    (base->FDCBT) = ((base->FDCBT) | (CAN_FDCBT_FPROPSEG(timeSeg->propSeg) |
                                      CAN_FDCBT_FPSEG2(timeSeg->phaseSeg2) |
                                      CAN_FDCBT_FPSEG1(timeSeg->phaseSeg1) |
                                      CAN_FDCBT_FPRESDIV(timeSeg->preDivider) |
                                      CAN_FDCBT_FRJW(timeSeg->rJumpwidth)));
}
#endif

/*!
 * @brief Gets the FlexCAN time segments to calculate the bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments read for bit rate
 */
static inline void FLEXCAN_GetTimeSegments(const CAN_Type * base, flexcan_time_segment_t *timeSeg)
{
    DEV_ASSERT(timeSeg != NULL);

    timeSeg->preDivider = ((base->CTRL1) & CAN_CTRL1_PRESDIV_MASK) >> CAN_CTRL1_PRESDIV_SHIFT;
    timeSeg->propSeg = ((base->CTRL1) & CAN_CTRL1_PROPSEG_MASK) >> CAN_CTRL1_PROPSEG_SHIFT;
    timeSeg->phaseSeg1 = ((base->CTRL1) & CAN_CTRL1_PSEG1_MASK) >> CAN_CTRL1_PSEG1_SHIFT;
    timeSeg->phaseSeg2 = ((base->CTRL1) & CAN_CTRL1_PSEG2_MASK) >> CAN_CTRL1_PSEG2_SHIFT;
    timeSeg->rJumpwidth = ((base->CTRL1) & CAN_CTRL1_RJW_MASK) >> CAN_CTRL1_RJW_SHIFT;
}

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Gets the  FlexCAN time segments for FD BRS to calculate the bit rate.
 *
 * @param   base The FlexCAN base address
 * @param   timeSeg    FlexCAN time segments read for bit rate
 */
static inline void FLEXCAN_GetFDTimeSegments(const CAN_Type * base, flexcan_time_segment_t *timeSeg)
{
    DEV_ASSERT(timeSeg != NULL);

    timeSeg->preDivider = ((base->FDCBT) & CAN_FDCBT_FPRESDIV_MASK) >> CAN_FDCBT_FPRESDIV_SHIFT;
    timeSeg->propSeg = ((base->FDCBT) & CAN_FDCBT_FPROPSEG_MASK) >> CAN_FDCBT_FPROPSEG_SHIFT;
    timeSeg->phaseSeg1 = ((base->FDCBT) & CAN_FDCBT_FPSEG1_MASK) >> CAN_FDCBT_FPSEG1_SHIFT;
    timeSeg->phaseSeg2 = ((base->FDCBT) & CAN_FDCBT_FPSEG2_MASK) >> CAN_FDCBT_FPSEG2_SHIFT;
    timeSeg->rJumpwidth = ((base->FDCBT) & CAN_FDCBT_FRJW_MASK) >> CAN_FDCBT_FRJW_SHIFT;
}
#endif

/*!
 * @brief Un freezes the FlexCAN module.
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_ExitFreezeMode(CAN_Type * base);

/*!
 * @brief Freezes the FlexCAN module.
 * In case of Bus-Off error  this function can Soft reset,
 * the FlexCan interface, check errata 9595
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_EnterFreezeMode(CAN_Type * base);

/*!
 * @brief Set operation mode.
 *
 * @param   base  The FlexCAN base address
 * @param   mode  Set an operation mode
 */
void FLEXCAN_SetOperationMode(
    CAN_Type * base,
    flexcan_operation_modes_t mode);

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Enables/Disables Flexible Data rate (if supported).
 *
 * @param   base    The FlexCAN base address
 * @param   enable  true to enable; false to disable
 */
static inline void FLEXCAN_SetFDEnabled(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_FDEN_MASK) | CAN_MCR_FDEN(enable? 1UL : 0UL);

    /* Enable the use of extended bit time definitions */
    base->CBT = base->CBT | CAN_CBT_BTF(enable? 1UL : 0UL);
}

/*!
 * @brief Checks if the Flexible Data rate feature is enabled.
 *
 * @param   base    The FlexCAN base address
 * @return  true if enabled; false if disabled
 */
static inline bool FLEXCAN_IsFDEnabled(const CAN_Type * base)
{
    return (((base->MCR & CAN_MCR_FDEN_MASK) >> CAN_MCR_FDEN_SHIFT) != 0U);
}
#endif

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Sets the payload size of the MBs.
 *
 * @param   base         The FlexCAN base address
 * @param   payloadSize  The payload size
 */
void FLEXCAN_SetPayloadSize(
    CAN_Type * base,
    flexcan_fd_payload_size_t payloadSize);

/*!
 * @brief Gets the payload size of the MBs.
 *
 * @param   base         The FlexCAN base address
 * @return  The payload size in bytes
 */
uint8_t FLEXCAN_GetPayloadSize(const CAN_Type * base);
#endif

/*@}*/

/*!
 * @name Data transfer
 * @{
 */

/*!
 * @brief Sets the FlexCAN message buffer fields for transmitting.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   cs           CODE/status values (TX)
 * @param   msgId       ID of the message to transmit
 * @param   msgData      Bytes of the FlexCAN message
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetTxMsgBuff(
    CAN_Type * base,
    uint32_t msgBuffIdx,
    const flexcan_msgbuff_code_status_t *cs,
    uint32_t msgId,
    const uint8_t *msgData);

/*!
 * @brief Writes the abort code into the CODE field of the requested Tx message
 * buffer.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx Index of the message buffer
 */
void FLEXCAN_AbortTxMsgBuff(CAN_Type * base, uint32_t msgBuffIdx);

/*!
 * @brief Sets the FlexCAN message buffer fields for receiving.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   cs           CODE/status values (RX)
 * @param   msgId       ID of the message to receive
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetRxMsgBuff(
    CAN_Type * base,
    uint32_t msgBuffIdx,
    const flexcan_msgbuff_code_status_t *cs,
    uint32_t msgId);

/*!
 * @brief Gets the FlexCAN message buffer fields.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   msgBuff           The fields of the message buffer
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_GetMsgBuff(
    CAN_Type * base,
    uint32_t msgBuffIdx,
    flexcan_msgbuff_t *msgBuff);

/*!
 * @brief Locks the FlexCAN Rx message buffer.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_LockRxMsgBuff(
    CAN_Type * base,
    uint32_t msgBuffIdx);

/*!
 * @brief Unlocks the FlexCAN Rx message buffer.
 *
 * @param   base     The FlexCAN base address
 */
static inline void FLEXCAN_UnlockRxMsgBuff(const CAN_Type * base)
{
    /* Unlock the mailbox by reading the free running timer */
    (void)base->TIMER;
}

/*!
 * @brief Enables the Rx FIFO.
 *
 * @param   base     The FlexCAN base address
 * @param   numOfFilters    The number of Rx FIFO filters
 * @return  The status of the operation
 * @retval  STATUS_SUCCESS RxFIFO was successfully enabled
 * @retval  STATUS_ERROR RxFIFO could not be enabled (e.g. the FD feature
 *          was enabled, and these two features are not compatible)
 */
status_t FLEXCAN_EnableRxFifo(CAN_Type * base, uint32_t numOfFilters);

/*!
 * @brief Checks if Rx FIFO is enabled.
 *
 * @param   base     The FlexCAN base address
 * @return  RxFifo status (true = enabled / false = disabled)
 */
static inline bool FLEXCAN_IsRxFifoEnabled(const CAN_Type * base)
{
    return (((base->MCR & CAN_MCR_RFEN_MASK) >> CAN_MCR_RFEN_SHIFT) != 0U);
}

/*!
 * @brief Sets the number of the Rx FIFO filters.
 *
 * @param   base  The FlexCAN base address
 * @param   number       The number of Rx FIFO filters
 */
static inline void FLEXCAN_SetRxFifoFilterNum(CAN_Type * base, uint32_t number)
{
    (base->CTRL2) = (((base->CTRL2) & ~(CAN_CTRL2_RFFN_MASK)) |
                     (((uint32_t)(((uint32_t)(number)) << CAN_CTRL2_RFFN_SHIFT)) &
                      CAN_CTRL2_RFFN_MASK));
}

/*!
 * @brief Sets  the maximum number of Message Buffers.
 *
 * @param   base  The FlexCAN base address
 * @param   maxMsgBuffNum     Maximum number of message buffers
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetMaxMsgBuffNum(
    CAN_Type * base,
    uint32_t maxMsgBuffNum);

/*!
 * @brief Gets the maximum number of Message Buffers.
 *
 * @param   base  The FlexCAN base address
 * @return  the configured number of message buffers
 */
static inline uint32_t FLEXCAN_GetMaxMsgBuffNum(const CAN_Type * base)
{
    return (((base->MCR) & CAN_MCR_MAXMB_MASK) >> CAN_MCR_MAXMB_SHIFT);
}

/*!
 * @brief Sets the FlexCAN Rx FIFO fields.
 *
 * @param   base             The FlexCAN base address
 * @param   idFormat         The format of the Rx FIFO ID Filter Table Elements
 * @param   idFilterTable    The ID filter table elements which contain RTR bit,
 *                           IDE bit, and RX message ID.
 */
void FLEXCAN_SetRxFifoFilter(
    CAN_Type * base,
    flexcan_rx_fifo_id_element_format_t idFormat,
    const flexcan_id_table_t *idFilterTable);

/*!
 * @brief Gets the FlexCAN Rx FIFO data.
 *
 * @param   base    The FlexCAN base address
 * @param   rxFifo  The FlexCAN receive FIFO data
 */
void FLEXCAN_ReadRxFifo(
    const CAN_Type * base,
    flexcan_msgbuff_t *rxFifo);

/*@}*/

/*!
 * @name Interrupts
 * @{
 */

/*!
 * @brief Enables/Disables the FlexCAN Message Buffer interrupt.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   enable       choose enable or disable
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
status_t FLEXCAN_SetMsgBuffIntCmd(
    CAN_Type * base,
    uint32_t msgBuffIdx, bool enable);

/*!
 * @brief Enables error interrupt of the FlexCAN module.
 * @param   base     The FlexCAN base address
 * @param   errType     The interrupt type
 * @param   enable       choose enable or disable
 */
void FLEXCAN_SetErrIntCmd(CAN_Type * base, flexcan_int_type_t errType, bool enable);

/*@}*/

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Gets the individual FlexCAN MB interrupt flag.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @return  the individual Message Buffer interrupt flag (0 and 1 are the flag value)
 */
static inline uint8_t FLEXCAN_GetMsgBuffIntStatusFlag(const CAN_Type * base, uint32_t msgBuffIdx)
{
    uint8_t flag = 0;
    uint32_t mask;

    if (msgBuffIdx < 32U)
    {
        mask = base->IMASK1 & CAN_IMASK1_BUF31TO0M_MASK;
        flag = (uint8_t)(((base->IFLAG1 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }
#if FEATURE_CAN_MAX_MB_NUM > 32U
    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U))
    {
        mask = base->IMASK2 & CAN_IMASK2_BUF63TO32M_MASK;
        flag = (uint8_t)(((base->IFLAG2 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }
#endif
#if FEATURE_CAN_MAX_MB_NUM > 64U
    if (msgBuffIdx >= 64U)
    {
        mask = base->IMASK3 & CAN_IMASK3_BUF95TO64M_MASK;
        flag = (uint8_t)(((base->IFLAG3 & mask) >> (msgBuffIdx % 32U)) & 1U);
    }
#endif

    return flag;
}

/*!
 * @brief Clears the interrupt flag of the message buffers.
 *
 * @param   base  The FlexCAN base address
 * @param   flag      The value to be written to the interrupt flag1 register.
 */
static inline void FLEXCAN_ClearMsgBuffIntStatusFlag(CAN_Type * base, uint32_t msgBuffIdx)
{
    uint32_t flag = ((uint32_t)1U << (msgBuffIdx % 32U));

    /* Clear the corresponding message buffer interrupt flag*/
    if (msgBuffIdx < 32U)
    {
        (base->IFLAG1) = (flag);
    }
#if FEATURE_CAN_MAX_MB_NUM > 32U
    if ((msgBuffIdx >= 32U) && (msgBuffIdx < 64U))
    {
        (base->IFLAG2) = (flag);
    }
#endif
#if FEATURE_CAN_MAX_MB_NUM > 64U
    if (msgBuffIdx >= 64U)
    {
        (base->IFLAG3) = (flag);
    }
#endif
}

/*!
 * @brief Clears all other interrupts in ERRSTAT register (Error, Busoff, Wakeup).
 *
 * @param   base     The FlexCAN base address
 */
void FLEXCAN_ClearErrIntStatusFlag(CAN_Type * base);

/*!
 * @brief Gets the error counters.
 *
 * @param   base  The FlexCAN base address
 * @return  the value of the FlexCAN error counters
 */
static inline uint32_t FLEXCAN_GetErrorCounters(const CAN_Type * base)
{
    return base->ECR;
}

/*@}*/

/*!
 * @name Mask
 * @{
 */

/*!
 * @brief Sets the Rx masking type.
 *
 * @param   base  The FlexCAN base address
 * @param   type         The FlexCAN Rx mask type
 */
static inline void FLEXCAN_SetRxMaskType(CAN_Type * base, flexcan_rx_mask_type_t type)
{
    /* Set RX masking type (RX global mask or RX individual mask)*/
    if (type == FLEXCAN_RX_MASK_GLOBAL)
    {
        /* Enable Global RX masking */
        base->MCR = (base->MCR & ~CAN_MCR_IRMQ_MASK) | CAN_MCR_IRMQ(0U);
    }
    else
    {
        /* Enable Individual Rx Masking and Queue */
        base->MCR = (base->MCR & ~CAN_MCR_IRMQ_MASK) | CAN_MCR_IRMQ(1U);
    }
}

/*!
 * @brief Sets the FlexCAN RX FIFO global standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxFifoGlobalStdMask(CAN_Type * base, uint32_t stdMask)
{
    (base->RXFGMASK) = (((uint32_t)(((uint32_t)(stdMask)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK);
}

/*!
 * @brief Sets the FlexCAN Rx FIFO global extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxFifoGlobalExtMask(CAN_Type * base, uint32_t extMask)
{
    (base->RXFGMASK) = (((uint32_t)(((uint32_t)(extMask)) << CAN_ID_EXT_SHIFT)) &
                        (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));
}

/*!
 * @brief Sets the FlexCAN Rx individual standard mask for ID filtering in the Rx MBs and the Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   stdMask     Individual standard mask
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
static inline void FLEXCAN_SetRxIndividualStdMask(
    CAN_Type * base,
    uint32_t msgBuffIdx,
    uint32_t stdMask)
{
    (base->RXIMR[msgBuffIdx]) = (stdMask << CAN_ID_STD_SHIFT) & CAN_ID_STD_MASK;
}

/*!
 * @brief Sets the FlexCAN Rx individual extended mask for ID filtering in the Rx Message Buffers and the Rx FIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   msgBuffIdx       Index of the message buffer
 * @param   extMask     Individual extended mask
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_FLEXCAN_MB_OUT_OF_RANGE if the index of the
 *          message buffer is invalid
 */
static inline void FLEXCAN_SetRxIndividualExtMask(
    CAN_Type * base,
    uint32_t msgBuffIdx,
    uint32_t extMask)
{
    base->RXIMR[msgBuffIdx] = (extMask << CAN_ID_EXT_SHIFT) & (CAN_ID_STD_MASK | CAN_ID_EXT_MASK);
}

/*!
 * @brief Sets the FlexCAN Rx Message Buffer global standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuffGlobalStdMask(CAN_Type * base, uint32_t stdMask)
{
    (base->RXMGMASK) = (((uint32_t)(((uint32_t)(stdMask)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK);
}

/*!
 * @brief Sets the FlexCAN RX Message Buffer global extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuffGlobalExtMask(CAN_Type * base, uint32_t extMask)
{
    (base->RXMGMASK) = (((uint32_t)(((uint32_t)(extMask)) << CAN_ID_EXT_SHIFT)) &
                        (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));
}

/*!
 * @brief Sets the FlexCAN Rx Message Buffer 14 standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuff14StdMask(CAN_Type * base, uint32_t stdMask)
{
    (base->RX14MASK) = (((uint32_t)(((uint32_t)(stdMask)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK);
}

/*!
 * @brief Sets the FlexCAN RX Message Buffer 14 extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuff14ExtMask(CAN_Type * base, uint32_t extMask)
{
    (base->RX14MASK) = (((uint32_t)(((uint32_t)(extMask)) << CAN_ID_EXT_SHIFT)) &
                        (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));
}

/*!
 * @brief Sets the FlexCAN Rx Message Buffer 15 standard mask.
 *
 * @param   base  The FlexCAN base address
 * @param   stdMask     Standard mask
 */
static inline void FLEXCAN_SetRxMsgBuff15StdMask(CAN_Type * base, uint32_t stdMask)
{
    (base->RX15MASK) = (((uint32_t)(((uint32_t)(stdMask)) << CAN_ID_STD_SHIFT)) & CAN_ID_STD_MASK);
}

/*!
 * @brief Sets the FlexCAN RX Message Buffer 15 extended mask.
 *
 * @param   base  The FlexCAN base address
 * @param   extMask     Extended mask
 */
static inline void FLEXCAN_SetRxMsgBuff15ExtMask(CAN_Type * base, uint32_t extMask)
{
    (base->RX15MASK) = (((uint32_t)(((uint32_t)(extMask)) << CAN_ID_EXT_SHIFT)) &
                        (CAN_ID_STD_MASK | CAN_ID_EXT_MASK));
}

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Enables/Disables the Stuff Bit Count for CAN FD frames.
 *
 * If enabled, the modulo 8 count of variable stuff bits inserted plus the respective
 * parity bit (even parity calculated over the 3-bit modulo 8 count) are combined as
 * the 4-bit Stuff Count field and inserted before the CRC Sequence field. CRC
 * calculation extends beyond the end of Data field and takes the Stuff Count field bits
 * into account.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Stuff Bit Count
 */
static inline void FLEXCAN_SetStuffBitCount(CAN_Type * base, bool enable)
{
#if FEATURE_CAN_HAS_STFCNTEN_ENABLE
    base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_STFCNTEN_MASK) | CAN_CTRL2_STFCNTEN(enable? 1UL : 0UL);
#elif FEATURE_CAN_HAS_ISOCANFDEN_ENABLE
    base->CTRL2 = (base->CTRL2 & ~CAN_CTRL2_ISOCANFDEN_MASK) |
                   CAN_CTRL2_ISOCANFDEN(enable? 1UL : 0UL);

#ifdef ERRATA_E8759
    uint8_t flag = (uint8_t)(((base->CTRL2) & CAN_CTRL2_ISOCANFDEN_MASK) >> CAN_CTRL2_ISOCANFDEN_SHIFT);

    DEV_ASSERT((enable == false) || (flag == 1UL));
    (void)flag;
#endif

#endif
}
#endif

/*!
 * @brief Enables/Disables the Self Reception feature.
 *
 * If enabled, FlexCAN is allowed to receive frames transmitted by itself.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Self Reception
 */
static inline void FLEXCAN_SetSelfReception(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_SRXDIS_MASK) | CAN_MCR_SRXDIS(enable? 0UL : 1UL);
}

#if FEATURE_CAN_HAS_FD
/*!
 * @brief Enables/Disables the Transceiver Delay Compensation feature and sets
 * the Transceiver Delay Compensation Offset (offset value to be added to the
 * measured transceiver's loop delay in order to define the position of the
 * delayed comparison point when bit rate switching is active).
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable Transceiver Delay Compensation
 * @param   offset Transceiver Delay Compensation Offset
 */
static inline void FLEXCAN_SetTDCOffset(CAN_Type * base, bool enable, uint8_t offset)
{
    uint32_t tmp;

    tmp = base->FDCTRL;
    tmp &= ~(CAN_FDCTRL_TDCEN_MASK | CAN_FDCTRL_TDCOFF_MASK);

    if (enable)
    {
        tmp = tmp | CAN_FDCTRL_TDCEN_MASK;
        tmp = tmp | CAN_FDCTRL_TDCOFF(offset);
    }

    base->FDCTRL = tmp;
}
#endif

#if FEATURE_CAN_HAS_DMA_ENABLE
/*!
 * @brief Enables/Disables the DMA support for RxFIFO.
 *
 * @param   base  The FlexCAN base address
 * @param   enable Enable/Disable DMA support
 * @return  STATUS_SUCCESS if successfully enabled; STATUS_ERROR
 *    if not enabled (due to the RxFIFO feature being disabled).
 */
static inline void FLEXCAN_SetRxFifoDMA(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_DMA_MASK) | CAN_MCR_DMA(enable? 1UL : 0UL);
}
#endif

#if FEATURE_CAN_HAS_PRETENDED_NETWORKING

/*!
 * @brief Configures Pretended Networking mode filtering selection.
 *
 * @param   base  The FlexCAN base address
 * @param   wakeUpTimeout  enable/disable wake up by timeout
 * @param   wakeUpMatch  enable/disable wake up by match
 * @param   numMatches  set number of messages matching the same filtering criteria
 * @param   filterComb  set filtering combination selection
 * @param   idFilterType  set ID filtering selection
 * @param   payloadFilterType  set payload filtering selection
 *
 */
static inline void FLEXCAN_SetPNFilteringSelection(
        CAN_Type * base,
        bool wakeUpTimeout,
        bool wakeUpMatch,
        uint16_t numMatches,
        flexcan_pn_filter_combination_t filterComb,
        flexcan_pn_filter_selection_t idFilterType,
        flexcan_pn_filter_selection_t payloadFilterType)
{
    uint32_t tmp;

    tmp = base->CTRL1_PN;
    tmp &= ~(CAN_CTRL1_PN_WTOF_MSK_MASK |
             CAN_CTRL1_PN_WUMF_MSK_MASK |
             CAN_CTRL1_PN_NMATCH_MASK |
             CAN_CTRL1_PN_PLFS_MASK |
             CAN_CTRL1_PN_IDFS_MASK |
             CAN_CTRL1_PN_FCS_MASK);
    tmp |= CAN_CTRL1_PN_WTOF_MSK(wakeUpTimeout ? 1UL : 0UL);
    tmp |= CAN_CTRL1_PN_WUMF_MSK(wakeUpMatch ? 1UL : 0UL);
    tmp |= CAN_CTRL1_PN_NMATCH(numMatches);
    tmp |= CAN_CTRL1_PN_FCS(filterComb);
    tmp |= CAN_CTRL1_PN_IDFS(idFilterType);
    tmp |= CAN_CTRL1_PN_PLFS(payloadFilterType);
    base->CTRL1_PN = tmp;
}

/*!
 * @brief Set PN timeout value.
 *
 * @param   base  The FlexCAN base address
 * @param   timeoutValue  timeout for no message matching
 */
static inline void FLEXCAN_SetPNTimeoutValue(CAN_Type * base, uint16_t timeoutValue)
{
    base->CTRL2_PN = (base->CTRL2_PN & ~CAN_CTRL2_PN_MATCHTO_MASK) |
                      CAN_CTRL2_PN_MATCHTO(timeoutValue);
}

/*!
 * @brief Configures the Pretended Networking ID Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   idFilter  The ID Filter configuration
 */
static inline void FLEXCAN_SetPNIdFilter1(CAN_Type * base, flexcan_pn_id_filter_t idFilter)
{
    uint32_t tmp;

    tmp = base->FLT_ID1;
    tmp &= ~(CAN_FLT_ID1_FLT_IDE_MASK | CAN_FLT_ID1_FLT_RTR_MASK | CAN_FLT_ID1_FLT_ID1_MASK);
    tmp |= CAN_FLT_ID1_FLT_IDE(idFilter.extendedId ? 1UL : 0UL);
    tmp |= CAN_FLT_ID1_FLT_RTR(idFilter.remoteFrame ? 1UL : 0UL);
    if (idFilter.extendedId)
    {
        tmp |= CAN_FLT_ID1_FLT_ID1(idFilter.id);
    }
    else
    {
        tmp |= CAN_FLT_ID1_FLT_ID1(idFilter.id << CAN_ID_STD_SHIFT);
    }
    base->FLT_ID1 = tmp;
}

/*!
 * @brief Configures the Pretended Networking ID Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   idFilter  The ID Filter configuration
 */
static inline void FLEXCAN_SetPNIdFilter2(CAN_Type * base, flexcan_pn_id_filter_t idFilter)
{
    uint32_t tmp;

    tmp = base->FLT_ID2_IDMASK;
    tmp &= ~(CAN_FLT_ID2_IDMASK_IDE_MSK_MASK | CAN_FLT_ID2_IDMASK_RTR_MSK_MASK | CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK);
    tmp |= CAN_FLT_ID2_IDMASK_IDE_MSK(idFilter.extendedId ? 1UL : 0UL);
    tmp |= CAN_FLT_ID2_IDMASK_RTR_MSK(idFilter.remoteFrame ? 1UL : 0UL);
    if (idFilter.extendedId)
    {
        tmp |= CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(idFilter.id);
    }
    else
    {
        tmp |= CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(idFilter.id << CAN_ID_STD_SHIFT);
    }
    base->FLT_ID2_IDMASK = tmp;
}

/*!
 * @brief Set PN DLC Filter.
 *
 * @param   base  The FlexCAN base address
 * @param   timeoutValue  timeout for no message matching
 */
static inline void FLEXCAN_SetPNDlcFilter(CAN_Type * base, uint8_t dlcLow, uint8_t dlcHigh)
{
    uint32_t tmp;

    tmp = base->FLT_DLC;
    tmp &= ~(CAN_FLT_DLC_FLT_DLC_HI_MASK | CAN_FLT_DLC_FLT_DLC_HI_MASK);
    tmp |= CAN_FLT_DLC_FLT_DLC_HI(dlcHigh);
    tmp |= CAN_FLT_DLC_FLT_DLC_LO(dlcLow);
    base->FLT_DLC = tmp;
}

/*!
 * @brief Set PN Payload High Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadHighFilter1(CAN_Type * base, const uint8_t *payload)
{
    base->PL1_HI = CAN_PL1_HI_Data_byte_4(payload[4]) |
                   CAN_PL1_HI_Data_byte_5(payload[5]) |
                   CAN_PL1_HI_Data_byte_6(payload[6]) |
                   CAN_PL1_HI_Data_byte_7(payload[7]);
}

/*!
 * @brief Set PN Payload Low Filter 1.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadLowFilter1(CAN_Type * base, const uint8_t *payload)
{
    base->PL1_LO = CAN_PL1_LO_Data_byte_0(payload[0]) |
                   CAN_PL1_LO_Data_byte_1(payload[1]) |
                   CAN_PL1_LO_Data_byte_2(payload[2]) |
                   CAN_PL1_LO_Data_byte_3(payload[3]);
}

/*!
 * @brief Set PN Payload High Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadHighFilter2(CAN_Type * base, const uint8_t *payload)
{
    base->PL2_PLMASK_HI = CAN_PL2_PLMASK_HI_Data_byte_4(payload[4]) |
                          CAN_PL2_PLMASK_HI_Data_byte_5(payload[5]) |
                          CAN_PL2_PLMASK_HI_Data_byte_6(payload[6]) |
                          CAN_PL2_PLMASK_HI_Data_byte_7(payload[7]);
}

/*!
 * @brief Set PN Payload Low Filter 2.
 *
 * @param   base  The FlexCAN base address
 * @param   payload  message payload filter
 */
static inline void FLEXCAN_SetPNPayloadLowFilter2(CAN_Type * base, const uint8_t *payload)
{
    base->PL2_PLMASK_LO = CAN_PL2_PLMASK_LO_Data_byte_0(payload[0]) |
                          CAN_PL2_PLMASK_LO_Data_byte_1(payload[1]) |
                          CAN_PL2_PLMASK_LO_Data_byte_2(payload[2]) |
                          CAN_PL2_PLMASK_LO_Data_byte_3(payload[3]);
}

/*!
 * @brief Configures the Pretended Networking mode.
 *
 * @param   base  The FlexCAN base address
 * @param   pnConfig  The pretended networking configuration
 */
void FLEXCAN_ConfigPN(CAN_Type * base, const flexcan_pn_config_t *pnConfig);

/*!
 * @brief Enables/Disables the Pretended Networking mode.
 *
 * @param   base  The FlexCAN base address
 * @param   enable  Enable/Disable Pretending Networking
 */
static inline void FLEXCAN_SetPN(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_PNET_EN_MASK) | CAN_MCR_PNET_EN(enable ? 1UL : 0UL);
}

/*!
 * @brief Checks if the Pretended Networking mode is enabled/disabled.
 *
 * @param   base  The FlexCAN base address
 * @return  false if Pretended Networking mode is disabled;
 *          true if Pretended Networking mode is enabled
 */
static inline bool FLEXCAN_IsPNEnabled(const CAN_Type * base)
{
    return (((base->MCR & CAN_MCR_PNET_EN_MASK) >> CAN_MCR_PNET_EN_SHIFT) != 0U);
}

/*!
 * @brief Gets the Wake Up by Timeout Flag Bit.
 *
 * @param   base  The FlexCAN base address
 * @return  the Wake Up by Timeout Flag Bit
 */
static inline uint8_t FLEXCAN_GetWTOF(const CAN_Type * base)
{
    return (uint8_t)((base->WU_MTC & CAN_WU_MTC_WTOF_MASK) >> CAN_WU_MTC_WTOF_SHIFT);
}

/*!
 * @brief Clears the Wake Up by Timeout Flag Bit.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearWTOF(CAN_Type * base)
{
    base->WU_MTC |= CAN_WU_MTC_WTOF_MASK;
}

/*!
 * @brief Gets the Wake Up by Match Flag Bit.
 *
 * @param   base  The FlexCAN base address
 * @return  the Wake Up by Match Flag Bit
 */
static inline uint8_t FLEXCAN_GetWUMF(const CAN_Type * base)
{
    return (uint8_t)((base->WU_MTC & CAN_WU_MTC_WUMF_MASK) >> CAN_WU_MTC_WUMF_SHIFT);
}

/*!
 * @brief Clears the Wake Up by Match Flag Bit.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearWUMF(CAN_Type * base)
{
    base->WU_MTC |= CAN_WU_MTC_WUMF_MASK;
}

#endif /* FEATURE_CAN_HAS_PRETENDED_NETWORKING */

#if FEATURE_CAN_HAS_SELF_WAKE_UP

/*!
 * @brief Enables/Disables the Self Wake Up mode.
 *
 * @param   base  The FlexCAN base address
 * @param   enable  Enable/Disable Self Wake Up
 * @return  STATUS_SUCCESS if successful;
 *          STATUS_ERROR if Self Wake Up mode could not be enabled
 */
static inline status_t FLEXCAN_SetSelfWakeUp(CAN_Type * base, bool enable)
{
    uint32_t flag;
    status_t status = STATUS_SUCCESS;

    /* Enable Self Wake Up */
    base->MCR = (base->MCR & ~CAN_MCR_SLFWAK_MASK) | CAN_MCR_SLFWAK(enable ? 1UL : 0UL);

    /* Check if Self Wake Up was successfully enabled */
    flag = ((base->MCR & CAN_MCR_SLFWAK_MASK) >> CAN_MCR_SLFWAK_SHIFT);

    if (enable && (flag != 1U))
    {
        status = STATUS_ERROR;
    }
    else
    {
        /* Enable Wake Up Interrupt */
        base->MCR = (base->MCR & ~CAN_MCR_WAKMSK_MASK) | CAN_MCR_WAKMSK(enable ? 1UL : 0UL);
    }

    return status;
}

/*!
 * @brief Enables/Disables the integrated low-pass filter applied to protect
 * the Rx CAN input from spurious wake up.
 *
 * @param   base  The FlexCAN base address
 * @param   enable  Enable/Disable the integrated Rx CAN low-pass filter
 */
static inline void FLEXCAN_SetWakSrc(CAN_Type * base, bool enable)
{
    base->MCR = (base->MCR & ~CAN_MCR_WAKSRC_MASK) | CAN_MCR_WAKSRC(enable ? 1UL : 0UL);
}

/*!
 * @brief Checks if the Self Wake Up mode is enabled/disabled.
 *
 * @param   base  The FlexCAN base address
 * @return  false if Self Wake Up mode is disabled;
 *          true if Self Wake Up mode is enabled
 */
static inline bool FLEXCAN_IsSelfWakeUpEnabled(const CAN_Type * base)
{
    return (((base->MCR & CAN_MCR_SLFWAK_MASK) >> CAN_MCR_SLFWAK_SHIFT) != 0U);
}

/*!
 * @brief Gets the Wake-Up Interrupt Flag.
 *
 * @param   base  The FlexCAN base address
 * @return  the  Wake-Up Interrupt Flag
 */
static inline uint8_t FLEXCAN_GetWAKINT(const CAN_Type * base)
{
    return (uint8_t)((base->ESR1 & CAN_ESR1_WAKINT_MASK) >> CAN_ESR1_WAKINT_SHIFT);
}

/*!
 * @brief Clears the Wake-Up Interrupt Flag.
 *
 * @param   base  The FlexCAN base address
 */
static inline void FLEXCAN_ClearWAKINT(CAN_Type * base)
{
    base->ESR1 |= CAN_ESR1_WAKINT_MASK;
}

#endif /* FEATURE_CAN_HAS_SELF_WAKE_UP */

/*@}*/

#if defined(__cplusplus)
}
#endif

#endif /* FLEXCAN_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
