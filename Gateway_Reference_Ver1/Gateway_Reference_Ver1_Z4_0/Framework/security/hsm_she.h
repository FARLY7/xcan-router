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
 * csec_utils.h
 *
 *  Created on: Nov 10, 2016
 *      Author: B50609
 */

#ifndef HSM_SHE_H_
#define HSM_SHE_H_

//#include "hsm_driver.h"

/* This function sets the MASTER_ECU key with a key (g_authIdKey) defined in the
 * csec_utils.c file. This key will be used as an authorization secret for updating
 * user keys.
 * Setting the MASTER_ECU key will work only for the first time, in order to use
 * another value for the key, there are two options:
 * - erase the keys and then update g_authIdKey;
 * - use loadKey with counter > 1 and then update g_authIdKey;
 * */
bool HSM_SetAuthKey(void);

/* This function erases all the key. After using it, the Flash needs to be partitioned
 * again.
 * */
bool HSM_EraseKeys(void);

/* This function loads/updates a non-volatile key.
 * When updating the key, the counter needs to be greater then the previous one.
 * */
bool HSM_LoadKey(security_key_id_t keyId, uint8_t *keyNew, uint8_t counter);

///* Extracts the UID. */
//bool HSM_GetUID(uint8_t *uid);
//
///* Derives a key with a given constant */
//status_t HSM_DeriveKey(const uint8_t *key, uint8_t *constant, uint8_t *derivedKey);
//
///* Computes the M1-M3 values */
//status_t HSM_ComputeM1M2M3(uint8_t *authKey, hsm_key_id_t authId, hsm_key_id_t keyId, const uint8_t *key, uint32_t counter,
//                                uint8_t *uid, uint8_t *m1, uint8_t *m2, uint8_t *m3);
//
///* Computes the M4 and M5 values */
//status_t HSM_ComputeM4M5(hsm_key_id_t authId, hsm_key_id_t keyId, const uint8_t *key, uint32_t counter,
//                                uint8_t *uid, uint8_t *m4, uint8_t *m5);


void HSM_initialise(void);

#endif /* SOURCES_CSEC_UTILS_H_ */

