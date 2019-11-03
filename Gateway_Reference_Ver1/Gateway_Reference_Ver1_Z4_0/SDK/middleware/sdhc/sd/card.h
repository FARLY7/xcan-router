/*****************************************************************************/
/* FILE NAME: card.h COPYRIGHT (c) NXP Semiconductors 2017                   */
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

#ifndef SD_CARD_H
#define SD_CARD_H
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "usdhc_driver.h"
#include "sd/spec.h"

/*!
 * @addtogroup SD_CARD
 * @ingroup sdhc_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Driver version. */
#define FSL_SDMMC_DRIVER_VERSION (MAKE_VERSION(2U, 0U, 0U)) /*2.0.0*/

/*! @brief Default block size */
#define FSL_SDMMC_DEFAULT_BLOCK_SIZE (512U)

/*! @brief SD/MMC card API's running status. */
enum _sdmmc_status
{
    aStatus_SDMMC_NotSupportYet = (int32_t)STATUS_TDM_DIARY_FULL + 1,             /*!< Haven't supported */
    aStatus_SDMMC_TransferFailed,            /*!< Send command failed */
    aStatus_SDMMC_SetCardBlockSizeFailed,    /*!< Set block size failed */
    aStatus_SDMMC_HostNotSupport,            /*!< Host doesn't support */
    aStatus_SDMMC_CardNotSupport,            /*!< Card doesn't support */
    aStatus_SDMMC_AllSendCidFailed,          /*!< Send CID failed */
    aStatus_SDMMC_SendRelativeAddressFailed, /*!< Send relative address failed */
    aStatus_SDMMC_SendCsdFailed,             /*!< Send CSD failed */
    aStatus_SDMMC_SelectCardFailed,          /*!< Select card failed */
    aStatus_SDMMC_SendScrFailed,             /*!< Send SCR failed */
    aStatus_SDMMC_SetDataBusWidthFailed,    /*!< Set bus width failed */
    aStatus_SDMMC_GoIdleFailed,             /*!< Go idle failed */
    aStatus_SDMMC_HandShakeOperationConditionFailed, /*!< Send Operation Condition failed */
    aStatus_SDMMC_SendApplicationCommandFailed,      /*!< Send application command failed */
    aStatus_SDMMC_SwitchFailed,           /*!< Switch command failed */
    aStatus_SDMMC_StopTransmissionFailed, /*!< Stop transmission failed */
    aStatus_SDMMC_WaitWriteCompleteFailed,    /*!< Wait write complete failed */
    aStatus_SDMMC_SetBlockCountFailed,        /*!< Set block count failed */
    aStatus_SDMMC_SetRelativeAddressFailed,   /*!< Set relative address failed */
    aStatus_SDMMC_SwitchHighSpeedFailed,      /*!< Switch high speed failed */
    aStatus_SDMMC_SendExtendedCsdFailed,      /*!< Send EXT_CSD failed */
    aStatus_SDMMC_ConfigureBootFailed,        /*!< Configure boot failed */
    aStatus_SDMMC_ConfigureExtendedCsdFailed, /*!< Configure EXT_CSD failed */
    aStatus_SDMMC_EnableHighCapacityEraseFailed, /*!< Enable high capacity erase failed */
    aStatus_SDMMC_SendTestPatternFailed,    /*!< Send test pattern failed */
    aStatus_SDMMC_ReceiveTestPatternFailed, /*!< Receive test pattern failed */
    aStatus_SDMMC_CardWriteProtected,            /*!< Card write protection lock enabled */
    aStatus_SDMMC_SwitchVoltageFailed,           /*!< Card switch voltage failed */
};

/*! @brief SD card flags */
typedef enum _sd_card_flag
{
    aSD_SupportSpeedClassControlCmd = (1U << 0U), /*!< Card supports speed class */
    aSD_SupportSetBlockCountCmd = (1U << 1U), /*!< Card supports CMD23 */
    aSD_Support4BitWidthFlag = (1U << 2U),    /*!< Support 4-bit data width */
    aSD_SupportHighCapacityFlag = (1U << 3U), /*!< Support high capacity */
    aSD_SupportSdhcFlag = (1U << 4U),         /*!< Card is SDHC */
    aSD_SupportSdxcFlag = (1U << 5U),         /*!< Card is SDXC */
    aSD_WriteProtectFlag = (1U << 6U),         /*!< Card has write protection lock enabled */
	aSD_SupportVoltage180v = (1U << 7U),       /*!< Card supports 1.8V */
} sd_card_flag_t;

/*! @brief SDHC transfer function. */
typedef status_t (*usdhc_transfer_function_t)(uint32_t instance, usdhc_transfer_t *content);

/*! @brief SDHC host descriptor */
typedef struct
{
    uint32_t instance;                   /*!< SDHC instance number */
    usdhc_config_t config;              /*!< SDHC configuration */
    usdhc_capability_t capability;      /*!< SDHC capability information */
    usdhc_transfer_function_t transfer; /*!< SDHC transfer function */
} usdhc_host_t;

