/*****************************************************************************/
/* FILE NAME: sdspi.c COPYRIGHT (c) NXP Semiconductors 2017                  */
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

#include "sd/card.h"
#include "sd/spec.h"
#include "sd/sdspi.h"

#include <stdbool.h>
#include <string.h>

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is needed for DEV_ASSERT macro condition.
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
 * Definitons
 ******************************************************************************/
/* Maximum number of retries for setting a card in idle mode [GO_IDLE state] */
#define SD_SDSPI_GOIDLE_MAX_RETRIES (200U)
/* Maximun time for waiting the command */
#define SD_SDSPI_TIMEOUT (1000U)
/* Mximum time for sending command */
#define SD_SDSPI_APPSENDCOND_TIMEOUT (500U)
/* Mximum time for reading command */
#define SD_SDSPI_READ_TIMEOUT (100U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Wait card to be ready state.
 *
 * @param host Host state.
 * @param milliseconds Timeout time in millseconds.
 * @retval aStatus_SDSPI_ExchangeFailed Exchange data over SPI failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_WaitReady(sdspi_host_t *host, uint32_t milliseconds);

/*!
 * @brief Calculate CRC7
 *
 * @param buffer Data buffer.
 * @param length Data length.
 * @param crc The orginal crc value.
 * @return Generated CRC7.
 */
static uint32_t SDSPI_GenerateCRC7(const uint8_t *buffer, const uint32_t length, const uint32_t crc);

/*!
 * @brief Send command.
 *
 * @param host Host state.
 * @param command The command to be wrote.
 * @param timeout The timeout value.
 * @retval aStatus_SDSPI_WaitReadyFailed Wait ready failed.
 * @retval aStatus_SDSPI_ExchangeFailed Exchange data over SPI failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval aStatus_Fail Send command failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendCommand(sdspi_host_t *host, sdspi_command_t *command_l, uint32_t timeout);

/*!
 * @brief Send GO_IDLE command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_ExchangeFailed Send timing byte failed.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_GoIdle(sdspi_card_t *card);

/*!
 * @brief Send GET_INTERFACE_CONDITION command.
 *
 * This function checks card interface condition, which includes host supply voltage information and asks the card
 * whether it supports voltage.
 *
 * @param card Card descriptor.
 * @param pattern The check pattern.
 * @param response Buffer to save the command response.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendInterfaceCondition(sdspi_card_t *card, uint8_t pattern, uint8_t *response_l);

/*!
 * @brief Send SEND_APPLICATION_COMMAND command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendApplicationCmd(sdspi_card_t *card);

/*!
 * @brief Send GET_OPERATION_CONDITION command.
 *
 * @param card Card descriptor.
 * @param argument Operation condition.
 * @param response Buffer to save command response.
 * @retval aStatus_Timeout Timeout.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_ApplicationSendOperationCondition(sdspi_card_t *card, uint32_t argument, uint8_t *response_l);

/*!
 * @brief Retry GET_OPERATION_CONDITION command.
 *
 * @param card Card descriptor.
 * @param applicationCmd41Arg Operation condition.
 * @param applicationCmd41Resp Buffer to save command response.
 * @param host Host state.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_TryApplicationSendOperationCondition(sdspi_card_t *card, uint32_t applicationCmd41Arg, uint8_t *applicationCmd41Resp, sdspi_host_t *host);

/*!
 * @brief Send READ_OCR command to get OCR register content.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_ReadOcr(sdspi_card_t *card);

/*!
 * @brief Send SET_BLOCK_SIZE command.
 *
 * This function sets the block length in bytes for SDSC cards. For SDHC cards, it does not affect memory
 * read or write commands, always 512 bytes fixed block length is used.
 * @param card Card descriptor.
 * @param blockSize Block size.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SetBlockSize(sdspi_card_t *card, uint32_t blockSize);

/*!
 * @brief Read data from card
 *
 * @param host Host state.
 * @param buffer Buffer to save data.
 * @param size The data size to read.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval aStatus_SDSPI_ExchangeFailed Exchange data over SPI failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_Read(sdspi_host_t *host, uint8_t *buffer, uint32_t size);

/*!
 * @brief Decode CSD register
 *
 * @param card Card descriptor.
 * @param rawCsd Raw CSD register content.
 */
static void SDSPI_DecodeCsd(sdspi_card_t *card, uint8_t *rawCsd);

/*!
 * @brief Send GET-CSD command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ReadFailed Read data blocks failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendCsd(sdspi_card_t *card);

/*!
 * @brief Set card to max frequence in normal mode.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SetFrequencyFailed Set frequency failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SetMaxFrequencyNormalMode(sdspi_card_t *card);

/*!
 * @brief Check the capacity of the card
 *
 * @param card Card descriptor.
 */
static void SDSPI_CheckCapacity(sdspi_card_t *card);

/*!
 * @brief Decode raw CID register.
 *
 * @param card Card descriptor.
 * @param rawCid Raw CID register content.
 */
static void SDSPI_DecodeCid(sdspi_card_t *card, uint8_t *rawCid);

/*!
 * @brief Send GET-CID command
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ReadFailed Read data blocks failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendCid(sdspi_card_t *card);

/*!
 * @brief Decode SCR register.
 *
 * @param card Card descriptor.
 * @param rawScr Raw SCR register content.
 */
