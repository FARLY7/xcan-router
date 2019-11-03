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
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. Also, the called functions
 * do not store the address into variables with lifetime longer then its own call.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * These are symbols weak symbols defined in platform startup files (.s).
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different
 * essential type.
 * This is required by the conversion of a bit-field of a register into enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from
 * 'essentially unsigned' to 'essentially enum<i>'.
 * All possible values are covered by the enumeration, direct casting is used to optimize code.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.8, Impermissible cast of composite
 * expression (different essential type categories).
 * This is required by the conversion of a bit-field of a register into enum type.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required for checking buffer address alignment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long.
 * The cast is required for checking buffer address alignment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 15.5, Return statement before end of function.
 * The return statement before end of function is used for simpler code structure
 * and better readability.
 */

#include "hsm_hw_access.h"


/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Pointer to runtime state structure.*/
static hsm_state_t * s_hsmStatePtr = NULL;


/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/* Waits on the synchronization object and updates the internal flags */
static void HSM_DRV_WaitCommandCompletion(uint32_t timeout);
/* Copies data from source to destination buffer */
static void HSM_DRV_CopyBuff(const uint8_t * srcBuff, uint8_t * destBuff, uint32_t len);
/* Performs bitwise XOR between two buffers, storing the result in the output buffer */
static void HSM_DRV_XorBuff(const uint8_t * inBuff, uint8_t * outBuff, uint32_t len);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_Init
 * Description   : This function initializes the internal state of the driver
 * and enables the HSM interrupt.
 *
 * Implements    : HSM_DRV_Init_Activity
 * END**************************************************************************/
