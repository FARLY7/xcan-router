/*****************************************************************************/
/* FILE NAME: spec.h COPYRIGHT (c) NXP Semiconductors 2017                   */
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

#ifndef SD_SPEC_H
#define SD_SPEC_H

#include <stdint.h>

/*!
 * @sd.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 *
  @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, A function should be used in preference
 * to a function-like macro where they are interchangeable.
 * The macro is used to define static or dynamic implementation of API functions.
 * This way is more efficient.
 */

/*!
 * @addtogroup Card Specification
 * @ingroup sdhc_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief SD/MMC generic status. */
typedef int32_t common_status_t;

/*! @brief SD/MMC card initialization clock frequency */
#define SDMMC_CLOCK_400KHZ (400000U)
/*! @brief SD card bus frequency 1 in high speed mode */
#define SD_CLOCK_25MHZ (25000000U)
/*! @brief SD card bus frequency 1 in high speed mode */
#define SD_CLOCK_26MHZ (26000000U)
/*! @brief SD card bus frequency 2 in high speed mode */
#define SD_CLOCK_40MHZ (40000000U)
/*! @brief SD card bus frequency 3 in high speed mode */
#define SD_CLOCK_50MHZ (50000000U)
/*! @brief MMC card bus frequency 1 in high speed mode */
#define MMC_CLOCK_26MHZ (26000000U)
/*! @brief MMC card bus frequency 2 in high speed mode */
#define MMC_CLOCK_52MHZ (52000000U)

/*! @brief Card status bit in R1 */
typedef enum _sdmmc_r1_card_status_flag
{
#define aSDMMC_R1OutOfRange ((uint32_t)0x80000000U)
#ifdef __ghs__
    aSDMMC_R1OutOfRangeFlag = (1 << 31),               /*!< (1 << 31) Out of range status bit */
#else
    aSDMMC_R1OutOfRangeFlag = aSDMMC_R1OutOfRange,               /*!< (1 << 31) Out of range status bit */
#endif
#define aSDMMC_R1AddressError ((uint32_t)0x40000000U)
	aSDMMC_R1AddressErrorFlag = aSDMMC_R1AddressError,               /*!< (1 << 30) Address error status bit */
#define aSDMMC_R1BlockLengthError ((uint32_t)0x20000000U)
	aSDMMC_R1BlockLengthErrorFlag = aSDMMC_R1BlockLengthError,           /*!< (1 << 29) Block length error status bit */
#define aSDMMC_R1EraseSequenceError ((uint32_t)0x10000000U)
	aSDMMC_R1EraseSequenceErrorFlag = aSDMMC_R1EraseSequenceError,         /*!< (1 << 28) Erase sequence error status bit */
#define aSDMMC_R1EraseParameterError ((uint32_t)0x8000000U)
	aSDMMC_R1EraseParameterErrorFlag = aSDMMC_R1EraseParameterError,        /*!< (1 << 27) Erase parameter error status bit */
#define aSDMMC_R1WriteProtectViolation ((uint32_t)0x4000000U)
	aSDMMC_R1WriteProtectViolationFlag = aSDMMC_R1WriteProtectViolation,      /*!< (1 << 26) Write protection violation status bit */
#define aSDMMC_R1CardIsLocked ((uint32_t)0x2000000U)
	aSDMMC_R1CardIsLockedFlag = aSDMMC_R1CardIsLocked,               /*!< (1 << 25) Card locked status bit */
#define aSDMMC_R1LockUnlockFailed ((uint32_t)0x1000000U)
	aSDMMC_R1LockUnlockFailedFlag = aSDMMC_R1LockUnlockFailed,           /*!< (1 << 24) lock/unlock error status bit */
#define aSDMMC_R1CommandCrcError ((uint32_t)0x800000U)
	aSDMMC_R1CommandCrcErrorFlag = aSDMMC_R1CommandCrcError,            /*!< (1 << 23) CRC error status bit */
#define aSDMMC_R1IllegalCommand ((uint32_t)0x400000U)
	aSDMMC_R1IllegalCommandFlag = aSDMMC_R1IllegalCommand,             /*!< (1 << 22) Illegal command status bit */
#define aSDMMC_R1CardEccFailed ((uint32_t)0x200000U)
	aSDMMC_R1CardEccFailedFlag = aSDMMC_R1CardEccFailed,              /*!< (1 << 21) Card ecc error status bit */
#define aSDMMC_R1CardControllerError ((uint32_t)0x100000U)
	aSDMMC_R1CardControllerErrorFlag = aSDMMC_R1CardControllerError,        /*!< (1 << 20) Internal card controller error status bit */
#define aSDMMC_R1Error ((uint32_t)0x80000U)
	aSDMMC_R1ErrorFlag = aSDMMC_R1Error,                      /*!< (1 << 19) A general or an unknown error status bit */
#define aSDMMC_R1CidCsdOverwrite ((uint32_t)0x10000U)
	aSDMMC_R1CidCsdOverwriteFlag = aSDMMC_R1CidCsdOverwrite,            /*!< (1 << 16) Cid/csd overwrite status bit */
    aSDMMC_R1WriteProtectEraseSkipFlag = (uint32_t)0x8000U,      /*!< (1 << 15) Write protection erase skip status bit */
    aSDMMC_R1CardEccDisabledFlag = (uint32_t)0x4000U,            /*!< (1 << 14) Card ecc disabled status bit */
    aSDMMC_R1EraseResetFlag = (uint32_t)0x2000U,                 /*!< (1 << 13) Erase reset status bit */
    aSDMMC_R1ReadyForDataFlag = (uint32_t)0x100U,                /*!< (1 << 8) Ready for data status bit */
	aSDMMC_R1SwitchErrorFlag = (uint32_t)0x80U,                 /*!< (1 << 7) Switch error status bit */
    aSDMMC_R1ApplicationCommandFlag = (uint32_t)0x20U,          /*!< (1 << 5) Application command enabled status bit */
#define aSDMMC_R1AuthenticationSequenceError ((uint32_t)0x8U)
	aSDMMC_R1AuthenticationSequenceErrorFlag = aSDMMC_R1AuthenticationSequenceError, /*!< (1 << 3) error in the sequence of authentication process */

#define aSDMMC_R1ErrorAll_Set (aSDMMC_R1OutOfRange | aSDMMC_R1AddressError | aSDMMC_R1BlockLengthError | \
    aSDMMC_R1EraseSequenceError | aSDMMC_R1EraseParameterError | aSDMMC_R1WriteProtectViolation | \
    aSDMMC_R1CardIsLocked | aSDMMC_R1LockUnlockFailed | aSDMMC_R1CommandCrcError | \
    aSDMMC_R1IllegalCommand | aSDMMC_R1CardEccFailed | aSDMMC_R1CardControllerError | \
    aSDMMC_R1Error | aSDMMC_R1CidCsdOverwrite | \
    aSDMMC_R1AuthenticationSequenceError)
