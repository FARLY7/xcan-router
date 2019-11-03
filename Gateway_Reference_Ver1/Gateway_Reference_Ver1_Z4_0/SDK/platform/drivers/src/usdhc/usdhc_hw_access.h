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

/*!
 * @file usdhc_hw_access.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially Boolean' type to 'essentially unsigned'.
 * This is required by the conversion of a bit/bitfield of a register into boolean or a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different
 * essential type.
 * The cast is required to perform a conversion between an unsigned integer and an enum type with
 * many values.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite expression
 * (different essential type categories).
 * This is required by the conversion of a bit/bitfield of a register into boolean or a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros are public and used by application.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, global typedef not referenced
 * This structure is used by user.
 *
 */

#ifndef USDHC_HW_ACCESS_H
#define USDHC_HW_ACCESS_H

#include "usdhc_driver.h"
#include "device_registers.h"
#include <stdlib.h>


/******************************************************************************
 * Definitions.
 *****************************************************************************/

/*! @brief Maximum block count can be set one time */
#define uSDHC_MAX_BLOCK_COUNT       (uSDHC_BLK_ATT_BLKCNT_MASK >> uSDHC_BLK_ATT_BLKCNT_SHIFT)

/*! @brief Definitions for composed values of interrupt flags */
/*!< Command error */
#define uSDHC_COMMAND_ERROR_INT   (uSDHC_INT_STATUS_CTOE_MASK |          \
                                   uSDHC_INT_STATUS_CCE_MASK |           \
                                   uSDHC_INT_STATUS_CEBE_MASK |          \
                                   uSDHC_INT_STATUS_CIE_MASK)

/*!< Data error */
#define uSDHC_DATA_ERROR_INT   (uSDHC_INT_STATUS_DTOE_MASK |             \
                                uSDHC_INT_STATUS_DCE_MASK |              \
                                uSDHC_INT_STATUS_DEBE_MASK |             \
                                uSDHC_INT_STATUS_AC12E_MASK)

/*!< All error */
#define uSDHC_ERROR_INT   (uSDHC_COMMAND_ERROR_INT |    \
                           uSDHC_DATA_ERROR_INT |       \
                           uSDHC_INT_STATUS_DMAE_MASK)

/*!< Data interrupts */
#define uSDHC_DATA_INT   (uSDHC_INT_STATUS_TC_MASK |         \
                          uSDHC_INT_STATUS_DINT_MASK |       \
                          uSDHC_INT_STATUS_BWR_MASK |        \
                          uSDHC_INT_STATUS_BRR_MASK |        \
                          uSDHC_DATA_ERROR_INT |             \
                          uSDHC_INT_STATUS_DMAE_MASK)

/*!< Command interrupts */
#define uSDHC_COMMAND_INT   (uSDHC_COMMAND_ERROR_INT |      \
                             uSDHC_INT_STATUS_CC_MASK)

/*!< Card detection interrupts */
#define uSDHC_CARD_DETECT_INT    (uSDHC_INT_STATUS_CINS_MASK |   \
                                 uSDHC_INT_STATUS_CRM_MASK)


/* ADMA1 descriptor table
 * |------------------------|---------|--------------------------|
 * | Address/page field     |Reserved |         Attribute        |
 * |------------------------|---------|--------------------------|
 * |31                    12|11      6|05  |04  |03|02 |01 |00   |
 * |------------------------|---------|----|----|--|---|---|-----|
 * | address or data length | 000000  |Act2|Act1| 0|Int|End|Valid|
 * |------------------------|---------|----|----|--|---|---|-----|
 *
 *
 * |------|------|-----------------|-------|-------------|
 * | Act2 | Act1 |     Comment     | 31-28 | 27 - 12     |
 * |------|------|-----------------|---------------------|
 * |   0  |   0  | No op           | Don't care          |
 * |------|------|-----------------|-------|-------------|
 * |   0  |   1  | Set data length |  0000 | Data Length |
 * |------|------|-----------------|-------|-------------|
 * |   1  |   0  | Transfer data   | Data address        |
 * |------|------|-----------------|---------------------|
 * |   1  |   1  | Link descriptor | Descriptor address  |
 * |------|------|-----------------|---------------------|
 */
