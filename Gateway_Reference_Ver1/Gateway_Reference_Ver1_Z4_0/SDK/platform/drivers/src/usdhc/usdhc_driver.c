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
 * @file usdhc_driver.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from pointer to unsigned long,
 * cast from unsigned long to pointer and cast from unsigned int to pointer.
 * The cast is required to perform a conversion between a pointer
 * and an unsigned long define, representing an address or vice versa.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and
 * integer type.
 * The cast is required to initialize a pointer with an unsigned long define,
 * representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * The symbol is declared in the hardware access file as external; it is needed
 * for accessing the installed callback, but is not a part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable
 * The code is not dynamically linked. An absolute stack address is obtained when
 * taking the address of the near auto variable. A source of error in writing
 * dynamic code is that the stack segment may be different from the data segment.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, Function-like macro
 * Function-like macros are used instead of inline functions in order to ensure
 * that the performance will not be decreased if the functions will not be
 * inlined by the compiler.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for providing ADMA table from application, which can be ADMA1 or ADMA2.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, Could define variable at block scope
 * The variables are defined in the common source file and this rule can't be
 * applied.
 *
 */

#include "usdhc_driver.h"
#include "usdhc_hw_access.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Clock setting */
/* Max SD clock divisor from base clock */
#define uSDHC_MAX_DVS           ((uSDHC_SYS_CTRL_DVS_MASK >> uSDHC_SYS_CTRL_DVS_SHIFT) + 1U)
#define uSDHC_INITIAL_DVS       (1U)    /* Initial value of SD clock divisor */
#define uSDHC_INITIAL_CLKFS     (1U)    /* Initial value of SD clock frequency selector */
#define uSDHC_NEXT_DVS(x)       ((x) += 1U)
#define uSDHC_PREV_DVS(x)       ((x) -= 1U)
#define uSDHC_MAX_CLKFS         ((uSDHC_SYS_CTRL_SDCLKFS_MASK >> uSDHC_SYS_CTRL_SDCLKFS_SHIFT) + 1U)
#define uSDHC_NEXT_CLKFS(x)     ((x) <<= 1U)
#define uSDHC_PREV_CLKFS(x)     ((x) >>= 1U)

/*!< All flags mask */
#define uSDHC_ALL_INTERRUPTS_MASK       (0xFFFFFFFFU)
/*!< Timeout value to reset the uSDHC */
#define uSDHC_RESET_TIMEOUT             (1000U)


/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief uSDHC transmit IRQ number for each instance. */
static const IRQn_Type s_usdhcTxIrqId[] = uSDHC_IRQS;

/*! @brief uSDHC base pointer array */
uSDHC_Type * const g_usdhcBases[uSDHC_INSTANCE_COUNT] = uSDHC_BASE_PTRS;

/*! @brief Pointers to uSDHC internal driver state for each instance. */
usdhc_state_t * g_usdhcStates[uSDHC_INSTANCE_COUNT] = { NULL };

/*! @brief Table to save uSDHC clock indexes in clock configuration */
static const clock_names_t s_usdhcClkNames[] = FEATURE_USDHC_CLOCK_NAMES;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : GetDefaultConfig
 * Description   : Gets the default configuration structure
 *
 * Implements    : uSDHC_DRV_GetDefaultConfig_Activity
 *END**************************************************************************/
