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

#ifndef HSM_HW_ACCESS_H
#define HSM_HW_ACCESS_H

#include "hsm_driver.h"
#include "device_registers.h"
#include "interrupt_manager.h"

/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros defines register access masks for HSM registers, not covered by
 * the platform header file.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro defined.
 * Function-like macros are used to extract key bank and key ID from the key
 * index; also used for converting register values to error code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required for passing buffer addresses to HSM firmware; the HSM firmware
 * command parameters are defined as unsigned integers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long.
 * The cast is required for passing buffer addresses to HSM firmware; the HSM firmware
 * command parameters are defined as unsigned integers.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address (base address of the module).
 */

/*! @file hsm_hw_access.h */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Macro used for unused command parameters */
#define HSM_UNUSED_PARAM          0U

/*! @brief Macro used to enable/disable HSM interrupt request */
#define HSM_IRQ_NUMBER            HSM_1_IRQn

/*! @brief Macro used for checking buffer length */
#define HSM_BUFF_LEN_CHECK_MASK   0x0FU

/*! @brief Macro used for checking buffer address */
#define HSM_BUFF_ADDR_CHECK_MASK   0x03U

/*! @brief Macro used to convert buffer length in bytes to number of 128-bits blocks */
#define HSM_BUFF_BLOCK_COUNT(x)   ((x) >> 4U)

/*! @brief Security error codes offset in status.h */
#define HSM_ERC_OFFSET            0x400U

/*! @brief Macro used to convert HSM error code (HSM2HTS register) to SDK error code */
#define HSM_CONVERT_ERC(x)        ((status_t)((x) | HSM_ERC_OFFSET))

/*! @brief Mask for stripping the key id from 'hsm_key_id_t' */
#define HSM_KEY_ID_MASK           0x0FU

/*! @brief Shift value for retrieving KBS bit from 'hsm_key_id_t' */
#define HSM_KEY_ID_SHIFT          0x04U

/*! @brief Macro used for retrieving key id to be passed as command parameter */
#define HSM_CMD_KEY_ID(x)         (((uint32_t)(x)) & HSM_KEY_ID_MASK)

/*! @brief Macro used for retrieving KBS bit to update command structure */
#define HSM_CMD_KBS(x)            ((((uint32_t)(x)) & ~HSM_KEY_ID_MASK) >> HSM_KEY_ID_SHIFT)

/*! @brief Mask of the key bank select bit inside the 32 bits command */
#define HSM_CMD_KBS_MASK          0x80000000U

/*! @brief Maximum length for the MAC message */
#define HSM_MAC_MAX_MSG_LEN       0x800000000U

/*! @brief Macro used for checking 'HSM enabled' bit in SSCM */
#define HSM_ENABLED               ((SSCM->UOPS & SSCM_UOPS_HSE_MASK) != 0U)

/*! @brief Macro used for checking security firmware start address */
#define HSM_VALID_START_ADDR      (SSCM->HSA != 0U)

/*! @brief HSM to HOST interface register layout */
typedef struct {
    __IO uint32_t HSM2HTF;           /**< Priority Registers Slave, array offset: 0x0, array step: 0x100 */
    __IO uint32_t HSM2HTIE;          /**< Priority Registers Slave, array offset: 0x0, array step: 0x100 */
    __IO uint32_t HT2HSMF;           /**< Priority Registers Slave, array offset: 0x0, array step: 0x100 */
    uint8_t RESERVED[4];
    __IO uint32_t HSM2HTS;           /**< Control Register, array offset: 0x10, array step: 0x100 */
    __IO uint32_t HT2HSMS;           /**< Priority Registers Slave, array offset: 0x0, array step: 0x100 */
} HSM_Type;

/*! @brief HSM base pointer */
#define HSM                       ((HSM_Type *)HSM_BASE)

/*! @brief HSM to HOST interface register fields */
#define HSM2HTF_CMD_COMPLETE_MASK                0x2u
#define HSM2HTF_CMD_COMPLETE_SHIFT               1u
#define HSM2HTF_CMD_COMPLETE_WIDTH               1u
#define HSM2HTIE_CIE_MASK                        0x2u
#define HSM2HTIE_CIE_SHIFT                       1u
#define HSM2HTIE_CIE_WIDTH                       1u
#define HT2HSMF_CMD_INT_MASK                     0x2u
#define HT2HSMF_CMD_INT_SHIFT                    1u
#define HT2HSMF_CMD_INT_WIDTH                    1u
#define HSM2HTS_ERROR_CODE_MASK                  0xFFFF0000u
#define HSM2HTS_ERROR_CODE_SHIFT                 16u
#define HSM2HTS_ERROR_CODE_WIDTH                 16u
#define HSM2HTS_HSM_INIT_MASK                    0x200u
#define HSM2HTS_HSM_INIT_SHIFT                   9u
#define HSM2HTS_HSM_INIT_WIDTH                   1u
#define HSM2HTS_RAND_INIT_MASK                   0x20u
#define HSM2HTS_RAND_INIT_SHIFT                  5u
#define HSM2HTS_RAND_INIT_WIDTH                  1u
#define HSM2HTS_BUSY_MASK                        0x1u
#define HSM2HTS_BUSY_SHIFT                       0u
#define HSM2HTS_BUSY_WIDTH                       1u