#ifdef __ghs__
    aSDMMC_R1ErrorAllFlag =
        (aSDMMC_R1OutOfRangeFlag | aSDMMC_R1AddressErrorFlag | aSDMMC_R1BlockLengthErrorFlag |
         aSDMMC_R1EraseSequenceErrorFlag | aSDMMC_R1EraseParameterErrorFlag | aSDMMC_R1WriteProtectViolationFlag |
         aSDMMC_R1CardIsLockedFlag | aSDMMC_R1LockUnlockFailedFlag | aSDMMC_R1CommandCrcErrorFlag |
         aSDMMC_R1IllegalCommandFlag | aSDMMC_R1CardEccFailedFlag | aSDMMC_R1CardControllerErrorFlag |
         aSDMMC_R1ErrorFlag | aSDMMC_R1CidCsdOverwriteFlag |
         aSDMMC_R1AuthenticationSequenceErrorFlag), /*!< Card error status */
#else
	aSDMMC_R1ErrorAllFlag = aSDMMC_R1ErrorAll_Set,
#endif
} sdmmc_r1_card_status_flag_t;

/*! @brief R1: current state */
#define SDMMC_R1_CURRENT_STATE(x) (((x)&0x00001E00U) >> 9U)

/*! @brief CURRENT_STATE filed in R1 */
typedef enum _sdmmc_r1_current_state
{
    aSDMMC_R1StateIdle = 0U,        /*!< R1: current state: idle */
    aSDMMC_R1StateReady = 1U,       /*!< R1: current state: ready */
    aSDMMC_R1StateIdentify = 2U,    /*!< R1: current state: identification */
    aSDMMC_R1StateStandby = 3U,     /*!< R1: current state: standby */
    aSDMMC_R1StateTransfer = 4U,    /*!< R1: current state: transfer */
    aSDMMC_R1StateSendData = 5U,    /*!< R1: current state: sending data */
    aSDMMC_R1StateReceiveData = 6U, /*!< R1: current state: receiving data */
    aSDMMC_R1StateProgram = 7U,     /*!< R1: current state: programming */
    aSDMMC_R1StateDisconnect = 8U,  /*!< R1: current state: disconnect */
} sdmmc_r1_current_state_t;

/*! @brief Error bit in SPI mode R1 */
typedef enum _sdspi_r1_error_status_flag
{
    aSDSPI_R1InIdleStateFlag = (1U << 0U),        /*!< In idle state */
    aSDSPI_R1EraseResetFlag = (1U << 1U),         /*!< Erase reset */
    aSDSPI_R1IllegalCommandFlag = (1U << 2U),     /*!< Illegal command */
    aSDSPI_R1CommandCrcErrorFlag = (1U << 3U),    /*!< Com crc error */
    aSDSPI_R1EraseSequenceErrorFlag = (1U << 4U), /*!< Erase sequence error */
    aSDSPI_R1AddressErrorFlag = (1U << 5U),       /*!< Address error */
    aSDSPI_R1ParameterErrorFlag = (1U << 6U),     /*!< Parameter error */
} sdspi_r1_error_status_flag_t;

/*! @brief Error bit in SPI mode R2 */
typedef enum _sdspi_r2_error_status_flag
{
    aSDSPI_R2CardLockedFlag = (1U << 0U),            /*!< Card is locked */
    aSDSPI_R2WriteProtectEraseSkip = (1U << 1U),     /*!< Write protect erase skip */
    aSDSPI_R2LockUnlockFailed = (1U << 1U),          /*!< Lock/unlock command failed */
    aSDSPI_R2ErrorFlag = (1U << 2U),                 /*!< Unknown error */
    aSDSPI_R2CardControllerErrorFlag = (1U << 3U),   /*!< Card controller error */
    aSDSPI_R2CardEccFailedFlag = (1U << 4U),         /*!< Card ecc failed */
    aSDSPI_R2WriteProtectViolationFlag = (1U << 5U), /*!< Write protect violation */
    aSDSPI_R2EraseParameterErrorFlag = (1U << 6U),   /*!< Erase parameter error */
    aSDSPI_R2OutOfRangeFlag = (1U << 7U),            /*!< Out of range */
    aSDSPI_R2CsdOverwriteFlag = (1U << 7U),          /*!< CSD overwrite */
} sdspi_r2_error_status_flag_t;

/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_SHIFT (28U)
/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_MASK (0xFU)
/*! @brief The bit shift for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_SHIFT (8U)
/*! @brief The bit mask for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_MASK (0xFU)
/*! @brief The bit mask for VOLTAGE 2.7V to 3.6V field in R7 */
#define SDSPI_R7_VOLTAGE_27_36_MASK (0x1U << SDSPI_R7_VOLTAGE_SHIFT)
/*! @brief The bit shift for ECHO field in R7 */
#define SDSPI_R7_ECHO_SHIFT (0U)
/*! @brief The bit mask for ECHO field in R7 */
#define SDSPI_R7_ECHO_MASK (0xFFU)

/*! @brief Data error token mask */
#define SDSPI_DATA_ERROR_TOKEN_MASK (0xFU)
/*! @brief Data Error Token mask bit */
typedef enum _sdspi_data_error_token
{
    aSDSPI_DataErrorTokenError = (1U << 0U),               /*!< Data error */
    aSDSPI_DataErrorTokenCardControllerError = (1U << 1U), /*!< Card controller error */
    aSDSPI_DataErrorTokenCardEccFailed = (1U << 2U),       /*!< Card ecc error */
    aSDSPI_DataErrorTokenOutOfRange = (1U << 3U),          /*!< Out of range */
} sdspi_data_error_token_t;

/*! @brief Data Token */
typedef enum _sdspi_data_token
{
    aSDSPI_DataTokenBlockRead = 0xFEU,          /*!< Single block read, multiple block read */
    aSDSPI_DataTokenSingleBlockWrite = 0xFEU,   /*!< Single block write */
    aSDSPI_DataTokenMultipleBlockWrite = 0xFCU, /*!< Multiple block write */
    aSDSPI_DataTokenStopTransfer = 0xFDU,       /*!< Stop transmission */
} sdspi_data_token_t;