/*! @brief Define the adma1 descriptor structure. */
typedef uint32_t usdhc_adma1_descriptor_t;

/*! @brief The alignment size for ADDRESS filed in ADMA1's descriptor */
#define uSDHC_ADMA1_ADDRESS_ALIGN   (4096U)
/*! @brief The alignment size for LENGTH field in ADMA1's descriptor */
#define uSDHC_ADMA1_LENGTH_ALIGN    (4096U)

/*! @brief Definition the length in bytes of ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_LENGTH                   (4)
/*! @brief The bit shift for ADDRESS field in ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_ADDR_SHIFT               (12U)
/*! @brief The bit mask for ADDRESS field in ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_ADDR_MASK                (0xFFFFFU)
/*! @brief The bit shift for LENGTH filed in ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT             (12U)
/*! @brief The mask for LENGTH field in ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_LENGTH_MASK              (0xFFFFU)
/*! @brief The max value of LENGTH filed in ADMA1's descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY     (uSDHC_ADMA1_DESCRIPTOR_LENGTH_MASK + 1U)

/*! @brief The mask for the control/status fields in ADMA1 descriptor */
#define uSDHC_ADMA1_DESCRIPTOR_VALID_MASK               (0x01u)
#define uSDHC_ADMA1_DESCRIPTOR_END_MASK                 (0x02u)
#define uSDHC_ADMA1_DESCRIPTOR_INTERRUPT_MASK           (0x04u)
#define uSDHC_ADMA1_DESCRIPTOR_ACT1_MASK                (0x10u)
#define uSDHC_ADMA1_DESCRIPTOR_ACT2_MASK                (0x20u)
#define uSDHC_ADMA1_DESCRIPTOR_TYPE_NOP_MASK            (uSDHC_ADMA1_DESCRIPTOR_VALID_MASK)
#define uSDHC_ADMA1_DESCRIPTOR_TYPE_TRANSFER_MASK       (uSDHC_ADMA1_DESCRIPTOR_ACT2_MASK | uSDHC_ADMA1_DESCRIPTOR_VALID_MASK)
#define uSDHC_ADMA1_DESCRIPTOR_TYPE_LINK_MASK           (uSDHC_ADMA1_DESCRIPTOR_ACT1_MASK | uSDHC_ADMA1_DESCRIPTOR_ACT2_MASK | uSDHC_ADMA1_DESCRIPTOR_VALID_MASK)
#define uSDHC_ADMA1_DESCRIPTOR_TYPE_SET_LENGTH_MASK     (uSDHC_ADMA1_DESCRIPTOR_ACT1_MASK | uSDHC_ADMA1_DESCRIPTOR_VALID_MASK)



/* ADMA2 descriptor table
 * |----------------|---------------|-------------|--------------------------|
 * | Address field  |     Length    | Reserved    |         Attribute        |
 * |----------------|---------------|-------------|--------------------------|
 * |63            32|31           16|15         06|05  |04  |03|02 |01 |00   |
 * |----------------|---------------|-------------|----|----|--|---|---|-----|
 * | 32-bit address | 16-bit length | 0000000000  |Act2|Act1| 0|Int|End|Valid|
 * |----------------|---------------|-------------|----|----|--|---|---|-----|
 *
 *
 * | Act2 | Act1 |     Comment     | Operation                                                         |
 * |------|------|-----------------|-------------------------------------------------------------------|
 * |   0  |   0  | No op           | Don't care                                                        |
 * |------|------|-----------------|-------------------------------------------------------------------|
 * |   0  |   1  | Reserved        | Read this line and go to next one                                 |
 * |------|------|-----------------|-------------------------------------------------------------------|
 * |   1  |   0  | Transfer data   | Transfer data with address and length set in this descriptor line |
 * |------|------|-----------------|-------------------------------------------------------------------|
 * |   1  |   1  | Link descriptor | Link to another descriptor                                        |
 * |------|------|-----------------|-------------------------------------------------------------------|
 */

