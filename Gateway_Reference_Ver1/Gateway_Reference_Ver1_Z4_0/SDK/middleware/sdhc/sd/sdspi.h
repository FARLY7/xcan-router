/*****************************************************************************/
/* FILE NAME: sdspi.h COPYRIGHT (c) NXP Semiconductors 2017                  */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: SD implementation                                            */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

/*
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SD_SDSPI_H
#define SD_SDSPI_H

#include "sd/common.h"
#include "sd/spec.h"

/******************************************************************************
 * Definitions
 *****************************************************************************/
/*! @brief Driver version. */
#define SD_SDSPI_DRIVER_VERSION (MAKE_VERSION(2U, 0U, 0U)) /*2.0.0*/

/*! @brief Default block size */
#define SD_SDSPI_DEFAULT_BLOCK_SIZE (512U)

/*!
 * @addtogroup SD_SDSPI
 * @ingroup sdhc_mw
 * @{
 */

/*! @brief SDSPI API status */
typedef enum _sdspi_status
{
    aStatus_SDSPI_SetFrequencyFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 0), /*!< Set frequency failed */
    aStatus_SDSPI_ExchangeFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 1),     /*!< Exchange data on SPI bus failed */
    aStatus_SDSPI_WaitReadyFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 2),    /*!< Wait card ready failed */
    aStatus_SDSPI_ResponseError = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 3),      /*!< Response is error */
    aStatus_SDSPI_WriteProtected = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 4),     /*!< Write protected */
    aStatus_SDSPI_GoIdleFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 5),       /*!< Go idle failed */
    aStatus_SDSPI_SendCommandFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 6),  /*!< Send command failed */
    aStatus_SDSPI_ReadFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 7),         /*!< Read data failed */
    aStatus_SDSPI_WriteFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 8),        /*!< Write data failed */
    aStatus_SDSPI_SendInterfaceConditionFailed =
        MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 9), /*!< Send interface condition failed */
    aStatus_SDSPI_SendOperationConditionFailed =
        MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 10),                                    /*!< Send operation condition failed */
    aStatus_SDSPI_ReadOcrFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 11),          /*!< Read OCR failed */
    aStatus_SDSPI_SetBlockSizeFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 12),     /*!< Set block size failed */
    aStatus_SDSPI_SendCsdFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 13),          /*!< Send CSD failed */
    aStatus_SDSPI_SendCidFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 14),          /*!< Send CID failed */
    aStatus_SDSPI_StopTransmissionFailed = MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 15), /*!< Stop transmission failed */
    aStatus_SDSPI_SendApplicationCommandFailed =
        MAKE_STATUS((int32_t)kStatusGroup_SDSPI, 16), /*!< Send application command failed */
} sdspi_status_t;

/*! @brief SDSPI response type */
typedef enum _sdspi_response_type
{
    aSDSPI_ResponseTypeR1 = 0U,  /*!< Response 1 */
    aSDSPI_ResponseTypeR1b = 1U, /*!< Response 1 with busy */
    aSDSPI_ResponseTypeR2 = 2U,  /*!< Response 2 */
    aSDSPI_ResponseTypeR3 = 3U,  /*!< Response 3 */
    aSDSPI_ResponseTypeR7 = 4U,  /*!< Response 7 */
} sdspi_response_type_t;

/*! @brief SDSPI card flag */
typedef enum _sdspi_card_flag
{
    aSDSPI_SupportHighCapacityFlag = (1U << 0U), /*!< Card is high capacity */
    aSDSPI_SupportSdhcFlag = (1U << 1U),         /*!< Card is SDHC */
    aSDSPI_SupportSdxcFlag = (1U << 2U),         /*!< Card is SDXC */
    aSDSPI_SupportSdscFlag = (1U << 3U),         /*!< Card is SDSC */
} sdspi_card_flag_t;

/*! @brief SDSPI command */
typedef struct _sdspi_command
{
    uint8_t index;        /*!< Command index */
    uint32_t argument;    /*!< Command argument */
    uint8_t responseType; /*!< Response type */
    uint8_t response[5U]; /*!< Response content */
} sdspi_command_t;