void uSDHC_DRV_GetDefaultConfig(usdhc_config_t * config)
{
    DEV_ASSERT(config != NULL);

    config->cardDetectDat3 = false;
    config->dmaMode = uSDHC_DMA_MODE_NO;
    config->endianMode = uSDHC_ENDIAN_MODE_LITTLE;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_Init
 * Description   : Initializes a uSDHC module.
 *
 * Implements    : uSDHC_DRV_Init_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_Init(uint32_t instance,
                        usdhc_state_t * state,
                        const usdhc_config_t * config)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    DEV_ASSERT(config != NULL);
#if !defined uSDHC_ENABLE_ADMA1
    DEV_ASSERT(config->dmaMode != uSDHC_DMA_MODE_ADMA1);
#endif

    uSDHC_Type * base = g_usdhcBases[instance];
    status_t error = STATUS_SUCCESS;

    /* Reset uSDHC. */
    (void)uSDHC_DRV_Reset(instance, uSDHC_SYS_CTRL_RSTA_MASK, uSDHC_RESET_TIMEOUT);
    /* Configures the usdhc protocol */
    uSDHC_ConfigProtocolControl(base, config);

    /* Disable all clock auto gated off feature because of DAT0 line logic(card buffer full status) can't be updated
    correctly when clock auto gated off is enabled. */
    uSDHC_DisableAutoGateOff(base);

    uSDHC_ConfigByteSwap(true);

    state->admaTable = config->admaTable;
    state->admaTableSize = config->admaTableSize;
    g_usdhcStates[instance] = state;

    error = OSIF_SemaCreate(&(g_usdhcStates[instance]->semaTransferComplete), 0);

    /* Disable all interrupts */
    uSDHC_DisableInterrupt(g_usdhcBases[instance], uSDHC_ALL_INTERRUPTS_MASK);

    /* Enables transfer interrupts */
    uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_CC_MASK | uSDHC_INT_STATUS_TC_MASK);
    /* Enables error interrupts */
    uSDHC_EnableInterrupt(base, uSDHC_COMMAND_ERROR_INT | uSDHC_DATA_ERROR_INT);
    /* Enables DMA interrupts if enabled */
    switch (config->dmaMode)
    {
        case uSDHC_DMA_MODE_ADMA1:
        case uSDHC_DMA_MODE_ADMA2:
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_DMAE_MASK | uSDHC_INT_STATUS_DINT_MASK);
            break;
        case uSDHC_DMA_MODE_NO:
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_BRR_MASK | uSDHC_INT_STATUS_BWR_MASK);
            break;
        default:
            /* Do nothing */
            break;
    }

    INT_SYS_EnableIRQ(s_usdhcTxIrqId[instance]);

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_Deinit
 * Description   : Deinitializes a uSDHC module.
 *
 * Implements    : uSDHC_DRV_Deinit_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_Deinit(uint32_t instance)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);

    /* Disable interrupts. */
    uSDHC_DisableInterrupt(g_usdhcBases[instance], uSDHC_ALL_INTERRUPTS_MASK);
    INT_SYS_DisableIRQ(s_usdhcTxIrqId[instance]);

    /* Destroy the synchronization objects */
    (void)OSIF_SemaDestroy(&(g_usdhcStates[instance]->semaTransferComplete));

    return STATUS_SUCCESS;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_Reset
 * Description   : Reset the uSDHC module
 *
 * Implements    : uSDHC_DRV_Reset_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_Reset(uint32_t instance,
                         uint32_t mask,
                         uint32_t timeoutMs)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);

    uSDHC_Type * base = g_usdhcBases[instance];
    uint32_t startTime, crtTime;
    status_t status = STATUS_TIMEOUT;

    base->SYS_CTRL |= (mask & (uSDHC_SYS_CTRL_RSTA_MASK | uSDHC_SYS_CTRL_RSTC_MASK | uSDHC_SYS_CTRL_RSTD_MASK));

    /* Wait for completion */
    startTime = OSIF_GetMilliseconds();
    do
    {
        if ((base->SYS_CTRL & mask) == 0U)
        {
            status = STATUS_SUCCESS;
            break;
        }
        crtTime = OSIF_GetMilliseconds();
    }
    while (crtTime < (startTime + timeoutMs));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_InstallCallback
 * Description   : Register the callback functions for uSDHC events
 *
 * Implements    : uSDHC_DRV_InstallCallback_Activity
 *END**************************************************************************/