/*! @brief Definition the ADMA2 descriptor structure. */
typedef struct
{
    uint32_t         attribute;     /*!< The control and status field */
    const uint32_t * address;       /*!< The address field */
} usdhc_adma2_descriptor_t;

/*! @brief The alignment size for ADDRESS field in ADMA2's descriptor */
#define uSDHC_ADMA2_ADDRESS_ALIGN   (4U)
/*! @brief The alignment size for LENGTH filed in ADMA2's descriptor */
#define uSDHC_ADMA2_LENGTH_ALIGN    (4U)

/*! @brief Definition the length in bytes of ADMA2's descriptor */
#define uSDHC_ADMA2_DESCRIPTOR_LENGTH                   (8)
/*! @brief The bit shift for LENGTH field in ADMA2's descriptor */
#define uSDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT             (16U)
/*! @brief The bit mask for LENGTH field in ADMA2's descriptor */
#define uSDHC_ADMA2_DESCRIPTOR_LENGTH_MASK              (0xFFFFU)
/*! @brief The max value of LENGTH field in ADMA2's descriptor */
#define uSDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY     (uSDHC_ADMA2_DESCRIPTOR_LENGTH_MASK)

/*! @brief ADMA2 descriptor control and status masks */
#define uSDHC_ADMA2_DESCRIPTOR_VALID_MASK               (0x01U)
#define uSDHC_ADMA2_DESCRIPTOR_END_MASK                 (0x02U)
#define uSDHC_ADMA2_DESCRIPTOR_INTERRUPT_MASK           (0x04U)
#define uSDHC_ADMA2_DESCRIPTOR_ACT1_MASK                (0x10U)
#define uSDHC_ADMA2_DESCRIPTOR_ACT2_MASK                (0x20U)
#define uSDHC_ADMA2_DESCRIPTOR_TYPE_NOP_MASK            (uSDHC_ADMA2_DESCRIPTOR_VALID_MASK)
#define uSDHC_ADMA2_DESCRIPTOR_TYPE_TRANSFER_MASK       (uSDHC_ADMA2_DESCRIPTOR_ACT2_MASK | uSDHC_ADMA2_DESCRIPTOR_VALID_MASK)
#define uSDHC_ADMA2_DESCRIPTOR_TYPE_LINK_MASK           (uSDHC_ADMA2_DESCRIPTOR_ACT1_MASK | uSDHC_ADMA2_DESCRIPTOR_ACT2_MASK | uSDHC_ADMA2_DESCRIPTOR_VALID_MASK)

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Disable interrupt.
 *
 * @param[in] base  uSDHC peripheral base address.
 * @param[in] mask  The interrupt flags mask(a logical OR of usdhc_interrupt_t).
 */
static inline void uSDHC_DisableInterrupt(uSDHC_Type * base,
                                                uint32_t mask)
{
    base->INT_STATUS_EN &= ~mask;    /* Disable interrupt status */
    base->INT_SIGNAL_EN &= ~mask;    /* Disable interrupt signal */
}

/*!
 * @brief Enable interrupt
 *
 * @param[in] base  uSDHC peripheral base address.
 * @param[in] mask  Interrupt flags mask (a logical OR of usdhc_interrupt_t).
 */
static inline void uSDHC_EnableInterrupt(uSDHC_Type * base,
                                               uint32_t mask)
{
    base->INT_STATUS_EN |= mask;    /* Enable interrupt status */
    base->INT_SIGNAL_EN |= mask;    /* Enable interrupt signal */
}