/*! @brief SDSPI host state. */
typedef struct _sdspi_host
{
    uint32_t busBaudRate; /*!< Bus baud rate */

    status_t (*setFrequency)(uint32_t frequency);                   /*!< Set frequency of SPI */
    status_t (*exchange)(uint8_t *in, uint8_t *out, uint32_t size); /*!< Exchange data over SPI */
    uint32_t (*getCurrentMilliseconds)(void);                       /*!< Get current time in milliseconds */
} sdspi_host_t;

/*!
 * @brief SD Card Structure
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _sdspi_card
{
    sdspi_host_t *host;       /*!< Host state information */
    uint32_t relativeAddress; /*!< Relative address of the card */
    uint32_t flags;           /*!< Flags defined in sdspi_card_flag_t. */
    uint8_t rawCid[16U];      /*!< Raw CID content */
    uint8_t rawCsd[16U];      /*!< Raw CSD content */
    uint8_t rawScr[8U];       /*!< Raw SCR content */
    uint32_t ocr;             /*!< Raw OCR content */
    sd_cid_t cid;             /*!< CID */
    sd_csd_t csd;             /*!< CSD */
    sd_scr_t scr;             /*!< SCR */
    uint32_t blockCount;      /*!< Card total block number */
    uint32_t blockSize;       /*!< Card block size */
} sdspi_card_t;

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDSPI Function
 * @{
 */

/*!
 * @brief Initialize the card on a specific SPI instance.
 *
 * This function initializes the card on a specific SPI instance.
 *
 * @param card Card descriptor
 * @retval aStatus_SDSPI_SetFrequencyFailed Set frequency failed.
 * @retval aStatus_SDSPI_GoIdleFailed Go idle failed.
 * @retval aStatus_SDSPI_SendInterfaceConditionFailed Send interface condition failed.
 * @retval aStatus_SDSPI_SendOperationConditionFailed Send operation condition failed.
 * @retval aStatus_Timeout Send command timeout.
 * @retval aStatus_SDSPI_NotSupportYet Not support yet.
 * @retval aStatus_SDSPI_ReadOcrFailed Read OCR failed.
 * @retval aStatus_SDSPI_SetBlockSizeFailed Set block size failed.
 * @retval aStatus_SDSPI_SendCsdFailed Send CSD failed.
 * @retval aStatus_SDSPI_SendCidFailed Send CID failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SDSPI_Init(sdspi_card_t *card);

/*!
 * @brief Deinitialize the card.
 *
 * This function deinitializes the specific card.
 *
 * @param card Card descriptor
 */
void SDSPI_Deinit(sdspi_card_t *card);

/*!
 * @brief Check whether the card is write-protected.
 *
 * This function checks if the card is write-protected via CSD register.
 *
 * @param card Card descriptor.
 * @retval true Card is read only.
 * @retval false Card isn't read only.
 */
bool SDSPI_CheckReadOnly(sdspi_card_t *card);

/*!
 * @brief Read blocks from the specific card.
 *
 * This function reads blocks from specific card.
 *
 * @param card Card descriptor.
 * @param buffer the buffer to hold the data read from card
 * @param startBlock the start block index
 * @param blockCount the number of blocks to read
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ReadFailed Read data failed.
 * @retval aStatus_SDSPI_StopTransmissionFailed Stop transmission failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SDSPI_ReadBlocks(sdspi_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Write blocks of data to the specific card.
 *
 * This function writes blocks to specific card
 *
 * @param card Card descriptor.
 * @param buffer the buffer holding the data to be written to the card
 * @param startBlock the start block index
 * @param blockCount the number of blocks to write
 * @retval aStatus_SDSPI_WriteProtected Card is write protected.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval aStatus_SDSPI_WriteFailed Write data failed.
 * @retval aStatus_SDSPI_ExchangeFailed Exchange data over SPI failed.
 * @retval aStatus_SDSPI_WaitReadyFailed Wait card to be ready status failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SDSPI_WriteBlocks(sdspi_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/* @} */
#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* SD_SDSPI_H */