void uSDHC_DRV_InstallCallback(uint32_t instance,
                               usdhc_event_t event,
                               usdhc_callback_t function,
                               void * param)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    uSDHC_Type * base = g_usdhcBases[instance];

    switch (event)
    {
    case uSDHC_EVENT_TRANSFER_COMPLETE:
        g_usdhcStates[instance]->transferCallback = function;
        g_usdhcStates[instance]->transferCallbackParam = param;
        break;
    case uSDHC_EVENT_CARD_INSERT:
        g_usdhcStates[instance]->cardInsertCallback = function;
        g_usdhcStates[instance]->cardInsertCallbackParam = param;

        if (function != NULL)
        {
            /* Enable card insertion interrupt */
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_CINS_MASK);
        }
        else
        {
            /* Disable card insertion interrupt */
            uSDHC_DisableInterrupt(base, uSDHC_INT_STATUS_CINS_MASK);
        }
        break;
    case uSDHC_EVENT_CARD_REMOVE:
        g_usdhcStates[instance]->cardRemoveCallback = function;
        g_usdhcStates[instance]->cardRemoveCallbackParam = param;

        if (function != NULL)
        {
            /* Enable card removal interrupt */
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_CRM_MASK);
        }
        else
        {
            /* Disable card removal interrupt */
            uSDHC_DisableInterrupt(base, uSDHC_INT_STATUS_CRM_MASK);
        }
        break;
    case uSDHC_EVENT_BLOCK_GAP:
        g_usdhcStates[instance]->blockGapCallback = function;
        g_usdhcStates[instance]->blockGapCallbackParam = param;

        if (function != NULL)
        {
            /* Enable block gap interrupt */
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_BGE_MASK);
        }
        else
        {
            /* Disable block gap interrupt */
            uSDHC_DisableInterrupt(base, uSDHC_INT_STATUS_BGE_MASK);
        }
        break;
    case uSDHC_EVENT_CARD_INTERRUPT:
        g_usdhcStates[instance]->cardIntCallback = function;
        g_usdhcStates[instance]->cardIntCallbackParam = param;

        if (function != NULL)
        {
            /* Enable card interrupt */
            uSDHC_EnableInterrupt(base, uSDHC_INT_STATUS_CINT_MASK);
        }
        else
        {
            /* Disable card interrupt */
            uSDHC_DisableInterrupt(base, uSDHC_INT_STATUS_CINT_MASK);
        }
        break;
    default:
        /* Do nothing */
        break;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_SetDataBusWidth
 * Description   : Set data bus width for uSDHC module
 *
 * Implements    : uSDHC_DRV_SetDataBusWidth_Activity
 *END**************************************************************************/
void uSDHC_DRV_SetDataBusWidth(uint32_t instance,
                               usdhc_data_bus_width_t width)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);

    uSDHC_Type * base = g_usdhcBases[instance];

    base->PROT_CTRL = ((base->PROT_CTRL & ~uSDHC_PROT_CTRL_DTW_MASK) | uSDHC_PROT_CTRL_DTW(width));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_GetCapability
 * Description   : Get supported capability of uSDHC module
 *
 * Implements    : uSDHC_DRV_GetCapability_Activity
 *END**************************************************************************/
void uSDHC_DRV_GetCapability(uint32_t instance,
                             usdhc_capability_t * capability)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    DEV_ASSERT(capability != NULL);

    const uSDHC_Type * base = g_usdhcBases[instance];

    uint32_t htCapability;
    uint32_t hostVer;
    uint32_t maxBlockLength;

    hostVer = uSDHC_GetHostVersion(base);
    htCapability = uSDHC_GetHostCapacities(base);

    /* Get the capability of uSDHC. */
    capability->specVersion = ((hostVer & uSDHC_HOST_CTRL_VER_SVN_MASK) >> uSDHC_HOST_CTRL_VER_SVN_SHIFT);
    capability->vendorVersion = ((hostVer & uSDHC_HOST_CTRL_VER_VVN_MASK) >> uSDHC_HOST_CTRL_VER_VVN_SHIFT);
    maxBlockLength = ((htCapability & uSDHC_HOST_CTRL_CAP_MBL_MASK) >> uSDHC_HOST_CTRL_CAP_MBL_SHIFT);
    capability->maxBlockLength = (512UL << maxBlockLength);
    /* Other attributes not in HTCAPBLT register. */
    capability->maxBlockCount = uSDHC_MAX_BLOCK_COUNT;
    capability->flags = (htCapability & (uSDHC_HOST_CTRL_CAP_ADMAS_MASK | uSDHC_HOST_CTRL_CAP_HSS_MASK | uSDHC_HOST_CTRL_CAP_DMAS_MASK |
                                         uSDHC_HOST_CTRL_CAP_SRS_MASK | uSDHC_HOST_CTRL_CAP_VS33_MASK | uSDHC_HOST_CTRL_CAP_VS30_MASK |
                                         uSDHC_HOST_CTRL_CAP_VS18_MASK));

    capability->flags |= (uSDHC_HOST_CTRL_CAP_SUPPORT_4BIT | uSDHC_HOST_CTRL_CAP_SUPPORT_8BIT);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_SetBusClock
 * Description   : Set SD bus clock frequency.
 *
 * Implements    : uSDHC_DRV_SetBusClock_Activity
 *END**************************************************************************/
