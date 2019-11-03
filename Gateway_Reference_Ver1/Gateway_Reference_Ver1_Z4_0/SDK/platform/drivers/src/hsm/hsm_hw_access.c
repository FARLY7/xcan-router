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

#include "hsm_hw_access.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Static structure holding HSM command to be passed to security
 * firmware.
 */
static hsm_fw_command_t s_cmd;

/*! @brief Static variable storing the 64-bits message length; a reference to this
 * variable is passed to HSM for 'generate MAC' command;
 */
static uint64_t s_msgLen;

/*! @brief Static variable storing the 8-bits MAC length; a reference to this
 * variable is passed to HSM for 'verify MAC' command;
 */
static uint32_t s_macLen;


/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareCommand
 * Description   : Prepares the HSM command structure.
 *
 *END**************************************************************************/
void HSM_PrepareCommand(uint32_t cmd,
                        uint32_t param1,
                        uint32_t param2,
                        uint32_t param3,
                        uint32_t param4,
                        uint32_t param5)
{
    s_cmd.CMD = cmd;
    s_cmd.PARAM_1 = param1;
    s_cmd.PARAM_2 = param2;
    s_cmd.PARAM_3 = param3;
    s_cmd.PARAM_4 = param4;
    s_cmd.PARAM_5 = param5;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareEncryptEcbCmd
 * Description   : Prepares the HSM ECB encrypt command.
 *
 *END**************************************************************************/
void HSM_PrepareEncryptEcbCmd(hsm_key_id_t keyId, const uint8_t *plainText,
                              uint32_t length, uint8_t *cipherText)
{
    uint32_t cmd, cmdKeyId;

    /* Set ECB encryption command */
    cmd = (uint32_t)HSM_CMD_ENC_ECB;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    HSM_PrepareCommand(cmd, cmdKeyId, HSM_BUFF_BLOCK_COUNT(length), (uint32_t)plainText,
                       (uint32_t)cipherText, HSM_UNUSED_PARAM);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareDecryptEcbCmd
 * Description   : Prepares the HSM ECB decrypt command.
 *
 *END**************************************************************************/
void HSM_PrepareDecryptEcbCmd(hsm_key_id_t keyId, const uint8_t *cipherText,
                              uint32_t length, uint8_t *plainText)
{
    uint32_t cmd, cmdKeyId;

    /* Set ECB decryption command */
    cmd = (uint32_t)HSM_CMD_DEC_ECB;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    HSM_PrepareCommand(cmd, cmdKeyId, HSM_BUFF_BLOCK_COUNT(length), (uint32_t)cipherText,
                       (uint32_t)plainText, HSM_UNUSED_PARAM);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareEncryptCbcCmd
 * Description   : Prepares the HSM CBC encrypt command.
 *
 *END**************************************************************************/
void HSM_PrepareEncryptCbcCmd(hsm_key_id_t keyId, const uint8_t *plainText,
                              uint32_t length, const uint8_t *iv, uint8_t *cipherText)
{
    uint32_t cmd, cmdKeyId;

    /* Set CBC encryption command */
    cmd = (uint32_t)HSM_CMD_ENC_CBC;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    HSM_PrepareCommand(cmd, cmdKeyId, (uint32_t)iv, HSM_BUFF_BLOCK_COUNT(length),
                       (uint32_t)plainText, (uint32_t)cipherText);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareDecryptCbcCmd
 * Description   : Prepares the HSM CBC decrypt command.
 *
 *END**************************************************************************/
void HSM_PrepareDecryptCbcCmd(hsm_key_id_t keyId, const uint8_t *cipherText,
                              uint32_t length, const uint8_t *iv, uint8_t *plainText)
{
    uint32_t cmd, cmdKeyId;

    /* Set CBC decryption command */
    cmd = (uint32_t)HSM_CMD_DEC_CBC;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    HSM_PrepareCommand(cmd, cmdKeyId, (uint32_t)iv, HSM_BUFF_BLOCK_COUNT(length),
                       (uint32_t)cipherText, (uint32_t)plainText);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareGenerateMacCmd
 * Description   : Prepares the HSM generate mac command.
 *
 *END**************************************************************************/
void HSM_PrepareGenerateMacCmd(hsm_key_id_t keyId, const uint8_t *msg,
                               uint64_t msgLen, uint8_t *mac)
{
    uint32_t cmd, cmdKeyId;

    /* Set MAC generate command */
    cmd = (uint32_t)HSM_CMD_GENERATE_MAC;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    /* Save the message length in the internal driver variable */
    s_msgLen = msgLen;

    HSM_PrepareCommand(cmd, cmdKeyId, (uint32_t)(&s_msgLen),
                       (uint32_t)msg, (uint32_t)mac, HSM_UNUSED_PARAM);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareVerifyMacCmd
 * Description   : Prepares the HSM verify mac command.
 *
 *END**************************************************************************/
void HSM_PrepareVerifyMacCmd(hsm_key_id_t keyId, const uint8_t *msg,
                             uint64_t msgLen, const uint8_t *mac, uint8_t macLen)
{
    uint32_t cmd, cmdKeyId;

    /* Set MAC verify command */
    cmd = (uint32_t)HSM_CMD_VERIFY_MAC;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }
    /* Get the command key ID (strip the KBS bit) */
    cmdKeyId = HSM_CMD_KEY_ID(keyId);

    /* Save the message length in the internal driver variable */
    s_msgLen = msgLen;
    /* Save the MAC length in the internal driver variable */
    s_macLen = macLen;

    HSM_PrepareCommand(cmd, cmdKeyId, (uint32_t)(&s_msgLen),
                       (uint32_t)msg, (uint32_t)mac, (uint32_t)(&s_macLen));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_PrepareLoadKeyCmd
 * Description   : Prepares the HSM load key command.
 *
 *END**************************************************************************/
void HSM_PrepareLoadKeyCmd(hsm_key_id_t keyId, const uint8_t *m1, const uint8_t *m2,
                           const uint8_t *m3, uint8_t *m4, uint8_t *m5)
{
    uint32_t cmd;

    /* Set load key command */
    cmd = (uint32_t)HSM_CMD_LOAD_KEY;
    /* Check the key bank and update command accordingly */
    if (HSM_CMD_KBS(keyId) != 0U)
    {
        cmd |= HSM_CMD_KBS_MASK;
    }

    HSM_PrepareCommand(cmd, (uint32_t)m1, (uint32_t)m2, (uint32_t)m3,
                       (uint32_t)m4, (uint32_t)m5);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_SendCmd
 * Description   : Sends the command to HSM.
 *
 *END**************************************************************************/
void HSM_SendCmd(void)
{
    /* Write the command structure address in the HSM register */
    HSM->HT2HSMS = ((uint32_t)(&s_cmd));
    /* Send the command to HSM */
    HSM->HT2HSMF |= HT2HSMF_CMD_INT_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : HSM_GetMacVerifResult
 * Description   : Returns the result of the last MAC verification.
 *
 *END**************************************************************************/
bool HSM_GetMacVerifResult(void)
{
    return (s_macLen == 0U);
}


/*******************************************************************************
 * EOF
 ******************************************************************************/