/* Data Response Token mask */
#define SDSPI_DATA_RESPONSE_TOKEN_MASK (0x1FU) /*!< Mask for data response bits */
/*! @brief Data Response Token */
typedef enum _sdspi_data_response_token
{
    aSDSPI_DataResponseTokenAccepted = 0x05U,   /*!< Data accepted */
    aSDSPI_DataResponseTokenCrcError = 0x0BU,   /*!< Data rejected due to CRC error */
    aSDSPI_DataResponseTokenWriteError = 0x0DU, /*!< Data rejected due to write error */
} sdspi_data_response_token_t;

/*! @brief SD card individual commands */
typedef enum _sd_command
{
    aSD_SendRelativeAddress = 3U,    /*!< Send Relative Address */
    aSD_Switch = 6U,                 /*!< Switch Function */
    aSD_SendInterfaceCondition = 8U, /*!< Send Interface Condition */
    aSD_VoltageSwitch = 11U,         /*!< Voltage Switch */
    aSD_SpeedClassControl = 20U,     /*!< Speed Class control */
    aSD_EraseWriteBlockStart = 32U,  /*!< Write Block Start */
    aSD_EraseWriteBlockEnd = 33U,    /*!< Write Block End */
} sd_command_t;

/*! @brief SD card individual application commands */
typedef enum _sd_application_command
{
    aSD_ApplicationSetBusWidth = 6U,              /*!< Set Bus Width */
    aSD_ApplicationStatus = 13U,                  /*!< Send SD status */
    aSD_ApplicationSendNumberWriteBlocks = 22U,   /*!< Send Number Of Written Blocks */
    aSD_ApplicationSetWriteBlockEraseCount = 23U, /*!< Set Write Block Erase Count */
    aSD_ApplicationSendOperationCondition = 41U,  /*!< Send Operation Condition */
    aSD_ApplicationSetClearCardDetect = 42U,      /*!< Set Connnect/Disconnect pull up on detect pin */
    aSD_ApplicationSendScr = 51U,                 /*!< Send Scr */
} sd_application_command_t;

/*! @brief SD/MMC card common commands */
typedef enum _sdmmc_command
{
    aSDMMC_GoIdleState = 0U,         /*!< Go Idle State */
    aSDMMC_AllSendCid = 2U,          /*!< All Send CID */
    aSDMMC_SetDsr = 4U,              /*!< Set DSR */
    aSDMMC_SelectCard = 7U,          /*!< Select Card */
    aSDMMC_SendCsd = 9U,             /*!< Send CSD */
    aSDMMC_SendCid = 10U,            /*!< Send CID */
    aSDMMC_StopTransmission = 12U,   /*!< Stop Transmission */
    aSDMMC_SendStatus = 13U,         /*!< Send Status */
    aSDMMC_GoInactiveState = 15U,    /*!< Go Inactive State */
    aSDMMC_SetBlockLength = 16U,     /*!< Set Block Length */
    aSDMMC_ReadSingleBlock = 17U,    /*!< Read Single Block */
    aSDMMC_ReadMultipleBlock = 18U,  /*!< Read Multiple Block */
    aSDMMC_SendTuningBlock = 19U,    /*!< Send Tuning Block */
    aSDMMC_SetBlockCount = 23U,      /*!< Set Block Count */
    aSDMMC_WriteSingleBlock = 24U,   /*!< Write Single Block */
    aSDMMC_WriteMultipleBlock = 25U, /*!< Write Multiple Block */
    aSDMMC_ProgramCsd = 27U,         /*!< Program CSD */
    aSDMMC_SetWriteProtect = 28U,    /*!< Set Write Protect */
    aSDMMC_ClearWriteProtect = 29U,  /*!< Clear Write Protect */
    aSDMMC_SendWriteProtect = 30U,   /*!< Send Write Protect */
    aSDMMC_Erase = 38U,              /*!< Erase */
    aSDMMC_LockUnlock = 42U,         /*!< Lock Unlock */
    aSDMMC_ApplicationCommand = 55U, /*!< Send Application Command */
    aSDMMC_GeneralCommand = 56U,     /*!< General Purpose Command */
    aSDMMC_ReadOcr = 58U,            /*!< Read OCR */
} sdmmc_command_t;

/*! @brief MMC card individual commands */
typedef enum _mmc_command
{
    aMMC_SendOperationCondition = 1U, /*!< Send Operation Condition */
    aMMC_SetRelativeAddress = 3U,     /*!< Set Relative Address */
    aMMC_SleepAwake = 5U,             /*!< Sleep Awake */
    aMMC_Switch = 6U,                 /*!< Switch */
    aMMC_SendExtendedCsd = 8U,        /*!< Send EXT_CSD */
    aMMC_ReadDataUntilStop = 11U,     /*!< Read Data Until Stop */
    aMMC_BusTestRead = 14U,           /*!< Test Read */
    aMMC_WriteDataUntilStop = 20U,    /*!< Write Data Until Stop */
    aMMC_ProgramCid = 26U,            /*!< Program CID */
    aMMC_EraseGroupStart = 35U,       /*!< Erase Group Start */
    aMMC_EraseGroupEnd = 36U,         /*!< Erase Group End */
    aMMC_FastInputOutput = 39U,       /*!< Fast IO */
    aMMC_GoInterruptState = 40U,      /*!< Go interrupt State */
} mmc_command_t;

/*! @brief SD card command class */
typedef enum _sdmmc_command_class
{
    aSDMMC_CommandClassBasic = 0x1U,               /*!< (1U << 0U) Card command class 0 */
    aSDMMC_CommandClassBlockRead = 0x4U,           /*!< (1U << 2U) Card command class 2 */
    aSDMMC_CommandClassBlockWrite = 0x10U,          /*!< (1U << 4U) Card command class 4 */
    aSDMMC_CommandClassErase = 0x20U,               /*!< (1U << 5U) Card command class 5 */
    aSDMMC_CommandClassWriteProtect = 0x40U,        /*!< (1U << 6U) Card command class 6 */
    aSDMMC_CommandClassLockCard = 0x80U,            /*!< (1U << 7U) Card command class 7 */
    aSDMMC_CommandClassApplicationSpecific = 0x100U, /*!< (1U << 8U) Card command class 8 */
    aSDMMC_CommandClassInputOutputMode = 0x200U,     /*!< (1U << 9U) Card command class 9 */
    aSDMMC_CommandClassSwitch = 0x400U,             /*!< (1U << 10U) Card command class 10 */
} sdmmc_command_class_t;

