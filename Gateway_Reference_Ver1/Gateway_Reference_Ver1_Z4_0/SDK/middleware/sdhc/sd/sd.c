/*****************************************************************************/
/* FILE NAME: sd.c COPYRIGHT (c) NXP Semiconductors 2017                     */
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

#include "Cpu.h"
#include "osif.h"

#include <stdbool.h>
#include <string.h>

#include "usdhc_driver.h"
#include "sd/card.h"
#include "sd/sdmmc.h"
#include "sd/common.h"


/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer
 * to object type and a pointer to a different object type.
 * This cast is to required write 8bit or 16bit data into corresponding address.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, A conversion should not be performed between a pointer to object and an integer type.
 * This cast is to required for byte-reversal functions.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, A cast shall not be performed between pointer to void and an arithmetic type.
 * This cast is to required for byte-reversal functions.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is need for DEV_ASSERT macro condition.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, The value of a composite expression shall not be cast
 * to a different essential type category or a wider essential type.
 * This is need for bit-shift DATA0 macro declared in usdhc driver source.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 12.2, The right hand operand of a shift operator shall lie
 * in the range zero to one less than the width than the width in bits of the essential type of the left hand operand.
 * This is need for bit-shift DATA0 macro declared in usdhc driver source.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.3, All declarations of an object or function shall use the same names and type qualifiers.
 * This is needed for stdio routines.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be visible when an object or function with external linkage is defined.
 * This is needed for stdio routines.
 *
 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Send SELECT_CARD command to set the card to be transfer state or not.
 *
 * @param card Card descriptor.
 * @param isSelected True to set the card into transfer state.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_SelectCard(sd_card_t *card, bool isSelected);

/*!
 * @brief Wait write process complete.
 *
 * @param card Card descriptor.
 * @retval aStatus_Timeout Send command timeout.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_WaitWriteComplete(sd_card_t *card);

/*!
 * @brief Send SEND_APPLICATION_COMMAND command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_SendApplicationCmd(sd_card_t *card);

/*!
 * @brief Send GO_IDLE command to set the card to be idle state.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_GoIdle(sd_card_t *card);

/*!
 * @brief Send STOP_TRANSMISSION command after multiple blocks read/write.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_StopTransmission(sd_card_t *card);

/*!
 * @brief Wrapper over SD_StopTransmission routine.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_HandleStopTransmission(sd_card_t *card);

/*!
 * @brief Send SET_BLOCK_SIZE command.
 *
 * @param card Card descriptor.
 * @param blockSize Block size.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_SetBlockSize(sd_card_t *card, uint32_t blockSize);

/*!
* @brief Wrapper over SD_SetBlockSize routine.
 *
 * @param card Card descriptor.
 * @param blockCount Number of blocks.
 * @param blockSize Block size.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_HandleSetBlockCountSize(sd_card_t *card, uint32_t blockCount, uint32_t blockSize);

/*!
 * @brief Send SET_BLOCK_COUNT command.
 *
 * @param card Card descriptor.
 * @param blockCount Block count.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static inline common_status_t SD_SetBlockCount(sd_card_t *card, uint32_t blockCount);

/*!
 * @brief Send GET_RCA command to get card relative address.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SendRca(sd_card_t *card);

/*!
 * @brief Send SWITCH_FUNCTION command to switch the card function group.
 *
 * @param card Card descriptor.
 * @param mode 0 to check function group. 1 to switch function group
 * @param group Function group
 * @param number Function number in the function group.
 * @param status Switch function status.
 * @retval aStatus_SDMMC_SetCardBlockSizeFailed Set card block size failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SwitchFunction(sd_card_t *card, sd_switch_mode_t mode, sd_group_num group, uint32_t number, uint32_t *status);

/*!
 * @brief Wrapper on uSDHC_SwitchVoltage routine.
 *
 * @param card Card descriptor.
 * @retval void.
 */
static common_status_t SD_SwitchVoltage(sd_card_t *card);

/*!
 * @brief Decode raw SCR register content in the data blocks.
 *
 * @param card Card descriptor.
 * @param rawScr Raw SCR register content.
 */
static void SD_DecodeScr(sd_card_t *card, uint32_t *rawScr);

/*!
 * @brief Send GET_SCR command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_SendApplicationCommandFailed Send application command failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_NotSupportYet Not support yet.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SendScr(sd_card_t *card);

/*!
 * @brief Switch the card to be high speed mode.
 *
 * @param card Card descriptor.
 * @param group group number.
 * @param function function number.
 * @retval aStatus_SDMMC_CardNotSupport Card not support.
 * @retval aStatus_SDMMC_SwitchFailed Switch failed.
 * @retval aStatus_SDMMC_NotSupportYet Not support yet.
 * @retval aStatus_Fail Switch failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_CheckAndSwitchFunction(sd_card_t *card, sd_group_num group, uint32_t function);

/*!
 * @brief Send SET_DATA_WIDTH command to set SD bus width.
 *
 * @param card Card descriptor.
 * @param width Data bus width.
 * @retval aStatus_SDMMC_SendApplicationCommandFailed Send application command failed.
 * @retval aStatus_InvalidArgument Invalid argument.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SetDataBusWidth(sd_card_t *card, sd_data_bus_width_t width);

/*!
 * @brief Decode raw CSD register content in the data blocks.
 *
 * @param card Card descriptor.
 * @param rawCsd Raw CSD register content.
 */
static void SD_DecodeCsd(sd_card_t *card, uint32_t *rawCsd);

/*!
 * @brief Send SEND_CSD command to get CSD register content from Card.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SendCsd(sd_card_t *card);

/*!
 * @brief Decode raw CID register content in the data blocks.
 *
 * @param rawCid raw CID register content.
 * @param card Card descriptor.
 */
static void SD_DecodeCid(sd_card_t *card, uint32_t *rawCid);

/*!
 * @brief Send GET_CID command to get CID from card.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_AllSendCid(sd_card_t *card);

/*!
 * @brief Send SEND_OPERATION_CONDITION command.
 *
 * This function sends host capacity support information and asks the accessed card to send its operating condition
 * register content.
 *
 * @param card Card descriptor.
 * @param argument The argument of the send operation condition ncomamnd.
 * @retval aStatus_SDMMC_SendApplicationCommandFailed Send application command failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_Timeout Timeout.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_ApplicationSendOperationCondition(sd_card_t *card, uint32_t argument);

/*!
 * @brief Send GET_INTERFACE_CONDITION command to get card interface condition.
 *
 * This function checks card interface condition, which includes host supply voltage information and asks the card
 * whether card supports the specified host voltage.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SendInterfaceCondition(sd_card_t *card);

/*!
 * @brief Read data from specific SD card.
 *
 * @param card Card descriptor.
 * @param buffer Buffer to save data blocks read.
 * @param startBlock Card start block number to be read.
 * @param blockSize Block size.
 * @param blockCount Block count.
 * @retval aStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval aStatus_SDMMC_WaitWriteCompleteFailed Wait write complete failed.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_Read(sd_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount);

/*!
 * @brief Write data to specific card
 *
 * @param card Card descriptor.
 * @param buffer Buffer to be sent.
 * @param startBlock Card start block number to be written.
 * @param blockSize Block size.
 * @param blockCount Block count.
 * @retval aStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval aStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_Write(
		sd_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount);

/*!
 * @brief Erase data for the given block range.
 *
 * @param card Card descriptor.
 * @param startBlock Card start block number to be erased.
 * @param blockCount The block count to be erased.
 * @retval aStatus_SDMMC_TransferFailed Transfer failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_Erase(sd_card_t *card, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Change voltage VSELECT setting.
 *
 * @param card Card descriptor.
 * @param isLowVoltage flag if low voltage should be activated.
 * @retval void.
 */
static void uSDHC_SwitchVoltage(sd_card_t *card, bool isLowVoltage);