/*!
 * @brief Get current interrupt status.
 *
 * @param[in] base  uSDHC peripheral base address.
 *
 * @return Current interrupt status flags mask (a logical OR of usdhc_interrupt_t).
 */
static inline uint32_t uSDHC_GetInterruptStatusFlags(const uSDHC_Type * base)
{
    return base->INT_STATUS;
}

/*!
 * @brief Clear specified interrupt status.
 *
 * @param[in] base  uSDHC peripheral base address.
 * @param[in] mask  The interrupt status flags mask(a logical OR of usdhc_interrupt_t).
 */
static inline void uSDHC_ClearInterruptStatusFlags(uSDHC_Type * base,
                                                   uint32_t mask)
{
    base->INT_STATUS = mask;
}

/*!
 * @brief Get the command response.
 *
 * @param[in] base      uSDHC peripheral base address.
 * @param[in] index     The index of response register, range from 0 to 3.
 *
 * @return Response register transfer.
 */
static inline uint32_t uSDHC_GetCommandResponse(const uSDHC_Type * base,
                                                uint32_t index)
{
    uint32_t cmdRespond = 0;
    if (index == 0U)
    {
        cmdRespond = base->CMD_RSP0;
    }
    else if (index == 1U)
    {
        cmdRespond = base->CMD_RSP1;
    }
    else if (index == 2U)
    {
        cmdRespond = base->CMD_RSP2;
    }
    else if (index == 3U)
    {
        cmdRespond = base->CMD_RSP3;
    }
    else
    {
        cmdRespond = 0U;
    }
    return cmdRespond;
}

/*!
 * @brief Get the host version
 *
 * @param[in] base  uSDHC peripheral base address.
 *
 * @return The Host Controller version information.
 */
static inline uint32_t uSDHC_GetHostVersion(const uSDHC_Type * base)
{
    return base->HOST_CTRL_VER;
}

/*!
 * @brief Get the host capacities
 *
 * @param[in] base  uSDHC peripheral base address.
 *
 * @return The Host Controller capacities.
 */
static inline uint32_t uSDHC_GetHostCapacities(const uSDHC_Type * base)
{
    return base->HOST_CTRL_CAP;
}

/*!
 * @brief Retrieve the data from the data port.
 *
 * This function is mainly used to implement the data transfer by Data Port instead of DMA.
 *
 * @param[in] base  uSDHC peripheral base address.
 *
 * @return The data has been read.
 */
static inline uint32_t uSDHC_ReadData(const uSDHC_Type * base)
{
    return base->DATA_BUFF_ACC_PORT;
}

/*!
 * @brief Fill the the data port.
 *
 * This function is mainly used to implement the data transfer by Data Port instead of DMA.
 *
 * @param[in] base  uSDHC peripheral base address.
 * @param[in] data  The data about to be sent.
 */
static inline void uSDHC_WriteData(uSDHC_Type * base,
                                   uint32_t data)
{
    base->DATA_BUFF_ACC_PORT = data;
}

/*!
 * @brief Get the current DMA mode that configured before
 *
 * @param[in] base  uSDHC peripheral base address.
 *
 * @return  One of the values in usdhc_dma_mode_t
 */
static inline usdhc_dma_mode_t uSDHC_GetCurrentDMAMode(const uSDHC_Type * base)
{
    return (usdhc_dma_mode_t)((base->PROT_CTRL & uSDHC_PROT_CTRL_DMASEL_MASK) >> uSDHC_PROT_CTRL_DMASEL_SHIFT);
}

/*!
 * @brief Disable the SD bus clock
 *
 * @param[in] base  uSDHC peripheral base address.
 */
static inline void uSDHC_DisableSDClock(uSDHC_Type * base)
{
    base->VEND_SPEC &= ~uSDHC_VEND_SPEC_FRC_SDCLK_ON_MASK;
}