/*! @brief Structure definition to store command and parameters as required by the HSM firmware */
typedef struct
{
    uint32_t CMD;
    uint32_t PARAM_1;
    uint32_t PARAM_2;
    uint32_t PARAM_3;
    uint32_t PARAM_4;
    uint32_t PARAM_5;
} hsm_fw_command_t;


/*******************************************************************************
 * CODE
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Prepares the HSM command structure.
 *
 * This function writes the command structure with the appropriate command and
 * parameters.
 *
 * @param[in] cmd Command.
 * @param[in] param1 Command parameter 1.
 * @param[in] param2 Command parameter 2.
 * @param[in] param3 Command parameter 3.
 * @param[in] param4 Command parameter 4.
 * @param[in] param5 Command parameter 5.
 */
void HSM_PrepareCommand(uint32_t cmd,
                        uint32_t param1,
                        uint32_t param2,
                        uint32_t param3,
                        uint32_t param4,
                        uint32_t param5);

/*!
 * @brief Prepares the HSM ECB encrypt command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 * @param[out] cipherText Pointer to the cipher text buffer.
 */
void HSM_PrepareEncryptEcbCmd(hsm_key_id_t keyId, const uint8_t *plainText,
                              uint32_t length, uint8_t *cipherText);

/*!
 * @brief Prepares the HSM ECB decrypt command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] cipherText Pointer to the cipher text buffer.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 * @param[out] plainText Pointer to the plain text buffer.
 */
void HSM_PrepareDecryptEcbCmd(hsm_key_id_t keyId, const uint8_t *cipherText,
                              uint32_t length, uint8_t *plainText);

/*!
 * @brief Prepares the HSM CBC encrypt command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 * @param[in] iv Pointer to the initialization vector buffer.
 * @param[out] cipherText Pointer to the cipher text buffer.
 */
void HSM_PrepareEncryptCbcCmd(hsm_key_id_t keyId, const uint8_t *plainText,
                              uint32_t length, const uint8_t *iv, uint8_t *cipherText);

/*!
 * @brief Prepares the HSM CBC decrypt command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] cipherText Pointer to the plain text buffer.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 * @param[in] iv Pointer to the initialization vector buffer.
 * @param[out] plainText Text Pointer to the cipher text buffer.
 */
void HSM_PrepareDecryptCbcCmd(hsm_key_id_t keyId, const uint8_t *cipherText,
                              uint32_t length, const uint8_t *iv, uint8_t *plainText);

/*!
 * @brief Prepares the HSM generate MAC command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[out] mac Pointer to the buffer containing the result of the CMAC
 * computation.
 */
void HSM_PrepareGenerateMacCmd(hsm_key_id_t keyId, const uint8_t *msg,
                               uint64_t msgLen, uint8_t *mac);

/*!
 * @brief Prepares the HSM generate mac command.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[in] mac Pointer to the buffer containing the result of the CMAC
 * computation.
 * @param[in] macLen Number of bits of the CMAC to be compared.
 */
void HSM_PrepareVerifyMacCmd(hsm_key_id_t keyId, const uint8_t *msg,
                             uint64_t msgLen, const uint8_t *mac, uint8_t macLen);

/*!
 * @brief Prepares the HSM load key command.
 *
 * @param[in] keyId Key ID used to perform the cryptographic operation.
 * @param[in] m1 Pointer to the buffer containing m1.
 * @param[in] m2 Pointer to the buffer containing m2.
 * @param[in] m3 Pointer to the buffer containing m3.
 * @param[out] m4 Pointer to the buffer containing m4.
 * @param[out] m5 Pointer to the buffer containing m5.
 */
void HSM_PrepareLoadKeyCmd(hsm_key_id_t keyId, const uint8_t *m1, const uint8_t *m2,
                           const uint8_t *m3, uint8_t *m4, uint8_t *m5);

/*!
 * @brief Prepares the HSM load plain key command.
 *
 * @param[in] plainKey Pointer to the 128-bit buffer containing the key
 */
static inline void HSM_PrepareLoadPlainKeyCmd(const uint8_t *plainKey)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_LOAD_PLAIN_KEY, (uint32_t)plainKey, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM export RAM key command.
 *
 * @param[out] m1 Pointer to the buffer containing m1.
 * @param[out] m2 Pointer to the buffer containing m2.
 * @param[out] m3 Pointer to the buffer containing m3.
 * @param[out] m4 Pointer to the buffer containing m4.
 * @param[out] m5 Pointer to the buffer containing m5.
 */