/*!
 * @brief Switch card from initialization state into data transfer state.
 *
 * @param card Card descriptor.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SwitchToDataTransfer(sd_card_t *card);

/*!
 * @brief Set SD bus width, switch card into high speed mode if supported.
 *
 * @param card Card descriptor.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SD_SwitchHighSpeed(sd_card_t *card);

/*******************************************************************************
 * Global functions
 ******************************************************************************/

uint8_t load_hword(const volatile uint8_t *dstAddr, uintptr_t offset)
{
	uint8_t *lAddr = (uint8_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	return *lAddr;
}

void swap_hword(volatile uint8_t *dstAddr, uintptr_t offset, volatile uint8_t srcData)
{
	(void)dstAddr;
	(void)offset;
	(void)srcData;
}

#if (defined(__GNUC__) && ((defined(__powerpc__) || defined(__ppc__) || defined(__PPC__))))
uint16_t load_word(const volatile uint16_t *dstAddr, uintptr_t offset)
{
	uint16_t result;
	volatile uint16_t *lAddr = (volatile uint16_t *)((uintptr_t)(dstAddr) + offset);
	__asm__ volatile("lhbrx %0,%2,%1" : "=r" (result) : "r" (dstAddr), "bO" (offset), "m" (*lAddr));
	return result;
}

void swap_word(volatile uint16_t *dstAddr, uintptr_t offset, volatile uint16_t srcData)
{
	volatile uint16_t *lAddr = (volatile uint16_t *)((uintptr_t)(dstAddr) + offset);
	__asm__ volatile("sthbrx %1,%3,%2" : "=m" (*lAddr) : "r" (srcData), "r" (dstAddr), "bO" (offset));
}

uint32_t load_dword(const volatile uint32_t *dstAddr, uintptr_t offset)
{
	uint32_t result;
	volatile uint32_t *lAddr = (volatile uint32_t *)((uintptr_t)(dstAddr) + offset);
	__asm__ volatile("lwbrx %0,%2,%1" : "=r" (result) : "r" (dstAddr), "bO" (offset), "m" (*lAddr));
	return result;
}

void swap_dword(volatile uint32_t *dstAddr, uintptr_t offset, volatile uint32_t srcData)
{
	volatile uint32_t *lAddr = (volatile uint32_t *)((uintptr_t)(dstAddr) + offset);
	__asm__ volatile("stwbrx %1,%3,%2" : "=m" (*lAddr) : "r" (srcData), "r" (dstAddr), "bO" (offset));
}

uint64_t load_qword(const volatile uint64_t *dstAddr, uintptr_t offset)
{
	volatile uint64_t *lAddr = (volatile uint64_t *)((uintptr_t)(dstAddr) + offset);
	union {
		uint64_t u64;
		uint32_t u32[2];
	} result;
	__asm__ volatile("lwbrx %0, %3, %2\n\t"
			"lwbrx %1, %4, %2"
			: "=&r" (result.u32[1]), "=r" (result.u32[0])
			  : "r" (dstAddr), "bO" (offset), "b" (offset + 4U), "m" (*lAddr));
	return result.u64;
}

void swap_qword(volatile uint64_t *dstAddr, uintptr_t offset, volatile uint64_t srcData)
{
	volatile uint64_t *lAddr = (volatile uint64_t *)((uintptr_t)(dstAddr) + offset);
	uint32_t dataHi = (uint32_t)(srcData >> 32U);
	uint32_t dataLo = (uint32_t)(srcData & 0xFFFFFFFFU);
	__asm__ volatile("stwbrx %1, %4, %3\n\t"
			"stwbrx %2, %5, %3"
			: "=m" (*lAddr)
			  : "r" (dataLo), "r" (dataHi), "r" (dstAddr),
				"bO" (offset), "b" (offset + 4U));
}
#else
uint16_t load_word(const volatile uint16_t *dstAddr, uintptr_t offset)
{
	/*! @brief Returned data for swap_word function */
	static uint16_t rvw;

	uint16_t *lAddr = (uint16_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	rvw = *lAddr;
	swap_word(&rvw, 0U, rvw);

	return rvw;
}

void swap_word(volatile uint16_t *dstAddr, uintptr_t offset, volatile uint16_t srcData)
{
	uint16_t *lAddr = (uint16_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	uint16_t rv = srcData;
	uint16_t swapped = (rv << 8U) | (rv >> 8U);
	*lAddr = swapped;
}

uint32_t load_dword(const volatile uint32_t *dstAddr, uintptr_t offset)
{
	/*! @brief Returned data for swap_dword function */
	static uint32_t rvdw;

	uint32_t *lAddr = (uint32_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	rvdw = *lAddr;
	swap_dword(&rvdw, 0U, rvdw);

	return rvdw;
}

void swap_dword(volatile uint32_t *dstAddr, uintptr_t offset, volatile uint32_t srcData)
{
	uint32_t *lAddr = (uint32_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	uint32_t rv = srcData;
	uint32_t swapped = ((rv >> 24U) & 0xFFU) | ((rv << 8U) & 0xFF0000U) | ((rv >> 8U) & 0xFF00U) | ((rv << 24U) & 0xFF000000U);
	*lAddr = swapped;
}

uint64_t load_qword(const volatile uint64_t *dstAddr, uintptr_t offset)
{
	/*! @brief Returned data for swap_qword function */
	static uint64_t rvq;

	uint64_t *lAddr = (uint64_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	rvq = *lAddr;
	swap_qword(&rvq, 0U, rvq);

	return rvq;
}

void swap_qword(volatile uint64_t *dstAddr, uintptr_t offset, volatile uint64_t srcData)
{
	uint64_t *lAddr = (uint64_t *)((uintptr_t)(dstAddr) + offset);
	(void)lAddr;
	uint64_t rv = srcData;
	rv = ((rv << 8U) & 0xFF00FF00FF00FF00ULL) | ((rv >> 8U) & 0x00FF00FF00FF00FFULL);
	rv = ((rv << 16U) & 0xFFFF0000FFFF0000ULL) | ((rv >> 16U) & 0x0000FFFF0000FFFFULL);
	*lAddr = (rv << 32U) | (rv >> 32U);
}
#endif

void swap_dwords(uint8_t *buff, uint32_t size)
{
	uint32_t ind;
	for (ind = 0U; ind < size; ind += 4U)
	{
		swap_dword((uint32_t *)buff, ind, (uint32_t)(*(uint32_t *)(&buff[ind])));
	}
}

/*******************************************************************************
 * Local functions
 ******************************************************************************/

static inline common_status_t SD_SelectCard(sd_card_t *card, bool isSelected)
{
	(void)card;
	return SDMMC_SelectCard(card->host.instance, card->host.transfer, card->relativeAddress, isSelected);
}

static inline common_status_t SD_WaitWriteComplete(sd_card_t *card)
{
	(void)card;
	return SDMMC_WaitWriteComplete(card->host.instance, card->host.transfer, card->relativeAddress);
}

static inline common_status_t SD_SendApplicationCmd(sd_card_t *card)
{
	(void)card;
	return SDMMC_SendApplicationCommand(card->host.instance, card->host.transfer, card->relativeAddress);
}

static void uSDHC_SwitchVoltage(sd_card_t *card, bool isLowVoltage)
{
	(void)card;
    uSDHC_Type *base = g_usdhcBases[card->host.instance];

    base->VEND_SPEC &= ~uSDHC_VEND_SPEC_FRC_SDCLK_ON_MASK;

    /* host switch to 1.8V */
    base->VEND_SPEC = (base->VEND_SPEC & ~uSDHC_VEND_SPEC_VSELECT_MASK) | uSDHC_VEND_SPEC_VSELECT((true == isLowVoltage) ? 1U : 0U);
    OSIF_TimeDelay(100U);
    /*uSDHC_EnableSDClock(base);*/
    base->VEND_SPEC |= uSDHC_VEND_SPEC_FRC_SDCLK_ON_MASK;
    OSIF_TimeDelay(10U);
    while (0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_SD_CLOCK_STABLE))
    {
    }

	while (0U != (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)(uSDHC_PRES_STATE_CIHB_MASK |
			uSDHC_PRES_STATE_CDIHB_MASK)))
	{
	}
    if (true == isLowVoltage)
    {
    	/* check data line and cmd line status */
    	while (0U != (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)(uSDHC_DATA0_LINE_LEVEL)))
    	{
    	}
    }
	return;
}

static inline common_status_t SD_SwitchVoltage(sd_card_t *card)
{
	(void)card;
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	uint32_t argument = 0U;
	if (0U != (card->flags & (uint32_t)aSD_SupportVoltage180v))
	{
		argument |= (uint32_t)aSD_OcrSwitch18AcceptFlag;
	}
	if (0U != argument)
	{
		status = SDMMC_SwitchVoltage(card->host.instance, card->host.transfer);
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			uSDHC_SwitchVoltage(card, true);
		}
	}
	return status;
}

static inline common_status_t SD_GoIdle(sd_card_t *card)
{
	(void)card;
	return SDMMC_GoIdle(card->host.instance, card->host.transfer);
}

static inline common_status_t SD_StopTransmission(sd_card_t *card)
{
	(void)card;
	return SDMMC_StopTransmission(card->host.instance, card->host.transfer);
}

static inline common_status_t SD_SetBlockSize(sd_card_t *card, uint32_t blockSize)
{
	(void)card;
	return SDMMC_SetBlockSize(card->host.instance, card->host.transfer, blockSize);
}

static inline common_status_t SD_SetBlockCount(sd_card_t *card, uint32_t blockCount)
{
	(void)card;
	return SDMMC_SetBlockCount(card->host.instance, card->host.transfer, blockCount);
}

static usdhc_transfer_storage_t transfer_storage;

void SD_Transfer_Init(usdhc_transfer_storage_t *l_transfer_storage)
{
	void *p = l_transfer_storage;
	(void)memset(p, 0U, sizeof(usdhc_transfer_storage_t));
}

static common_status_t SD_SendRca(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSD_SendRelativeAddress;
	transfer_storage.command.argument = 0U;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R6;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	if (STATUS_SUCCESS == card->host.transfer(card->host.instance, &transfer_storage.content))
	{
		card->relativeAddress = (transfer_storage.command.response[0U] >> 16U);
	}
	else
	{
		status = (common_status_t)aStatus_SDMMC_TransferFailed;
	}
	return status;
}

static common_status_t SD_SwitchFunction(sd_card_t *card, sd_switch_mode_t mode, sd_group_num group, uint32_t number, uint32_t *status)
{
	common_status_t state = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSD_Switch;
	transfer_storage.command.argument = (((uint32_t)(mode) << 31U) | 0x00FFFFFFU);
	transfer_storage.command.argument &= ~((uint32_t)(0xFU) << ((uint32_t)group * 4U));
	transfer_storage.command.argument |= (number << ((uint32_t)group * 4U));
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

	transfer_storage.data.blockSize = 64U;
	transfer_storage.data.blockCount = 1U;
	transfer_storage.data.rxData = status;

	if ((common_status_t)STATUS_SUCCESS != SD_SetBlockSize(card, transfer_storage.data.blockSize))
	{
		state = (common_status_t)aStatus_SDMMC_SetCardBlockSizeFailed;
	}
	else
	{
		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = &transfer_storage.data;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != (transfer_storage.command.response[0U] & (uint32_t)(aSDMMC_R1ErrorAllFlag))))
		{
			state = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	return state;
}

static void SD_DecodeScr(sd_card_t *card, uint32_t *rawScr)
{
	sd_scr_t *scr;
	(void)rawScr;

	scr = &(card->scr);
	scr->scrStructure = (uint8_t)((rawScr[0U] & 0xF0000000U) >> 28U);
	scr->sdSpecification = (uint8_t)((rawScr[0U] & 0xF000000U) >> 24U);
	if (((rawScr[0U] & 0x800000U) >> 23U) != 0U)
	{
		scr->flags |= (uint16_t)aSD_ScrDataStatusAfterErase;
	}
	scr->sdSecurity = (uint8_t)((rawScr[0U] & 0x700000U) >> 20U);
	scr->sdBusWidths = (uint8_t)((rawScr[0U] & 0xF0000U) >> 16U);
	if (((uint8_t)((rawScr[0U] & 0x8000U) >> 15U)) != 0U)
	{
		scr->flags |= (uint16_t)aSD_ScrSdSpecification3;
	}
	scr->extendedSecurity = (uint8_t)((rawScr[0U] & 0x7800U) >> 10U);
	scr->commandSupport = (uint8_t)(rawScr[0U] & 0x3U);
	scr->reservedForManufacturer = rawScr[1U];
	/* Get specification version. */
	switch (scr->sdSpecification)
	{
	case 0U:
		card->version = (uint32_t)aSD_SpecificationVersion1_0;
		break;
	case 1U:
		card->version = (uint32_t)aSD_SpecificationVersion1_1;
		break;
	case 2U:
		card->version = (uint32_t)aSD_SpecificationVersion2_0;
		if (0U != (card->scr.flags & (uint16_t)aSD_ScrSdSpecification3))
		{
			card->version = (uint32_t)aSD_SpecificationVersion3_0;
		}
		break;
	default:
		/* Others! */
		break;
	}
	if (0U != (card->scr.sdBusWidths & (uint8_t)aSD_DataBusWidth4Bit))
	{
		card->flags |= (uint32_t)aSD_Support4BitWidthFlag;
	}
	if (0U != (card->scr.commandSupport & (uint8_t)aSD_SupportSpeedClassControlCmd))
	{
		card->flags |= (uint32_t)aSD_SupportSpeedClassControlCmd;
	}
	/* SD specification v.3.00 states, CMD23 should be disabled for ADMA1 */
	if ((0U != (card->scr.commandSupport & (uint8_t)aSD_SupportSetBlockCountCmd)) &&
			(uSDHC_DMA_MODE_ADMA1 != card->host.config.dmaMode))
	{
		card->flags |= (uint32_t)aSD_SupportSetBlockCountCmd;
	}
}

static common_status_t SD_SendScr(sd_card_t *card)
{
	/*
	 * uSDHC Host Controller Capabilities register contains MBL (Maximum Block Length) bit set to 011b which means 4096 bytes.
	 * This value indicates the maximum block size that the Host Driver can read and write to the buffer in the uSDHC.
	 * The buffer shall transfer block size without wait cycles.
	 */
	static ALIGNED(4096) uint32_t rawScr[2U] = {0U};
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	SD_Transfer_Init(&transfer_storage);

	if ((common_status_t)STATUS_SUCCESS != SD_SendApplicationCmd(card))
	{
		status = (common_status_t)aStatus_SDMMC_SendApplicationCommandFailed;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		transfer_storage.command.index = (uint32_t)aSD_ApplicationSendScr;
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;
		transfer_storage.command.argument = 0U;

		transfer_storage.data.blockSize = 8U;
		transfer_storage.data.blockCount = 1U;
		transfer_storage.data.rxData = rawScr;

		transfer_storage.content.data = &transfer_storage.data;
		transfer_storage.content.command = &transfer_storage.command;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* SCR register data byte sequence from card is big endian(MSB first). */
		switch (card->host.config.endianMode)
		{
		case uSDHC_ENDIAN_MODE_LITTLE:
			/* In little endian mode, SD bus byte transferred first is the byte stored in lowest byte position in a
            word which will cause 4 byte's sequence in a word is not consistent with their original sequence from
            card. So the sequence of 4 bytes received in a word should be converted. */
			rawScr[0U] = load_dword(rawScr, 0U);
			rawScr[1U] = load_dword(rawScr, 1U * sizeof(uint32_t));
			break;
		case uSDHC_ENDIAN_MODE_BIG:
			break; /* Doesn't need to switch byte sequence when decodes bytes as big endian sequence. */
		case uSDHC_ENDIAN_MODE_HALF_WORD_BIG:
			rawScr[0U] = load_hword((uint8_t *)rawScr, 0U);
			rawScr[1U] = load_hword((uint8_t *)rawScr, 1U * sizeof(uint32_t));
			break;
		default:
			status = (common_status_t)aStatus_SDMMC_NotSupportYet;
			break;
		}
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			(void)memcpy(card->rawScr, rawScr, sizeof(card->rawScr));
			SD_DecodeScr(card, rawScr);
		}
	}

	return status;
}

static common_status_t SD_CheckAndSwitchFunction(sd_card_t *card, sd_group_num group, uint32_t function)
{
	static uint32_t functionStatus[16U] = {0U};
	static uint16_t functionGroupInfo[6U] = {0};
	uint32_t currentFunctionStatus = 0U;
	common_status_t status = (int32_t)STATUS_SUCCESS;
	if ((card->version < (uint32_t)aSD_SpecificationVersion1_0) || (0U == (card->csd.cardCommandClass & (uint16_t)aSDMMC_CommandClassSwitch)))
	{
		status = (common_status_t)aStatus_SDMMC_CardNotSupport;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Check if card support high speed mode. */
		if ((common_status_t)STATUS_SUCCESS != SD_SwitchFunction(card, aSD_SwitchCheck, group, function, functionStatus))
		{
			status = (common_status_t)aStatus_SDMMC_SwitchFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Switch function status byte sequence from card is big endian(MSB first). */
		switch (card->host.config.endianMode)
		{
		case uSDHC_ENDIAN_MODE_LITTLE:
			/* In little endian mode, SD bus byte transferred first is the byte stored in lowest byte position in
			a word which will cause 4 byte's sequence in a word is not consistent with their original sequence from
			card. So the sequence of 4 bytes received in a word should be converted. */
			functionStatus[0U] = load_dword(functionStatus, 0U);
			functionStatus[1U] = load_dword(functionStatus, 1U * sizeof(uint32_t));
			functionStatus[2U] = load_dword(functionStatus, 2U * sizeof(uint32_t));
			functionStatus[3U] = load_dword(functionStatus, 3U * sizeof(uint32_t));
			functionStatus[4U] = load_dword(functionStatus, 4U * sizeof(uint32_t));
			break;
		case uSDHC_ENDIAN_MODE_BIG:
			break;
		case uSDHC_ENDIAN_MODE_HALF_WORD_BIG:
			functionStatus[0U] = load_hword((uint8_t *)functionStatus, 0U);
			functionStatus[1U] = load_hword((uint8_t *)functionStatus, 1U * sizeof(uint32_t));
			functionStatus[2U] = load_hword((uint8_t *)functionStatus, 2U * sizeof(uint32_t));
			functionStatus[3U] = load_hword((uint8_t *)functionStatus, 3U * sizeof(uint32_t));
			functionStatus[4U] = load_hword((uint8_t *)functionStatus, 4U * sizeof(uint32_t));
			break;
		default:
			status = (common_status_t)aStatus_SDMMC_NotSupportYet;
			break;
		}
	}

	functionGroupInfo[5U] = (uint16_t)functionStatus[0U];
	functionGroupInfo[4U] = (uint16_t)(functionStatus[1U] >> 16U);
	functionGroupInfo[3U] = (uint16_t)(functionStatus[1U]);
	functionGroupInfo[2U] = (uint16_t)(functionStatus[2U] >> 16U);
	functionGroupInfo[1U] = (uint16_t)(functionStatus[2U]);
	functionGroupInfo[0U] = (uint16_t)(functionStatus[3U] >> 16U);
	currentFunctionStatus = ((functionStatus[3U] & 0xFFU) << 8U) | (functionStatus[4U] >> 24U);
	/* check if function is supported. */
	if ((((uint32_t)(functionGroupInfo[group]) & ((uint32_t)1U << function)) == 0U) ||
			(((currentFunctionStatus >> ((uint32_t)group * 4U)) & 0x0FU) != function))
	{
		status = (common_status_t)aStatus_SDMMC_CardNotSupport;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Switch to high speed mode. */
		if ((common_status_t)STATUS_SUCCESS != SD_SwitchFunction(card, aSD_SwitchSet, group, function, functionStatus))
		{
			status = (common_status_t)aStatus_SDMMC_SwitchFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Switch function status byte sequence from card is big endian(MSB first). */
		switch (card->host.config.endianMode)
		{
		case uSDHC_ENDIAN_MODE_LITTLE:
			/* In little endian mode is little endian, SD bus byte transferred first is the byte stored in lowest byte
			position in a word which will cause 4 byte's sequence in a word is not consistent with their original
			sequence from card. So the sequence of 4 bytes received in a word should be converted.*/
			functionStatus[3U] = load_dword(functionStatus, 3U * sizeof(uint32_t));
			functionStatus[4U] = load_dword(functionStatus, 4U * sizeof(uint32_t));
			break;
		case uSDHC_ENDIAN_MODE_BIG:
			break; /* Doesn't need to switch byte sequence when decodes bytes as big endian sequence. */
		case uSDHC_ENDIAN_MODE_HALF_WORD_BIG:
			functionStatus[3U] = load_hword((uint8_t *)functionStatus, 3U * sizeof(uint32_t));
			functionStatus[4U] = load_hword((uint8_t *)functionStatus, 4U * sizeof(uint32_t));
			break;
		default:
			status = (common_status_t)aStatus_SDMMC_NotSupportYet;
			break;
		}
	}
	if (status == (common_status_t)STATUS_SUCCESS)
	{
		/* If switch function group failed, function group will be returned. */
		currentFunctionStatus = ((functionStatus[3U] & 0xFFU) << 8U) | (functionStatus[4U] >> 24U);
		if (((currentFunctionStatus >> ((uint32_t)group * 4U)) & 0x0FU) != function)
		{
			status = (common_status_t)aStatus_SDMMC_SwitchFailed;
		}
	}

	return status;
}

static common_status_t SD_SetDataBusWidth(sd_card_t *card, sd_data_bus_width_t width)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	SD_Transfer_Init(&transfer_storage);

	if ((common_status_t)STATUS_SUCCESS != SD_SendApplicationCmd(card))
	{
		status = (common_status_t)aStatus_SDMMC_SendApplicationCommandFailed;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		transfer_storage.command.index = (uint32_t)aSD_ApplicationSetBusWidth;
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;
		switch ((uint32_t)width)
		{
		case (int32_t)aSD_DataBusWidth1Bit:
				transfer_storage.command.argument = 0U;
		break;
		case (int32_t)aSD_DataBusWidth4Bit:
				transfer_storage.command.argument = 2U;
		break;
		default:
			status = (common_status_t)STATUS_ERROR;
			break;
		}
	}
	if (status == (common_status_t)STATUS_SUCCESS)
	{
		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = NULL;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}

	return status;
}

static void SD_DecodeCsd(sd_card_t *card, uint32_t *rawCsd)
{
	sd_csd_t *csd;
	(void)rawCsd;

	csd = &(card->csd);
	csd->csdStructure = (uint8_t)((rawCsd[3U] & 0xC0000000U) >> 30U);
	csd->dataReadAccessTime1 = (uint8_t)((rawCsd[3U] & 0xFF0000U) >> 16U);
	csd->dataReadAccessTime2 = (uint8_t)((rawCsd[3U] & 0xFF00U) >> 8U);
	csd->transferSpeed = (uint8_t)(rawCsd[3U] & 0xFFU);
	csd->cardCommandClass = (uint16_t)((rawCsd[2U] & 0xFFF00000U) >> 20U);
	csd->readBlockLength = (uint8_t)((rawCsd[2U] & 0xF0000U) >> 16U);
	if (0U != (rawCsd[2U] & 0x8000U))
	{
		csd->flags |= (uint16_t)aSD_CsdReadBlockPartialFlag;
	}
	if (0U != (rawCsd[2U] & 0x4000U))
	{
		csd->flags |= (uint16_t)aSD_CsdReadBlockPartialFlag;
	}
	if (0U != (rawCsd[2U] & 0x2000U))
	{
		csd->flags |= (uint16_t)aSD_CsdReadBlockMisalignFlag;
	}
	if (0U != (rawCsd[2U] & 0x1000U))
	{
		csd->flags |= (uint16_t)aSD_CsdDsrImplementedFlag;
	}
	switch (csd->csdStructure)
	{
	case 0:
		csd->deviceSize = (uint32_t)((rawCsd[2U] & 0x3FFU) << 2U);
		csd->deviceSize |= (uint32_t)((rawCsd[1U] & 0xC0000000U) >> 30U);
		csd->readCurrentVddMin = (uint8_t)((rawCsd[1U] & 0x38000000U) >> 27U);
		csd->readCurrentVddMax = (uint8_t)((rawCsd[1U] & 0x7000000U) >> 24U);
		csd->writeCurrentVddMin = (uint8_t)((rawCsd[1U] & 0xE00000U) >> 20U);
		csd->writeCurrentVddMax = (uint8_t)((rawCsd[1U] & 0x1C0000U) >> 18U);
		csd->deviceSizeMultiplier = (uint8_t)((rawCsd[1U] & 0x38000U) >> 15U);

		/* Get card total block count and block size. */
		card->blockCount = ((csd->deviceSize + 1U) << (csd->deviceSizeMultiplier + 2U));
		card->blockSize = ((uint32_t)1U << csd->readBlockLength);
		if (FSL_SDMMC_DEFAULT_BLOCK_SIZE != card->blockSize)
		{
			card->blockCount = (card->blockCount * card->blockSize);
			card->blockSize = FSL_SDMMC_DEFAULT_BLOCK_SIZE;
			card->blockCount = (card->blockCount / card->blockSize);
		}
		break;
	case 1:
		card->blockSize = FSL_SDMMC_DEFAULT_BLOCK_SIZE;

		csd->deviceSize = (uint32_t)((rawCsd[2U] & 0x3FU) << 16U);
		csd->deviceSize |= (uint32_t)((rawCsd[1U] & 0xFFFF0000U) >> 16U);
		if (csd->deviceSize >= 0xFFFFU)
		{
			card->flags |= (uint16_t)aSD_SupportSdxcFlag;
		}

		card->blockCount = ((csd->deviceSize + 1U) * 1024U);
		break;
	default:
		/*others */
		break;
	}
	if (0U != ((uint8_t)((rawCsd[1U] & 0x4000U) >> 14U)))
	{
		csd->flags |= (uint16_t)aSD_CsdEraseBlockEnabledFlag;
	}
	csd->eraseSectorSize = (uint8_t)((rawCsd[1U] & 0x3F80U) >> 7U);
	csd->writeProtectGroupSize = (uint8_t)(rawCsd[1U] & 0x7FU);
	if (0U != ((uint8_t)(rawCsd[0U] & 0x80000000U)))
	{
		csd->flags |= (uint16_t)aSD_CsdWriteProtectGroupEnabledFlag;
	}
	csd->writeSpeedFactor = (uint8_t)((rawCsd[0U] & 0x1C000000U) >> 26U);
	csd->writeBlockLength = (uint8_t)((rawCsd[0U] & 0x3C00000U) >> 22U);
	if (0U != ((uint8_t)((rawCsd[0U] & 0x200000U) >> 21U)))
	{
		csd->flags |= (uint16_t)aSD_CsdWriteBlockPartialFlag;
	}
	if (0U != ((uint8_t)((rawCsd[0U] & 0x8000U) >> 15U)))
	{
		csd->flags |= (uint16_t)aSD_CsdFileFormatGroupFlag;
	}
	if (0U != ((uint8_t)((rawCsd[0U] & 0x4000U) >> 14U)))
	{
		csd->flags |= (uint16_t)aSD_CsdCopyFlag;
	}
	if (0U != ((uint8_t)((rawCsd[0U] & 0x2000U) >> 13U)))
	{
		csd->flags |= (uint16_t)aSD_CsdPermanentWriteProtectFlag;
	}
	if (0U != ((uint8_t)((rawCsd[0U] & 0x1000U) >> 12U)))
	{
		csd->flags |= (uint16_t)aSD_CsdTemporaryWriteProtectFlag;
	}
	csd->fileFormat = (uint8_t)((rawCsd[0U] & 0xC00U) >> 10U);
}

static common_status_t SD_SendCsd(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSDMMC_SendCsd;
	transfer_storage.command.argument = (card->relativeAddress << 16U);
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R2;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	status = (common_status_t)card->host.transfer(card->host.instance, &transfer_storage.content);
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		(void)memcpy(card->rawCsd, transfer_storage.command.response, sizeof(card->rawCsd));
		/* The response is from bit 127:8 in R2, corresponding to transfer_storage.command.response[3U]:transfer_storage.command.response[0U][31U:8]. */
		SD_DecodeCsd(card, transfer_storage.command.response);
	}
	else
	{
		status = (common_status_t)aStatus_SDMMC_TransferFailed;
	}
	return status;
}

static void SD_DecodeCid(sd_card_t *card, uint32_t *rawCid)
{
	sd_cid_t *cid;
	(void)rawCid;

	cid = &(card->cid);
	cid->manufacturerID = (uint8_t)((rawCid[3U] & 0xFF000000U) >> 24U);
	cid->applicationID = (uint16_t)((rawCid[3U] & 0xFFFF00U) >> 8U);

	cid->productName[0U] = (uint8_t)((rawCid[3U] & 0xFFU));
	cid->productName[1U] = (uint8_t)((rawCid[2U] & 0xFF000000U) >> 24U);
	cid->productName[2U] = (uint8_t)((rawCid[2U] & 0xFF0000U) >> 16U);
	cid->productName[3U] = (uint8_t)((rawCid[2U] & 0xFF00U) >> 8U);
	cid->productName[4U] = (uint8_t)((rawCid[2U] & 0xFFU));

	cid->productVersion = (uint8_t)((rawCid[1U] & 0xFF000000U) >> 24U);

	cid->productSerialNumber = (uint32_t)((rawCid[1U] & 0xFFFFFFU) << 8U);
	cid->productSerialNumber |= (uint32_t)((rawCid[0U] & 0xFF000000U) >> 24U);

	cid->manufacturerData = (uint16_t)((rawCid[0U] & 0xFFF00U) >> 8U);
}

static common_status_t SD_AllSendCid(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSDMMC_AllSendCid;
	transfer_storage.command.argument = 0U;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R2;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	status = (common_status_t)card->host.transfer(card->host.instance, &transfer_storage.content);
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		(void)memcpy(card->rawCid, transfer_storage.command.response, sizeof(card->rawCid));
		SD_DecodeCid(card, transfer_storage.command.response);
	}
	else
	{
		status = (common_status_t)aStatus_SDMMC_TransferFailed;
	}
	return status;
}