uint32_t uSDHC_DRV_SetBusClock(uint32_t instance,
                               uint32_t busClock)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    DEV_ASSERT(busClock > 0U);

    uSDHC_Type * base = g_usdhcBases[instance];
    uint32_t srcClock;
    uint32_t divisor;
    uint32_t prescaler;
    uint32_t nearestFrequency = 0U;

    (void)CLOCK_SYS_GetFreq(s_usdhcClkNames[instance], &srcClock);

    divisor = uSDHC_INITIAL_DVS;
    prescaler = uSDHC_INITIAL_CLKFS;

    /* Disable SD clock. It should be disabled before changing the SD clock frequency.*/
    uSDHC_DisableSDClock(base);

    while ((((srcClock / prescaler) / uSDHC_MAX_DVS) > busClock) && (prescaler < uSDHC_MAX_CLKFS))
    {
        uSDHC_NEXT_CLKFS(prescaler);
    }
    while ((((srcClock / prescaler) / divisor) > busClock) && (divisor < uSDHC_MAX_DVS))
    {
        uSDHC_NEXT_DVS(divisor);
    }
    nearestFrequency = ((srcClock / prescaler) / divisor);
    uSDHC_PREV_CLKFS(prescaler);
    uSDHC_PREV_DVS(divisor);

    /* Set the SD clock frequency divisor, SD clock frequency select */
    uSDHC_SetSDClock(base, divisor, prescaler);

    /* Wait until the SD clock is stable. */
    while ((uSDHC_DRV_GetPresentStatusFlags(instance) & uSDHC_PRES_STATE_SDSTB_MASK) == 0U)
    {
    }
    /* Enable the SD clock. */
    uSDHC_EnableSDClock(base);

    return nearestFrequency;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_SetCardActive
 * Description   : Send 80 clock cycles to card to activate the card
 *
 * Implements    : uSDHC_DRV_SetCardActive_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_SetCardActive(uint32_t instance,
                                 uint32_t timeoutMs)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);

    uSDHC_Type * base = g_usdhcBases[instance];
    uint32_t startTime, crtTime;
    status_t status = STATUS_TIMEOUT;

    base->SYS_CTRL |= uSDHC_SYS_CTRL_INITA_MASK;

    /* Wait for completion */
    startTime = OSIF_GetMilliseconds();
    do
    {
        if ((base->SYS_CTRL & uSDHC_SYS_CTRL_INITA_MASK) == 0U)
        {
            status = STATUS_SUCCESS;
            break;
        }
        crtTime = OSIF_GetMilliseconds();
    }
    while (crtTime < (startTime + timeoutMs));

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_TransferBlocking
 * Description   : Send a command or data using blocking mode to the card and get the response from card if any
 *
 * Implements    : uSDHC_DRV_TransferBlocking_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_TransferBlocking(uint32_t instance,
                                    const usdhc_transfer_t * transfer,
                                    uint32_t timeoutMs)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    DEV_ASSERT(transfer != NULL);
    DEV_ASSERT(transfer->command != NULL);  /* Command must not be NULL, data can be NULL. */

    uSDHC_Type * base = g_usdhcBases[instance];
    status_t error = STATUS_SUCCESS;
    status_t syncStatus;
    uint32_t presentStatus = 0U;

    usdhc_dma_mode_t dmaMode = uSDHC_GetCurrentDMAMode(base);
    usdhc_command_t *command = transfer->command;
    usdhc_data_t *data = transfer->data;

    /* Command must not be NULL
     * If data != NULL then blockSize must be divisible by 4 */
    if ((data != NULL) && ((data->blockSize % 4U) != 0U))
    {
        error = STATUS_ERROR;
    }
    else
    {
        /* Wait until command/data bus out of busy status. */
        presentStatus = uSDHC_DRV_GetPresentStatusFlags(instance);
        if ((presentStatus & uSDHC_PRES_STATE_CIHB_MASK) != 0U)
        {
            error = STATUS_BUSY;
        }
        else if ((data != NULL) && ((presentStatus & uSDHC_PRES_STATE_CDIHB_MASK) != 0U))
        {
            error = STATUS_BUSY;
        }
        else
        {
            /* Update ADMA descriptor table and reset transferred words if data isn't NULL. */
            if (data != NULL)
            {
                error = uSDHC_SetAdmaTableConfig(base, dmaMode, g_usdhcStates[instance]->admaTable,
                                                 g_usdhcStates[instance]->admaTableSize,
                                                 ((data->rxData != NULL) ? data->rxData : data->txData),
                                                 (data->blockCount * data->blockSize));

                if (STATUS_SUCCESS != error)
                {
                    error = STATUS_USDHC_PREPARE_ADMA_FAILED;
                }
            }

            if (STATUS_SUCCESS == error)
            {
                /* Save command and data into handle before transferring. */
                g_usdhcStates[instance]->isBlocking = true;
                g_usdhcStates[instance]->command = command;
                g_usdhcStates[instance]->data = data;
                /* transferredWords will only be updated in ISR when transfer way is DATAPORT. */
                g_usdhcStates[instance]->transferredWords = 0U;
                uSDHC_StartTransfer(base, command, data);
                /* Wait for completion */
                syncStatus = OSIF_SemaWait(&(g_usdhcStates[instance]->semaTransferComplete), timeoutMs);
                if (syncStatus == STATUS_TIMEOUT)
                {
                    g_usdhcStates[instance]->isBlocking = false;

                    error = STATUS_TIMEOUT;
                }
            }
        }
    }

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_Transfer
 * Description   : Send a command or data using non-blocking mode to the card and get the response from card if any
 *
 * Implements    : uSDHC_DRV_Transfer_Activity
 *END**************************************************************************/