/*! @brief OCR register in SD card */
typedef enum _sd_ocr_flag
{
#ifdef __ghs__
    aSD_OcrPowerUpBusyFlag = (1 << 31),                              /*!< Power up busy status */
#else
    aSD_OcrPowerUpBusyFlag = 0x80000000U,                              /*!< (int)(1U << 31U)Power up busy status */
#endif
    aSD_OcrHostCapacitySupportFlag = 0x40000000U,                    /*!< (1U << 30U) Card capacity status */
    aSD_OcrCardCapacitySupportFlag = aSD_OcrHostCapacitySupportFlag, /*!< Card capacity status */
    aSD_OcrSwitch18RequestFlag = 0x1000000U,                        /*!< (1U << 24U) Switch to 1.8V request */
    aSD_OcrSwitch18AcceptFlag = aSD_OcrSwitch18RequestFlag,          /*!< Switch to 1.8V accepted */
    aSD_OcrSwitchSDXCPowerControlFlag = 0x10000000U,                 /*!< (1U << 28U) SDXC Power Control */
    aSD_OcrVdd27_28Flag = 0x8000U,                               /*!< (1U << 15U) VDD 2.7-2.8 */
    aSD_OcrVdd28_29Flag = 0x10000U,                               /*!< (1U << 16U) VDD 2.8-2.9 */
    aSD_OcrVdd29_30Flag = 0x20000U,                               /*!< (1U << 17U) VDD 2.9-3.0 */
    aSD_OcrVdd30_31Flag = 0x40000U,                               /*!< (1U << 18U) VDD 2.9-3.0 */
    aSD_OcrVdd31_32Flag = 0x80000U,                               /*!< (1U << 19U) VDD 3.0-3.1 */
    aSD_OcrVdd32_33Flag = 0x100000U,                               /*!< (1U << 20U) VDD 3.1-3.2 */
    aSD_OcrVdd33_34Flag = 0x200000U,                               /*!< (1U << 21U) VDD 3.2-3.3 */
    aSD_OcrVdd34_35Flag = 0x400000U,                               /*!< (1U << 22U)VDD 3.3-3.4 */
    aSD_OcrVdd35_36Flag = 0x800000U,                               /*!< (1U << 23U) VDD 3.4-3.5 */
} sd_ocr_flag_t;

/*! @brief SD card specification version number */
typedef enum _sd_specification_version
{
    aSD_SpecificationVersion1_0 = 0x1U, /*!< (1U << 0U) SD card version 1.0-1.01 */
    aSD_SpecificationVersion1_1 = 0x2U, /*!< (1U << 1U) SD card version 1.10 */
    aSD_SpecificationVersion2_0 = 0x4U, /*!< (1U << 2U) SD card version 2.00 */
    aSD_SpecificationVersion3_0 = 0x8U, /*!< (1U << 3U) SD card version 3.0 */
} sd_specification_version_t;

/*! @brief SD card bus width */
typedef enum _sd_data_bus_width
{
    aSD_DataBusWidth1Bit = 0x1U, /*!< (1U << 0U) SD data bus width 1-bit mode */
    aSD_DataBusWidth4Bit = 0x4U, /*!< (1U << 2U) SD data bus width 4-bit mode */
} sd_data_bus_width_t;

/*! @brief SD card switch mode */
typedef enum _sd_switch_mode
{
    aSD_SwitchCheck = 0U, /*!< SD switch mode 0: check function */
    aSD_SwitchSet = 1U,   /*!< SD switch mode 1: set function */
} sd_switch_mode_t;

/*! @brief SD timing function number */
typedef enum _sd_timing_function
{
    aSD_FunctionSDR12Default = 0U,   /*!< SDR12 mode & default */
    aSD_FunctionSDR25HighSpeed = 1U, /*!< SDR25 & high speed */
    aSD_FunctionSDR50 = 2U,          /*!< SDR50 mode */
    aSD_FunctionSDR104 = 3U,         /*!< SDR104 mode */
    aSD_FunctionDDR50 = 4U,          /*!< DDR50 mode */
} sd_timing_function;

/*! @brief SD group number */
typedef enum _sd_group_num
{
    aSD_GroupTimingMode = 0U,     /*!< access mode group */
    aSD_GroupCommandSystem = 1U,  /*!< command system group */
    aSD_GroupDriverStrength = 2U, /*!< driver strength group */
    aSD_GroupCurrentLimit = 3U,   /*!< current limit group */
} sd_group_num;

/*! @brief SD card driver strength */
typedef enum _sd_driver_strength
{
    aSD_DriverStrengthTypeB = 0U, /*!< default driver strength*/
    aSD_DriverStrengthTypeA = 1U, /*!< driver strength TYPE A */
    aSD_DriverStrengthTypeC = 2U, /*!< driver strength TYPE C */
    aSD_DriverStrengthTypeD = 3U, /*!< driver strength TYPE D */
} sd_driver_strength_t;

/*! @brief SD card current limit */
typedef enum _sd_max_current
{
    aSD_CurrentLimit200MA = 0U, /*!< default current limit */
    aSD_CurrentLimit400MA = 1U, /*!< current limit to 400MA */
    aSD_CurrentLimit600MA = 2U, /*!< current limit to 600MA */
    aSD_CurrentLimit800MA = 3U, /*!< current limit to 800MA */
} sd_max_current_t;

/*! @brief SD card timing mode flags */
typedef enum _sd_timing_mode
{
    aSD_TimingSDR12DefaultMode = 0U,   /*!< Identification mode & SDR12 */
    aSD_TimingSDR25HighSpeedMode = 1U, /*!< High speed mode & SDR25 */
    aSD_TimingSDR50Mode = 2U,          /*!< SDR50 mode */
    aSD_TimingSDR104Mode = 3U,         /*!< SDR104 mode */
    aSD_TimingDDR50Mode = 4U,          /*!< DDR50 mode */
} sd_timing_mode_t;

/*! @brief SD card CSD register flags */
typedef enum _sd_csd_flag
{
    aSD_CsdReadBlockPartialFlag = 0x1U,         /*!< (1U << 0U) Partial blocks for read allowed [79:79] */
    aSD_CsdWriteBlockMisalignFlag = 0x2U,       /*!< (1U << 1U) Write block misalignment [78:78] */
    aSD_CsdReadBlockMisalignFlag = 0x4U,        /*!< (1U << 2U) Read block misalignment [77:77] */
    aSD_CsdDsrImplementedFlag = 0x8U,           /*!< (1U << 3U) DSR implemented [76:76] */
    aSD_CsdEraseBlockEnabledFlag = 0x10U,        /*!< (1U << 4U) Erase single block enabled [46:46] */
    aSD_CsdWriteProtectGroupEnabledFlag = 0x20U, /*!< (1U << 5U) Write protect group enabled [31:31] */
    aSD_CsdWriteBlockPartialFlag = 0x40U,        /*!< (1U << 6U) Partial blocks for write allowed [21:21] */
    aSD_CsdFileFormatGroupFlag = 0x80U,          /*!< (1U << 7U) File format group [15:15] */
    aSD_CsdCopyFlag = 0x100U,                     /*!< (1U << 8U) Copy flag [14:14] */
    aSD_CsdPermanentWriteProtectFlag = 0x200U,    /*!< (1U << 9U) Permanent write protection [13:13] */
    aSD_CsdTemporaryWriteProtectFlag = 0x400U,   /*!< (1U << 10U) Temporary write protection [12:12] */
} sd_csd_flag_t;