static common_status_t SD_ApplicationSendOperationCondition(sd_card_t *card, uint32_t argument)
{
	SD_Transfer_Init(&transfer_storage);

	common_status_t status = (common_status_t)STATUS_SUCCESS;
	uint32_t i = FSL_SDMMC_MAX_VOLTAGE_RETRIES;
	uint8_t voltageProbes = 3U;
	uint32_t capabilities = 0U;

	transfer_storage.command.index = (uint32_t)aSD_ApplicationSendOperationCondition;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R3;

	while ((i--) != 0U)
	{
		status = SD_SendApplicationCmd(card);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SendApplicationCommandFailed;
		}
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			if ((((FSL_SDMMC_MAX_VOLTAGE_RETRIES - 1U) == i) ||
					(1U == i)) && (0U < voltageProbes))
			{
				voltageProbes--;
				capabilities = 0U;
				if (2U == voltageProbes)
				{
					if (0U != (argument & (uint32_t)aSD_OcrSwitch18RequestFlag))
					{
						capabilities |= (uint32_t)aSD_OcrSwitch18RequestFlag;
					}
				}
				else
				{
					i = FSL_SDMMC_MAX_VOLTAGE_RETRIES - 1U;
				}
				/*
				 * Vdd window bits[08:23] should be completed otherwise
				 * the ACMD41 command is considered as "inquiry" and
				 * can't be used for initialization purpose.
				 */
				if (0U != (argument & (uint32_t)aSD_OcrVdd32_33Flag))
				{
					capabilities |= (uint32_t)aSD_OcrVdd32_33Flag;
				}
				if (0U != (argument & (uint32_t)aSD_OcrVdd33_34Flag))
				{
					capabilities |= (uint32_t)aSD_OcrVdd33_34Flag;
				}
				if (0U != (argument & (uint32_t)aSD_OcrHostCapacitySupportFlag))
				{
					capabilities |= (uint32_t)aSD_OcrHostCapacitySupportFlag;
				}
				transfer_storage.command.argument = capabilities;
			}
			transfer_storage.content.command = &transfer_storage.command;
			transfer_storage.content.data = NULL;
			status = (common_status_t)card->host.transfer(card->host.instance, &transfer_storage.content);
			if ((common_status_t)STATUS_SUCCESS != status)
			{
				status = (common_status_t)aStatus_SDMMC_TransferFailed;
			}
		}
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			/* Wait until card exit busy state. */
			if (0U != (transfer_storage.command.response[0U] & (uint32_t)aSD_OcrPowerUpBusyFlag))
			{
				if ((transfer_storage.command.response[0U] & (uint32_t)aSD_OcrCardCapacitySupportFlag) != 0U)
				{
					card->flags |= (uint32_t)aSD_SupportHighCapacityFlag;
				}
				/* 1.8V support */
				if (0U != (transfer_storage.command.response[0U] & (uint32_t)aSD_OcrSwitch18AcceptFlag))
				{
					card->flags |= (uint32_t)aSD_SupportVoltage180v;
				}
				status = (common_status_t)STATUS_SUCCESS;
				card->ocr = transfer_storage.command.response[0U];
			}
			else
			{
				status = (common_status_t)STATUS_TIMEOUT;
			}
		}
		if ((common_status_t)STATUS_TIMEOUT != status)
		{
			break;
		}
	}

	return status;
}

