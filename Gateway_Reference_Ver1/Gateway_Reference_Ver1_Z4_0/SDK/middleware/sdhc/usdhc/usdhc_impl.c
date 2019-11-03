/*****************************************************************************/
/* FILE NAME: usdhc_impl.c         COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: uSDHC implementation                                         */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "usdhc_driver.h"
#include "usdhc1.h"

#include "osif.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "usdhc/usdhc_impl.h"
#include "sd/card.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer
 * to object type and a pointer to a different object type.
 * This cast is for DEV_ASSERT macro condition.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.9, Conversion from pointer to void into pointer to object.
 * This is need for DEV_ASSERT macro condition.
 *
 */

/*******************************************************************************
* Constants and macros
*******************************************************************************/

#define SDC_INSTANCE0                   (0U)
#define uSDHC_TC_TIMEOUT_MS	            (5000U)

/*******************************************************************************
* Global variables
*******************************************************************************/

/*! @brief uSDHC transfer complete (TC) state (0 - no TC, 1 - TC, 2 - error in transfer) */
volatile usdhc_tc_state_t g_usdhcTransferComplete = uSDHC_TC_STATE_ERROR;
/*! @brief uSDHC presence status (0 - no card inserted, 1 - card is present) */
volatile usdhc_ps_state_t g_usdhcPresenceStatus = uSDHC_PS_STATE_NO_CARD;
/*! @brief uSDHC initialization flag (0 - not initialized, 1 - initialized) */
volatile usdhc_init_state_t g_usdhcInitialized = uSDHC_INIT_STATE_NOT_INITIALIZED;
sd_card_t g_sd;
semaphore_t g_TransferCompleteSem;

/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/

static void uSDHC_CopyHostConfig(sd_card_t *card, const usdhc_config_t *usdhc_config);

static void uSDHC_SwitchCardDetectDat3(sd_card_t *card, bool flag);

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/

static void uSDHC_CopyHostConfig(sd_card_t *card, const usdhc_config_t *usdhc_config)
{
	(void)card;
	(void)usdhc_config;
	DEV_ASSERT(NULL != card);
	DEV_ASSERT(NULL != usdhc_config);

	usdhc_config_t *card_config = &card->host.config;

    (void)memset((void *)card_config, 0U, sizeof(usdhc_config_t));
	/*
	 * Card detection isn't implemented in SD layer,
	 * we are using usdhc driver' uSDHC_DRV_GetPresentStatusFlags().
	 */
	card_config->cardDetectDat3 = false;
	card_config->endianMode = usdhc_config->endianMode;
	/* The rest is to be filled in sync with usdhc_config_t structure. */
	card_config->dmaMode = usdhc_config->dmaMode;
	card_config->admaTable = usdhc_config->admaTable;
	card_config->admaTableSize = usdhc_config->admaTableSize;
}

static void uSDHC_SwitchCardDetectDat3(sd_card_t *card, bool flag)
{
	(void)card;
    uSDHC_Type *base = g_usdhcBases[card->host.instance];
    uint32_t protctl = 0;

    protctl = base->PROT_CTRL;

    protctl &= ~(uint32_t)uSDHC_PROT_CTRL_D3CD_MASK;
    if(true == flag)
    {
        protctl |= uSDHC_PROT_CTRL_D3CD_MASK;
    }

    base->PROT_CTRL = protctl;
}

/*******************************************************************************
* Global functions
*******************************************************************************/

/*
 * Implements uSDHC_IsSDCardWriteProtect_Activity
 */
bool uSDHC_IsSDCardWriteProtect(void)
{
    const sd_card_t *card = &g_sd;
	(void)card;

    return (0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_WRITE_PROTECTED));
}

/*
 * Implements uSDHC_IsSDCardPresent_Activity
 */
bool uSDHC_IsSDCardPresent(void)
{
    const sd_card_t *card = &g_sd;
    bool status = true;
    (void)card;

    if(0U == (uSDHC_DRV_GetPresentStatusFlags(card->host.instance) & (uint32_t)uSDHC_CARD_INSERTED))
    {
        status = false;
    }
    else
    {
        g_usdhcPresenceStatus = uSDHC_PS_STATE_CARD_INSERTED;
    }
    return status;
}

/*
 * Implements uSDHC_GetSDCard_Activity
 */
sd_card_t *uSDHC_GetSDCard(void)
{
    sd_card_t *card = &g_sd;
    /* Wait until card presence state is OK. */
    while(false == uSDHC_IsSDCardPresent()) {}
    return card;
}

/*
 * Implements uSDHC_Init_Activity
 */
