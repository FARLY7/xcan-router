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

#ifndef HSM_DRV_H
#define HSM_DRV_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "status.h"
#include "osif.h"
#include "callbacks.h"

/*! @file hsm_driver.h */

/*!
 * @addtogroup hsm_driver
 * @{
 */

/*!
 * @brief Specify the KeyID to be used to implement the requested cryptographic
 * operation.
 *
 * Implements : hsm_key_id_t_Class
 */
typedef enum {
    HSM_SECRET_KEY = 0x0U,
    HSM_MASTER_ECU,
    HSM_BOOT_MAC_KEY,
    HSM_BOOT_MAC,
    HSM_KEY_1,
    HSM_KEY_2,
    HSM_KEY_3,
    HSM_KEY_4,
    HSM_KEY_5,
    HSM_KEY_6,
    HSM_KEY_7,
    HSM_KEY_8,
    HSM_KEY_9,
    HSM_KEY_10,
    HSM_RAM_KEY,
    HSM_KEY_11 = 0x14U,
    HSM_KEY_12,
    HSM_KEY_13,
    HSM_KEY_14,
    HSM_KEY_15,
    HSM_KEY_16,
    HSM_KEY_17,
    HSM_KEY_18,
    HSM_KEY_19,
    HSM_KEY_20
} hsm_key_id_t;

/*!
 * @brief HSM commands which follow the same values as the SHE command definition.
 *
 * Implements : hsm_cmd_t_Class
 */
typedef enum {
    HSM_CMD_NONE = 0U,
    HSM_CMD_ENC_ECB,
    HSM_CMD_ENC_CBC,
    HSM_CMD_DEC_ECB,
    HSM_CMD_DEC_CBC,
    HSM_CMD_GENERATE_MAC,
    HSM_CMD_VERIFY_MAC,
    HSM_CMD_LOAD_KEY,
    HSM_CMD_LOAD_PLAIN_KEY,
    HSM_CMD_EXPORT_RAM_KEY,
    HSM_CMD_INIT_RNG,
    HSM_CMD_EXTEND_SEED,
    HSM_CMD_RND,
    HSM_CMD_SECURE_BOOT,
    HSM_CMD_BOOT_FAILURE,
    HSM_CMD_BOOT_OK,
    HSM_CMD_GET_ID,
    HSM_CMD_CANCEL,
    HSM_CMD_DBG_CHAL,
    HSM_CMD_DBG_AUTH,
    HSM_CMD_TRNG_RND,
    HSM_CMD_GET_VER,
    HSM_CMD_CHANGE_TRNG_CLK_SOURCE
} hsm_cmd_t;

/*!
 * @brief Internal driver state information.
 *
 * @note The contents of this structure are internal to the driver and should not be
 *       modified by users. Also, contents of the structure are subject to change in
 *       future releases.
 *
 * Implements : hsm_state_t_Class
 */
typedef struct {
    bool cmdInProgress;           /*!< Specifies if a command is in progress */
    bool blockingCmd;             /*!< Specifies if a command is blocking or asynchronous */
    hsm_cmd_t cmd;                /*!< Specifies the type of the command in execution */
    security_callback_t callback; /*!< The callback invoked when a command is complete */
    void *callbackParam;          /*!< User parameter for the command completion callback */
    semaphore_t cmdComplete;      /*!< Synchronization object for synchronous operation */
    status_t cmdStatus;           /*!< Error code for the last command */
    bool rngInit;                 /*!< Specifies if the internal RNG state is initialized */
    bool *macVerifStatus;         /*!< Specifies the result of the last executed MAC verification command */
} hsm_state_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Initializes the internal state of the driver and enables the HSM interrupt.
 *
 * @param[in] state Pointer to the state structure which will be used for holding
 * the internal state of the driver.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_Init(hsm_state_t *state);

/*!
 * @brief Clears the internal state of the driver and disables the HSM interrupt.
 *
 * @return STATUS_SUCCESS.
 */
status_t HSM_DRV_Deinit(void);

/*!
 * @brief Installs a user callback for the command complete event.
 *
 * This function installs a user callback for the command complete event.
 *
 * @return Pointer to the previous callback.
 */
security_callback_t HSM_DRV_InstallCallback(security_callback_t callbackFunction, void * callbackParam);

