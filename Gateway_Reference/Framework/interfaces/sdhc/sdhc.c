/*
 * sdhc.c
 *
 *  Created on: 6 Feb 2018
 *      Author: B49855
 */
#include "sdhc.h"
#include "config.h"
#include "status.h"


void SD_initialise()
{
	status_t stat;

	/* Mount SD card to the device */
	stat = SD_initCard();

    /* Copy contents of ECU list held in RAM to SD card */
	stat = SD_storeAllECU();

	/* Copy contents of ECU list held in SD card to RAM */
	/* stat = SD_LoadAllECU(); */

	/* Build list of ECUs by loading ECU data from SD card, and querying ECUs for latest information */
	/* ECU_initList(); */
}


/*
 * @brief: Initialise SD card by mounting FAT FS
 * @return : STATUS_SUCCESS, STATUS_ERROR
 */
status_t SD_initCard()
{
    uSDHC_fatfs_user_initialize();

	/* File function return code */
	FRESULT status;
	TCHAR string[] = DRIVE0;

	/* Immediate mount. */
	status = f_mount(&fs,string, 1U);
	return status;
}

/*
 * @brief: Store ECU info from RAM buffer (ecu_list) to SD card
 * @param ecu : Pointer to ECU struct
 * @return    : STATUS_SUCCESS, STATUS_ERROR
 */
status_t SD_storeECU(uint8_t ecu_id)
{
	FIL fdst;
	FRESULT res;
	UINT bw;

	char path[50];

	ECU *ecu = ECU_getECU(ecu_id);

	sprintf(path, "%d", ecu_id); /* Open ECU info file */
	res = f_mkdir(path); /* Create directory if it does not exist already */

	sprintf(path, "%d/%d.bin", ecu_id, ecu_id); /* Open ECU info file */
	res = f_open(&fdst, path, FA_CREATE_ALWAYS | FA_WRITE);

	if(res == FR_OK) {

		res = f_write(&fdst, ecu, sizeof(ECU), &bw); /* Write ecu_info struct to bin file */

		if(bw < sizeof(ECU) || res != FR_OK) {
			return STATUS_ERROR;
		}

		f_close(&fdst);
	}

	return STATUS_SUCCESS;
}

/*
 * @brief: Load ECU info from SD card to RAM buffer (ecu_list)
 * @param ecu : Pointer to ECU struct
 * @return    : STATUS_SUCCESS, STATUS_ERROR
 */
status_t SD_loadECU(uint8_t ecu_id)
{
	FIL fsrc;
	FRESULT res;
	UINT br;
	char path[50];
	ECU *ecu = &ecu_list[ecu_id];

	sprintf(path, "%d/%d.bin", ecu_id, ecu_id);

	res = f_open(&fsrc, path, FA_OPEN_EXISTING | FA_READ);

	if(res == FR_OK)
	{
		/* Load ECU info directly into ecu_info struct */
		res = f_read(&fsrc, ecu, sizeof(ECU), &br);
		f_close(&fsrc);

		if(br != sizeof(ECU) || res != FR_OK) {
			printf("ERROR: ECU %d. (Status Code: %03x) \n\r", ecu_id, res);
			return STATUS_ERROR;
		}

	} else return STATUS_ERROR;

	return STATUS_SUCCESS;
}

/*
 * @brief: Store whole list of ECUs in RAM buffer onto SD
 * @return : STATUS_SUCCESS, STATUS_ERROR
 */
status_t SD_storeAllECU()
{
	uint32_t i;
	status_t stat = STATUS_SUCCESS;

	for(i = 0 ; i < NUM_OF_ECUS ; i++)
	{
		if(SD_storeECU(i) != STATUS_SUCCESS)
		{
			stat = STATUS_ERROR;
		}
	}
	return stat;
}

/*
 * @brief: Load all ECUs found on SD card into RAM buffer (ecu_list)
 * @return : STATUS_SUCCESS, STATUS_ERROR
 */