static void SDSPI_DecodeScr(sdspi_card_t *card, uint8_t *rawScr);

/*!
 * @brief Send SEND_SCR command.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval aStatus_SDSPI_ReadFailed Read data blocks failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_SendScr(sdspi_card_t *card);

/*!
 * @brief Send STOP_TRANSMISSION command to card to stop ongoing data transferring.
 *
 * @param card Card descriptor.
 * @retval aStatus_SDSPI_SendCommandFailed Send command failed.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_StopTransmission(sdspi_card_t *card);

/*!
 * @brief Write data to card
 *
 * @param host Host state.
 * @param buffer Data to send.
 * @param size Data size.
 * @param token The data token.
 * @retval aStatus_SDSPI_WaitReadyFailed Card is busy error.
 * @retval aStatus_SDSPI_ExchangeFailed Exchange data over SPI failed.
 * @retval aStatus_InvalidArgument Invalid argument.
 * @retval aStatus_SDSPI_ResponseError Response is error.
 * @retval STATUS_SUCCESS Operate successfully.
 */
static common_status_t SDSPI_Write(sdspi_host_t *host, uint8_t *buffer, uint32_t size, uint8_t token);

void SD_Command_Init(sdspi_command_t *command_l);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Out for Exchange data over SPI */
static uint8_t response[5U];
/* In param for Exchange data over SPI */
static uint8_t timingByte = 0xFFU;
static sdspi_command_t command;

void SD_Command_Init(sdspi_command_t *command_l)
{
	void *p = command_l;
	(void)memset(p, 0U, sizeof(sdspi_command_t));
}

/*******************************************************************************
 * Code
 ******************************************************************************/
