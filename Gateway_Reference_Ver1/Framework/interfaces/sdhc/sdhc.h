/*
 * sdhc.h
 *
 *  Created on: 6 Feb 2018
 *      Author: B49855
 */
#ifndef SDHC_H_
#define SDHC_H_

#include "usdhc1.h"
#include "fatfs/usdhc_fatfs_impl.h"
#include "common/fatfs_impl.h"
#include "ff.h"
#include "ecu.h"

void SD_initialise();
FRESULT SD_initCard();
status_t SD_storeECU(uint8_t ecu_id);
status_t SD_loadECU(uint8_t ecu_id);
status_t SD_storeAllECU();
status_t SD_loadAllECU();
FRESULT SD_openLatestFW(uint8_t ecu_id, FIL *fw);
FRESULT SD_openFW(uint8_t ecu_id, uint32_t ver, FIL *fw);

void FatFs_DiskIo_Read_Callback(uint32_t status, void *param);
void FatFs_DiskIo_Write_Callback(uint32_t status, void *param);
void uSDHC_fatfs_user_initialize(void);

#define DRIVE0 "0:"
#define DRIVE0_INST (0U)

FATFS fs;

#endif
