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
 * @file usdhc_hw_access.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.5, object/function previously declared.
 * This requirement is fulfilled since the array of uSDHC base address is declared
 * in driver source file and needed to be used in this file.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to
 * object type and a pointer to a different object type.
 * The cast is required in order to cast the address of ADMA table to ADMA1 or ADMA2
 * depending on DMA mode.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Unusual pointer cast (incompatible
 * indirect types).
 * The cast is required to cast the address of ADMA table to ADMA1 or ADMA2
 * depending on DMA mode.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
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
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially Boolean' type to 'essentially unsigned'.
 * This is required by the conversion of a bit/bitfield of a register into boolean or a enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 18.4, Pointer arithmetic other than array
 * indexing used.
 * This is required to increment the address of a pointer.
 *
 */

#include "usdhc_hw_access.h"

/*! @brief Transfer flags */
#define uSDHC_ENABLE_DMA_FLAG              uSDHC_MIX_CTRL_DMAEN_MASK            /*!< Enable DMA */
#define uSDHC_ENABLE_BLOCK_COUNT_FLAG      uSDHC_MIX_CTRL_BCEN_MASK             /*!< Enable block count */
#define uSDHC_ENABLE_AUTO_CMD12_FLAG       uSDHC_MIX_CTRL_AC12EN_MASK           /*!< Enable auto CMD12 */
#define uSDHC_DATA_READ_FLAG               uSDHC_MIX_CTRL_DTDSEL_MASK           /*!< Enable data read */
#define uSDHC_MULTIPLE_BLOCK_FLAG          uSDHC_MIX_CTRL_MSBSEL_MASK           /*!< Multiple block data read/write */

#define uSDHC_CMD_TYPE_ABORT_FLAG          (uSDHC_CMD_XFR_TYP_CMDTYP(3U))       /*!< Abort command */

#define uSDHC_RESPONSE_LENGTH136_FLAG      (uSDHC_CMD_XFR_TYP_RSPTYP(1U))       /*!< 136 bit response length */
#define uSDHC_RESPONSE_LENGTH48_FLAG       (uSDHC_CMD_XFR_TYP_RSPTYP(2U))       /*!< 48 bit response length */
#define uSDHC_RESPONSE_LENGTH48_BUSY_FLAG  (uSDHC_CMD_XFR_TYP_RSPTYP(3U))       /*!< 48 bit response length with busy status */

#define uSDHC_ENABLE_CRC_CHECK_FLAG        uSDHC_CMD_XFR_TYP_CCCEN_MASK         /*!< Enable CRC check */
#define uSDHC_ENABLE_INDEX_CHECK_FLAG      uSDHC_CMD_XFR_TYP_CICEN_MASK         /*!< Enable index check */
#define uSDHC_DATA_PRESENT_FLAG            uSDHC_CMD_XFR_TYP_DPSEL_MASK         /*!< Data present flag */


/*! @brief Card transfer configuration.
 *
 * Define structure to configure the transfer-related command index/argument/flags and data block
 * size/data block numbers. This structure needs to be filled each time a command is sent to the card.
 */
typedef struct
{
    uint32_t dataBlockSize;     /*!< Data block size */
    uint32_t dataBlockCount;    /*!< Data block count */
    uint32_t commandArgument;   /*!< Command argument */
    uint32_t commandIndex;      /*!< Command index */
    uint32_t flags;             /*!< Transfer flags (a logical OR of usdhc_transfer_flag_t) */
} usdhc_transfer_config_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
 /*! @brief Pointers to uSDHC base address for each instance. */
extern uSDHC_Type * const g_usdhcBases[uSDHC_INSTANCE_COUNT];

/*! @brief Pointers to uSDHC state for each instance. */
extern usdhc_state_t * g_usdhcStates[uSDHC_INSTANCE_COUNT];

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_SetTransferConfig
 * Description   : Configures the transfer before transferring
 *
 *END**************************************************************************/
static void uSDHC_SetTransferConfig(uSDHC_Type * base,
                                    const usdhc_transfer_config_t * config);

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_ConfigProtocolControl
 * Description   : Configures the uSDHC protocol.
 *
 *END**************************************************************************/