status_t uSDHC_DRV_Transfer(uint32_t instance,
                            const usdhc_transfer_t * transfer)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    DEV_ASSERT(transfer != NULL);
    DEV_ASSERT(transfer->command != NULL);  /* Command must not be NULL, data can be NULL. */

    uSDHC_Type * base = g_usdhcBases[instance];

    usdhc_dma_mode_t dmaMode = uSDHC_GetCurrentDMAMode(base);
    status_t error = STATUS_SUCCESS;
    usdhc_command_t * command = transfer->command;
    usdhc_data_t * data = transfer->data;
    uint32_t presentStatus = 0U;

    /* Command must not be NULL
     * If data != NULL then blockSize must be divisible by 4 */
    if ((transfer->data != NULL) && ((transfer->data->blockSize % 4U) != 0U))
    {
        error = STATUS_ERROR;
    }
    else
    {
        /* Wait until command/data bus out of busy status. */
        presentStatus = uSDHC_DRV_GetPresentStatusFlags(instance);
        if ((presentStatus & uSDHC_PRES_STATE_CIHB_MASK) != 0U)
        {
            error = STATUS_BUSY;
        }
        else if  ((data != NULL) && ((presentStatus & uSDHC_PRES_STATE_CDIHB_MASK) != 0U))
        {
            error = STATUS_BUSY;
        }
        else
        {
            /* Update ADMA descriptor table and reset transferred words if data isn't NULL. */
            if (data != NULL)
            {
                error = uSDHC_SetAdmaTableConfig(base, dmaMode, g_usdhcStates[instance]->admaTable,
                                                 g_usdhcStates[instance]->admaTableSize,
                                                 ((data->rxData != NULL) ? data->rxData : data->txData),
                                                 (data->blockCount * data->blockSize));

                if (STATUS_SUCCESS != error)
                {
                    error = STATUS_USDHC_PREPARE_ADMA_FAILED;
                }
            }

            if (STATUS_SUCCESS == error)
            {
                /* Save command and data into handle before transferring. */
                g_usdhcStates[instance]->isBlocking = false;
                g_usdhcStates[instance]->command = command;
                g_usdhcStates[instance]->data = data;
                /* transferredWords will only be updated in ISR when transfer way is DATAPORT. */
                g_usdhcStates[instance]->transferredWords = 0U;
                uSDHC_StartTransfer(base, command, data);
            }
        }
    }

    return error;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : uSDHC_DRV_EnableCardControl
 * Description   : Enable or disable some features for uSDHC.
 *
 * Implements    : uSDHC_DRV_EnableCardControl_Activity
 *END**************************************************************************/
void uSDHC_DRV_EnableCardControl(uint32_t instance,
                                 uint32_t mask,
                                 bool enable)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    uSDHC_Type * base = g_usdhcBases[instance];

    if (enable)
    {
        base->PROT_CTRL |= mask;
    }
    else
    {
        base->PROT_CTRL &= ~mask;
    }
}