/*!
 * @brief Performs the AES-128 encryption in ECB mode.
 *
 * This function performs the AES-128 encryption in ECB mode of the input
 * plain text buffer
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[out] cipherText Pointer to the cipher text buffer. The buffer shall
 * have the same size as the plain text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_EncryptECB(hsm_key_id_t keyId, const uint8_t *plainText,
                            uint32_t length, uint8_t *cipherText, uint32_t timeout);

/*!
 * @brief Performs the AES-128 decryption in ECB mode.
 *
 * This function performs the AES-128 decryption in ECB mode of the input
 * cipher text buffer.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation
 * @param[in] cipherText Pointer to the cipher text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of cipher text message to be decrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[out] plainText Pointer to the plain text buffer. The buffer shall
 * have the same size as the cipher text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_DecryptECB(hsm_key_id_t keyId, const uint8_t *cipherText,
                             uint32_t length, uint8_t *plainText, uint32_t timeout);

/*!
 * @brief Performs the AES-128 encryption in CBC mode.
 *
 * This function performs the AES-128 encryption in CBC mode of the input
 * plaintext buffer.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[in] iv Pointer to the initialization vector buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] cipherText Pointer to the cipher text buffer. The buffer shall
 * have the same size as the plain text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_EncryptCBC(hsm_key_id_t keyId, const uint8_t *plainText, uint32_t length,
                            const uint8_t *iv, uint8_t *cipherText, uint32_t timeout);

/*!
 * @brief Performs the AES-128 decryption in CBC mode.
 *
 * This function performs the AES-128 decryption in CBC mode of the input
 * cipher text buffer.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] cipherText Pointer to the cipher text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of cipher text message to be decrypted.
 * It should be multiple of 16 bytes.
 * @param[in] iv Pointer to the initialization vector buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] plainText Pointer to the plain text buffer. The buffer shall
 * have the same size as the cipher text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_DecryptCBC(hsm_key_id_t keyId, const uint8_t *cipherText, uint32_t length,
                            const uint8_t* iv, uint8_t *plainText, uint32_t timeout);

/*!
 * @brief Calculates the MAC of a given message using CMAC with AES-128.
 *
 * This function calculates the MAC of a given message using CMAC with AES-128.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[out] mac Pointer to the buffer containing the result of the CMAC
 * computation.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_GenerateMAC(hsm_key_id_t keyId, const uint8_t *msg,
                             uint64_t msgLen, uint8_t *mac, uint32_t timeout);

/*!
 * @brief Verifies the MAC of a given message using CMAC with AES-128.
 *
 * This function verifies the MAC of a given message using CMAC with AES-128.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[in] mac Pointer to the buffer containing the CMAC to be verified.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] macLen Number of bits of the CMAC to be compared. A macLength
 * value of zero indicates that all 128-bits are compared.
 * @param[out] verifStatus Status of MAC verification command (true:
 * verification operation passed, false: verification operation failed).
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_VerifyMAC(hsm_key_id_t keyId, const uint8_t *msg, uint64_t msgLen,
                           const uint8_t *mac, uint8_t macLen,
                           bool *verifStatus, uint32_t timeout);

/*!
 * @brief Asynchronously performs the AES-128 encryption in ECB mode.
 *
 * This function performs the AES-128 encryption in ECB mode of the input
 * plain text buffer, in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[out] cipherText Pointer to the cipher text buffer. The buffer shall
 * have the same size as the plain text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_EncryptECBAsync(hsm_key_id_t keyId, const uint8_t *plainText,
                                 uint32_t length, uint8_t *cipherText);

/*!
 * @brief Asynchronously performs the AES-128 decryption in ECB mode.
 *
 * This function performs the AES-128 decryption in ECB mode of the input
 * cipher text buffer, in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation
 * @param[in] cipherText Pointer to the cipher text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of cipher text message to be decrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[out] plainText Pointer to the plain text buffer. The buffer shall
 * have the same size as the cipher text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_DecryptECBAsync(hsm_key_id_t keyId, const uint8_t *cipherText,
                                  uint32_t length, uint8_t *plainText);

/*!
 * @brief Asynchronously performs the AES-128 encryption in CBC mode.
 *
 * This function performs the AES-128 encryption in CBC mode of the input
 * plaintext buffer, in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] plainText Pointer to the plain text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of plain text message to be encrypted.
 *            @note Should be multiple of 16 bytes.
 * @param[in] iv Pointer to the initialization vector buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] cipherText Pointer to the cipher text buffer. The buffer shall
 * have the same size as the plain text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_EncryptCBCAsync(hsm_key_id_t keyId, const uint8_t *plainText,
                                 uint32_t length, const uint8_t *iv, uint8_t *cipherText);

/*!
 * @brief Asynchronously performs the AES-128 decryption in CBC mode.
 *
 * This function performs the AES-128 decryption in CBC mode of the input
 * cipher text buffer, in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] cipherText Pointer to the cipher text buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] length Number of bytes of cipher text message to be decrypted.
 * It should be multiple of 16 bytes.
 * @param[in] iv Pointer to the initialization vector buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] plainText Pointer to the plain text buffer. The buffer shall
 * have the same size as the cipher text buffer.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_DecryptCBCAsync(hsm_key_id_t keyId, const uint8_t *cipherText,
                                 uint32_t length, const uint8_t* iv, uint8_t *plainText);

/*!
 * @brief Asynchronously calculates the MAC of a given message using CMAC with AES-128.
 *
 * This function calculates the MAC of a given message using CMAC with AES-128,
 * in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[out] mac Pointer to the buffer containing the result of the CMAC
 * computation.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_GenerateMACAsync(hsm_key_id_t keyId, const uint8_t *msg,
                                  uint64_t msgLen, uint8_t *mac);

/*!
 * @brief Asynchronously verifies the MAC of a given message using CMAC with AES-128.
 *
 * This function verifies the MAC of a given message using CMAC with AES-128,
 * in an asynchronous manner.
 *
 * @param[in] keyId KeyID used to perform the cryptographic operation.
 * @param[in] msg Pointer to the message buffer.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] msgLen Number of bits of message on which CMAC will be computed.
 * @param[in] mac Pointer to the buffer containing the CMAC to be verified.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] macLen Number of bits of the CMAC to be compared. A macLength
 * value of zero indicates that all 128-bits are compared.
 * @param[out] verifStatus Status of MAC verification command (true:
 * verification operation passed, false: verification operation failed).
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_VerifyMACAsync(hsm_key_id_t keyId, const uint8_t *msg, uint64_t msgLen,
                                const uint8_t *mac, uint8_t macLen, bool *verifStatus);

/*!
 * @brief Updates an internal key per the SHE specification.
 *
 * This function updates an internal key per the SHE specification.
 *
 * @param[in] keyId KeyID of the key to be updated.
 * @param[in] m1 Pointer to the 128-bit M1 message containing the UID, Key ID
 * and Authentication Key ID.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] m2 Pointer to the 256-bit M2 message contains the new security
 * flags, counter and the key value all encrypted using a derived key generated
 * from the Authentication Key.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] m3 Pointer to the 128-bit M3 message is a MAC generated over
 * messages M1 and M2.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m4 Pointer to a 256 bits buffer where the computed M4 parameter
 * is stored.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m5 Pointer to a 128 bits buffer where the computed M5 parameter
 * is stored.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_LoadKey(hsm_key_id_t keyId, const uint8_t *m1, const uint8_t *m2,
                         const uint8_t *m3, uint8_t *m4, uint8_t *m5, uint32_t timeout);

/*!
 * @brief Updates the RAM key memory slot with a 128-bit plaintext.
 *
 * The function updates the RAM key memory slot with a 128-bit plaintext. The
 * key is loaded without encryption and verification of the key, i.e. the key is
 * handed over in plaintext. A plain key can only be loaded into the RAM_KEY
 * slot.
 *
 * @param[in] plainKey Pointer to the 128-bit buffer containing the key that
 * needs to be copied in RAM_KEY slot.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_LoadPlainKey(const uint8_t *plainKey, uint32_t timeout);

/*!
 * @brief Exports the RAM_KEY into a format compatible with the messages
 * used for LOAD_KEY.
 *
 * @param[out] m1 Pointer to a buffer where the M1 parameter will be exported.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m2 Pointer to a buffer where the M2 parameter will be exported.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m3 Pointer to a buffer where the M3 parameter will be exported.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m4 Pointer to a buffer where the M4 parameter will be exported.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] m5 Pointer to a buffer where the M5 parameter will be exported.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_ExportRAMKey(uint8_t *m1, uint8_t *m2, uint8_t *m3,
                              uint8_t *m4, uint8_t *m5, uint32_t timeout);

/*!
 * @brief Initializes the seed for the PRNG.
 *
 * The function must be called before CMD_RND after every power cycle/reset.
 *
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_InitRNG(uint32_t timeout);

/*!
 * @brief Extends the seed of the PRNG.
 *
 * Extends the seed of the PRNG by compressing the former seed value and the
 * supplied entropy into a new seed. This new seed is then to be used to
 * generate a random number by invoking the CMD_RND command. The random number
 * generator must be initialized by CMD_INIT_RNG before the seed may be
 * extended.
 *
 * @param[in] entropy Pointer to a 128-bit buffer containing the entropy.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_ExtendSeed(const uint8_t *entropy, uint32_t timeout);

/*!
 * @brief Generates a vector of 128 random bits.
 *
 * The function returns a vector of 128 random bits. The random number generator
 * has to be initialized by calling HSM_DRV_InitRNG before random numbers can
 * be supplied.
 *
 * @param[out] rnd Pointer to a 128-bit buffer where the generated random number
 * has to be stored.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_GenerateRND(uint8_t *rnd, uint32_t timeout);

/*!
 * @brief Returns the identity (UID) and the value of the status register
 * protected by a MAC over a challenge and the data.
 *
 * This function returns the identity (UID) and the value of the status register
 * protected by a MAC over a challenge and the data.
 *
 * @param[in] challenge Pointer to the 128-bit buffer containing Challenge data.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[out] uid Pointer to 120 bit buffer where the UID will be stored.
 * @param[out] sreg Value of the status register.
 * @param[out] mac Pointer to the 128 bit buffer where the MAC generated over
 * challenge and UID and status  will be stored.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_GetID(const uint8_t *challenge, uint8_t *uid,
                       uint8_t *sreg, uint8_t *mac, uint32_t timeout);

/*!
 * @brief Generates a vector of 128 random bits using TRNG.
 *
 * The function returns a vector of 128 true random bits, using the TRNG.
 *
 * @param[out] trnd Pointer to a 128-bit buffer where the generated random number
 * has to be stored.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_GenerateTRND(uint8_t *trnd, uint32_t timeout);

/*!
 * @brief Executes the SHE secure boot protocol.
 *
 * The function loads the command processor firmware and memory slot data from
 * the HSM Flash blocks, and then it executes the SHE secure boot protocol.
 *
 * @param[in] bootImageSize Boot image size (in bytes).
 * @param[in] bootImagePtr Boot image start address.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_SecureBoot(uint32_t bootImageSize, const uint8_t *bootImagePtr,
                            uint32_t timeout);

/*!
 * @brief Signals a failure detected during later stages of the boot process.
 *
 * The function is called during later stages of the boot process to detect a
 * failure.
 *
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_BootFailure(uint32_t timeout);

/*!
 * @brief Marks a successful boot verification during later stages of the boot
 * process.
 *
 * The function is called during later stages of the boot process to mark
 * successful boot verification.
 *
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_BootOK(uint32_t timeout);

/*!
 * @brief Obtains a random number which the user shall use along with the
 * MASTER_ECU_KEY and UID to return an authorization request.
 *
 * This function obtains a random number which the user shall use along with the
 * MASTER_ECU_KEY and UID to return an authorization request.
 *
 * @param[out] challenge Pointer to the 128-bit buffer where the challenge data
 * will be stored.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_DbgChal(uint8_t *challenge, uint32_t timeout);

/*!
 * @brief Erases all user keys and enables internal debugging if the
 * authorization is confirmed by HSM.
 *
 * This function erases all user keys and enables internal debugging if the
 * authorization is confirmed by HSM.
 *
 * @param[in] authorization Pointer to the 128-bit buffer containing the
 * authorization value.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution.
 */