/*! @brief SD card SCR register flags */
typedef enum _sd_scr_flag
{
    aSD_ScrDataStatusAfterErase = 0x1U, /*!< (1U << 0U) Data status after erases [55:55] */
    aSD_ScrSdSpecification3 = 0x2U,     /*!< (1U << 1U) Specification version 3.00 or higher [47:47]*/
} sd_scr_flag_t;

/*! @brief MMC card classified as voltage range */
typedef enum _mmc_classified_voltage
{
    aMMC_ClassifiedVoltageHigh = 0U, /*!< High voltage MMC card */
    aMMC_ClassifiedVoltageDual = 1U, /*!< Dual voltage MMC card */
} mmc_classified_voltage_t;

/*! @brief MMC card classified as density level */
typedef enum _mmc_classified_density
{
    aMMC_ClassifiedDensityWithin2GB = 0U, /*!< Density byte is less than or equal 2GB */
    aMMC_ClassifiedDensityHigher2GB = 1U, /* Density byte is higher than 2GB */
} mmc_classified_density_t;

/*! @brief The bit mask for VOLTAGE WINDOW 1.70V to 1.95V field in OCR */
#define MMC_OCR_V170TO195_SHIFT (7U)
/*! @brief The bit mask for VOLTAGE WINDOW 1.70V to 1.95V field in OCR */
#define MMC_OCR_V170TO195_MASK (0x00000080U)
/*! @brief The bit shift for VOLTAGE WINDOW 2.00V to 2.60V field in OCR */
#define MMC_OCR_V200TO260_SHIFT (8U)
/*! @brief The bit mask for VOLTAGE WINDOW 2.00V to 2.60V field in OCR */
#define MMC_OCR_V200TO260_MASK (0x00007F00U)
/*! @brief The bit shift for VOLTAGE WINDOW 2.70V to 3.60V field in OCR */
#define MMC_OCR_V270TO360_SHIFT (15U)
/*! @brief The bit mask for VOLTAGE WINDOW 2.70V to 3.60V field in OCR */
#define MMC_OCR_V270TO360_MASK (0x00FF8000U)
/*! @brief The bit shift for ACCESS MODE field in OCR */
#define MMC_OCR_ACCESS_MODE_SHIFT (29U)
/*! @brief The bit mask for ACCESS MODE field in OCR */
#define MMC_OCR_ACCESS_MODE_MASK (0x60000000U)
/*! @brief The bit shift for BUSY field in OCR */
#define MMC_OCR_BUSY_SHIFT (31U)
/*! @brief The bit mask for BUSY field in OCR */
#define MMC_OCR_BUSY_MASK (1U << MMC_OCR_BUSY_SHIFT)

/*! @brief MMC card access mode(Access mode in OCR). */
typedef enum _mmc_access_mode
{
    aMMC_AccessModeByte = 0U,   /*!< The card should be accessed as byte */
    aMMC_AccessModeSector = 2U, /*!< The card should be accessed as sector */
} mmc_access_mode_t;

/*! @brief MMC card voltage window(VDD voltage window in OCR). */
typedef enum _mmc_voltage_window
{
    aMMC_VoltageWindow170to195 = 0x01U,   /*!< Voltage window is 1.70V to 1.95V */
    aMMC_VoltageWindows270to360 = 0x1FFU, /*!< Voltage window is 2.70V to 3.60V */
} mmc_voltage_window_t;

/*! @brief CSD structure version(CSD_STRUCTURE in CSD). */
typedef enum _mmc_csd_structure_version
{
    aMMC_CsdStrucureVersion10 = 0U,       /*!< CSD version No. 1.0 */
    aMMC_CsdStrucureVersion11 = 1U,       /*!< CSD version No. 1.1 */
    aMMC_CsdStrucureVersion12 = 2U,       /*!< CSD version No. 1.2 */
    aMMC_CsdStrucureVersionInExtcsd = 3U, /*!< Version coded in Extended CSD */
} mmc_csd_structure_version_t;

/*! @brief MMC card specification version(SPEC_VERS in CSD). */
typedef enum _mmc_specification_version
{
    aMMC_SpecificationVersion0 = 0U, /*!< Allocated by MMCA */
    aMMC_SpecificationVersion1 = 1U, /*!< Allocated by MMCA */
    aMMC_SpecificationVersion2 = 2U, /*!< Allocated by MMCA */
    aMMC_SpecificationVersion3 = 3U, /*!< Allocated by MMCA */
    aMMC_SpecificationVersion4 = 4U, /*!< Version 4.1/4.2/4.3 */
} mmc_specification_version_t;

/*! @brief The bit shift for FREQUENCY UNIT field in TRANSFER SPEED(TRAN-SPEED in Extended CSD) */
#define MMC_TRANSFER_FREQUENCY_UNIT_SHIFT (0U)
/*! @brief The bit mask for FRQEUENCY UNIT in TRANSFER SPEED */
#define MMC_TRANSFER__FREQUENCY_UNIT_MASK (0x07U)
/*! @brief The bit shift for MULTIPLIER field in TRANSFER SPEED */
#define MMC_TRANSFER_MULTIPLIER_SHIFT (3U)
/*! @brief The bit mask for MULTIPLIER field in TRANSFER SPEED  */
#define MMC_TRANSFER_MULTIPLIER_MASK (0x78U)

/*! @brief Read the value of FREQUENCY UNIT in TRANSFER SPEED. */
#define READ_MMC_TRANSFER_SPEED_FREQUENCY_UNIT(CSD) \
    (((CSD.transferSpeed) & MMC_TRANSFER__FREQUENCY_UNIT_MASK) >> MMC_TRANSFER_FREQUENCY_UNIT_SHIFT)
/*! @brief Read the value of MULTIPLER filed in TRANSFER SPEED. */
#define READ_MMC_TRANSFER_SPEED_MULTIPLIER(CSD) \
    (((CSD.transferSpeed) & MMC_TRANSFER_MULTIPLIER_MASK) >> MMC_TRANSFER_MULTIPLIER_SHIFT)