static uint32_t SD_FindBusClock(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	uint32_t frequencyTable[] = {SD_CLOCK_25MHZ, 16000000U, 10000000U, 8000000U, 4000000, 2000000U};
	uint32_t nextFrequency;
	uint32_t prevFrequency = 0U;
	uint32_t i = 5;

	do
	{
		nextFrequency = frequencyTable[i];
		(void)uSDHC_DRV_SetBusClock(card->host.instance, nextFrequency);
		status = SD_SetBlockSize(card, 64U);
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			prevFrequency = nextFrequency;
		}
		else
		{
			break;
		}
		i--;
	}
	while (i != 0U);

	(void)uSDHC_DRV_SetBusClock(card->host.instance, prevFrequency);
	return prevFrequency;
}

static common_status_t SD_SendInterfaceCondition(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	(void)card;

	SD_Transfer_Init(&transfer_storage);
	transfer_storage.command.index = (uint32_t)aSD_SendInterfaceCondition;
	transfer_storage.command.argument = 0x1AAU;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R7;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	if (STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content))
	{
		status =  (common_status_t)aStatus_SDMMC_TransferFailed;
	}
	else
	{
		/*
		 * CMD8 (Send Interface Condition) argument contains host supply voltage and check pattern (any 8-bit value).
		 * If the card accept supply voltage, and it responds and echoes supply voltage and check pattern back.
		 * Check pattern value 0xAA is recommended by SD Physical Layer Simplified Specification Version 2.00 p. 40.
		 */
		if (0xAAU != (transfer_storage.command.response[0U] & 0xFFU))
		{
			status = (common_status_t)aStatus_SDMMC_CardNotSupport;
		}
	}

	return status;
}