static common_status_t SDSPI_WaitReady(sdspi_host_t *host, uint32_t milliseconds)
{
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t elapsedTime;
	common_status_t status = (common_status_t)STATUS_SUCCESS;

	(void)host;
    startTime = host->getCurrentMilliseconds();
    do
    {
        if (STATUS_SUCCESS != host->exchange(&timingByte, &response[0], 1U))
        {
            status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
            break;
        }
        currentTime = host->getCurrentMilliseconds();
        elapsedTime = (currentTime - startTime);
    } while ((response[0] != 0xFFU) && (elapsedTime < milliseconds));

    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Response 0xFF means card is still busy. */
    	if (response[0] != 0xFFU)
    	{
    		status = (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }
    return status;
}

static uint32_t SDSPI_GenerateCRC7(const uint8_t *buffer, const uint32_t length, const uint32_t crc)
{
    uint32_t length_l = length;
    uint32_t crc_l = crc;
    uint32_t index_l;

    static const uint8_t crcTable[] = {0x00U, 0x09U, 0x12U, 0x1BU, 0x24U, 0x2DU, 0x36U, 0x3FU,
                                       0x48U, 0x41U, 0x5AU, 0x53U, 0x6CU, 0x65U, 0x7EU, 0x77U};

    (void)buffer;
    while (0U != length_l)
    {
        index_l = (((crc_l >> 3U) & 0x0FU) ^ ((uint32_t)(*buffer) >> 4U));
        crc_l = ((crc_l << 4U) ^ crcTable[index_l]);

        index_l = (((crc_l >> 3U) & 0x0FU) ^ ((uint32_t)(*buffer) & 0x0FU));
        crc_l = ((crc_l << 4U) ^ crcTable[index_l]);

        buffer++;
        length_l--;
    }

    return (crc_l & 0x7FU);
}

static common_status_t SDSPI_SendCommand(sdspi_host_t *host, sdspi_command_t *command_l, uint32_t timeout)
{
    DEV_ASSERT(host != NULL);
    DEV_ASSERT(command_l != NULL);

    /* buffer command */
    static uint8_t buffer_in[6U];
    common_status_t status = (int32_t)STATUS_SUCCESS;
    uint8_t i = 0U;
    bool needBreak = false;

    if (((common_status_t)STATUS_SUCCESS != SDSPI_WaitReady(host, timeout)) && (command_l->index != (uint8_t)aSDMMC_GoIdleState))
    {
        status = (common_status_t)aStatus_SDSPI_WaitReadyFailed;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Send command. */
    	buffer_in[0U] = (command_l->index | (uint8_t)0x40U);
    	buffer_in[1U] = (uint8_t)((command_l->argument >> 24U) & 0xFFU);
    	buffer_in[2U] = (uint8_t)((command_l->argument >> 16U) & 0xFFU);
    	buffer_in[3U] = (uint8_t)((command_l->argument >> 8U) & 0xFFU);
    	buffer_in[4U] = (uint8_t)(command_l->argument & 0xFFU);
    	buffer_in[5U] = (uint8_t)((uint32_t)(SDSPI_GenerateCRC7(buffer_in, 5U, 0U) << 1U) | 1U);
    	if (STATUS_SUCCESS != (host->exchange(buffer_in, NULL, sizeof(buffer_in))))
    	{
    		status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Wait for the response coming, the left most bit which is transfered first in first response byte is 0 */
    	for (i = 0U; i < 9U; i++)
    	{
    		if (STATUS_SUCCESS != host->exchange(&timingByte, &response[0], 1U))
    		{
    			status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    			needBreak = true;
    		}
    		else
    		{
    			/* Check if response 0 coming. */
    			if (0U == (response[0] & 0x80U))
    			{
    				needBreak = true;
    			}
    		}
    		if (true == needBreak)
    		{
    			break;
    		}
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (0U != (response[0] & 0x80U)) /* Max index byte is high means response comming. */
    	{
    		status =  (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Receve all the response content. */
    	command_l->response[0U] = response[0];
    	switch (command_l->responseType)
    	{
    	case (int32_t)aSDSPI_ResponseTypeR1:
            break;
    	case (int32_t)aSDSPI_ResponseTypeR1b:
            if ((common_status_t)STATUS_SUCCESS != SDSPI_WaitReady(host, timeout))
            {
    		    status = (common_status_t)aStatus_SDSPI_WaitReadyFailed;
            }
            break;
    	case (int32_t)aSDSPI_ResponseTypeR2:
            if (STATUS_SUCCESS != host->exchange(&timingByte, &(command_l->response[1U]), 1U))
            {
    		    status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
            }
            break;
    	case (int32_t)aSDSPI_ResponseTypeR3:
    	case (int32_t)aSDSPI_ResponseTypeR7:
            /* Left 4 bytes in response type R3 and R7(total 5 bytes in SPI mode) */
            if (STATUS_SUCCESS != host->exchange(&timingByte, &(command_l->response[1U]), 4U))
            {
			    status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
            }
            break;
    	default:
            status = (common_status_t)aStatus_Fail;
            break;
    	}
    }

    return status;
}

static common_status_t SDSPI_GoIdle(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    uint32_t retryCount = SD_SDSPI_GOIDLE_MAX_RETRIES;
    (void)card;

    SD_Command_Init(&command);

    /* SD card will enter SPI mode if the CS is DEV_ASSERTed (negative) during the reception of the reset command (CMD0)
    and the card will be IDLE state. */
    while (0U != retryCount--)
    {
        command.index = (uint8_t)aSDMMC_GoIdleState;
        command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
        if (((common_status_t)STATUS_SUCCESS == SDSPI_SendCommand(card->host, &command, SD_SDSPI_TIMEOUT)) &&
            (command.response[0U] == (uint8_t)aSDSPI_R1InIdleStateFlag))
        {
            break;
        }
    }

    return (common_status_t)STATUS_SUCCESS;
}

static common_status_t SDSPI_SendInterfaceCondition(sdspi_card_t *card, uint8_t pattern, uint8_t *response_l)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    (void)card;
    SD_Command_Init(&command);
    sdspi_host_t *host;
	common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSD_SendInterfaceCondition;
    command.argument = (0x100U | ((uint32_t)pattern & 0xFFU));
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR7;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }
    else
    {
    	(void)memcpy(response_l, command.response, sizeof(command.response));
    }

    return status;
}

static common_status_t SDSPI_SendApplicationCmd(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    (void)card;
    sdspi_host_t *host;
    SD_Command_Init(&command);
	common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSDMMC_ApplicationCommand;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }
    else
    {
    	if ((0U != (command.response[0U])) && (0U == (command.response[0U] & (uint8_t)aSDSPI_R1InIdleStateFlag)))
    	{
    		status = (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }
    return status;
}

static common_status_t SDSPI_ApplicationSendOperationCondition(sdspi_card_t *card, uint32_t argument, uint8_t *response_l)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);
    DEV_ASSERT(response_l != NULL);

    SD_Command_Init(&command);
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t elapsedTime = 0U;
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSD_ApplicationSendOperationCondition;
    command.argument = argument;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    startTime = host->getCurrentMilliseconds();
    do
    {
    	status = SDSPI_SendApplicationCmd(card);
        if ((common_status_t)STATUS_SUCCESS == status)
        {
        	status = SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT);
            if ((common_status_t)STATUS_SUCCESS == status)
            {
                if (0U != ((uint32_t)command.response[0U]))
                {
                    break;
                }
            }
        }

        currentTime = host->getCurrentMilliseconds();
        elapsedTime = (currentTime - startTime);
    } while (elapsedTime < SD_SDSPI_TIMEOUT);

    if (elapsedTime > SD_SDSPI_TIMEOUT)
    {
        status = (common_status_t)aStatus_Timeout;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (NULL != response_l)
    	{
    		(void)memcpy(response_l, command.response, sizeof(command.response));
    	}
    }

    return status;
}

static common_status_t SDSPI_ReadOcr(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    uint32_t i;
    sdspi_host_t *host;
    SD_Command_Init(&command);
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSDMMC_ReadOcr;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR3;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (0U != command.response[0U])
    	{
    		status = (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Switch the bytes sequence. All register's content is transferred from highest byte to lowest byte. */
    	card->ocr = 0U;
    	for (i = 4U; i > 0U; i--)
    	{
    		card->ocr |= (uint32_t)command.response[i] << ((4U - i) * 8U);
    	}
    }

    return status;
}

static common_status_t SDSPI_SetBlockSize(sdspi_card_t *card, uint32_t blockSize)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    (void)card;
    SD_Command_Init(&command);
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSDMMC_SetBlockLength;
    command.argument = blockSize;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }

    return status;
}