/*! @brief MMC card Extended CSD fix version(EXT_CSD_REV in Extended CSD) */
typedef enum _mmc_extended_csd_revision
{
    aMMC_ExtendedCsdRevision10 = 0U, /*!< Revision 1.0 */
    aMMC_ExtendedCsdRevision11 = 1U, /*!< Revision 1.1 */
    aMMC_ExtendedCsdRevision12 = 2U, /*!< Revision 1.2 */
    aMMC_ExtendedCsdRevision13 = 3U, /*!< Revision 1.3 */
} mmc_extended_csd_revision_t;

/*! @brief MMC card command set(COMMAND_SET in Extended CSD) */
typedef enum _mmc_command_set
{
    aMMC_CommandSetStandard = 0U, /*!< Standard MMC */
    aMMC_CommandSet1 = 1U,        /*!< Command set 1 */
    aMMC_CommandSet2 = 2U,        /*!< Command set 2 */
    aMMC_CommandSet3 = 3U,        /*!< Command set 3 */
    aMMC_CommandSet4 = 4U,        /*!< Command set 4 */
} mmc_command_set_t;

/*! @brief Alternate boot support(BOOT_INFO in Extended CSD) */
#define MMC_ALTERNATE_BOOT_SUPPORT_MASK (0x01)

/*! @brief The power class value bit mask when bus in 4 bit mode */
#define MMC_POWER_CLASS_4BIT_MASK (0x0FU)
/*! @brief The power class current value bit mask when bus in 8 bit mode */
#define MMC_POWER_CLASS_8BIT_MASK (0xF0U)

/*! @brief MMC card high speed timing(HS_TIMING in Extended CSD) */
typedef enum _mmc_high_speed_timing
{
    aMMC_HighSpeedTimingNone = 0U, /*!< MMC card using none high speed timing */
    aMMC_HighSpeedTiming = 1U,     /*!< MMC card using high speed timing */
} mmc_high_speed_timing_t;

/*! @brief MMC card high speed frequency type(CARD_TYPE in Extended CSD) */
typedef enum _mmc_high_speed_frequency
{
    aMMC_HighSpeedFrequency26MHZ = 0x1U, /*!< The bit mask for high speed frequency at 26MHZ flag */
    aMMC_HighSpeedFrequency52MHZ = 0x2U, /*!< The bit mask for high speed frequency at 52MHZ flag */
} mmc_high_speed_frequency_t;

/*! @brief The number of data bus width type */
#define MMC_DATA_BUS_WIDTH_TYPE_NUMBER (3U)
/*! @brief MMC card data bus width(BUS_WIDTH in Extended CSD) */
typedef enum _mmc_data_bus_width
{
    aMMC_DataBusWidth1bit = 0U, /*!< MMC data bus width is 1 bit */
    aMMC_DataBusWidth4bit = 1U, /*!< MMC data bus width is 4 bits */
    aMMC_DataBusWidth8bit = 2U, /*!< MMC data bus width is 8 bits */
} mmc_data_bus_width_t;

/*! @brief MMC card boot partition enabled(BOOT_PARTITION_ENABLE in Extended CSD) */
typedef enum _mmc_boot_partition_enable
{
    aMMC_BootEnableNot = 0U,        /*!< Device not boot enabled (default) */
    aMMC_BootEnablePartition1 = 1U, /*!< Boot partition 1 enabled for boot */
    aMMC_BootEnablePartition2 = 2U, /*!< Boot partition 2 enabled for boot */
    aMMC_BootEnableUserAera = 7U,   /*!< User area enabled for boot */
} mmc_boot_partition_enable_t;

/*! @brief MMC card partition to be accessed(BOOT_PARTITION_ACCESS in Extended CSD) */
typedef enum _mmc_access_partition
{
    aMMC_AccessPartitionUserAera = 0U, /*!< No access to boot partition (default), normal partition */
    aMMC_AccessPartitionBoot1 = 1U,    /*!< Read/Write boot partition 1 */
    aMMC_AccessPartitionBoot2 = 2U,    /*!< Read/Write boot partition 2*/
} mmc_access_partition_t;

/*! @brief The bit shift for PARTITION ACCESS filed in BOOT CONFIG (BOOT_CONFIG in Extend CSD) */
#define MMC_BOOT_CONFIG_ACCESS_SHIFT (0U)
/*! @brief The bit mask for PARTITION ACCESS field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ACCESS_MASK (0x00000007U)
/*! @brief The bit shift for PARTITION ENABLE field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ENABLE_SHIFT (3U)
/*! @brief The bit mask for PARTITION ENABLE field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ENABLE_MASK (0x00000038U)
/*! @brief The bit shift for ACK field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ACK_SHIFT (6U)
/*! @brief The bit mask for ACK field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ACK_MASK (0x00000040U)
/*! @brief The bit shift for BOOT BUS WIDTH field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_WIDTH_SHIFT (8U)
/*! @brief The bit mask for BOOT BUS WIDTH field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_WIDTH_MASK (0x00000300U)
/*! @brief The bit shift for BOOT BUS WIDTH RESET field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_RESET_SHIFT (10U)
/*! @brief The bit mask for BOOT BUS WIDTH RESET field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_RESET_MASK (0x00000400U)

/*! @brief MMC card CSD register flags */
typedef enum _mmc_csd_flag
{
    aMMC_CsdReadBlockPartialFlag = 0x1U,         /*!< (1U << 0U) Partial blocks for read allowed */
    aMMC_CsdWriteBlockMisalignFlag = 0x02U,       /*!< (1U << 1U) Write block misalignment */
    aMMC_CsdReadBlockMisalignFlag = 0x4U,        /*!< (1U << 2U) Read block misalignment */
    aMMC_CsdDsrImplementedFlag = 0x8U,           /*!< (1U << 3U) DSR implemented */
    aMMC_CsdWriteProtectGroupEnabledFlag = 0x10U, /*!< (1U << 4U) Write protect group enabled */
    aMMC_CsdWriteBlockPartialFlag = 0x20U,        /*!< (1U << 5U) Partial blocks for write allowed */
    aMMC_ContentProtectApplicationFlag = 0x40U,   /*!< (1U << 6U) Content protect application */
    aMMC_CsdFileFormatGroupFlag = 0x80U,          /*!< (1U << 7U) File format group */
    aMMC_CsdCopyFlag = 0x100U,                     /*!< (1U << 8U) Copy flag */
    aMMC_CsdPermanentWriteProtectFlag = 0x200U,    /*!< (1U << 9U) Permanent write protection */
    aMMC_CsdTemporaryWriteProtectFlag = 0x400U,   /*!< (1U << 10U) Temporary write protection */
} mmc_csd_flag_t;