status_t SD_loadAllECU()
{
	/* Open FW file */
	char path[50];
	FRESULT res;
	FIL fsrc;
	UINT br;

	uint32_t current_ecu = 0;
	uint32_t next_ecu = current_ecu + 1;

	while(1)
	{
		sprintf(path, "%d/%d.bin", current_ecu, current_ecu);
		res = f_open(&fsrc, path, FA_OPEN_EXISTING | FA_READ);

		if(FR_OK == res)
		{
			f_read(&fsrc, &ecu_list[current_ecu], sizeof(ecu_list[0]), &br);
			f_close(&fsrc);

			next_ecu++; /* Found file, try searching for newer file */
		}
		else if(FR_NO_FILE == res) break;
	}
	return STATUS_SUCCESS;
}

///*
// * @brief: Loads all ECU info held on SD into RAM buffer (ecu_list)
// * @return : STATUS_SUCCESS, STATUS_ERROR
// */
//status_t SD_LoadAllECUIDs()
//{
//	FRESULT res;
//	FILINFO fno;
//	DIR dir;
//
//	char buff[30];
//	char *ptr;
//
//	uint32_t no_ecus = 0;
//
//	res = f_opendir(&dir, "/");
//	if(res == FR_OK)
//	{
//		f_readdir(&dir, &fno);
//
//		while(&fno.fname[0] != 0) /* Loop until no more items found */
//		{
//			strncpy(&buff, &fno.fname, sizeof(fno.fname));
//
//			uint8_t ecu_id = strtol(&buff, ptr, 10);
//
//			ecu_list[no_ecus].id = strtol(&buff, ptr, 10);
//
//			f_readdir(&dir, &fno);
//			no_ecus++;
//		}
//
//		f_closedir(&dir);
//	}
//	return STATUS_SUCCESS;
//}

/*
 * @brief: Open latest FW file for a given ECU
 * @param ecu : Pointer to ECU struct
 * @param fw  : Pointer to FIL object to reference
 * @return    : FRESULT
 */
//FRESULT SD_OpenLatestFW(ECU *ecu, FIL *fw)
//{
//	return SD_OpenFW(ecu, ecu->fw_ver_new, fw);
//}

/*
 * @brief: Open a given ECU FW file based on version
 * @param ecu : Pointer to ECU struct
 * @param ver : Version of FW to open
 * @param fw  : Pointer to FIL object to reference
 * @return    : FRESULT
 */
FRESULT SD_openFW(uint8_t ecu_id, uint32_t ver, FIL *fw)
{
	char path[30];
	FRESULT res;

	sprintf(path, "0:%d/%d_fw_%d.bin", ecu_id, ecu_id, ver);
	res = f_open(fw, path, FA_OPEN_EXISTING | FA_READ);
	return res;
}

FRESULT SD_openLatestFW(uint8_t ecu_id, FIL *fw)
{

	ECU *ecu = &ecu_list[ecu_id];

	FRESULT res;
	uint32_t current_ecu_ver = ecu->fw_version;
	uint32_t ver_pointer = current_ecu_ver + 1;

	while(1)
	{
		/* Open FW file */
		res = SD_openFW(ecu_id, ver_pointer, fw);

		if(FR_OK == res)
		{
			ver_pointer++; /* Found file, try searching for newer file */
			f_close(fw);
		}
		else if(FR_NO_FILE == res)
		{
			ver_pointer--; /* No newer file, move pointer back to latest file */
			break;
		}
	}

	if(ver_pointer >= current_ecu_ver) {
		res = SD_OpenFW(ecu_id, ver_pointer, fw);
	}

	return res;
}

void uSDHC_fatfs_user_initialize(void)
{
	uSDHC_fatfs_initialize();
	g_fatfsCallbacks[FATFS_DISKIO_READ]
		.postFunction = FatFs_DiskIo_Read_Callback;
	g_fatfsCallbacks[FATFS_DISKIO_WRITE]
		.postFunction = FatFs_DiskIo_Write_Callback;
}

void FatFs_DiskIo_Read_Callback(uint32_t status, void *param)
{
//	(void)param;
//	if(STATUS_SUCCESS == status)
//	{
//		PINS_DRV_TogglePins(PTA, (pins_channel_type_t)(1U << 7));
//	}
}

void FatFs_DiskIo_Write_Callback(uint32_t status, void *param)
{
//	(void)param;
//	if(STATUS_SUCCESS == status)
//	{
//		PINS_DRV_TogglePins(PTA, (pins_channel_type_t)(1U << 10));
//	}
}