static common_status_t SDSPI_Read(sdspi_host_t *host, uint8_t *buffer, uint32_t size)
{
    DEV_ASSERT(host != NULL);
    DEV_ASSERT(host->exchange != NULL);
    DEV_ASSERT(buffer != NULL);
    DEV_ASSERT(size != 0U);

    (void)host;
    uint32_t startTime;
    uint32_t currentTime;
    uint32_t elapsedTime;
    uint8_t i;

   /* The byte need to be sent as read/write data block timing requirement */
    common_status_t status = (common_status_t)STATUS_SUCCESS;

(void)memset(buffer, 0xFFU, size);

    /* Wait data token comming */
    startTime = host->getCurrentMilliseconds();
    do
    {
        if (STATUS_SUCCESS != host->exchange(&timingByte, &response[0], 1U))
        {
            status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
            break;
        }

        currentTime = host->getCurrentMilliseconds();
        elapsedTime = (currentTime - startTime);
    } while ((response[0] == 0xFFU) && (elapsedTime < SD_SDSPI_READ_TIMEOUT));

    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Check data token and exchange data. */
    	if (response[0] != (uint8_t)aSDSPI_DataTokenBlockRead)
    	{
    		status = (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (STATUS_SUCCESS  != (host->exchange(buffer, buffer, size)))
    	{
    		status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Get 16 bit CRC */
    	for (i = 0U; i < 2U; i++)
    	{
    		if (STATUS_SUCCESS != host->exchange(&timingByte, &response[0], 1U))
    		{
    			status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    			break;
    		}
    	}
    }

    return status;
}

static void SDSPI_DecodeCsd(sdspi_card_t *card, uint8_t *rawCsd)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(rawCsd != NULL);

    sd_csd_t *csd;
    (void)rawCsd;

    csd = &(card->csd);
    csd->csdStructure = (rawCsd[0U] >> 6U);
    csd->dataReadAccessTime1 = rawCsd[1U];
    csd->dataReadAccessTime2 = rawCsd[2U];
    csd->transferSpeed = rawCsd[3U];
    csd->cardCommandClass = (uint16_t)(((uint32_t)rawCsd[4U] << 4U) | ((uint32_t)rawCsd[5U] >> 4U));
    csd->readBlockLength = ((rawCsd)[5U] & 0xFU);
    if (0U != (rawCsd[6U] & 0x80U))
    {
        csd->flags |= (uint16_t)aSD_CsdReadBlockPartialFlag;
    }
    if (0U != (rawCsd[6U] & 0x40U))
    {
        csd->flags |= (uint16_t)aSD_CsdWriteBlockMisalignFlag;
    }
    if (0U != (rawCsd[6U] & 0x20U))
    {
        csd->flags |= (uint16_t)aSD_CsdReadBlockMisalignFlag;
    }
    if (0U != (rawCsd[6U] & 0x10U))
    {
        csd->flags |= (uint16_t)aSD_CsdDsrImplementedFlag;
    }

    /* Some fileds is different when csdStructure is different. */
    if (csd->csdStructure == 0U) /* Decode the bits when CSD structure is version 1.0 */
    {
        csd->deviceSize =
            ((((uint32_t)rawCsd[6] & 0x3U) << 10U) | ((uint32_t)rawCsd[7U] << 2U) | ((uint32_t)rawCsd[8U] >> 6U));
        csd->readCurrentVddMin = ((rawCsd[8U] >> 3U) & 7U);
        csd->readCurrentVddMax = (rawCsd[8U] >> 7U);
        csd->writeCurrentVddMin = ((rawCsd[9U] >> 5U) & 7U);
        csd->writeCurrentVddMax = (rawCsd[9U] >> 2U);
        csd->deviceSizeMultiplier = (uint8_t)(((rawCsd[9U] & 3U) << 1U) | (rawCsd[10U] >> 7U));
        card->blockCount = (csd->deviceSize + 1U) << (csd->deviceSizeMultiplier + 2U);
        card->blockSize = ((uint32_t)1U << (uint32_t)(csd->readBlockLength));
        if (card->blockSize != SD_SDSPI_DEFAULT_BLOCK_SIZE)
        {
            card->blockCount = (card->blockCount * card->blockSize);
            card->blockSize = SD_SDSPI_DEFAULT_BLOCK_SIZE;
            card->blockCount = (card->blockCount / card->blockSize);
        }
    }
    else if (csd->csdStructure == 1U) /* Decode the bits when CSD structure is version 2.0 */
    {
        card->blockSize = SD_SDSPI_DEFAULT_BLOCK_SIZE;
        csd->deviceSize =
            ((((uint32_t)rawCsd[7U] & 0x3FU) << 16U) | ((uint32_t)rawCsd[8U] << 8U) | ((uint32_t)rawCsd[9U]));
        if (csd->deviceSize >= 0xFFFFU)
        {
            card->flags |= (uint32_t)aSDSPI_SupportSdxcFlag;
        }
        card->blockCount = ((csd->deviceSize + 1U) * 1024U);
    }
    else
    {
    	/* Nothing to do! */
    }

    if (0U != ((rawCsd[10U] >> 6U) & 1U))
    {
        csd->flags |= (uint16_t)aSD_CsdEraseBlockEnabledFlag;
    }
    csd->eraseSectorSize = (uint8_t)(((rawCsd[10U] & 0x3FU) << 1U) | (rawCsd[11U] >> 7U));
    csd->writeProtectGroupSize = (rawCsd[11U] & 0x7FU);
    if (0U != (rawCsd[12U] >> 7U))
    {
        csd->flags |= (uint16_t)aSD_CsdWriteProtectGroupEnabledFlag;
    }
    csd->writeSpeedFactor = ((rawCsd[12U] >> 2U) & 7U);
    csd->writeBlockLength = (uint8_t)(((rawCsd[12U] & 3U) << 2U) | (rawCsd[13U] >> 6U));
    if (0U != ((rawCsd[13U] >> 5U) & 1U))
    {
        csd->flags |= (uint16_t)aSD_CsdWriteBlockPartialFlag;
    }
    if (0U != (rawCsd[14U] >> 7U))
    {
        csd->flags |= (uint16_t)aSD_CsdFileFormatGroupFlag;
    }
    if (0U != ((rawCsd[14U] >> 6U) & 1U))
    {
        csd->flags |= (uint16_t)aSD_CsdCopyFlag;
    }
    if (0U != ((rawCsd[14U] >> 5U) & 1U))
    {
        csd->flags |= (uint16_t)aSD_CsdPermanentWriteProtectFlag;
    }
    if (0U != ((rawCsd[14U] >> 4U) & 1U))
    {
        csd->flags |= (uint16_t)aSD_CsdTemporaryWriteProtectFlag;
    }
    csd->fileFormat = ((rawCsd[14U] >> 2U) & 3U);
}

static common_status_t SDSPI_SendCsd(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    SD_Command_Init(&command);
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSDMMC_SendCsd;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }
    else
    {
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_Read(host, card->rawCsd, sizeof(card->rawCsd)))
    	{
    		status = (common_status_t)aStatus_SDSPI_ReadFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	SDSPI_DecodeCsd(card, card->rawCsd);
    }

    return status;
}