static common_status_t SD_HandleStopTransmission(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	/* Send STOP_TRANSMISSION command in multiple block transmission and host's AUTO_COMMAND12 isn't enabled. */
	if ((transfer_storage.data.blockCount > 1U) &&
			(false == transfer_storage.data.enableAutoCMD12) &&
			(0U == (card->flags & (uint32_t)aSD_SupportSetBlockCountCmd)))
	{
		if ((common_status_t)STATUS_SUCCESS != SD_StopTransmission(card))
		{
			status = (common_status_t)aStatus_SDMMC_StopTransmissionFailed;
		}
	}

	return status;
}

static common_status_t SD_HandleSetBlockCountSize(sd_card_t *card, uint32_t blockCount, uint32_t blockSize)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	if (((0U != (card->flags & (uint32_t)aSD_SupportHighCapacityFlag)) && (blockSize != 512U)) || (blockSize > card->blockSize) ||
			(blockSize > card->host.capability.maxBlockLength) || (0U != (blockSize % 4U)))
	{
		status = (common_status_t)aStatus_SDMMC_CardNotSupport;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		if ((1U < blockCount) && (false == transfer_storage.data.enableAutoCMD12) &&
				(0U != (card->flags & (uint32_t)aSD_SupportSetBlockCountCmd)))
		{
			if ((common_status_t)STATUS_SUCCESS != SD_SetBlockCount(card, blockCount))
			{
				status = (common_status_t)aStatus_SDMMC_SetBlockCountFailed;
			}
		}
	}

	return status;
}