common_status_t uSDHC_Init(bool wouldBlock)
{
    common_status_t status = (common_status_t)STATUS_ERROR;
    void* p = &g_sd;

    sd_card_t *card;
    usdhc_config_t *usdhc_config;

    /* Reset if has been already initialized. */
    (void)uSDHC_DeInit();

    (void)memset(p, 0U, sizeof(sd_card_t));

    card = &g_sd;
    usdhc_config = &usdhc1_Config0;
	(void)OSIF_SemaCreate(&g_TransferCompleteSem, 0U);
    /* PEX generated configuration. */
    (void)uSDHC_DRV_Init(SDC_INSTANCE0, &usdhc1_State, usdhc_config);
    uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_TRANSFER_COMPLETE, uSDHC_TransferCompleteCallback, NULL);
    uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_CARD_INSERT, uSDHC_CardInsertCallback, NULL);
    uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_CARD_REMOVE, uSDHC_CardRemoveCallback, NULL);

    /* Save host information. */
    card->host.instance = SDC_INSTANCE0;
    card->host.transfer = uSDHC_TransferFunction;
    uSDHC_CopyHostConfig(card, usdhc_config);
    /* Now get card instance and check for card presence. */
    if(true == wouldBlock)
    {
    	while(false == uSDHC_IsSDCardPresent()) {}
    	status = (common_status_t)STATUS_SUCCESS;
    }
    else
    {
    	if(true == uSDHC_IsSDCardPresent())
    	{
        	status = (common_status_t)STATUS_SUCCESS;
    	}
    }

    if((common_status_t)STATUS_SUCCESS == status)
    {
    	/* Initialize SD card. */
    	do
    	{
    		status = SD_Init(card);
    		if((common_status_t)STATUS_SUCCESS == status)
    		{
    			break;
    		}
    	}while(false == uSDHC_IsSDCardPresent());
    }
    uSDHC_SwitchCardDetectDat3(card, false);
	g_usdhcInitialized = uSDHC_INIT_STATE_INITIALIZED;
    return status;
}

/*
 * Implements uSDHC_DeInit_Activity
 */
common_status_t uSDHC_DeInit(void)
{
    common_status_t stdRet = (common_status_t)STATUS_ERROR;
    sd_card_t *card;
    if(uSDHC_INIT_STATE_INITIALIZED == g_usdhcInitialized)
    {
        card = uSDHC_GetSDCard();
        if(NULL != card)
        {
             stdRet = SD_DeInit(card);
             if((common_status_t)STATUS_SUCCESS == stdRet)
             {
                  (void)uSDHC_DRV_Deinit(SDC_INSTANCE0);
                  (void)uSDHC_DRV_Reset(SDC_INSTANCE0, (uSDHC_SYS_CTRL_RSTC_MASK | uSDHC_SYS_CTRL_RSTD_MASK), 2U);
                  uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_TRANSFER_COMPLETE, NULL, NULL);
                  uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_CARD_INSERT, NULL, NULL);
                  uSDHC_DRV_InstallCallback(SDC_INSTANCE0, uSDHC_EVENT_CARD_REMOVE, NULL, NULL);
             }
        }
		(void)OSIF_SemaDestroy(&g_TransferCompleteSem);
        g_usdhcInitialized = uSDHC_INIT_STATE_NOT_INITIALIZED;
    }
    return stdRet;
}

/*
 * Implements uSDHC_TransferCompleteCallback_Activity
 */
void uSDHC_TransferCompleteCallback(uint32_t instance, uint32_t status, void *userData)
{
    (void)instance;
    (void)userData;
    if((uint32_t)STATUS_SUCCESS == status)
    {
        g_usdhcTransferComplete = uSDHC_TC_STATE_DONE;
    }
    else
    {
    	g_usdhcTransferComplete = uSDHC_TC_STATE_ERROR;
    }
	(void)OSIF_SemaPost(&g_TransferCompleteSem);
}

/*
 * Implements uSDHC_TransferFunction_Activity
 */
status_t uSDHC_TransferFunction(uint32_t instance, usdhc_transfer_t *content)
{
	status_t stdRet = STATUS_SUCCESS;
	g_usdhcTransferComplete = uSDHC_TC_STATE_ERROR;
	do
	{
		stdRet = uSDHC_DRV_Transfer(instance, content);
		if(STATUS_SUCCESS == stdRet)
		{
			/* Waiting for TransferComplete callback. */
			stdRet = OSIF_SemaWait(&g_TransferCompleteSem, uSDHC_TC_TIMEOUT_MS);
		}
		else
		{
			if(uSDHC_PS_STATE_NO_CARD == g_usdhcPresenceStatus)
			{
				stdRet = STATUS_ERROR;
			}
		}
		/* Report generic error on transfer complete failure. */
		if(uSDHC_TC_STATE_ERROR == g_usdhcTransferComplete)
		{
			stdRet = STATUS_ERROR;
		}
	}while(STATUS_BUSY == stdRet);
	return stdRet;
}

void uSDHC_CardInsertCallback(uint32_t instance, uint32_t status, void *userData)
{
    (void)instance;
    (void)userData;
    if((uint32_t)STATUS_SUCCESS == status)
    {
        g_usdhcPresenceStatus = uSDHC_PS_STATE_CARD_INSERTED;
    }
}

void uSDHC_CardRemoveCallback(uint32_t instance, uint32_t status, void *userData)
{
    (void)instance;
    (void)userData;
    if((uint32_t)STATUS_SUCCESS == status)
    {
        g_usdhcPresenceStatus = uSDHC_PS_STATE_NO_CARD;
    }
}
