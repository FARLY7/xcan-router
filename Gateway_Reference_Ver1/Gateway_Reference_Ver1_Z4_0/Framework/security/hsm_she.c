/*
 * Copyright (c) 2015 - 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
/*
 * HSM_utils.c
 *
 *  Created on: Nov 10, 2016
 *      Author: B50609
 */

#include <security_pal.h>
#include <stdint.h>
#include <stdbool.h>

#include "hsm_she.h"
#include "config.h"

/* AuthId is the MASTER_ECU key */
uint8_t g_emptyKey[16] 		 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t g_authIdKey[16]		 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
/* Constants defined by the SHE spec */
uint8_t key_update_enc_c[16] = {0x01, 0x01, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};
uint8_t key_update_mac_c[16] = {0x01, 0x02, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};
uint8_t key_debug_key_c[16]  = {0x01, 0x03, 0x53, 0x48, 0x45, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb0};
security_key_id_t g_authId = SECURITY_MASTER_ECU;

/* Global variables */
static security_user_config_t g_tSecurityUserConfig;


void HSM_initialise()
{
	uint8_t BOOT_MAC_KEY[16] =	BOOT_MAC_KEY_VALUE;
	uint8_t KEY_1[16]	 	 =	KEY_1_VALUE;

	SECURITY_Init(SECURITY_INSTANCE0, &g_tSecurityUserConfig);

	/* Set the AuthID key (MASTER_ECU_KEY) for the first time */
	HSM_SetAuthKey();

    /* Load key into HSM_KEY_1 slot */
	HSM_LoadKey(SECURITY_KEY_1, KEY_1, 3); /* 3 = counter value */

    /* Load key into HSM_BOOT_MAC_KEY slot */
    HSM_LoadKey(SECURITY_BOOT_MAC_KEY, BOOT_MAC_KEY, 2);
}

/* Derives a key with a given constant */
status_t HSM_DeriveKey(const uint8_t *key, uint8_t *constant, uint8_t *derivedKey)
{
    uint8_t concat[32];
    int i;

    for (i = 0; i < 16; i++)
    {
        concat[i] = key[i];
        concat[i+16] = constant[i];
    }

    return SECURITY_MPCompress(SECURITY_INSTANCE0, concat, 32U, derivedKey, 500U);
}

/* Computes the M1-M3 values */
status_t HSM_ComputeM1M2M3(uint8_t *authKey, security_key_id_t authId, security_key_id_t keyId, const uint8_t *key, uint32_t counter,
                                uint8_t *uid, uint8_t *m1, uint8_t *m2, uint8_t *m3)
{
    status_t stat;
    int i;
    uint8_t iv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint8_t k1[16];
    uint8_t k2[16];

    uint8_t m2Plain[32];

    uint8_t m1m2[48];

    /* Derive K1 and K2 from AuthID */
    HSM_DeriveKey(authKey, key_update_enc_c, k1);
    HSM_DeriveKey(authKey, key_update_mac_c, k2);

    /* Compute M1 = UID | ID | AuthID */
    for (i = 0; i < 15; i++)
    {
        m1[i] = uid[i];
    }
    m1[15] = ((keyId & 0xF) << 4) | (authId & 0xF);

    /* Compute M2 (C = counter, F = 0) */
    for(i = 0; i < 16; i++)
    {
        m2Plain[i] = 0;
        m2Plain[16 + i] = key[i];
    }
    m2Plain[0] = (counter & 0xFF00000) >> 20;
    m2Plain[1] = (counter & 0xFF000) >> 12;
    m2Plain[2] = (counter & 0xFF0) >> 4;
    m2Plain[3] = (counter & 0xF) << 4;

    /* Encrypt M2 */
    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, k1, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = SECURITY_EncryptCbcBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, m2Plain, 32U, iv, m2, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    /* Compute M3 as CMAC(key=k2, m1|m2)*/
    for (i = 0; i < 16; i++)
    {
        m1m2[i] = m1[i];
    }
    for(i = 0; i < 32; i++)
    {
        m1m2[16 + i] = m2[i];
    }

    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, k2, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = SECURITY_GenerateMacBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, m1m2, 384U, m3, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    return STATUS_SUCCESS;
}