static common_status_t SD_Read(sd_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	/* Wait for the card write process complete because of that card read process and write process use one buffer. */
	if ((common_status_t)STATUS_SUCCESS != SD_WaitWriteComplete(card))
	{
		status = (common_status_t)aStatus_SDMMC_WaitWriteCompleteFailed;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_HandleSetBlockCountSize(card, blockCount, blockSize);
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		transfer_storage.data.blockSize = blockSize;
		transfer_storage.data.blockCount = blockCount;
		transfer_storage.data.rxData = (uint32_t *)buffer;

		transfer_storage.command.index = (uint32_t)aSDMMC_ReadMultipleBlock;
		if (1U == transfer_storage.data.blockCount)
		{
			transfer_storage.command.index = (uint32_t)aSDMMC_ReadSingleBlock;
		}
		transfer_storage.command.argument = startBlock;
		if (0U == (card->flags & (uint32_t)aSD_SupportHighCapacityFlag))
		{
			transfer_storage.command.argument *= transfer_storage.data.blockSize;
		}
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = &transfer_storage.data;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_HandleStopTransmission(card);
	}

	return status;
}

static common_status_t SD_Write(
		sd_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockSize, uint32_t blockCount)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	SD_Transfer_Init(&transfer_storage);

	if (false != SD_CheckReadOnly(card))
	{
		status = (common_status_t)aStatus_SDMMC_CardWriteProtected;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Wait for the card's buffer to be not full. */
		while (0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_DATA0_LINE_LEVEL))
		{
		}
		if ((common_status_t)STATUS_SUCCESS != SD_WaitWriteComplete(card))
		{
			status = (common_status_t)aStatus_SDMMC_WaitWriteCompleteFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_HandleSetBlockCountSize(card, blockCount, blockSize);
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		transfer_storage.data.blockSize = blockSize;
		transfer_storage.data.blockCount = blockCount;
		transfer_storage.data.txData = (uint32_t *)buffer;

		transfer_storage.command.index = (uint32_t)aSDMMC_WriteMultipleBlock;
		if (1U == transfer_storage.data.blockCount)
		{
			transfer_storage.command.index = (uint32_t)aSDMMC_WriteSingleBlock;
		}
		transfer_storage.command.argument = startBlock;
		if (0U == (card->flags & (uint32_t)aSD_SupportHighCapacityFlag))
		{
			transfer_storage.command.argument *= transfer_storage.data.blockSize;
		}
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = &transfer_storage.data;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_HandleStopTransmission(card);
	}

	return status;
}