/*!
 * @brief Enable the SD bus clock
 *
 * @param[in] base uSDHC peripheral base address.
 */
static inline void uSDHC_EnableSDClock(uSDHC_Type * base)
{
    base->VEND_SPEC |= uSDHC_VEND_SPEC_FRC_SDCLK_ON_MASK;
}

/*!
 * @brief Set the SD bus clock with specific divisor and prescaler
 *
 * @param[in] base          uSDHC peripheral base address.
 * @param[in] divisor       This parameter is used to provide a more exact divisor to generate the desired SD clock frequency.
 * @param[in] prescaler     This parameter is used to select the frequency of the SDCLK pin.
 */
void uSDHC_SetSDClock(uSDHC_Type * base,
                      uint32_t divisor,
                      uint32_t prescaler);

/*!
 * @brief Configures the uSDHC protocol.
 *
 * @param[in] base      uSDHC peripheral base address.
 * @param[in] config    The user configuration structure
 */
void uSDHC_ConfigProtocolControl(uSDHC_Type * base,
                                 const usdhc_config_t * config);

/*!
 * @brief Enables all auto gated off feature.
 *
 * @param[in] base  uSDHC peripheral base address.
 */
static inline void uSDHC_EnableAutoGateOff(uSDHC_Type * base)
{
    base->VEND_SPEC &= ~(uSDHC_VEND_SPEC_IPG_PERCLK_SOFT_EN_MASK |
                         uSDHC_VEND_SPEC_HCLK_SOFT_EN_MASK |
                         uSDHC_VEND_SPEC_BUS_CLK_SOFT_EN_MASK);
}

/*!
 * @brief Disables all auto gated off feature.
 *
 * @param[in] base  uSDHC peripheral base address.
 */
static inline void uSDHC_DisableAutoGateOff(uSDHC_Type * base)
{
    base->VEND_SPEC |= (uSDHC_VEND_SPEC_IPG_PERCLK_SOFT_EN_MASK |
                        uSDHC_VEND_SPEC_HCLK_SOFT_EN_MASK |
                        uSDHC_VEND_SPEC_BUS_CLK_SOFT_EN_MASK);
}

/*!
 * @brief Configures byte swap for usdhc. To be able to use internal DMA, byte swap must be enabled.
 *
 * @param[in] swap  Enable swapping the AHB write/read data at the boundary of uSDHC master
 */
void uSDHC_ConfigByteSwap(bool swap);

/*!
 * @brief Configures the ADMA table.
 *
 * @param[in] base      uSDHC peripheral base address.
 * @param[in] dmaMode   The DMA mode that you want to use for transferring
 * @param[in] table     The DMA table
 * @param[in] tableSize The size of DMA table in bytes. Each table can transfer maximum is 65535 bytes in one time.
                        If you want to transfer more data, more table is needed.
 * @param[in] data      The data that you want to send out
 * @param[in] dataBytes The length of data that you want to send out
 */
status_t uSDHC_SetAdmaTableConfig(uSDHC_Type * base,
                                  usdhc_dma_mode_t dmaMode,
                                  uint32_t * table,
                                  uint32_t tableSize,
                                  const uint32_t * data,
                                  uint32_t dataBytes);

/*!
 * @brief Start a transfer
 *
 * @param[in] base      uSDHC peripheral base address.
 * @param[in] command   The command structure that you want to send out
 * @param[in] data      The data structure that you want to send out
 */
void uSDHC_StartTransfer(uSDHC_Type * base,
                         const usdhc_command_t * command,
                         const usdhc_data_t * data);

/*!
 * @brief The function to handle the uSDHC's interrupt
 *
 * @param[in] instance  uSDHC instance number
 */
void uSDHC_TransferHandleIRQ(uint32_t instance);

#if defined(__cplusplus)
}
#endif


#endif /* USDHC_HW_ACCESS_H */