status_t HSM_DRV_DbgAuth(const uint8_t *authorization, uint32_t timeout);

/*!
 * @brief Compresses the given messages using the Miyaguchi-Preneel
 * compression algorithm implemented in software.
 *
 * This function is a software implementation of Miyaguchi-Preneel compression,
 * running on the host. It is defined mainly for obtaining M1->M5 for secure
 * key update.
 *
 * @param[in] msg Pointer to the messages to be compressed. Messages must be
 * pre-processed per SHE specification if they do not already meet the full
 * 128-bit block size requirement.
 *            @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] msgLen The number of 128 bit messages to be compressed.
 * @param[out] mpCompress Pointer to the 128 bit buffer storing the compressed
 * data.
 *             @note Address passed in this parameter must be 32 bit aligned.
 * @param[in] timeout Timeout in ms; the function returns STATUS_TIMEOUT if the
 * command is not finished in the allocated period.
 * @return Error Code after command execution. Output parameters are valid if
 * the error code is STATUS_SUCCESS.
 */
status_t HSM_DRV_MPCompress(const uint8_t *msg, uint16_t msgLen,
                            uint8_t *mpCompress, uint32_t timeout);

/*!
 * @brief Checks the status of the execution of an asynchronous command.
 *
 * This function checks the status of the execution of an asynchronous command.
 * If the command is still in progress, returns STATUS_BUSY.
 *
 * @return Error Code after command execution.
 */
status_t HSM_DRV_GetAsyncCmdStatus(void);

/*!
 * @brief Cancels a previously initiated command.
 *
 * This function cancels any on-going HSM command.
 *
 * @return STATUS_SUCCESS.
 */
status_t HSM_DRV_CancelCommand(void);


#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* HSM_DRV_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