static common_status_t SDSPI_SetMaxFrequencyNormalMode(sdspi_card_t *card)
{
	/* Rate unit(divided by 1000) of transfer speed in non-high-speed mode. */
	static const uint32_t g_transferSpeedRateUnit[] = {
	    /* 100Kbps, 1Mbps, 10Mbps, 100Mbps*/
	    100U, 1000U, 10000U, 100000U,
	};
	/* Multiplier factor(multiplied by 1000) of transfer speed in non-high-speed mode. */
	static const uint32_t g_transferSpeedMultiplierFactor[] = {
	    0U, 1000U, 1200U, 1300U, 1500U, 2000U, 2500U, 3000U, 3500U, 4000U, 4500U, 5000U, 5500U, 6000U, 7000U, 8000U,
	};

    uint32_t maxFrequency;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    (void)card;
    /* Calculate max frequency card supported in non-high-speed mode. */
    maxFrequency = g_transferSpeedRateUnit[SD_RD_TRANSFER_SPEED_RATE_UNIT(card->csd)] *
                   g_transferSpeedMultiplierFactor[SD_RD_TRANSFER_SPEED_TIME_VALUE(card->csd)];
    if (maxFrequency > card->host->busBaudRate)
    {
        maxFrequency = card->host->busBaudRate;
    }

    if (STATUS_SUCCESS != card->host->setFrequency(maxFrequency))
    {
        status = (common_status_t)aStatus_SDSPI_SetFrequencyFailed;
    }

    return status;
}

static void SDSPI_CheckCapacity(sdspi_card_t *card)
{
    uint32_t deviceSize;
    uint32_t deviceSizeMultiplier;
    uint32_t readBlockLength;

    if (0U != (card->csd.csdStructure))
    {
        /* SD CSD structure v2.xx */
        deviceSize = card->csd.deviceSize;
        if (deviceSize >= 0xFFFFU) /* Bigger than 32GB */
        {
            /* extended capacity */
            card->flags |= (uint32_t)aSDSPI_SupportSdxcFlag;
        }
        else
        {
            card->flags |= (uint32_t)aSDSPI_SupportSdhcFlag;
        }
        deviceSizeMultiplier = 10U;
        deviceSize += 1U;
        readBlockLength = 9U;
    }
    else
    {
        /* SD CSD structure v1.xx */
        deviceSize = (card->csd.deviceSize + 1U);
        deviceSizeMultiplier = ((uint32_t)(card->csd.deviceSizeMultiplier) + (uint32_t)2U);
        readBlockLength = card->csd.readBlockLength;
        /* Card maximum capacity is 2GB when CSD structure version is 1.0 */
        card->flags |= (uint32_t)aSDSPI_SupportSdscFlag;
    }
    if (readBlockLength != 9U)
    {
        /* Force to use 512-byte length block */
        deviceSizeMultiplier += (readBlockLength - 9U);
        readBlockLength = 9U;
    }

    card->blockSize = ((uint32_t)1U << (uint32_t)(readBlockLength));
    card->blockCount = (deviceSize << deviceSizeMultiplier);
}

static void SDSPI_DecodeCid(sdspi_card_t *card, uint8_t *rawCid)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(rawCid != NULL);

    sd_cid_t *cid = &(card->cid);
    cid->manufacturerID = rawCid[0U];
    cid->applicationID = (uint16_t)(((uint32_t)rawCid[1U] << 8U) | (uint32_t)(rawCid[2U]));
    (void)memcpy(cid->productName, &rawCid[3U], SD_PRODUCT_NAME_BYTES);
    cid->productVersion = rawCid[8U];
    cid->productSerialNumber = (((uint32_t)rawCid[9U] << 24U) | ((uint32_t)rawCid[10U] << 16U) |
                                ((uint32_t)rawCid[11U] << 8U) | ((uint32_t)rawCid[12U]));
    cid->manufacturerData = (uint16_t)((((uint32_t)rawCid[13U] & 0x0FU) << 8U) | ((uint32_t)rawCid[14U]));
}