static common_status_t SD_Erase(sd_card_t *card, uint32_t startBlock, uint32_t blockCount)
{
	uint32_t eraseBlockStart;
	uint32_t eraseBlockEnd;
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	SD_Transfer_Init(&transfer_storage);
	eraseBlockStart = startBlock;
	eraseBlockEnd = eraseBlockStart + blockCount;

	if (false != SD_CheckReadOnly(card))
	{
		status = (common_status_t)aStatus_SDMMC_CardWriteProtected;
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Wait for the card's buffer to be not full to write to improve the write performance. */
		while (0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_DATA0_LINE_LEVEL))
		{
		}
		/* Wait for the card write process complete because of that card read process and write process use one buffer. */
		if ((common_status_t)STATUS_SUCCESS != SD_WaitWriteComplete(card))
		{
			status = (common_status_t)aStatus_SDMMC_WaitWriteCompleteFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		if (0U == (card->flags & (uint32_t)aSD_SupportHighCapacityFlag))
		{
			eraseBlockStart = eraseBlockStart * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
			eraseBlockEnd = eraseBlockEnd * FSL_SDMMC_DEFAULT_BLOCK_SIZE;
		}

		/* Send ERASE_WRITE_BLOCK_START command to set the start block number to erase. */
		transfer_storage.command.index = (uint32_t)aSD_EraseWriteBlockStart;
		transfer_storage.command.argument = eraseBlockStart;
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = NULL;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Send ERASE_WRITE_BLOCK_END command to set the end block number to erase. */
		transfer_storage.command.index = (uint32_t)aSD_EraseWriteBlockEnd;
		transfer_storage.command.argument = eraseBlockEnd;

		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = NULL;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Send ERASE command to start erase process. */
		transfer_storage.command.index = (uint32_t)aSDMMC_Erase;
		transfer_storage.command.argument = 0U;
		transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1b;

		transfer_storage.content.command = &transfer_storage.command;
		transfer_storage.content.data = NULL;
		if ((STATUS_SUCCESS != card->host.transfer(card->host.instance, &transfer_storage.content)) ||
				(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
		{
			status = (common_status_t)aStatus_SDMMC_TransferFailed;
		}
	}
	/* Low-class cards need a 1 ms granularity delay between consecutive erase operations. */
	OSIF_TimeDelay(2U);

	return status;
}

static common_status_t SD_SwitchHighSpeed(sd_card_t *card)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	common_status_t error = (common_status_t)STATUS_SUCCESS;

	/* Set to max frequency in non-high speed mode. */
	card->busClock_Hz = SD_FindBusClock(card);

	/* Set to 4-bit data bus mode. */
	if ((0U != ((card->host.capability.flags) & (uint32_t)uSDHC_SUPPORT_4BIT)) &&
			(0U != (card->flags & (uint32_t)aSD_Support4BitWidthFlag)))
	{
		status = SD_SetDataBusWidth(card, aSD_DataBusWidth4Bit);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SetDataBusWidthFailed;
		}
		else
		{
			uSDHC_DRV_SetDataBusWidth(card->host.instance, uSDHC_DATA_BUS_WIDTH_4BIT);
		}
	}

	if (0U != (card->flags & (uint32_t)aSD_SupportVoltage180v))
	{
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			error = SD_CheckAndSwitchFunction(card, aSD_GroupDriverStrength, (uint32_t)aSD_DriverStrengthTypeB);
			if (((common_status_t)STATUS_SUCCESS != error) &&
					((common_status_t)aStatus_SDMMC_CardNotSupport != error))
			{
				status = (common_status_t)aStatus_SDMMC_SwitchFailed;
			}
		}

		if ((common_status_t)STATUS_SUCCESS == status)
		{
			error = SD_CheckAndSwitchFunction(card, aSD_GroupCurrentLimit, (uint32_t)aSD_CurrentLimit200MA);
			if (((common_status_t)STATUS_SUCCESS != error) &&
					((common_status_t)aStatus_SDMMC_CardNotSupport != error))
			{
				status = (common_status_t)aStatus_SDMMC_SwitchFailed;
			}
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Switch the card to high speed mode */
		if (0U != (card->host.capability.flags & (uint32_t)uSDHC_SUPPORT_HIGH_SPEED))
		{
			error = SD_CheckAndSwitchFunction(card, aSD_GroupTimingMode, (uint32_t)aSD_FunctionSDR25HighSpeed);
			if (((common_status_t)STATUS_SUCCESS != error) &&
					((common_status_t)aStatus_SDMMC_CardNotSupport != error))
			{
				status = (common_status_t)aStatus_SDMMC_SwitchHighSpeedFailed;
			}
			else if ((common_status_t)STATUS_SUCCESS == error)
			{
				card->busClock_Hz = uSDHC_DRV_SetBusClock(card->host.instance, SD_CLOCK_50MHZ);
				status = (common_status_t)STATUS_SUCCESS;
			}
			else /* Not support high speed 50MHZ is also OK. */
			{
				status = (common_status_t)STATUS_SUCCESS;
			}
		}
	}
	return status;
}

static common_status_t SD_SwitchToDataTransfer(sd_card_t *card)
{
	common_status_t status;

	/* Initialize card if the card is SD card. */
	status = SD_AllSendCid(card);
	if ((common_status_t)STATUS_SUCCESS != status)
	{
		status = (common_status_t)aStatus_SDMMC_AllSendCidFailed;
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SendRca(card);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SendRelativeAddressFailed;
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SendCsd(card);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SendCsdFailed;
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SelectCard(card, true);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SelectCardFailed;
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SendScr(card);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SendScrFailed;
		}
	}
    return status;
}

/*
 * Implements SD_Init_Activity
 */