void uSDHC_ConfigProtocolControl(uSDHC_Type * base,
                                 const usdhc_config_t * config)
{
    uint32_t proctl = 0;

    proctl = base->PROT_CTRL;

    proctl &= ~(uSDHC_PROT_CTRL_D3CD_MASK | uSDHC_PROT_CTRL_EMODE_MASK | uSDHC_PROT_CTRL_DMASEL_MASK);
    /* Set DAT3 as card detection pin */
    if (config->cardDetectDat3)
    {
        proctl |= uSDHC_PROT_CTRL_D3CD_MASK;
    }
    /* Endian mode and DMA mode */
    proctl |= (uSDHC_PROT_CTRL_EMODE(config->endianMode) | uSDHC_PROT_CTRL_DMASEL(config->dmaMode));

    base->PROT_CTRL = proctl;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_SetSDClock
 * Description   : Configures SD clock with given divisor and prescaler
 *
 *END**************************************************************************/
void uSDHC_SetSDClock(uSDHC_Type * base,
                      uint32_t divisor,
                      uint32_t prescaler)
{
    uint32_t sysctl;

    sysctl = base->SYS_CTRL;
    sysctl &= ~(uSDHC_SYS_CTRL_DVS_MASK | uSDHC_SYS_CTRL_SDCLKFS_MASK | uSDHC_SYS_CTRL_DTOCV_MASK);
    sysctl |= (uSDHC_SYS_CTRL_DVS(divisor) | uSDHC_SYS_CTRL_SDCLKFS(prescaler) | uSDHC_SYS_CTRL_DTOCV(0xEU));
    base->SYS_CTRL = sysctl;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_ConfigByteSwap
 * Description   : Configures byte swap for uSDHC module.
 *
 *END**************************************************************************/
void uSDHC_ConfigByteSwap(bool swap)
{
    GPR->CTL &= ~ GPR_CTL_USDHC_BS_MASK;
    if (swap)
    {
        GPR->CTL |= GPR_CTL_USDHC_BS(1);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_SetAdmaTableConfig
 * Description   : Configures the ADMA table
 *
 *END**************************************************************************/
status_t uSDHC_SetAdmaTableConfig(uSDHC_Type * base,
                                  usdhc_dma_mode_t dmaMode,
                                  uint32_t * table,
                                  uint32_t tableSize,
                                  const uint32_t * data,
                                  uint32_t dataBytes)
{
    status_t error = STATUS_SUCCESS;
    const uint32_t * startAddress;
    uint32_t entries;
    uint32_t i;
#if defined uSDHC_ENABLE_ADMA1
    usdhc_adma1_descriptor_t *adma1EntryAddress;
#endif
    usdhc_adma2_descriptor_t *adma2EntryAddress;

    /* If dmaMode is ADMA1/ADMA2 and table is null then return error */
    if ((!table) && ((dmaMode == uSDHC_DMA_MODE_ADMA1) || (dmaMode == uSDHC_DMA_MODE_ADMA2)))
    {
        error = STATUS_ERROR;
    }
    else
    {
        switch (dmaMode)
        {
            case uSDHC_DMA_MODE_NO:
                /* Do nothing if ADMA is disabled */
                break;
#if defined uSDHC_ENABLE_ADMA1
            case uSDHC_DMA_MODE_ADMA1:
                startAddress = data;
                /* Check if ADMA descriptor's number is enough. */
                entries = ((dataBytes / uSDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY) + 1U);
                /* ADMA1 needs two descriptors to finish a transfer */
                entries <<= 1U;
                /* If the number of entries needed to transfer data is larger than number of ADMA table, return STATUS_USDHC_OUT_OF_RANGE */
                if (entries > (tableSize / sizeof(usdhc_adma1_descriptor_t)))
                {
                    error = STATUS_USDHC_OUT_OF_RANGE;
                }
                else
                {
                    adma1EntryAddress = (usdhc_adma1_descriptor_t *)(table);
                    for (i = 0U; i < entries; i += 2U)
                    {
                        /* Each descriptor for ADMA1 is 32-bit in length */
                        if ((dataBytes - sizeof(uint32_t) * (startAddress - data)) <= uSDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY)
                        {
                            /* The last piece of data, setting end flag in descriptor */
                            adma1EntryAddress[i] = ((uint32_t)(dataBytes - sizeof(uint32_t) * (startAddress - data)) << uSDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT);
                            adma1EntryAddress[i] |= uSDHC_ADMA1_DESCRIPTOR_TYPE_SET_LENGTH_MASK;
                            adma1EntryAddress[i + 1U] = ((uint32_t)(startAddress));
                            adma1EntryAddress[i + 1U] |= (uSDHC_ADMA1_DESCRIPTOR_TYPE_TRANSFER_MASK | uSDHC_ADMA1_DESCRIPTOR_END_MASK);
                        }
                        else
                        {
                            adma1EntryAddress[i] = ((uint32_t)uSDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY << uSDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT);
                            adma1EntryAddress[i] |= uSDHC_ADMA1_DESCRIPTOR_TYPE_SET_LENGTH_MASK;
                            adma1EntryAddress[i + 1U] = ((uint32_t)(startAddress) << uSDHC_ADMA1_DESCRIPTOR_ADDR_SHIFT);
                            adma1EntryAddress[i + 1U] |= uSDHC_ADMA1_DESCRIPTOR_TYPE_TRANSFER_MASK;
                            startAddress += uSDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY / sizeof(uint32_t);
                        }
                    }

                    /* When use ADMA, disable simple DMA */
                    base->S_ADDR = 0U;
                    base->ADMA_SYS_ADDR = (uint32_t)table;
                }
                break;
#endif /* uSDHC_ENABLE_ADMA1 */
            case uSDHC_DMA_MODE_ADMA2:
                startAddress = data;
                /* Check if ADMA descriptor's number is enough. */
                entries = ((dataBytes / uSDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY) + 1U);
                /* If the number of entries needed to transfer data is larger than number of ADMA table, return STATUS_USDHC_OUT_OF_RANGE */
                if (entries > (tableSize / sizeof(usdhc_adma2_descriptor_t)))
                {
                    error = STATUS_USDHC_OUT_OF_RANGE;
                }
                else
                {
                    adma2EntryAddress = (usdhc_adma2_descriptor_t *)(table);
                    for (i = 0U; i < entries; i++)
                    {
                        /* Each descriptor for ADMA2 is 64-bit in length */
                        if ((dataBytes - (sizeof(uint32_t) * ((uint32_t)startAddress - (uint32_t)data))) <= uSDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY)
                        {
                            /* The last piece of data, setting end flag in descriptor */
                            adma2EntryAddress[i].address = startAddress;
                            adma2EntryAddress[i].attribute = ((dataBytes - (sizeof(uint32_t) * ((uint32_t)startAddress - (uint32_t)data))) << uSDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT);
                            adma2EntryAddress[i].attribute |= (uSDHC_ADMA2_DESCRIPTOR_TYPE_TRANSFER_MASK | uSDHC_ADMA2_DESCRIPTOR_END_MASK);
                        }
                        else
                        {
                            adma2EntryAddress[i].address = startAddress;
                            adma2EntryAddress[i].attribute = (((uSDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY / sizeof(uint32_t)) * sizeof(uint32_t))
                                 << uSDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT);
                            adma2EntryAddress[i].attribute |= uSDHC_ADMA2_DESCRIPTOR_TYPE_TRANSFER_MASK;
                            startAddress += uSDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY / sizeof(uint32_t);
                        }
                    }

                    /* When use ADMA, disable simple DMA */
                    base->S_ADDR = 0U;
                    base->ADMA_SYS_ADDR = (uint32_t)table;
                }
                break;
            default:
                /* Do nothing */
                break;
        }
    }

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_StartTransfer
 * Description   : Start a transfer
 *
 *END**************************************************************************/
void uSDHC_StartTransfer(uSDHC_Type * base,
                         const usdhc_command_t * command,
                         const usdhc_data_t * data)
{
    uint32_t flags = 0U;
    usdhc_transfer_config_t transferConfig;
    usdhc_dma_mode_t dmaMode;

    /* Define the flag corresponding to each response type. */
    switch (command->responseType)
    {
        case uSDHC_RESPONSE_TYPE_NONE:
            break;
        case uSDHC_RESPONSE_TYPE_R1: /* Response 1 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG | uSDHC_ENABLE_INDEX_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R1b: /* Response 1 with busy */
            flags |= (uSDHC_RESPONSE_LENGTH48_BUSY_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG | uSDHC_ENABLE_INDEX_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R2: /* Response 2 */
            flags |= (uSDHC_RESPONSE_LENGTH136_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R3: /* Response 3 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R4: /* Response 4 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R5: /* Response 5 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R5b: /* Response 5 with busy */
            flags |= (uSDHC_RESPONSE_LENGTH48_BUSY_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG | uSDHC_ENABLE_INDEX_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R6: /* Response 6 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG | uSDHC_ENABLE_INDEX_CHECK_FLAG);
            break;
        case uSDHC_RESPONSE_TYPE_R7: /* Response 7 */
            flags |= (uSDHC_RESPONSE_LENGTH48_FLAG | uSDHC_ENABLE_CRC_CHECK_FLAG | uSDHC_ENABLE_INDEX_CHECK_FLAG);
            break;
        default:
            /* Do nothing */
            break;
    }
    if (command->type == uSDHC_COMMAND_TYPE_ABORT)
    {
        flags |= uSDHC_CMD_TYPE_ABORT_FLAG;
    }

    if (data != NULL)
    {
        flags |= uSDHC_DATA_PRESENT_FLAG;
        dmaMode = (usdhc_dma_mode_t)((base->PROT_CTRL & uSDHC_PROT_CTRL_DMASEL_MASK) >> uSDHC_PROT_CTRL_DMASEL_SHIFT);
        if (dmaMode != uSDHC_DMA_MODE_NO)
        {
            flags |= uSDHC_ENABLE_DMA_FLAG;
        }
        if (data->rxData != NULL)
        {
            flags |= uSDHC_DATA_READ_FLAG;
        }
        if (data->blockCount > 1U)
        {
            flags |= (uSDHC_MULTIPLE_BLOCK_FLAG | uSDHC_ENABLE_BLOCK_COUNT_FLAG);
            if (data->enableAutoCMD12)
            {
                /* Enable Auto command 12. */
                flags |= uSDHC_ENABLE_AUTO_CMD12_FLAG;
            }
        }
        if (data->blockCount > uSDHC_MAX_BLOCK_COUNT)
        {
            transferConfig.dataBlockSize = data->blockSize;
            transferConfig.dataBlockCount = uSDHC_MAX_BLOCK_COUNT;

            flags &= ~(uint32_t)uSDHC_ENABLE_BLOCK_COUNT_FLAG;
        }
        else
        {
            transferConfig.dataBlockSize = data->blockSize;
            transferConfig.dataBlockCount = data->blockCount;
        }
    }
    else
    {
        transferConfig.dataBlockSize = 0U;
        transferConfig.dataBlockCount = 0U;
    }

    transferConfig.commandArgument = command->argument;
    transferConfig.commandIndex = command->index;
    transferConfig.flags = flags;
    uSDHC_SetTransferConfig(base, &transferConfig);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_SetTransferConfig
 * Description   : Configure the transfer registers
 *
 *END**************************************************************************/
static void uSDHC_SetTransferConfig(uSDHC_Type * base,
                                    const usdhc_transfer_config_t * config)
{
    base->BLK_ATT = ((base->BLK_ATT & ~(uSDHC_BLK_ATT_BLKSIZE_MASK | uSDHC_BLK_ATT_BLKCNT_MASK)) |
                     (uSDHC_BLK_ATT_BLKSIZE(config->dataBlockSize) | uSDHC_BLK_ATT_BLKCNT(config->dataBlockCount)));
    base->CMD_ARG = config->commandArgument;
    base->MIX_CTRL = config->flags & (uSDHC_MIX_CTRL_DMAEN_MASK | uSDHC_MIX_CTRL_MSBSEL_MASK | uSDHC_MIX_CTRL_BCEN_MASK |
                        uSDHC_MIX_CTRL_DTDSEL_MASK | uSDHC_MIX_CTRL_AC12EN_MASK);
    base->CMD_XFR_TYP = (((config->commandIndex << uSDHC_CMD_XFR_TYP_CMDINX_SHIFT) & uSDHC_CMD_XFR_TYP_CMDINX_MASK) |
                    (config->flags & (uSDHC_CMD_XFR_TYP_DPSEL_MASK | uSDHC_CMD_XFR_TYP_CMDTYP_MASK | uSDHC_CMD_XFR_TYP_CICEN_MASK |
                    uSDHC_CMD_XFR_TYP_CCCEN_MASK | uSDHC_CMD_XFR_TYP_RSPTYP_MASK)));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_ReceiveCommandResponse
 * Description   : Get 4 bytes responses from card
 *
 *END**************************************************************************/
static void uSDHC_ReceiveCommandResponse(const uSDHC_Type * base,
                                         usdhc_command_t * command)
{
    uint32_t i;

    if (command->responseType != uSDHC_RESPONSE_TYPE_NONE)
    {
        command->response[0U] = uSDHC_GetCommandResponse(base, 0U);
        if (command->responseType == uSDHC_RESPONSE_TYPE_R2)
        {
            command->response[1U] = uSDHC_GetCommandResponse(base, 1U);
            command->response[2U] = uSDHC_GetCommandResponse(base, 2U);
            command->response[3U] = uSDHC_GetCommandResponse(base, 3U);

            i = 4U;
            /* R3-R2-R1-R0(lowest 8 bit is invalid bit) has the same format as R2 format in SD specification document
            after removed internal CRC7 and end bit. */
            do
            {
                command->response[i - 1U] <<= 8U;
                if (i > 1U)
                {
                    command->response[i - 1U] |= ((command->response[i - 2U] & uSDHC_RESPONSE_TYPE_R2_MASK) >> uSDHC_RESPONSE_TYPE_R2_SHIFT);
                }
            } while ((i--) > 0U);
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_ReadDataPort
 * Description   : Get the received data from card. This function is used to get the received data from card in case ADMA is not used
 *
 *END**************************************************************************/
static uint32_t uSDHC_ReadDataPort(const uSDHC_Type * base,
                                   const usdhc_data_t * data,
                                   uint32_t transferredWords)
{
    uint32_t i = 0;
    uint32_t totalWords;
    uint32_t wordsCanBeRead; /* The words can be read at this time. */
    uint32_t readWatermark = ((base->WTMK_LVL & uSDHC_WTMK_LVL_RD_WML_MASK) >> uSDHC_WTMK_LVL_RD_WML_SHIFT);

    totalWords = ((data->blockCount * data->blockSize) / sizeof(uint32_t));

    if (readWatermark >= totalWords)
    {
        wordsCanBeRead = totalWords;
    }
    else if ((readWatermark < totalWords) && ((totalWords - transferredWords) >= readWatermark))
    {
        wordsCanBeRead = readWatermark;
    }
    else
    {
        wordsCanBeRead = (totalWords - transferredWords);
    }

    while (i < wordsCanBeRead)
    {
        data->rxData[transferredWords] = uSDHC_ReadData(base);
        transferredWords++;
        i++;
    }

    return transferredWords;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_WriteDataPort
 * Description   : Write the data to the card. This function is used to write the desired data to the card in case ADMA is not used
 *
 *END**************************************************************************/
static uint32_t uSDHC_WriteDataPort(uSDHC_Type * base,
                                    const usdhc_data_t * data,
                                    uint32_t transferredWords)
{
    uint32_t i = 0;
    uint32_t totalWords;
    uint32_t wordsCanBeWritten;     /* Words can be written at this time. */
    uint32_t writeWatermark = ((base->WTMK_LVL & uSDHC_WTMK_LVL_WR_WML_MASK) >> uSDHC_WTMK_LVL_WR_WML_SHIFT);

    totalWords = ((data->blockCount * data->blockSize) / sizeof(uint32_t));

    if (writeWatermark >= totalWords)
    {
        wordsCanBeWritten = totalWords;
    }
    else if ((writeWatermark < totalWords) && ((totalWords - transferredWords) >= writeWatermark))
    {
        wordsCanBeWritten = writeWatermark;
    }
    else
    {
        wordsCanBeWritten = (totalWords - transferredWords);
    }

    while (i < wordsCanBeWritten)
    {
        uSDHC_WriteData(base, data->txData[transferredWords++]);
        i++;
    }

    return transferredWords;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_TransferHandleCommand
 * Description   : The interrupt handler for transferring command
 *
 *END**************************************************************************/
static void uSDHC_TransferHandleCommand(uint32_t instance,
                                        uint32_t interruptFlags)
{
    usdhc_state_t * state = g_usdhcStates[instance];

    if (((interruptFlags & uSDHC_COMMAND_ERROR_INT) != 0U) && (state->transferCallback != NULL))
    {
        /* Error occurs, invoke callback function with error code */
        state->transferCallback(instance, interruptFlags, state->transferCallbackParam);
    }
    else
    {
        /* Receive response */
        uSDHC_ReceiveCommandResponse(g_usdhcBases[instance], state->command);
        if (!(state->data))
        {
            if (state->isBlocking)
            {
                (void)OSIF_SemaPost(&(state->semaTransferComplete));
            }
            else if ((state->transferCallback) != NULL)
            {
                state->transferCallback(instance, 0U, state->transferCallbackParam);
            }
            else
            {
                /* Do nothing */
            }
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_TransferHandleData
 * Description   : The interrupt handler for transferring data
 *
 *END**************************************************************************/
static void uSDHC_TransferHandleData(uint32_t instance,
                                     uint32_t interruptFlags)
{
    usdhc_state_t * state = g_usdhcStates[instance];

    if (((interruptFlags & (uSDHC_DATA_ERROR_INT | uSDHC_INT_STATUS_DMAE_MASK)) != 0U) && (state->transferCallback != NULL))
    {
        /* Error occurs, invoke callback function with error code */
        state->transferCallback(instance, interruptFlags, state->transferCallbackParam);
    }
    else
    {
        if ((interruptFlags & uSDHC_INT_STATUS_BRR_MASK) != 0U)
        {
            state->transferredWords = uSDHC_ReadDataPort(g_usdhcBases[instance], state->data, state->transferredWords);
        }
        else if ((interruptFlags & uSDHC_INT_STATUS_BWR_MASK) != 0U)
        {
            state->transferredWords = uSDHC_WriteDataPort(g_usdhcBases[instance], state->data, state->transferredWords);
        }
        else if ((interruptFlags & uSDHC_INT_STATUS_TC_MASK) != 0U)
        {
            if (state->isBlocking)
            {
                (void)OSIF_SemaPost(&(state->semaTransferComplete));
            }
            else if ((state->transferCallback) != NULL)
            {
                state->transferCallback(instance, 0U, state->transferCallbackParam);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            /* Do nothing */
        }
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_TransferHandleCardDetect
 * Description   : The interrupt handler for detecting the card
 *
 *END**************************************************************************/
static void uSDHC_TransferHandleCardDetect(uint32_t instance,
                                           uint32_t interruptFlags)
{
    const usdhc_state_t * state = g_usdhcStates[instance];

    if ((interruptFlags & uSDHC_INT_STATUS_CINS_MASK) != 0U)
    {
        if ((state->cardInsertCallback) != NULL)
        {
            state->cardInsertCallback(instance, 0U, state->cardInsertCallbackParam);
        }
    }
    else if ((interruptFlags & uSDHC_INT_STATUS_CRM_MASK) != 0U)
    {
        if ((state->cardRemoveCallback) != NULL)
        {
            state->cardRemoveCallback(instance, 0U, state->cardRemoveCallbackParam);
        }
    }
    else
    {
        /* Do nothing */
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_HandleCardInterrupt
 * Description   : The interrupt handler for card interrupt
 *
 *END**************************************************************************/
static void uSDHC_HandleCardInterrupt(uint32_t instance)
{
    const usdhc_state_t * state = g_usdhcStates[instance];

    if ((state->cardIntCallback) != NULL)
    {
        state->cardIntCallback(instance, 0U, state->cardIntCallbackParam);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_HandleBlockGapInterrupt
 * Description   : The interrupt handler for block gap
 *
 *END**************************************************************************/
static void uSDHC_HandleBlockGapInterrupt(uint32_t instance)
{
    const usdhc_state_t * state = g_usdhcStates[instance];

    if ((state->blockGapCallback) != NULL)
    {
        state->blockGapCallback(instance, 0U, state->blockGapCallbackParam);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_TransferHandleIRQ
 * Description   : The common interrupt handler for uSDHC
 *
 *END**************************************************************************/
void uSDHC_TransferHandleIRQ(uint32_t instance)
{
    uSDHC_Type * base = g_usdhcBases[instance];
    uint32_t interruptFlags = 0;

    interruptFlags = uSDHC_GetInterruptStatusFlags(base);

    if ((interruptFlags & uSDHC_CARD_DETECT_INT) != 0U)
    {
        uSDHC_TransferHandleCardDetect(instance, (interruptFlags & uSDHC_CARD_DETECT_INT));
    }
    if ((interruptFlags & uSDHC_COMMAND_INT) != 0U)
    {
        uSDHC_TransferHandleCommand(instance, (interruptFlags & uSDHC_COMMAND_INT));
    }
    if ((interruptFlags & uSDHC_DATA_INT) != 0U)
    {
        uSDHC_TransferHandleData(instance, (interruptFlags & uSDHC_DATA_INT));
    }
    if ((interruptFlags & uSDHC_INT_STATUS_CINT_MASK) != 0U)
    {
        uSDHC_HandleCardInterrupt(instance);
    }
    if ((interruptFlags & uSDHC_INT_STATUS_BGE_MASK) != 0U)
    {
        uSDHC_HandleBlockGapInterrupt(instance);
    }

    uSDHC_ClearInterruptStatusFlags(base, interruptFlags);
}