static common_status_t SDSPI_SendCid(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    SD_Command_Init(&command);
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    command.index = (uint8_t)aSDMMC_SendCid;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }
    else
    {
    	if ((common_status_t)STATUS_SUCCESS != (SDSPI_Read(host, card->rawCid, sizeof(card->rawCid))))
    	{
    		status = (common_status_t)aStatus_SDSPI_ReadFailed;
    	}

    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	SDSPI_DecodeCid(card, card->rawCid);
    }
    return status;
}

static void SDSPI_DecodeScr(sdspi_card_t *card, uint8_t *rawScr)
{
    (void)rawScr;
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(rawScr != NULL);

    sd_scr_t *scr = &(card->scr);
    scr->scrStructure = ((rawScr[0U] & 0xF0U) >> 4U);
    scr->sdSpecification = (rawScr[0U] & 0x0FU);
    if (0U != (rawScr[1U] & 0x80U))
    {
        scr->flags |= (uint16_t)aSD_ScrDataStatusAfterErase;
    }
    scr->sdSecurity = (uint8_t)((rawScr[1U] & 0x70U) >> 4U);
    scr->sdBusWidths = (rawScr[1U] & 0x0FU);
    if (0U != (rawScr[2U] & 0x80U))
    {
        scr->flags |= (uint16_t)aSD_ScrSdSpecification3;
    }
    scr->extendedSecurity = (uint8_t)((rawScr[2U] & 0x78U) >> 3U);
    scr->commandSupport = (rawScr[3U] & 0x03U);
    scr->reservedForManufacturer = (((uint32_t)rawScr[4U] << 24U) | ((uint32_t)rawScr[5U] << 16U) |
                                    ((uint32_t)rawScr[6U] << 8U) | (uint32_t)rawScr[7U]);
}

static common_status_t SDSPI_SendScr(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);

    SD_Command_Init(&command);
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    host = card->host;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendApplicationCmd(card))
    {
        status = (common_status_t)aStatus_SDSPI_SendApplicationCommandFailed;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	command.index = (uint8_t)aSD_ApplicationSendScr;
    	command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    	{
    		status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if ((common_status_t)STATUS_SUCCESS != (SDSPI_Read(host, card->rawScr, sizeof(card->rawScr))))
    	{
    		status = (common_status_t)aStatus_SDSPI_ReadFailed;
    	}
    	else
    	{
    		SDSPI_DecodeScr(card, card->rawScr);
    	}
    }

    return status;
}

static common_status_t SDSPI_StopTransmission(sdspi_card_t *card)
{
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;
    (void)card;

    SD_Command_Init(&command);
	host = card->host;
    command.index = (uint8_t)aSDMMC_StopTransmission;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1b;
    if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    {
        status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    }

    return status;
}