/* Computes the M4 and M5 values */
status_t HSM_ComputeM4M5(security_key_id_t authId, security_key_id_t keyId, const uint8_t *key, uint32_t counter,
                                uint8_t *uid, uint8_t *m4, uint8_t *m5)
{
    status_t stat;
    int i;

    uint8_t k3[16];
    uint8_t k4[16];

    uint8_t m4StarPlain[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m4StarCipher[16];

    /* Derive K4 and K5 from key ID */
    HSM_DeriveKey(key, key_update_enc_c, k3);
    HSM_DeriveKey(key, key_update_mac_c, k4);

    m4StarPlain[0] = (counter & 0xFF00000) >> 20;
    m4StarPlain[1] = (counter & 0xFF000) >> 12;
    m4StarPlain[2] = (counter & 0xFF0) >> 4;
    m4StarPlain[3] = ((counter & 0xF) << 4) | 0x8;

    /* Encrypt M4* */
    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, k3, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = SECURITY_EncryptEcbBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, m4StarPlain, 16U, m4StarCipher, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    /* Compute M4 = UID | ID | AuthID | M4* */
    for (i = 0; i < 15; i++)
    {
        m4[i] = uid[i];
    }
    m4[15] = ((keyId & 0xF) << 4) | (authId & 0xF);
    for (i = 0; i < 16; i++)
    {
        m4[16 + i] = m4StarCipher[i];
    }

    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, k4, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    stat = SECURITY_GenerateMacBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, m4, 256U, m5, 500U);
    if (stat != STATUS_SUCCESS)
        return stat;

    return STATUS_SUCCESS;
}

/* Sets the AuthID key (MASTER_ECU_KEY) for the first time */
bool HSM_SetAuthKey(void)
{
    uint8_t uid[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m1[16];
    uint8_t m2[32];
    uint8_t m3[16];
    uint8_t m4[32];
    uint8_t m5[16];

    status_t stat;

    stat = HSM_ComputeM1M2M3(g_emptyKey, g_authId, SECURITY_MASTER_ECU, g_authIdKey, 1, uid, m1, m2, m3);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = SECURITY_LoadKey(SECURITY_INSTANCE0, SECURITY_MASTER_ECU, m1, m2, m3, m4, m5, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    return true;
}

/* Extracts the UID. */
bool HSM_GetUID(uint8_t *uid)
{
    status_t stat;
    uint8_t challenge[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    uint8_t sreg;
    uint8_t mac[16];
    uint8_t verif[32];
    bool verifStatus;
    uint8_t i;

    stat = SECURITY_GetId(SECURITY_INSTANCE0, challenge, uid, &sreg, mac, 1U);
    if (stat != STATUS_SUCCESS)
        return false;

    for (i = 0; i < 16; i++) {
        verif[i] = challenge[i];
    }
    for (i = 0; i < 15; i++) {
        verif[16 + i] = uid[i];
    }
    verif[31] = sreg;

    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, g_authIdKey, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = SECURITY_VerifyMacBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, verif, 256U, mac, 128U, &verifStatus, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    return verifStatus;
}

/* Erases all the keys. */
bool HSM_EraseKeys(void)
{
    status_t stat;
    uint8_t challenge[16];
    uint8_t auth[16];
    uint8_t authPlain[31];
    uint8_t k[16];
    uint8_t uid[15];

    uint8_t i;

    SECURITY_InitRng(SECURITY_INSTANCE0, 500u);

    HSM_GetUID(uid);

    HSM_DeriveKey(g_authIdKey, key_debug_key_c, k);

    stat = SECURITY_LoadPlainKey(SECURITY_INSTANCE0, k, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = SECURITY_DbgChal(SECURITY_INSTANCE0, challenge, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    for (i = 0; i < 16; i++)
    {
        authPlain[i] = challenge[i];
    }
    for (i = 0; i < 15; i++)
    {
        authPlain[i + 16] = uid[i];
    }

    stat = SECURITY_GenerateMacBlocking(SECURITY_INSTANCE0, SECURITY_RAM_KEY, authPlain, 248U, auth, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    stat = SECURITY_DbgAuth(SECURITY_INSTANCE0, auth, 500U);
    if (stat != STATUS_SUCCESS)
        return false;

    return true;
}

/* Loads/updates a non-volatile key. */
bool HSM_LoadKey(security_key_id_t keyId, uint8_t *keyNew, uint8_t counter)
{
    uint8_t uid[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t m1[16];
    uint8_t m2[32];
    uint8_t m3[16];
    uint8_t m4[32];
    uint8_t m5[16];

    status_t stat;

    stat = HSM_ComputeM1M2M3(g_authIdKey, g_authId, keyId, keyNew, counter, uid, m1, m2, m3);
    if (stat != STATUS_SUCCESS)
        return false;


    stat = SECURITY_LoadKey(SECURITY_INSTANCE0, keyId, m1, m2, m3, m4, m5, 500u);
    if (stat != STATUS_SUCCESS)
        return false;

    return true;
}