common_status_t SD_Init(sd_card_t *card)
{
	DEV_ASSERT(card != NULL);
	uint32_t applicationCommand41Argument = 0U;
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	common_status_t error = (common_status_t)STATUS_SUCCESS;

	/* Haven't supported this kind of card detect. */
	if (false != card->host.config.cardDetectDat3)
	{
		status = (common_status_t)aStatus_SDMMC_NotSupportYet;
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		uSDHC_DRV_SetDataBusWidth(card->host.instance, uSDHC_DATA_BUS_WIDTH_1BIT);
		/* Identify card type. */
		card->busClock_Hz = uSDHC_DRV_SetBusClock(card->host.instance, SDMMC_CLOCK_400KHZ);
		(void)uSDHC_SwitchVoltage(card, false);
		(void)uSDHC_DRV_SetCardActive(card->host.instance, 100U);

		if ((common_status_t)STATUS_SUCCESS != SD_GoIdle(card))
		{
			status = (common_status_t)aStatus_SDMMC_GoIdleFailed;
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
#if (defined(WRITE_PROTECT) && (WRITE_PROTECT > 0U))
		/* Implement write protection detect via WP pin. */
		if (0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_WRITE_PROTECTED))
		{
			card->flags |= (uint32_t)aSD_WriteProtectFlag;
		}
#endif
		/* Hand-shaking with card to validate the interface voltage range. */
		/* Get host capability. */
		uSDHC_DRV_GetCapability(card->host.instance, &(card->host.capability));
		if (0U != (card->host.capability.flags & (uint32_t)uSDHC_SUPPORT_3V3))
		{
			applicationCommand41Argument |= ((uint32_t)aSD_OcrVdd32_33Flag | (uint32_t)aSD_OcrVdd33_34Flag);
			//applicationCommand41Argument |= ((uint32_t)aSD_OcrVdd31_32Flag |(uint32_t)aSD_OcrVdd32_33Flag | (uint32_t)aSD_OcrVdd33_34Flag | aSD_OcrVdd34_35Flag);
		}
#if defined FEATURE_SDHC_HAS_V300_SUPPORT
		if (0U != ((card->host.capability.flags) & (uint32_t)kSDHC_SupportV300Flag))
		{
			applicationCommand41Argument |= (uint32_t)aSD_OcrVdd29_30Flag;
		}
#endif

		/* Check card's supported interface condition. */
		status = SD_SendInterfaceCondition(card);
		if ((common_status_t)STATUS_SUCCESS == status)
		{
			/* SDHC or SDXC card */
			applicationCommand41Argument |= (uint32_t)aSD_OcrHostCapacitySupportFlag;
			applicationCommand41Argument |= (uint32_t)aSD_OcrSwitchSDXCPowerControlFlag;
			card->flags |= (uint16_t)aSD_SupportSdhcFlag;
#if defined FEATURE_SDHC_HAS_V180_SUPPORT
			if (0U != (card->host.capability.flags & (uint32_t)uSDHC_SUPPORT_1V8))
			{
				applicationCommand41Argument |= (uint32_t)aSD_OcrSwitch18RequestFlag;
			}
#endif
		}
		else
		{
			/* SDSC card */
			status = SD_GoIdle(card);
			if ((common_status_t)STATUS_SUCCESS != status)
			{
				status = (common_status_t)aStatus_SDMMC_GoIdleFailed;
			}
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		/* Set card interface condition according to SDHC capability and card's supported interface condition. */
		error = SD_ApplicationSendOperationCondition(card, applicationCommand41Argument);
		if ((common_status_t)STATUS_TIMEOUT == error)
		{
			/* MMC card */
			status = (common_status_t)aStatus_SDMMC_NotSupportYet;
		}
		else
		{
			if ((common_status_t)STATUS_SUCCESS != error)
			{
				status = (common_status_t)aStatus_SDMMC_HandShakeOperationConditionFailed;
			}
		}
	}

	if ((0U != (card->flags & (uint16_t)aSD_SupportSdhcFlag)) &&
			((common_status_t)STATUS_SUCCESS == status))
	{
		status = SD_SwitchVoltage(card);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SwitchVoltageFailed;
		}
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SwitchToDataTransfer(card);
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SwitchHighSpeed(card);
	}

	if ((common_status_t)STATUS_SUCCESS == status)
	{
		status = SD_SetBlockSize(card, FSL_SDMMC_DEFAULT_BLOCK_SIZE);
		if ((common_status_t)STATUS_SUCCESS != status)
		{
			status = (common_status_t)aStatus_SDMMC_SetCardBlockSizeFailed;
		}
	}

	return status;
}

/*
 * Implements SD_DeInit_Activity
 */
common_status_t SD_DeInit(sd_card_t *card)
{
	DEV_ASSERT(card != NULL);
	return SD_SelectCard(card, false);
}

/*
 * Implements SD_CheckReadOnly_Activity
 */
bool SD_CheckReadOnly(sd_card_t *card)
{
	(void)card;
	DEV_ASSERT(card != NULL);
	return ((0U != (card->flags & (uint32_t)aSD_WriteProtectFlag)) ||
			(0U != (card->csd.flags & (uint16_t)aSD_CsdPermanentWriteProtectFlag)) ||
			(0U != (card->csd.flags & (uint16_t)aSD_CsdTemporaryWriteProtectFlag)));
}

/*
 * Implements SD_ReadBlocks_Activity
 */
common_status_t SD_ReadBlocks(sd_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
	uint32_t blockCountOneTime;
	uint32_t blockLeft;
	uint32_t blockDone;
	common_status_t error = (common_status_t)STATUS_SUCCESS;

	DEV_ASSERT(card != NULL);
	if ((blockCount + startBlock) > card->blockCount)
	{
		error = (common_status_t)STATUS_ERROR;
	}
	else
	{
		blockLeft = blockCount;
		blockDone = 0U;
		while (blockLeft != 0U)
		{
			if (blockLeft > card->host.capability.maxBlockCount)
			{
				blockLeft = (blockLeft - card->host.capability.maxBlockCount);
				blockCountOneTime = card->host.capability.maxBlockCount;
			}
			else
			{
				blockCountOneTime = blockLeft;
				blockLeft = 0U;
			}

			error = SD_Read(card, &buffer[blockDone * FSL_SDMMC_DEFAULT_BLOCK_SIZE], (startBlock + blockDone), FSL_SDMMC_DEFAULT_BLOCK_SIZE, blockCountOneTime);
			if ((common_status_t)STATUS_SUCCESS != error)
			{
				break;
			}

			blockDone += blockCountOneTime;
		}
	}

	return error;
}

/*
 * Implements SD_WriteBlocks_Activity
 */
common_status_t SD_WriteBlocks(sd_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
	uint32_t blockCountOneTime; /* The block count can be wrote in one time sending WRITE_BLOCKS transfer_storage.command. */
	uint32_t blockLeft;         /* Left block count to be wrote. */
	uint32_t blockDone = 0U;    /* The block count has been wrote. */
	common_status_t error = (common_status_t)STATUS_SUCCESS;

	DEV_ASSERT(card != NULL);
	if ((blockCount + startBlock) > card->blockCount)
	{
		error = (common_status_t)STATUS_ERROR;
	}
	else
	{
		blockLeft = blockCount;
		while (blockLeft != 0U)
		{
			if (blockLeft > card->host.capability.maxBlockCount)
			{
				blockLeft = (blockLeft - card->host.capability.maxBlockCount);
				blockCountOneTime = card->host.capability.maxBlockCount;
			}
			else
			{
				blockCountOneTime = blockLeft;
				blockLeft = 0U;
			}

			error = SD_Write(card, &buffer[blockDone * FSL_SDMMC_DEFAULT_BLOCK_SIZE], (startBlock + blockDone), FSL_SDMMC_DEFAULT_BLOCK_SIZE, blockCountOneTime);
			if ((common_status_t)STATUS_SUCCESS != error)
			{
				break;
			}

			blockDone += blockCountOneTime;
		}
	}

	return error;
}

/*
 * Implements SD_EraseBlocks_Activity
 */
common_status_t SD_EraseBlocks(sd_card_t *card, uint32_t startBlock, uint32_t blockCount)
{
	uint32_t blockCountOneTime; /* The block count can be erased in one time sending ERASE_BLOCKS transfer_storage.command. */
	uint32_t blockDone = 0U;    /* The block count has been erased. */
	uint32_t blockLeft;         /* Left block count to be erase. */
	common_status_t error = (common_status_t)STATUS_SUCCESS ;

	DEV_ASSERT(card != NULL);
	if ((blockCount + startBlock) > card->blockCount)
	{
		error = (common_status_t)STATUS_ERROR;
	}
	else
	{
		blockLeft = blockCount;
		while (blockLeft != 0U)
		{
			if (blockLeft > ((uint32_t)card->csd.eraseSectorSize + 1U))
			{
				blockCountOneTime = (uint32_t)card->csd.eraseSectorSize + 1U;
				blockLeft = blockLeft - blockCountOneTime;
			}
			else
			{
				blockCountOneTime = blockLeft;
				blockLeft = 0U;
			}

			error = SD_Erase(card, (startBlock + blockDone), blockCountOneTime);
			if ((common_status_t)STATUS_SUCCESS != error)
			{
				break;
			}

			blockDone += blockCountOneTime;
		}
	}
	return error;
}