/*! @brief Extended CSD register access mode(Access mode in CMD6). */
typedef enum _mmc_extended_csd_access_mode
{
    aMMC_ExtendedCsdAccessModeCommandSet = 0U, /*!< Command set related setting */
    aMMC_ExtendedCsdAccessModeSetBits = 1U,    /*!< Set bits in specific byte in Extended CSD  */
    aMMC_ExtendedCsdAccessModeClearBits = 2U,  /*!< Clear bits in specific byte in Extended CSD */
    aMMC_ExtendedCsdAccessModeWriteBits = 3U,  /*!< Write a value to specific byte in Extended CSD */
} mmc_extended_csd_access_mode_t;

/*! @brief EXT CSD byte index */
typedef enum _mmc_extended_csd_index
{
    aMMC_ExtendedCsdIndexEraseGroupDefinition = 175U, /*!< Erase Group Def */
    aMMC_ExtendedCsdIndexBootBusWidth = 177U,         /*!< Boot Bus Width */
    aMMC_ExtendedCsdIndexBootConfig = 179U,           /*!< Boot Config */
    aMMC_ExtendedCsdIndexBusWidth = 183U,             /*!< Bus Width */
    aMMC_ExtendedCsdIndexHighSpeedTiming = 185U,      /*!< High speed Timing */
    aMMC_ExtendedCsdIndexPowerClass = 187U,           /*!< Power Class */
    aMMC_ExtendedCsdIndexCommandSet = 191U,           /*!< Command Set */
} mmc_extended_csd_index_t;

/*! @brief The length of Extended CSD register, unit as bytes. */
#define MMC_EXTENDED_CSD_BYTES (512U)

/*! @brief MMC card default relative address */
#define MMC_DEFAULT_RELATIVE_ADDRESS (2U)

/*! @brief SD card product name length united as bytes. */
#define SD_PRODUCT_NAME_BYTES (5U)
/*! @brief SD card CID register */
typedef struct _sd_cid
{
    uint8_t manufacturerID;                     /*!< Manufacturer ID [127:120] */
    uint16_t applicationID;                     /*!< OEM/Application ID [119:104] */
    uint8_t productName[SD_PRODUCT_NAME_BYTES]; /*!< Product name [103:64] */
    uint8_t productVersion;                     /*!< Product revision [63:56] */
    uint32_t productSerialNumber;               /*!< Product serial number [55:24] */
    uint16_t manufacturerData;                  /*!< Manufacturing date [19:8] */
} sd_cid_t;

/*! @brief SD card CSD register */
typedef struct _sd_csd
{
    uint8_t csdStructure;        /*!< CSD structure [127:126] */
    uint8_t dataReadAccessTime1; /*!< Data read access-time-1 [119:112] */
    uint8_t dataReadAccessTime2; /*!< Data read access-time-2 in clock cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;       /*!< Maximum data transfer rate [103:96] */
    uint16_t cardCommandClass;   /*!< Card command classes [95:84] */
    uint8_t readBlockLength;     /*!< Maximum read data block length [83:80] */
    uint16_t flags;              /*!< Flags in sd_csd_flag_t */
    uint32_t deviceSize;         /*!< Device size [73:62] */
    /* Following fields from 'readCurrentVddMin' to 'deviceSizeMultiplier' exist in CSD version 1 */
    uint8_t readCurrentVddMin;    /*!< Maximum read current at VDD min [61:59] */
    uint8_t readCurrentVddMax;    /*!< Maximum read current at VDD max [58:56] */
    uint8_t writeCurrentVddMin;   /*!< Maximum write current at VDD min [55:53] */
    uint8_t writeCurrentVddMax;   /*!< Maximum write current at VDD max [52:50] */
    uint8_t deviceSizeMultiplier; /*!< Device size multiplier [49:47] */

    uint8_t eraseSectorSize;       /*!< Erase sector size [45:39] */
    uint8_t writeProtectGroupSize; /*!< Write protect group size [38:32] */
    uint8_t writeSpeedFactor;      /*!< Write speed factor [28:26] */
    uint8_t writeBlockLength;      /*!< Maximum write data block length [25:22] */
    uint8_t fileFormat;            /*!< File format [11:10] */
} sd_csd_t;

/*! @brief The bit shift for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_SHIFT (0U)
/*! @brief The bit mask for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_MASK (0x07U)
/*! @brief The bit shift for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_SHIFT (2U)
/*! @brief The bit mask for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_MASK (0x78U)
/*! @brief Read the value of FREQUENCY UNIT in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_RATE_UNIT(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_RATE_UNIT_MASK) >> SD_TRANSFER_SPEED_RATE_UNIT_SHIFT)
/*! @brief Read the value of TIME VALUE in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_TIME_VALUE(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_TIME_VALUE_MASK) >> SD_TRANSFER_SPEED_TIME_VALUE_SHIFT)

/*! @brief SD card SCR register */
typedef struct _sd_scr
{
    uint8_t scrStructure;             /*!< SCR Structure [63:60] */
    uint8_t sdSpecification;          /*!< SD memory card specification version [59:56] */
    uint16_t flags;                   /*!< SCR flags in sd_scr_flag_t */
    uint8_t sdSecurity;               /*!< Security specification supported [54:52] */
    uint8_t sdBusWidths;              /*!< Data bus widths supported [51:48] */
    uint8_t extendedSecurity;         /*!< Extended security support [46:43] */
    uint8_t commandSupport;           /*!< Command support bits [33:32] */
    uint32_t reservedForManufacturer; /*!< reserved for manufacturer usage [31:0] */
} sd_scr_t;

/*! @brief MMC card product name length united as bytes. */
#define MMC_PRODUCT_NAME_BYTES (6U)
/*! @brief MMC card CID register. */
typedef struct _mmc_cid
{
    uint8_t manufacturerID;                      /*!< Manufacturer ID */
    uint16_t applicationID;                      /*!< OEM/Application ID */
    uint8_t productName[MMC_PRODUCT_NAME_BYTES]; /*!< Product name */
    uint8_t productVersion;                      /*!< Product revision */
    uint32_t productSerialNumber;                /*!< Product serial number */
    uint8_t manufacturerData;                    /*!< Manufacturing date */
} mmc_cid_t;