/*! @brief SDHC transfer storage. */
typedef struct {
	usdhc_transfer_t content;     /* Transfer state */
	usdhc_command_t command;	  /* Card command-related attribute */
	usdhc_data_t data;            /* Data-related attribute */
} usdhc_transfer_storage_t;

/*!
 * @brief SD card state
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _sd_card
{
    usdhc_host_t host; /*!< Host information */

    uint32_t busClock_Hz;     /*!< SD bus clock frequency united in Hz */
    uint32_t relativeAddress; /*!< Relative address of the card */
    uint32_t version;         /*!< Card version */
    uint32_t flags;           /*!< Flags in sd_card_flag_t */
    uint32_t rawCid[4U];      /*!< Raw CID content */
    uint32_t rawCsd[4U];      /*!< Raw CSD content */
    uint32_t rawScr[2U];      /*!< Raw CSD content */
    uint32_t ocr;             /*!< Raw OCR content */
    sd_cid_t cid;             /*!< CID */
    sd_csd_t csd;             /*!< CSD */
    sd_scr_t scr;             /*!< SCR */
    uint32_t blockCount;      /*!< Card total block number */
    uint32_t blockSize;       /*!< Card block size */
} sd_card_t;

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDCARD Function
 * @{
 */

/*!
 * @brief Initialize usdhc_transfer_storage_t structure.
 *
 * @param l_transfer_storage The pointer to usdhc_transfer_storage_t.
 * @retval void.
 */
void SD_Transfer_Init(usdhc_transfer_storage_t *l_transfer_storage);

/*!
 * @brief Initialize the card on a specific host controller.
 *
 * This function initializes the card on a specific host controller.
 *
 * @param card The pointer to save card related information.
 * @retval aStatus_SDMMC_GoIdleFailed Go idle failed.
 * @retval aStatus_SDMMC_NotSupportYet Card not support.
 * @retval aStatus_SDMMC_SendOperationConditionFailed Send operation condition failed.
 * @retval aStatus_SDMMC_AllSendCidFailed Send CID failed.
 * @retval aStatus_SDMMC_SendRelativeAddressFailed Send relative address failed.
 * @retval aStatus_SDMMC_SendCsdFailed Send CSD failed.
 * @retval aStatus_SDMMC_SelectCardFailed Send SELECT_CARD command failed.
 * @retval aStatus_SDMMC_SendScrFailed Send SCR failed.
 * @retval aStatus_SDMMC_SetBusWidthFailed Set bus width failed.
 * @retval aStatus_SDMMC_SwitchHighSpeedFailed Switch high speed failed.
 * @retval aStatus_SDMMC_SetCardBlockSizeFailed Set card block size failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SD_Init(sd_card_t *card);

/*!
 * @brief Deinitialize the card.
 *
 * This function deinitializes the specific card.
 *
 * @param card The specific card.
 */
common_status_t SD_DeInit(sd_card_t *card);

/*!
 * @brief Check whether the card is write-protected.
 *
 * This function checks if the card is write-protected via CSD register or WP pin.
 *
 * @param card The specific card.
 * @retval true Card is read only.
 * @retval false Card isn't read only.
 */
bool SD_CheckReadOnly(sd_card_t *card);

/*!
 * @brief Read blocks from the specific card.
 *
 * This function reads blocks from specific card, with default block size defined by SDHC_CARD_DEFAULT_BLOCK_SIZE.
 *
 * @param card The card descriptor.
 * @param buffer The buffer to save the data read from card.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to read.
 * @retval aStatus_InvalidArgument Invalid argument.
 * @retval aStatus_SDMMC_CardNotSupport Card not support.
 * @retval aStatus_SDMMC_NotSupportYet Not support now.
 * @retval aStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SD_ReadBlocks(sd_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Write blocks of data to the specific card.
 *
 * This function writes blocks to specific card, with default block size 512 bytes.
 *
 * @param card The card descriptor.
 * @param buffer The buffer holding the data to be written to the card.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to write.
 * @retval aStatus_InvalidArgument Invalid argument.
 * @retval aStatus_SDMMC_CardWriteProtected Card write protected
 * @retval aStatus_SDMMC_NotSupportYet Not support now.
 * @retval aStatus_SDMMC_CardNotSupport Card not support.
 * @retval aStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SD_WriteBlocks(sd_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Erase blocks of the specific card.
 *
 * This function erases blocks of a specific card, with default block size 512 bytes.
 *
 * @param card The card descriptor.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to erase.
 * @retval aStatus_InvalidArgument Invalid argument.
 * @retval aStatus_SDMMC_CardWriteProtected Card write protected
 * @retval aStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval aStatus_Success Operate successfully.
 */
common_status_t SD_EraseBlocks(sd_card_t *card, uint32_t startBlock, uint32_t blockCount);

/* @} */

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* SD_CARD_H*/
