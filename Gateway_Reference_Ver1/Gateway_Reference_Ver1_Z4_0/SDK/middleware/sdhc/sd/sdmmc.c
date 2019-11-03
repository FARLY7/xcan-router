/*****************************************************************************/
/* FILE NAME: sdmmc.c COPYRIGHT (c) NXP Semiconductors 2017                  */
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

#include "sd/spec.h"
#include "sd/card.h"
#include "sd/sdmmc.h"

#include <stdbool.h>

/*!
 * @sdmmc.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object
 * This is need for DEV_ASSERT macro condition.
 *
 */

/*******************************************************************************
 * Code
 ******************************************************************************/

static usdhc_transfer_storage_t transfer_storage;

/*
 * Implements SDMMC_SelectCard_Activity
 */
common_status_t SDMMC_SelectCard(uint32_t instance, usdhc_transfer_function_t transfer, uint32_t relativeAddress, bool isSelected)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

    SD_Transfer_Init(&transfer_storage);
    transfer_storage.command.index = (uint32_t)aSDMMC_SelectCard;
    if (isSelected)
    {
    	transfer_storage.command.argument = relativeAddress << 16U;
    	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;
    }
    else
    {
    	transfer_storage.command.argument = 0U;
    	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_NONE;
    }

    transfer_storage.content.command = &transfer_storage.command;
    transfer_storage.content.data = NULL;
    if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

    /* Wait until card to transfer state */
    return status;
}

/*
 * Implements SDMMC_WaitWriteComplete_Activity
 */
common_status_t SDMMC_WaitWriteComplete(uint32_t instance, usdhc_transfer_function_t transfer, uint32_t relativeAddress)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
    bool needLoop = true;

    SD_Transfer_Init(&transfer_storage);
    transfer_storage.command.index = (uint32_t)aSDMMC_SendStatus;
    transfer_storage.command.argument = relativeAddress << 16U;
    transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

    do
    {
    	transfer_storage.content.command = &transfer_storage.command;
    	transfer_storage.content.data = NULL;
    	status = (common_status_t)transfer(instance, &transfer_storage.content);
    	if (((common_status_t)STATUS_SUCCESS != status) ||
    			(0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
        {
            status = (common_status_t)aStatus_SDMMC_TransferFailed;
            needLoop = false;
        }
    	else
    	{
    		if ((0U != ((transfer_storage.command.response[0U] & (uint32_t)aSDMMC_R1ReadyForDataFlag))) &&
    				(SDMMC_R1_CURRENT_STATE(transfer_storage.command.response[0U]) != (uint32_t)aSDMMC_R1StateProgram))
    		{
    			needLoop = false;
    		}
    	}
    } while (true == needLoop);

    return status;
}

/*
 * Implements SDMMC_SendApplicationCommand_Activity
 */
common_status_t SDMMC_SendApplicationCommand(uint32_t instance, usdhc_transfer_function_t transfer, uint32_t relativeAddress)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

    SD_Transfer_Init(&transfer_storage);
    transfer_storage.command.index = (uint32_t)aSDMMC_ApplicationCommand;
    transfer_storage.command.argument = 0U;
    transfer_storage.command.argument = (relativeAddress << 16U);
    transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

    transfer_storage.content.command = &transfer_storage.command;
    transfer_storage.content.data = NULL;
    if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }
    else
    {
    	if (0U == (transfer_storage.command.response[0U] & (uint32_t)aSDMMC_R1ApplicationCommandFlag))
    	{
    		status = (common_status_t)aStatus_SDMMC_CardNotSupport;
    	}
    }
    return status;
}

common_status_t SDMMC_SwitchVoltage(uint32_t instance, usdhc_transfer_function_t transfer)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;

    SD_Transfer_Init(&transfer_storage);
    transfer_storage.command.index = (uint32_t)aSD_VoltageSwitch;
    transfer_storage.command.argument = 0U;
    transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

    transfer_storage.content.command = &transfer_storage.command;
    transfer_storage.content.data = NULL;
	if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

	return status;
}

/*
 * Implements SDMMC_SetBlockCount_Activity
 */
common_status_t SDMMC_SetBlockCount(uint32_t instance, usdhc_transfer_function_t transfer, uint32_t blockCount)
{
    DEV_ASSERT(transfer != NULL);
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    SD_Transfer_Init(&transfer_storage);
    transfer_storage.command.index = (uint32_t)aSDMMC_SetBlockCount;
    transfer_storage.command.argument = blockCount;
    transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

    transfer_storage.content.command = &transfer_storage.command;
    transfer_storage.content.data = NULL;
    if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

    return status;
}

/*
 * Implements SDMMC_GoIdle_Activity
 */
common_status_t SDMMC_GoIdle(uint32_t instance, usdhc_transfer_function_t transfer)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSDMMC_GoIdleState;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
    if (STATUS_SUCCESS != transfer(instance, &transfer_storage.content))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

    return status;
}

/*
 * Implements SDMMC_StopTransmission_Activity
 */
common_status_t SDMMC_StopTransmission(uint32_t instance, usdhc_transfer_function_t transfer)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSDMMC_StopTransmission;
	transfer_storage.command.argument = 0U;
	transfer_storage.command.type = uSDHC_COMMAND_TYPE_ABORT;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1b;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

    return status;
}

/*
 * Implements SDMMC_SetBlockSize_Activity
 */
common_status_t SDMMC_SetBlockSize(uint32_t instance, usdhc_transfer_function_t transfer, uint32_t blockSize)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	SD_Transfer_Init(&transfer_storage);

	transfer_storage.command.index = (uint32_t)aSDMMC_SetBlockLength;
	transfer_storage.command.argument = blockSize;
	transfer_storage.command.responseType = uSDHC_RESPONSE_TYPE_R1;

	transfer_storage.content.command = &transfer_storage.command;
	transfer_storage.content.data = NULL;
	if ((STATUS_SUCCESS != transfer(instance, &transfer_storage.content)) || (0U != ((transfer_storage.command.response[0U]) & (uint32_t)aSDMMC_R1ErrorAllFlag)))
    {
        status = (common_status_t)aStatus_SDMMC_TransferFailed;
    }

    return status;
}