/*! @brief MMC card CSD register. */
typedef struct _mmc_csd
{
    uint8_t csdStructureVersion;        /*!< CSD structure [127:126] */
    uint8_t systemSpecificationVersion; /*!< System specification version [125:122] */
    uint8_t dataReadAccessTime1;        /*!< Data read access-time 1 [119:112] */
    uint8_t dataReadAccessTime2;        /*!< Data read access-time 2 in CLOCK cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;              /*!< Max. bus clock frequency [103:96] */
    uint16_t cardCommandClass;          /*!< card command classes [95:84] */
    uint8_t readBlockLength;            /*!< Max. read data block length [83:80] */
    uint16_t flags;                     /*!< Contain flags in mmc_csd_flag_t */
    uint16_t deviceSize;                /*!< Device size [73:62] */
    uint8_t readCurrentVddMin;          /*!< Max. read current @ VDD min [61:59] */
    uint8_t readCurrentVddMax;          /*!< Max. read current @ VDD max [58:56] */
    uint8_t writeCurrentVddMin;         /*!< Max. write current @ VDD min [55:53] */
    uint8_t writeCurrentVddMax;         /*!< Max. write current @ VDD max [52:50] */
    uint8_t deviceSizeMultiplier;       /*!< Device size multiplier [49:47] */
    uint8_t eraseGroupSize;             /*!< Erase group size [46:42] */
    uint8_t eraseGroupSizeMultiplier;   /*!< Erase group size multiplier [41:37] */
    uint8_t writeProtectGroupSize;      /*!< Write protect group size [36:32] */
    uint8_t defaultEcc;                 /*!< Manufacturer default ECC [30:29] */
    uint8_t writeSpeedFactor;           /*!< Write speed factor [28:26] */
    uint8_t maxWriteBlockLength;        /*!< Max. write data block length [25:22] */
    uint8_t fileFormat;                 /*!< File format [11:10] */
    uint8_t eccCode;                    /*!< ECC code [9:8] */
} mmc_csd_t;

/*! @brief MMC card Extended CSD register (unit: byte). */
typedef struct _mmc_extended_csd
{
    uint8_t highDensityEraseGroupDefinition;  /*!< High-density erase group definition [175] */
    uint8_t bootDataBusWidth;                 /*!< Boot bus width [177] */
    uint8_t bootConfig;                       /*!< Boot configuration [179] */
    uint8_t eraseMemoryContent;               /*!< Erased memory content [181] */
    uint8_t dataBusWidth;                     /*!< Data bus width mode [183] */
    uint8_t highSpeedTiming;                  /*!< High-speed interface timing [185] */
    uint8_t powerClass;                       /*!< Power class [187] */
    uint8_t commandSetRevision;               /*!< Command set revision [189] */
    uint8_t commandSet;                       /*!< Command set [191] */
    uint8_t extendecCsdVersion;               /*!< Extended CSD revision [192] */
    uint8_t csdStructureVersion;              /*!< CSD structure version [194] */
    uint8_t cardType;                         /*!< Card Type [196] */
    uint8_t powerClass52MHz195V;              /*!< Power Class for 52MHz @ 1.95V [200] */
    uint8_t powerClass26MHz195V;              /*!< Power Class for 26MHz @ 1.95V [201] */
    uint8_t powerClass52MHz360V;              /*!< Power Class for 52MHz @ 3.6V [202] */
    uint8_t powerClass26MHz360V;              /*!< Power Class for 26MHz @ 3.6V [203] */
    uint8_t minimumReadPerformance4Bit26MHz;  /*!< Minimum Read Performance for 4bit at 26MHz [205] */
    uint8_t minimumWritePerformance4Bit26MHz; /*!< Minimum Write Performance for 4bit at 26MHz [206] */
    uint8_t
        minimumReadPerformance8Bit26MHz4Bit52MHz; /*!< Minimum read Performance for 8bit at 26MHz/4bit @52MHz [207] */
    uint8_t
        minimumWritePerformance8Bit26MHz4Bit52MHz; /*!< Minimum Write Performance for 8bit at 26MHz/4bit @52MHz [208] */
    uint8_t minimumReadPerformance8Bit52MHz;       /*!< Minimum Read Performance for 8bit at 52MHz [209] */
    uint8_t minimumWritePerformance8Bit52MHz;      /*!< Minimum Write Performance for 8bit at 52MHz [210] */
    uint32_t sectorCount;                          /*!< Sector Count [215:212] */
    uint8_t sleepAwakeTimeout;                     /*!< Sleep/awake timeout [217] */
    uint8_t sleepCurrentVCCQ;                      /*!< Sleep current (VCCQ) [219] */
    uint8_t sleepCurrentVCC;                       /*!< Sleep current (VCC) [220] */
    uint8_t highCapacityWriteProtectGroupSize;     /*!< High-capacity write protect group size [221] */
    uint8_t reliableWriteSectorCount;              /*!< Reliable write sector count [222] */
    uint8_t highCapacityEraseTimeout;              /*!< High-capacity erase timeout [223] */
    uint8_t highCapacityEraseUnitSize;             /*!< High-capacity erase unit size [224] */
    uint8_t accessSize;                            /*!< Access size [225] */
    uint8_t bootSizeMultiplier;                    /*!< Boot partition size [226] */
    uint8_t bootInformation;                       /*!< Boot information [228] */
    uint8_t supportedCommandSet;                   /*!< Supported Command Sets [504] */
} mmc_extended_csd_t;

/*! @brief The bit shift for COMMAND SET field in SWITCH command. */
#define MMC_SWITCH_COMMAND_SET_SHIFT (0U)
/*! @brief The bit mask for COMMAND set field in SWITCH command. */
#define MMC_SWITCH_COMMAND_SET_MASK (0x00000007U)
/*! @brief The bit shift for VALUE field in SWITCH command */
#define MMC_SWITCH_VALUE_SHIFT (8U)
/*! @brief The bit mask for VALUE field in SWITCH command */
#define MMC_SWITCH_VALUE_MASK (0x0000FF00U)
/*! @brief The bit shift for BYTE INDEX field in SWITCH command */
#define MMC_SWITCH_BYTE_INDEX_SHIFT (16U)
/*! @brief The bit mask for BYTE INDEX field in SWITCH command */
#define MMC_SWITCH_BYTE_INDEX_MASK (0x00FF0000U)
/*! @brief The bit shift for ACCESS MODE field in SWITCH command */
#define MMC_SWITCH_ACCESS_MODE_SHIFT (24U)
/*! @brief The bit mask for ACCESS MODE field in SWITCH command */
#define MMC_SWTICH_ACCESS_MODE_MASK (0x03000000U)

/*! @brief MMC Extended CSD configuration. */
typedef struct _mmc_extended_csd_config
{
    mmc_command_set_t commandSet;              /*!< Command set */
    uint8_t ByteValue;                         /*!< The value to set */
    uint8_t ByteIndex;                         /*!< The byte index in Extended CSD(mmc_extended_csd_index_t) */
    mmc_extended_csd_access_mode_t accessMode; /*!< Access mode */
} mmc_extended_csd_config_t;

/*! @} */
#endif /* SD_SPEC_H */
