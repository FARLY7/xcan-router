/*****************************************************************************/
/* FILE NAME: usdhc_impl.h         COPYRIGHT (c) NXP Semiconductors 2017     */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: uSDHC implementation                                         */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#ifndef USDHC_USDHC_IMPL_H_
#define USDHC_USDHC_IMPL_H_

#include <stdbool.h>
#include "usdhc_driver.h"
#include "sd/card.h"

/*!
 * @addtogroup USDHC_IMPL
 * @ingroup sdhc_mw
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Global card instance structure filled by uSDHC_Init(). */
extern sd_card_t g_sd;
extern semaphore_t g_TransferCompleteSem;

/*! @brief uSDHC transfer complete */
typedef enum
{
	uSDHC_TC_STATE_DONE = 0,
	uSDHC_TC_STATE_ERROR = 1
} usdhc_tc_state_t;

/*! @brief uSDHC presence */
typedef enum
{
	uSDHC_PS_STATE_NO_CARD = 0,
	uSDHC_PS_STATE_CARD_INSERTED = 1
} usdhc_ps_state_t;

/*! @brief uSDHC initialization */
typedef enum
{
	uSDHC_INIT_STATE_NOT_INITIALIZED = 0,
	uSDHC_INIT_STATE_INITIALIZED = 1
} usdhc_init_state_t;

extern volatile usdhc_tc_state_t g_usdhcTransferComplete;
extern volatile usdhc_ps_state_t g_usdhcPresenceStatus;
extern volatile usdhc_init_state_t g_usdhcInitialized;
/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Check whether the card is write-protected.
 *
 * @param void.
 * @retval bool true if card has write protection switch position is locked.
 */
bool uSDHC_IsSDCardWriteProtect(void);

/*!
 * @brief Check whether the card is present in slot.
 *
 * This function returns card instance structure.
 *
 * @param void.
 * @retval bool true if card is present in slot.
 */
bool uSDHC_IsSDCardPresent(void);

/*!
 * @brief Wait until the card is present in slot.
 *
 * This function returns card instance structure.
 *
 * @param void.
 * @retval pointer to sd_card_t.
 */
sd_card_t *uSDHC_GetSDCard(void);

/*!
 * @brief Check whether uSDHC could be initialized, loops on error.
 *
 * This function calls uSDHC driver initialization routine and SD_Init().
 *
 * @param bool wouldBlock blocks if an error.
 * @retval status_t.
 */
common_status_t uSDHC_Init(bool wouldBlock);

/*!
 * @brief Should be called if uSDHC shouldn't be used anymore.
 *
 * This function calls SD_Deinit() and uSDHC driver de-initialization routine.
 *
 * @param void.
 * @retval status_t.
 */
common_status_t uSDHC_DeInit(void);

/*!
 * @brief Should be installed after uSDHC_DRV_Init().
 *
 * This function is uSDHC-specific asynchronous mechanism.
 * Updates g_usdhcTransferComplete on transfer complete event.
 *
 * @param uint8_t index in global array of uSDHC instances - g_usdhcBases.
 * @param uint32_t status_t transfer result code.
 * @param void *user-defined data.
 * @retval void.
 */
void uSDHC_TransferCompleteCallback(uint32_t instance, uint32_t status, void *userData);

/*!
 * @brief Should be installed after uSDHC_DRV_Init().
 *
 * This function is uSDHC-specific asynchronous mechanism.
 * Polls g_usdhcTransferComplete status until transfer is complete.
 *
 * @param uint32_t index in global array of uSDHC instances - g_usdhcBases.
 * @param usdhc_transfer_t * transfer state.
 * @retval status_t transfer result code.
 */
status_t uSDHC_TransferFunction(uint32_t instance, usdhc_transfer_t *content);

/*!
 * @brief Should be installed after uSDHC_DRV_Init().
 *
 * This function is uSDHC-specific event.
 * Updates g_usdhcg_usdhcPresenceStatus on card insert event.
 *
 * @param uint8_t index in global array of uSDHC instances - g_usdhcBases.
 * @param uint32_t status_t result code.
 * @param void *user-defined data.
 * @retval void.
 */
void uSDHC_CardInsertCallback(uint32_t instance, uint32_t status, void *userData);

/*!
 * @brief Should be installed after uSDHC_DRV_Init().
 *
 * This function is uSDHC-specific event.
 * Updates g_usdhcg_usdhcPresenceStatus on card remove event.
 *
 * @param uint8_t index in global array of uSDHC instances - g_usdhcBases.
 * @param uint32_t status_t result code.
 * @param void *user-defined data.
 * @retval void.
 */
void uSDHC_CardRemoveCallback(uint32_t instance, uint32_t status, void *userData);

#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* USDHC_USDHC_IMPL_H_ */