static common_status_t SDSPI_Write(sdspi_host_t *host, uint8_t *buffer, uint32_t size, uint8_t token)
{
    DEV_ASSERT(host != NULL);
    DEV_ASSERT(host->exchange != NULL);

    uint8_t i;
    response[0] = token;

   common_status_t status = (common_status_t)STATUS_SUCCESS;

    if ((common_status_t)STATUS_SUCCESS != SDSPI_WaitReady(host, SD_SDSPI_TIMEOUT))
    {
    	status = (common_status_t)aStatus_SDSPI_WaitReadyFailed;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Write data token. */
    	if (STATUS_SUCCESS != (host->exchange(&response[0], NULL, 1U)))
    	{
    		status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (response[0] == (uint8_t)aSDSPI_DataTokenStopTransfer)
    	{
    		status = (common_status_t)STATUS_SUCCESS;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if ((0U == size) || (0U == buffer))
    	{
    		status = (common_status_t)aStatus_InvalidArgument;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Write data. */
    	if (STATUS_SUCCESS != host->exchange(buffer, NULL, size))
    	{
    		status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Get the last two bytes CRC */
    	for (i = 0U; i < 2U; i++)
    	{
    		if (STATUS_SUCCESS != (host->exchange(&timingByte, NULL, 1U)))
    		{
    			status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    			break;
    		}
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Get the response token. */
    	if (STATUS_SUCCESS != (host->exchange(&timingByte, &response[1], 1U)))
    	{
    		status = (common_status_t)aStatus_SDSPI_ExchangeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if ((response[1] & (uint8_t)SDSPI_DATA_RESPONSE_TOKEN_MASK) != (uint8_t)aSDSPI_DataResponseTokenAccepted)
    	{
    		status = (common_status_t)aStatus_SDSPI_ResponseError;
    	}
    }

    return status;
}

static common_status_t SDSPI_TryApplicationSendOperationCondition(sdspi_card_t *card, uint32_t applicationCmd41Arg, uint8_t *applicationCmd41Resp, sdspi_host_t *host)
{
	common_status_t status = (common_status_t)STATUS_SUCCESS;
	uint32_t currentTime;
    uint32_t elapsedTime;
    uint32_t startTime;
    (void)host;

	/* Set card's interface condition according to host's capability and card's supported interface condition */
	startTime = host->getCurrentMilliseconds();
	do
	{
		if ((common_status_t)STATUS_SUCCESS !=
				SDSPI_ApplicationSendOperationCondition(card, applicationCmd41Arg, applicationCmd41Resp))
		{
			status = (common_status_t)aStatus_SDSPI_SendOperationConditionFailed;
		}

		if ((common_status_t)STATUS_SUCCESS == status)
		{
			currentTime = host->getCurrentMilliseconds();
			elapsedTime = (currentTime - startTime);
			if (elapsedTime > SD_SDSPI_APPSENDCOND_TIMEOUT)
			{
				status = (common_status_t)aStatus_Timeout;
			}
		}
		if (((common_status_t)STATUS_SUCCESS == status) && (0U == applicationCmd41Resp[0U]))
		{
			break;
		}
		if ((status == (common_status_t)aStatus_SDSPI_SendOperationConditionFailed)||(status == (common_status_t)aStatus_Timeout))
		{
			applicationCmd41Resp[0U] = 0U;
		}
	} while (0U != (applicationCmd41Resp[0U] & (uint8_t)aSDSPI_R1InIdleStateFlag));
	return status;
}

common_status_t SDSPI_Init(sdspi_card_t *card)
{
    (void)card;
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);
    DEV_ASSERT(card->host->setFrequency  != NULL);
    DEV_ASSERT(card->host->exchange != NULL);
    DEV_ASSERT(card->host->getCurrentMilliseconds != NULL);

    /* Response for Command41-check voltage pattern */
    static uint8_t applicationCommand41Response[5U];
    sdspi_host_t *host = card->host;
    uint32_t applicationCommand41Argument = 0U;
    bool likelySdV1 = false;
    common_status_t status = (common_status_t)STATUS_SUCCESS;
    (void)host;

    /* Card must be initialized in 400KHZ. */
    if (STATUS_SUCCESS != (host->setFrequency(SDMMC_CLOCK_400KHZ)))
    {
    	status = (common_status_t)aStatus_SDSPI_SetFrequencyFailed;
    }
    else
    {
    	/* Reset the card by CMD0. */
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_GoIdle(card))
    	{
    		status = (common_status_t)aStatus_SDSPI_GoIdleFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Check the card's supported interface condition. */
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendInterfaceCondition(card, 0xAAU, response))
    	{
    		likelySdV1 = true;
    	}
    	else if ((response[3U] == 0x1U) || (response[4U] == 0xAAU))
    	{
    		applicationCommand41Argument |= (uint32_t)aSD_OcrHostCapacitySupportFlag;
    	}
    	else
    	{
    		status = (common_status_t)aStatus_SDSPI_SendInterfaceConditionFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	status = SDSPI_TryApplicationSendOperationCondition(card, applicationCommand41Argument, applicationCommand41Response, host);
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if (!likelySdV1)
    	{
    		if ((common_status_t)STATUS_SUCCESS != SDSPI_ReadOcr(card))
    		{
    			status = (common_status_t)aStatus_SDSPI_ReadOcrFailed;
    		}
    		else
    		{
    			if (0U != (card->ocr & (uint32_t)aSD_OcrCardCapacitySupportFlag))
    			{
    				card->flags |= (uint32_t)aSDSPI_SupportHighCapacityFlag;
    			}
    		}
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Force to use 512-byte length block, no matter which version.  */
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SetBlockSize(card, 512U))
    	{
    		status = (common_status_t)aStatus_SDSPI_SetBlockSizeFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCsd(card))
    	{
    		status = (common_status_t)aStatus_SDSPI_SendCsdFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Set to max frequency according to the max frequency information in CSD register. */
    	(void)SDSPI_SetMaxFrequencyNormalMode(card);

    	/* Save capacity, read only attribute and CID, SCR registers. */
    	SDSPI_CheckCapacity(card);
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCid(card))
    	{
    		status = (common_status_t)aStatus_SDSPI_SendCidFailed;
    	}
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendScr(card))
    	{
    		status = (common_status_t)aStatus_SDSPI_SendCidFailed;
    	}
    }

    return status;
}

void SDSPI_Deinit(sdspi_card_t *card)
{
    DEV_ASSERT(card != NULL);
	void *p = card;

	(void)memset(p, 0, sizeof(sdspi_card_t));
}

bool SDSPI_CheckReadOnly(sdspi_card_t *card)
{
	(void)card;
    DEV_ASSERT(card != NULL);
    return ((0U != (card->flags & (uint16_t)aSD_WriteProtectFlag)) ||
    		(0U != (card->csd.flags & (uint16_t)aSD_CsdPermanentWriteProtectFlag)) ||
            (0U != (card->csd.flags & (uint16_t)aSD_CsdTemporaryWriteProtectFlag)));
}

common_status_t SDSPI_ReadBlocks(sdspi_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);
    DEV_ASSERT(buffer != NULL);
    DEV_ASSERT(blockCount != 0U);

    uint32_t offset;
    uint32_t i;
    SD_Command_Init(&command);
    sdspi_host_t *host;
    common_status_t status = (common_status_t)STATUS_SUCCESS;

    offset = startBlock;
    if (0U == (uint32_t)((card)->flags & (uint32_t)aSDSPI_SupportHighCapacityFlag))
    {
        offset *= card->blockSize;
    }

    /* Send command and reads data. */
    host = card->host;
    command.argument = offset;
    command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    if (blockCount == 1U)
    {
        command.index = (uint8_t)aSDMMC_ReadSingleBlock;
        if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
        {
            status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
        }
        else
        {
        	if ((common_status_t)STATUS_SUCCESS != SDSPI_Read(host, buffer, card->blockSize))
        	{
        		status = (common_status_t)aStatus_SDSPI_ReadFailed;
        	}
        }
    }
    else
    {
    	command.index = (uint8_t)aSDMMC_ReadMultipleBlock;
    	if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    	{
    		status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    	}
    	if ((common_status_t)STATUS_SUCCESS == status)
    	{
    		for (i = 0U; i < blockCount; i++)
    		{
    			if ((common_status_t)STATUS_SUCCESS != SDSPI_Read(host, &buffer[i * card->blockSize], card->blockSize))
    			{
    				status = (common_status_t)aStatus_SDSPI_ReadFailed;
    				break;
    			}
    		}
    	}
    	if ((common_status_t)STATUS_SUCCESS == status)
    	{
    		/* Write stop transmission command after the last data block. */
    		if ((common_status_t)STATUS_SUCCESS != SDSPI_StopTransmission(card))
    		{
    			status = (common_status_t)aStatus_SDSPI_StopTransmissionFailed;
    		}
    	}
    }

    return status;
}

common_status_t SDSPI_WriteBlocks(sdspi_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount)
{
    DEV_ASSERT(card != NULL);
    DEV_ASSERT(card->host != NULL);
    DEV_ASSERT(buffer != NULL);
    DEV_ASSERT(blockCount != 0U);

    uint32_t offset;
    uint32_t i;
    sdspi_host_t *host;
    SD_Command_Init(&command);
    common_status_t status = (common_status_t)STATUS_SUCCESS;
     void *p = &command;

    if (SDSPI_CheckReadOnly(card))
    {
        status = (common_status_t)aStatus_SDSPI_WriteProtected;
    }
    if ((common_status_t)STATUS_SUCCESS == status)
    {
    	offset = startBlock;
    	if (0U == (uint32_t)((card)->flags & (uint32_t)aSDSPI_SupportHighCapacityFlag))
    	{
    		offset *= card->blockSize;
    	}

    	/* Send command and writes data. */
    	host = card->host;
    	if (blockCount == 1U)
    	{
    		command.index = (uint8_t)aSDMMC_WriteSingleBlock;
    		command.argument = offset;
    		command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
    		if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
    		{
    			status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			if (0U != (command.response[0U]))
    			{
    				status = (common_status_t)aStatus_SDSPI_ResponseError;
    			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			if ((common_status_t)STATUS_SUCCESS != SDSPI_Write(host, buffer, card->blockSize, (uint8_t)aSDSPI_DataTokenSingleBlockWrite))
    			{
    				status = (common_status_t)aStatus_SDSPI_WriteFailed;
    			}
    		}
    	}
    	else
    	{
#if defined FSL_SDSPI_ENABLE_PRE_ERASE_ON_WRITE
    		/* Pre-erase before writing data */
    		if (STATUS_SUCCESS != SDSPI_SendApplicationCmd(card))
    		{
    			status = aStatus_SDSPI_SendApplicationCommandFailed;
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			command.index = kSDAppSetWrBlkEraseCount;
    			command.argument = blockCount;
    			command.responseType = aSDSPI_ResponseTypeR1;
    			if (STATUS_SUCCESS != SDSPI_SendCommand(host->base, &command, SD_SDSPI_TIMEOUT))
    			{
    				status = aStatus_SDSPI_SendCommandFailed;
    			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			if (req->response[0U])
    			{
    				status = aStatus_SDSPI_ResponseError;
    			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
#endif
    		(void)memset(p, 0U, sizeof(sdspi_command_t));
			command.index = (uint8_t)aSDMMC_WriteMultipleBlock;
			command.argument = offset;
			command.responseType = (uint8_t)aSDSPI_ResponseTypeR1;
			if ((common_status_t)STATUS_SUCCESS != SDSPI_SendCommand(host, &command, SD_SDSPI_TIMEOUT))
			{
				status = (common_status_t)aStatus_SDSPI_SendCommandFailed;
			}
#if defined FSL_SDSPI_ENABLE_PRE_ERASE_ON_WRITE
    		}
#endif
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			if (0U != (command.response[0U]))
    			{
    				status = (common_status_t)aStatus_SDSPI_ResponseError;
    			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			for (i = 0U; i < blockCount; i++)
    			{
    				if ((common_status_t)STATUS_SUCCESS != SDSPI_Write(host, &buffer[i * card->blockSize], card->blockSize, (uint8_t)aSDSPI_DataTokenMultipleBlockWrite))
    				{
    					status = (common_status_t)aStatus_SDSPI_WriteFailed;
    					break;
    				}
     			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			if ((common_status_t)STATUS_SUCCESS != SDSPI_Write(host, NULL, 0U, (uint8_t)aSDSPI_DataTokenStopTransfer))
    			{
    				status = (common_status_t)aStatus_SDSPI_WriteFailed;
    			}
    		}
    		if ((common_status_t)STATUS_SUCCESS == status)
    		{
    			/* Wait the card programming end. */
				if ((common_status_t)STATUS_SUCCESS != SDSPI_WaitReady(host, SD_SDSPI_TIMEOUT))
				{
					status = (common_status_t)aStatus_SDSPI_WaitReadyFailed;
				}
    		}
    	}
    }

    return status;
}