status_t HSM_DRV_Init(hsm_state_t * state)
{
    /* Check that HSM is enabled and the security firmware flashed in the device */
    DEV_ASSERT(HSM_ENABLED);
    DEV_ASSERT(HSM_VALID_START_ADDR);
    /* Check the driver is initialized */
    DEV_ASSERT(state != NULL);

    status_t semaStatus;

    /* Save the driver state structure */
    s_hsmStatePtr = state;

    /* Clear the contents of the state structure */
    s_hsmStatePtr->cmdInProgress = false;
    s_hsmStatePtr->blockingCmd = false;
    s_hsmStatePtr->callback = NULL;
    s_hsmStatePtr->callbackParam = NULL;
    s_hsmStatePtr->cmd = HSM_CMD_NONE;
    s_hsmStatePtr->cmdStatus = STATUS_SUCCESS;
    s_hsmStatePtr->rngInit = false;
    s_hsmStatePtr->macVerifStatus = NULL;

    /* Create the synchronization semaphore */
    semaStatus = OSIF_SemaCreate(&s_hsmStatePtr->cmdComplete, 0U);
    if (semaStatus == STATUS_ERROR)
    {
        return STATUS_ERROR;
    }

    /* Enable HSM irq */
    HSM_SetInterrupt(true);
    INT_SYS_EnableIRQ(HSM_IRQ_NUMBER);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_Deinit
 * Description   : This function clears the internal state of the driver and
 * disables the HSM interrupt.
 *
 * Implements    : HSM_DRV_Deinit_Activity
 * END**************************************************************************/
status_t HSM_DRV_Deinit()
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Clear the contents of the state structure */
    uint8_t *clearStructPtr;
    uint8_t idx;
    clearStructPtr = (uint8_t *)s_hsmStatePtr;
    for (idx = 0; idx < sizeof(hsm_state_t); idx++)
    {
        clearStructPtr[idx] = 0;
    }

    /* Free the internal state reference */
    s_hsmStatePtr = NULL;

    /* Disable HSM irq */
    INT_SYS_DisableIRQ(HSM_IRQ_NUMBER);
    HSM_SetInterrupt(false);

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_InstallCallback
 * Description   : This function installs a user callback for the command
 * complete event.
 *
 * Implements    : HSM_DRV_InstallCallback_Activity
 * END**************************************************************************/
security_callback_t HSM_DRV_InstallCallback(security_callback_t callbackFunction, void * callbackParam)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    security_callback_t currentCallback = s_hsmStatePtr->callback;
    s_hsmStatePtr->callback = callbackFunction;
    s_hsmStatePtr->callbackParam = callbackParam;

    return currentCallback;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_EncryptECB
 * Description   : This function performs the AES-128 encryption in ECB mode of
 * the input plain text buffer.
 *
 * Implements    : HSM_DRV_EncryptECB_Activity
 * END**************************************************************************/
status_t HSM_DRV_EncryptECB(hsm_key_id_t keyId, const uint8_t *plainText,
                            uint32_t length, uint8_t *cipherText, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_EncryptECBAsync(keyId, plainText, length, cipherText);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DecryptECB
 * Description   : This function performs the AES-128 decryption in ECB mode of
 * the input cipher text buffer.
 *
 * Implements    : HSM_DRV_DecryptECB_Activity
 * END**************************************************************************/
status_t HSM_DRV_DecryptECB(hsm_key_id_t keyId, const uint8_t *cipherText,
                             uint32_t length, uint8_t *plainText, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_DecryptECBAsync(keyId, cipherText, length, plainText);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_EncryptCBC
 * Description   : This function performs the AES-128 encryption in CBC mode of
 * the input plain text buffer.
 *
 * Implements    : HSM_DRV_EncryptCBC_Activity
 * END**************************************************************************/
status_t HSM_DRV_EncryptCBC(hsm_key_id_t keyId, const uint8_t *plainText, uint32_t length,
                            const uint8_t *iv, uint8_t *cipherText, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_EncryptCBCAsync(keyId, plainText, length, iv, cipherText);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DecryptCBC
 * Description   : This function performs the AES-128 decryption in CBC mode of
 * the input cipher text buffer.
 *
 * Implements    : HSM_DRV_DecryptCBC_Activity
 * END**************************************************************************/
status_t HSM_DRV_DecryptCBC(hsm_key_id_t keyId, const uint8_t *cipherText, uint32_t length,
                            const uint8_t* iv, uint8_t *plainText, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_DecryptCBCAsync(keyId, cipherText, length, iv, plainText);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GenerateMAC
 * Description   : This function calculates the MAC of a given message using CMAC
 * with AES-128.
 *
 * Implements    : HSM_DRV_GenerateMAC_Activity
 * END**************************************************************************/
status_t HSM_DRV_GenerateMAC(hsm_key_id_t keyId, const uint8_t *msg,
                             uint64_t msgLen, uint8_t *mac, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_GenerateMACAsync(keyId, msg, msgLen, mac);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_VerifyMAC
 * Description   : This function verifies the MAC of a given message using CMAC
 * with AES-128.
 *
 * Implements    : HSM_DRV_VerifyMAC_Activity
 * END**************************************************************************/
status_t HSM_DRV_VerifyMAC(hsm_key_id_t keyId, const uint8_t *msg, uint64_t msgLen,
                           const uint8_t *mac, uint8_t macLen,
                           bool *verifStatus, uint32_t timeout)
{
    status_t status;

    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Specify this is a blocking function - returns upon command completion */
    s_hsmStatePtr->blockingCmd = true;

    /* Launch the command with the parameters received */
    status = HSM_DRV_VerifyMACAsync(keyId, msg, msgLen, mac, macLen, verifStatus);

    if (status == STATUS_SUCCESS)
    {
        /* Wait for the command to complete */
        HSM_DRV_WaitCommandCompletion(timeout);

        return s_hsmStatePtr->cmdStatus;
    }
    else
    {
        return status;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_EncryptECBAsync
 * Description   : This function performs the AES-128 encryption in ECB mode of
 * the input plain text buffer, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_EncryptECBAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_EncryptECBAsync(hsm_key_id_t keyId, const uint8_t *plainText,
                                 uint32_t length, uint8_t *cipherText)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    /* Check the buffers addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)plainText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)cipherText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the buffer length is multiple of 16 bytes */
    DEV_ASSERT((length & HSM_BUFF_LEN_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_ENC_ECB;

    /* Prepare the command */
    HSM_PrepareEncryptEcbCmd(keyId, plainText, length, cipherText);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DecryptECBAsync
 * Description   : This function performs the AES-128 decryption in ECB mode of
 * the input cipher text buffer, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_DecryptECBAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_DecryptECBAsync(hsm_key_id_t keyId, const uint8_t *cipherText,
                                 uint32_t length, uint8_t *plainText)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    /* Check the buffers addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)plainText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)cipherText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the buffer length is multiple of 16 bytes */
    DEV_ASSERT((length & HSM_BUFF_LEN_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_DEC_ECB;

    /* Prepare the command */
    HSM_PrepareDecryptEcbCmd(keyId, cipherText, length, plainText);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_EncryptCBCAsync
 * Description   : This function performs the AES-128 encryption in CBC mode of
 * the input plain text buffer, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_EncryptCBCAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_EncryptCBCAsync(hsm_key_id_t keyId, const uint8_t *plainText,
                                 uint32_t length, const uint8_t *iv, uint8_t *cipherText)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    /* Check the buffers addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)plainText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)cipherText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)iv) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the buffer length is multiple of 16 bytes */
    DEV_ASSERT((length & HSM_BUFF_LEN_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_ENC_CBC;

    /* Prepare the command */
    HSM_PrepareEncryptCbcCmd(keyId, plainText, length, iv, cipherText);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DecryptCBCAsync
 * Description   : This function performs the AES-128 decryption in CBC mode of
 * the input cipher text buffer, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_DecryptCBCAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_DecryptCBCAsync(hsm_key_id_t keyId, const uint8_t *cipherText,
                                 uint32_t length, const uint8_t* iv, uint8_t *plainText)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(plainText != NULL);
    DEV_ASSERT(cipherText != NULL);
    DEV_ASSERT(iv != NULL);
    /* Check the buffers addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)plainText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)cipherText) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)iv) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the buffer length is multiple of 16 bytes */
    DEV_ASSERT((length & HSM_BUFF_LEN_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_DEC_CBC;

    /* Prepare the command */
    HSM_PrepareDecryptCbcCmd(keyId, cipherText, length, iv, plainText);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GenerateMACAsync
 * Description   : This function calculates the MAC of a given message using CMAC
 * with AES-128, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_GenerateMACAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_GenerateMACAsync(hsm_key_id_t keyId, const uint8_t *msg,
                             uint64_t msgLen, uint8_t *mac)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mac != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)mac) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)msg) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the message length is valid */
    DEV_ASSERT(msgLen < HSM_MAC_MAX_MSG_LEN);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_GENERATE_MAC;

    /* Prepare the command */
    HSM_PrepareGenerateMacCmd(keyId, msg, msgLen, mac);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_VerifyMACAsync
 * Description   : This function verifies the MAC of a given message using CMAC
 * with AES-128, in an asynchronous manner.
 *
 * Implements    : HSM_DRV_VerifyMACAsync_Activity
 * END**************************************************************************/
status_t HSM_DRV_VerifyMACAsync(hsm_key_id_t keyId, const uint8_t *msg, uint64_t msgLen,
                                const uint8_t *mac, uint8_t macLen, bool *verifStatus)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mac != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)msg) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)mac) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    /* Check the message length is valid */
    DEV_ASSERT(msgLen < HSM_MAC_MAX_MSG_LEN);
    /* Check the mac length is valid */
    DEV_ASSERT(macLen <= 128U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->macVerifStatus = verifStatus;
    s_hsmStatePtr->cmd = HSM_CMD_VERIFY_MAC;

    /* Prepare the command */
    HSM_PrepareVerifyMacCmd(keyId, msg, msgLen, mac, macLen);

    /* Send the command to HSM */
    HSM_SendCmd();

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_LoadKey
 * Description   : Updates an internal key per the SHE specification.
 *
 * Implements    : HSM_DRV_LoadKey_Activity
 * END**************************************************************************/
status_t HSM_DRV_LoadKey(hsm_key_id_t keyId, const uint8_t *m1, const uint8_t *m2,
                         const uint8_t *m3, uint8_t *m4, uint8_t *m5, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(m1 != NULL);
    DEV_ASSERT(m2 != NULL);
    DEV_ASSERT(m3 != NULL);
    DEV_ASSERT(m4 != NULL);
    DEV_ASSERT(m5 != NULL);
    /* Check the buffer addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)m1) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m2) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m3) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m4) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m5) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_LOAD_KEY;

    /* Prepare the command */
    HSM_PrepareLoadKeyCmd(keyId, m1, m2, m3, m4, m5);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_LoadPlainKey
 * Description   : Updates the RAM key memory slot with a 128-bit plaintext.
 *
 * Implements    : HSM_DRV_LoadPlainKey_Activity
 * END**************************************************************************/
status_t HSM_DRV_LoadPlainKey(const uint8_t * plainKey, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer address is valid */
    DEV_ASSERT(plainKey != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)plainKey) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_LOAD_PLAIN_KEY;

    /* Prepare the command */
    HSM_PrepareLoadPlainKeyCmd(plainKey);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_ExportRAMKey
 * Description   : Exports the RAM_KEY into a format compatible with the messages
 * used for LOAD_KEY.
 *
 * Implements    : HSM_DRV_ExportRAMKey_Activity
 * END**************************************************************************/
status_t HSM_DRV_ExportRAMKey(uint8_t *m1, uint8_t *m2, uint8_t *m3,
                              uint8_t *m4, uint8_t *m5, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(m1 != NULL);
    DEV_ASSERT(m2 != NULL);
    DEV_ASSERT(m3 != NULL);
    DEV_ASSERT(m4 != NULL);
    DEV_ASSERT(m5 != NULL);
    /* Check the buffer addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)m1) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m2) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m3) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m4) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)m5) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_EXPORT_RAM_KEY;

    /* Prepare the command */
    HSM_PrepareExportRamKeyCmd(m1, m2, m3, m4, m5);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_InitRNG
 * Description   : Initializes the seed for the PRNG.
 *
 * Implements    : HSM_DRV_InitRNG_Activity
 * END**************************************************************************/
status_t HSM_DRV_InitRNG(uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_INIT_RNG;

    /* Prepare the command */
    HSM_PrepareInitRngCmd();

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    if (s_hsmStatePtr->cmdStatus == STATUS_SUCCESS)
    {
        s_hsmStatePtr->rngInit = true;
    }
    else
    {
        s_hsmStatePtr->rngInit = false;
    }

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_ExtendSeed
 * Description   : Extends the seed for the PRNG.
 *
 * Implements    : HSM_DRV_ExtendSeed_Activity
 * END**************************************************************************/
status_t HSM_DRV_ExtendSeed(const uint8_t *entropy, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer address is valid */
    DEV_ASSERT(entropy != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)entropy) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* RNG must be initialized before extending the seed */
    DEV_ASSERT(s_hsmStatePtr->rngInit);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_EXTEND_SEED;

    /* Prepare the command */
    HSM_PrepareExtendPrngSeedCmd(entropy);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GenerateRND
 * Description   : Generates a vector of 128 random bits.
 *
 * Implements    : HSM_DRV_GenerateRND_Activity
 * END**************************************************************************/
status_t HSM_DRV_GenerateRND(uint8_t *rnd, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer address is valid */
    DEV_ASSERT(rnd != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)rnd) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* RNG must be initialized before generating the random value */
    DEV_ASSERT(s_hsmStatePtr->rngInit);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_RND;

    /* Prepare the command */
    HSM_PrepareGenerateRndCmd(rnd);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GetID
 * Description   : Returns the identity (UID) and the value of the status register
 * protected by a MAC over a challenge and the data.
 *
 * Implements    : HSM_DRV_GetID_Activity
 * END**************************************************************************/
status_t HSM_DRV_GetID(const uint8_t *challenge, uint8_t *uid,
                       uint8_t *sreg, uint8_t *mac, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(challenge != NULL);
    DEV_ASSERT(uid != NULL);
    DEV_ASSERT(sreg != NULL);
    DEV_ASSERT(mac != NULL);
    /* Check the buffer addresses are 32 bit aligned */
    DEV_ASSERT((((uint32_t)challenge) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);
    DEV_ASSERT((((uint32_t)mac) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_GET_ID;

    /* Prepare the command */
    HSM_PrepareGetIdCmd(challenge, uid, sreg, mac);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GenerateTRND
 * Description   :  Generates a vector of 128 random bits using TRNG.
 *
 * Implements    : HSM_DRV_GenerateTRND_Activity
 * END**************************************************************************/
status_t HSM_DRV_GenerateTRND(uint8_t *trnd, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer address is valid */
    DEV_ASSERT(trnd != NULL);
    /* Check the buffer address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)trnd) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_TRNG_RND;

    /* Prepare the command */
    HSM_PrepareGenerateTrndCmd(trnd);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_SecureBoot
 * Description   : This function executes the SHE secure boot protocol.
 *
 * Implements    : HSM_DRV_SecureBoot_Activity
 * END**************************************************************************/
status_t HSM_DRV_SecureBoot(uint32_t bootImageSize, const uint8_t *bootImagePtr,
                            uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the pointer is valid */
    DEV_ASSERT(bootImagePtr != NULL);
    /* Check the boot image address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)bootImagePtr) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_SECURE_BOOT;

    /* Prepare the command */
    HSM_PrepareSecureBootCmd(bootImageSize, bootImagePtr);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_BootFailure
 * Description   : This function signals a failure detected during later stages
 * of the boot process.
 *
 * Implements    : HSM_DRV_BootFailure_Activity
 * END**************************************************************************/
status_t HSM_DRV_BootFailure(uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_BOOT_FAILURE;

    /* Prepare the command */
    HSM_PrepareBootFailureCmd();

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_BootOK
 * Description   : This function marks a successful boot verification during
 * later stages of the boot process.
 *
 * Implements    : HSM_DRV_BootOK_Activity
 * END**************************************************************************/
status_t HSM_DRV_BootOK(uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_BOOT_OK;

    /* Prepare the command */
    HSM_PrepareBootOkCmd();

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DbgChal
 * Description   : This function obtains a random number which the user shall
 * use along with the MASTER_ECU_KEY and UID to return an authorization request.
 *
 * Implements    : HSM_DRV_DbgChal_Activity
 * END**************************************************************************/
status_t HSM_DRV_DbgChal(uint8_t *challenge, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the pointer is valid */
    DEV_ASSERT(challenge != NULL);
    /* Check the boot image address is 32 bit aligned */
    DEV_ASSERT((((uint32_t)challenge) & HSM_BUFF_ADDR_CHECK_MASK) == 0U);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmd = HSM_CMD_DBG_CHAL;

    /* Prepare the command */
    HSM_PrepareDbgChalCmd(challenge);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_DbgAuth
 * Description   : This function erases all user keys and enables internal
 * debugging if the authorization is confirmed by HSM.
 *
 * Implements    : HSM_DRV_DbgAuth_Activity
 * END**************************************************************************/
status_t HSM_DRV_DbgAuth(const uint8_t *authorization, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the pointer is valid */
    DEV_ASSERT(authorization != NULL);

    /* Check there is no other command in execution */
    if (HSM_IsBusy() || s_hsmStatePtr->cmdInProgress)
    {
        return STATUS_BUSY;
    }

    /* Update the internal flags */
    s_hsmStatePtr->blockingCmd = true;
    s_hsmStatePtr->cmdInProgress = true;
    s_hsmStatePtr->cmd = HSM_CMD_DBG_AUTH;

    /* Prepare the command */
    HSM_PrepareDbgAuthCmd(authorization);

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the command to complete */
    HSM_DRV_WaitCommandCompletion(timeout);

    return s_hsmStatePtr->cmdStatus;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_MPCompress
 * Description   : This function implements a Miyaguchi-Preneel compression
 * in software.
 *
 * Implements    : HSM_DRV_MPCompress_Activity
 * END**************************************************************************/
status_t HSM_DRV_MPCompress(const uint8_t * msg, uint16_t msgLen,
                            uint8_t * mpCompress, uint32_t timeout)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);
    /* Check the buffer addresses are valid */
    DEV_ASSERT(msg != NULL);
    DEV_ASSERT(mpCompress != NULL);

    uint32_t block = 0U;
    uint8_t key[16] = {0U, };
    uint32_t msgLenInBytes;
    status_t stat = STATUS_SUCCESS;

    /* Determine the number of bytes to compress (multiply by 16) */
    msgLenInBytes = (uint32_t)(msgLen << 4U);

    /* Perform Miyaguchi-Preneel compression */
    while (block < msgLenInBytes)
    {
        /* Use RAM key */
        stat = HSM_DRV_LoadPlainKey(key, timeout);
        if (stat != STATUS_SUCCESS)
        {
            return stat;
        }

        /* Encrypt this block using the previous compression output */
        stat = HSM_DRV_EncryptECB(HSM_RAM_KEY, &msg[block], 16U, mpCompress, timeout);
        if (stat != STATUS_SUCCESS)
        {
            return stat;
        }

        /* XOR message block, ciphertext and result from previous step */
        HSM_DRV_XorBuff(key, mpCompress, 16U);
        HSM_DRV_XorBuff(&msg[block], mpCompress, 16U);

        /* Update the key to be used for next step */
        HSM_DRV_CopyBuff(mpCompress, key, 16);

        block += 16U;
    }

    return stat;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_GetAsyncCmdStatus
 * Description   : This function checks the status of the execution of an
 * asynchronous command. If the command is still in progress, returns STATUS_BUSY.
 *
 * Implements    : HSM_DRV_GetAsyncCmdStatus_Activity
 * END**************************************************************************/
status_t HSM_DRV_GetAsyncCmdStatus(void)
{
    if (!s_hsmStatePtr->cmdInProgress)
    {
        return s_hsmStatePtr->cmdStatus;
    }

    return STATUS_BUSY;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_CancelCommand
 * Description   : Cancels a previously initiated command.
 *
 * Implements    : HSM_DRV_CancelCommand_Activity
 * END**************************************************************************/
status_t HSM_DRV_CancelCommand(void)
{
    /* Check the driver is initialized */
    DEV_ASSERT(s_hsmStatePtr != NULL);

    /* Set the blocking flag so the synchronization semaphore is posted in the ISR */
    s_hsmStatePtr->blockingCmd = true;

    /* Prepare the command */
    HSM_PrepareCancelCmd();

    /* Send the command to HSM */
    HSM_SendCmd();

    /* Wait for the cancelled command to complete */
    (void)OSIF_SemaWait(&s_hsmStatePtr->cmdComplete, 1U);

    /* Clear the blocking flag */
    s_hsmStatePtr->blockingCmd = false;

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_IRQHandler
 * Description   : Implementation of the HSM interrupt handler. Handles completed
 * command events.
 *
 * END**************************************************************************/
void HSM_DRV_IRQHandler(void)
{
    if (s_hsmStatePtr->cmdInProgress)
    {
        /* Wait for the BUSY flag to be cleared by hw */
        while (HSM_IsBusy()) {}

        /* Retrieve the error code of last command */
        uint32_t err = HSM_GetErrCode();

        /* Update the internal driver status */
        if (err == 0U)
        {
            s_hsmStatePtr->cmdStatus = STATUS_SUCCESS;
        }
        else
        if (err == 0xBU)
        {
            s_hsmStatePtr->cmdStatus = STATUS_BUSY;
        }
        else
        {
            s_hsmStatePtr->cmdStatus = HSM_CONVERT_ERC(err);
        }

        /* If the command was VERIFY_MAC, retrieve the result of the verification */
        if ((s_hsmStatePtr->cmd == HSM_CMD_VERIFY_MAC) && (s_hsmStatePtr->macVerifStatus != NULL))
        {
            *s_hsmStatePtr->macVerifStatus = HSM_GetMacVerifResult();
            s_hsmStatePtr->macVerifStatus = NULL;
        }

        /* Call the user callback, if available */
        if (s_hsmStatePtr->callback != NULL)
        {
            s_hsmStatePtr->callback((uint32_t)s_hsmStatePtr->cmd, s_hsmStatePtr->callbackParam);
        }

        if (s_hsmStatePtr->blockingCmd)
        {
            /* Update the internal blocking flag */
            s_hsmStatePtr->blockingCmd = false;

            /* Update the synchronization object */
            (void)OSIF_SemaPost(&s_hsmStatePtr->cmdComplete);
        }

        /* Update the internal busy flag */
        s_hsmStatePtr->cmdInProgress = false;
        /* No command in execution at this point */
        s_hsmStatePtr->cmd = HSM_CMD_NONE;
    }

    /* Clear the interrupt flag */
    HSM_ClearIntFlag();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_WaitCommandCompletion
 * Description   : Waits on the synchronization semaphore and updates the flags.
 *
 * END**************************************************************************/
static void HSM_DRV_WaitCommandCompletion(uint32_t timeout)
{
    status_t syncStatus;

    /* Wait for command completion */
    syncStatus = OSIF_SemaWait(&s_hsmStatePtr->cmdComplete, timeout);

    /* Update the busy flag and status if timeout expired */
    if (syncStatus == STATUS_TIMEOUT)
    {
        (void)HSM_DRV_CancelCommand();
        s_hsmStatePtr->blockingCmd = false;
        s_hsmStatePtr->cmdStatus = STATUS_TIMEOUT;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_CopyBuff
 * Description   : Copies data from source to destination buffer.
 *
 * END**************************************************************************/
static void HSM_DRV_CopyBuff(const uint8_t * srcBuff, uint8_t * destBuff, uint32_t len)
{
    uint32_t idx;
    for (idx = 0U; idx < len; idx++)
    {
        destBuff[idx] = srcBuff[idx];
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_DRV_XorBuff
 * Description   : Performs bitwise XOR between input buffers and stores the
 * result in the output buffer.
 *
 * END**************************************************************************/
static void HSM_DRV_XorBuff(const uint8_t * inBuff, uint8_t * outBuff, uint32_t len)
{
    uint32_t idx;
    for (idx = 0U; idx < len; idx++)
    {
        outBuff[idx] ^= inBuff[idx];
    }
}

/******************************************************************************
 * EOF
 *****************************************************************************/