static inline void HSM_PrepareExportRamKeyCmd(uint8_t *m1, uint8_t *m2, uint8_t *m3,
                                              uint8_t *m4, uint8_t *m5)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_EXPORT_RAM_KEY, (uint32_t)m1, (uint32_t)m2,
                       (uint32_t)m3, (uint32_t)m4, (uint32_t)m5);
}

/*!
 * @brief Prepares the HSM init RNG command.
 */
static inline void HSM_PrepareInitRngCmd(void)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_INIT_RNG, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM extend PRNG seed command.
 *
 * @param[in] entropy Pointer to the 128-bit buffer containing the entropy.
 */
static inline void HSM_PrepareExtendPrngSeedCmd(const uint8_t *entropy)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_EXTEND_SEED, (uint32_t)entropy, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM generate random number command.
 *
 * @param[out] rnd Pointer to the 128-bit buffer containing the random value.
 */
static inline void HSM_PrepareGenerateRndCmd(uint8_t *rnd)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_RND, (uint32_t)rnd, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM get ID command.
 *
 * @param[in] challenge Pointer to the 128-bit buffer containing Challenge data.
 * @param[out] uid Pointer to 120 bit buffer where the UID will be stored.
 * @param[out] sreg Value of the status register.
 * @param[out] mac Pointer to the 128 bit buffer where the MAC generated over
 * challenge and UID and status  will be stored.
 */
static inline void HSM_PrepareGetIdCmd(const uint8_t *challenge, uint8_t *uid, uint8_t *sreg, uint8_t *mac)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_GET_ID, (uint32_t)challenge, (uint32_t)uid,
                       (uint32_t)sreg, (uint32_t)mac, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM generate true random number command.
 *
 * @param[out] trnd Pointer to the 128-bit buffer containing the random value.
 */
static inline void HSM_PrepareGenerateTrndCmd(uint8_t *trnd)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_TRNG_RND, (uint32_t)trnd, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM secure boot command.
 */
static inline void HSM_PrepareSecureBootCmd(uint32_t bootImageSize, const uint8_t *bootImagePtr)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_SECURE_BOOT, bootImageSize, (uint32_t)bootImagePtr,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM boot failure command.
 */
static inline void HSM_PrepareBootFailureCmd(void)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_BOOT_FAILURE, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM boot ok command.
 */
static inline void HSM_PrepareBootOkCmd(void)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_BOOT_OK, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM debug challenge command.
 */
static inline void HSM_PrepareDbgChalCmd(uint8_t *challenge)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_DBG_CHAL, (uint32_t)challenge, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM debug authorization command.
 */
static inline void HSM_PrepareDbgAuthCmd(const uint8_t *authorization)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_DBG_AUTH, (uint32_t)authorization, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Prepares the HSM cancel command.
 */
static inline void HSM_PrepareCancelCmd(void)
{
    HSM_PrepareCommand((uint32_t)HSM_CMD_CANCEL, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM,
                       HSM_UNUSED_PARAM, HSM_UNUSED_PARAM, HSM_UNUSED_PARAM);
}

/*!
 * @brief Sends the command to HSM.
 */
void HSM_SendCmd(void);

/*!
 * @brief Returns the result of the last MAC verification (true - successful verification)
 */
bool HSM_GetMacVerifResult(void);

/*!
 * @brief Retrieves the HSM error code.
 */
static inline uint32_t HSM_GetErrCode(void)
{
    return (uint32_t)((HSM->HSM2HTS & HSM2HTS_ERROR_CODE_MASK) >> HSM2HTS_ERROR_CODE_SHIFT);
}

/*!
 * @brief Enables/Disables HSM interrupt.
 *
 * @param[in] enable Enables/Disables HSM interrupt.
 */
static inline void HSM_SetInterrupt(bool enable)
{
    if (enable)
    {
        HSM->HSM2HTIE |= HSM2HTIE_CIE_MASK;
    }
    else
    {
        HSM->HSM2HTIE &= (~HSM2HTIE_CIE_MASK);
    }
}

/*!
 * @brief Clears the HSM command complete interrupt flag.
 */
static inline void HSM_ClearIntFlag(void)
{
    HSM->HSM2HTF = HSM2HTF_CMD_COMPLETE_MASK;
}

/*!
 * @brief Returns true if HSM is busy processing a command
 * and false if the command has completed.
 */
static inline bool HSM_IsBusy(void)
{
    return ((HSM->HSM2HTS & HSM2HTS_BUSY_MASK) != 0U);
}


#if defined(__cplusplus)
}
#endif

#endif /* HSM_HW_ACCESS_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
