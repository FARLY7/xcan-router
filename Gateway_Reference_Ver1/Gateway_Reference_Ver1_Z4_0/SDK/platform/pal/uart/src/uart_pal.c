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
 * @file uart_pal.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, Taking address of near auto variable.
 * The code is not dynamically linked. An absolute stack address is obtained
 * when taking the address of the near auto variable. A source of error in
 * writing dynamic code is that the stack segment may be different from the data
 * segment.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior
 * declaration.
 * State structures are not part of the public API.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External could be made static.
 * Function is defined for usage by application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.9, An object should be defined at block
 * scope if its identifier only appears in a single function.
 * An object with static storage duration declared at block scope cannot be
 * accessed directly from outside the block.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 10.3, Expression assigned to a narrower or different essential type
 * This is needed for the conversion between generic UART types to lpuart types.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 10.5, Impermissible cast; cannot cast from 'essentially enum<i>'
 * to 'essentially enum<i>'
 * This is needed for the conversion between generic UART types to lpuart types.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.3, Cast performed between a pointer to object type
 * and a pointer to a different object type.
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.5, Conversion from pointer to void to pointer to other type
 * This is needed for the extension of the user configuration structure, for which the actual type
 * cannot be known.
 */

#include "uart_pal.h"
#include "device_registers.h"

/* Include PD files */
#if (defined(UART_OVER_LPUART))
    #include "lpuart_driver.h"
#endif

#if (defined(UART_OVER_FLEXIO))
    #include "flexio.h"
    #include "flexio_uart_driver.h"
#endif

#if (defined(UART_OVER_LINFLEXD))
    #include "linflexd_uart_driver.h"
#endif

/* Define state structures for LPUART */
#if (defined(UART_OVER_LPUART))
    /*! @brief LPUART state structures */
    static lpuart_state_t s_lpuartState[NO_OF_LPUART_INSTS_FOR_UART];
    /*! @brief LPUART state-instance matching */
    static uint8_t s_lpuartStateInstanceMapping[NO_OF_LPUART_INSTS_FOR_UART];
    /*! @brief LPUART available resources table */
    static bool s_lpuartStateIsAllocated[NO_OF_LPUART_INSTS_FOR_UART];
#endif

/* Define state structures for UART over FLEXIO */
#if (defined(UART_OVER_FLEXIO))
    /*! @brief FLEXIO state structures for Tx*/
    static flexio_uart_state_t s_flexioUartTxState[NO_OF_FLEXIO_INSTS_FOR_UART];
    /*! @brief FLEXIO state structures for Rx*/
    static flexio_uart_state_t s_flexioUartRxState[NO_OF_FLEXIO_INSTS_FOR_UART];
    flexio_device_state_t flexioDeviceState;
    /*! @brief FLEXIO state-instance matching */
    static uint8_t s_flexioUartStateInstanceMapping[NO_OF_FLEXIO_INSTS_FOR_UART];
    /*! @brief FLEXIO  available resources table */
    static bool s_flexioUartStateIsAllocated[NO_OF_FLEXIO_INSTS_FOR_UART];
#endif

/* Define state structures for LinFlexD */
#if (defined(UART_OVER_LINFLEXD))
    /*! @brief LinFlexD state structures */
    static linflexd_uart_state_t s_linFlexDState[NO_OF_LINFLEXD_INSTS_FOR_UART];
    /*! @brief LinFlexD state-instance matching */
    static uint8_t s_linFlexDStateInstanceMapping[NO_OF_LINFLEXD_INSTS_FOR_UART];
    /*! @brief LinFlexD  available resources table */
    static bool s_linFlexDStateIsAllocated[NO_OF_LINFLEXD_INSTS_FOR_UART];
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_AllocateState
 * Description   : Allocates one of the available state structures.
 *
 *END**************************************************************************/
static uint8_t UART_AllocateState(bool* isAllocated,
                                  uint8_t * instanceMapping,
                                  const uart_instance_t* const instance,
                                  uint8_t numberOfinstances)
{
    DEV_ASSERT(isAllocated != NULL);
    DEV_ASSERT(instanceMapping != NULL);
    DEV_ASSERT(instance != NULL);

    uint8_t i;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;
    /* Allocate one of the UART state structures for this instance */
    for (i = 0; i < numberOfinstances; i++)
    {
        if (isAllocated[i] == false)
        {
            instanceMapping[i] = (uint8_t)uartIdx;
            isAllocated[i] = true;
            break;
        }
    }
    return i;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_FreeState
 * Description   : Deallocates one of the available state structures.
 *
 *END**************************************************************************/
static void UART_FreeState(bool* isAllocated,
                           const uint8_t * instanceMapping,
                           const uart_instance_t * const instance,
                           uint8_t numberOfinstances)
{
    DEV_ASSERT(isAllocated != NULL);
    DEV_ASSERT(instanceMapping != NULL);
    DEV_ASSERT(instance != NULL);

    uint8_t i;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;
    /* Free one of the UART state structures for this instance */
    for (i = 0;i < numberOfinstances;i++)
    {
        if (instanceMapping[i] == (uint8_t)uartIdx)
        {
            isAllocated[i] = false;
            break;
        }
    }
}

#if (defined(UART_OVER_FLEXIO))
/*FUNCTION**********************************************************************
 *
 * Function Name : UART_FindFlexioState
 * Description   : Search the state structure of the FlexIO instance
 *
 *END**************************************************************************/
static uint8_t UART_FindFlexioState(const uart_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);

    uint8_t i;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    for (i = 0; i < NO_OF_FLEXIO_INSTS_FOR_UART; i++)
    {
        if (s_flexioUartStateInstanceMapping[i] == (uint8_t)uartIdx)
        {
            break;
        }
    }
    return i;
}
#endif

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Init
 * Description   : Configures the UART module
 *
 * Implements    : UART_Init_Activity
 *END**************************************************************************/
status_t UART_Init(const uart_instance_t * const instance, const uart_user_config_t *config)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(config != NULL);

    status_t status = STATUS_ERROR;
    uint8_t index = 0;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            lpuart_user_config_t lpuartConfig;
            lpuartConfig.baudRate = config->baudRate;

            /* LPUART supports only 8, 9 or 10 bits per character */
            DEV_ASSERT((config->bitCount >= UART_8_BITS_PER_CHAR) &&
                       (config->bitCount <= UART_10_BITS_PER_CHAR));
            switch (config->bitCount)
            {
                case UART_8_BITS_PER_CHAR:
                    lpuartConfig.bitCountPerChar = LPUART_8_BITS_PER_CHAR;
                    break;
                case UART_9_BITS_PER_CHAR:
                    lpuartConfig.bitCountPerChar = LPUART_9_BITS_PER_CHAR;
                    break;
                case UART_10_BITS_PER_CHAR:
                    lpuartConfig.bitCountPerChar = LPUART_10_BITS_PER_CHAR;
                    break;
                default:
                    /* Impossible type - do nothing */
                    break;
            }

            lpuartConfig.parityMode = (lpuart_parity_mode_t)(config->parityMode);
            lpuartConfig.stopBitCount = (lpuart_stop_bit_count_t)(config->stopBitCount);
            lpuartConfig.transferType = (lpuart_transfer_type_t)(config->transferType);
            lpuartConfig.rxDMAChannel = config->rxDMAChannel;
            lpuartConfig.txDMAChannel = config->txDMAChannel;

            /* Allocate one of the LPUART state structure for this instance */
            index = UART_AllocateState(s_lpuartStateIsAllocated,
                                       s_lpuartStateInstanceMapping,
                                       instance,
                                       NO_OF_LPUART_INSTS_FOR_UART);
            /* Initialize LPUART instance */
            status = LPUART_DRV_Init(uartIdx, (lpuart_state_t*)(&s_lpuartState[index]), &lpuartConfig);

            /* Install Rx callback */
            if (config->rxCallback != NULL)
            {
                (void)LPUART_DRV_InstallRxCallback(uartIdx,
                                                   config->rxCallback,
                                                   config->rxCallbackParam);
            }

            /* Install Tx callback */
            if (config->txCallback != NULL)
            {
                (void)LPUART_DRV_InstallTxCallback(uartIdx,
                                                   config->txCallback,
                                                   config->txCallbackParam);
            }
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            /* FlexIO driver can be used with parity disabled and one stop bit */
            DEV_ASSERT(config->parityMode == UART_PARITY_DISABLED);
            DEV_ASSERT(config->stopBitCount == UART_ONE_STOP_BIT);

            flexio_uart_user_config_t flexioUartTxConfig;
            flexio_uart_user_config_t flexioUartRxConfig;

            /* Set baud rate for Tx and Rx */
            flexioUartTxConfig.baudRate = config->baudRate;
            flexioUartRxConfig.baudRate = config->baudRate;

            /* Set transfer type for Tx and Rx */
            switch (config->transferType)
            {
                case UART_USING_DMA:
                    flexioUartTxConfig.driverType = FLEXIO_DRIVER_TYPE_DMA;
                    flexioUartRxConfig.driverType = FLEXIO_DRIVER_TYPE_DMA;
                    break;
                case UART_USING_INTERRUPTS:
                    flexioUartTxConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
                    flexioUartRxConfig.driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS;
                    break;
                default:
                    /* Impossible type - do nothing */
                    break;
            }

            /* Set bit count per char for Tx and Rx */
            switch (config->bitCount)
            {
                case UART_7_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 7U;
                    flexioUartRxConfig.bitCount = 7U;
                    break;
                case UART_8_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 8U;
                    flexioUartRxConfig.bitCount = 8U;
                    break;
                case UART_9_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 9U;
                    flexioUartRxConfig.bitCount = 9U;
                    break;
                case UART_10_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 10U;
                    flexioUartRxConfig.bitCount = 10U;
                    break;
                case UART_15_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 15U;
                    flexioUartRxConfig.bitCount = 15U;
                    break;
                case UART_16_BITS_PER_CHAR:
                    flexioUartTxConfig.bitCount = 16U;
                    flexioUartRxConfig.bitCount = 16U;
                    break;
                default:
                    /* Impossible type - do nothing */
                    break;
            }

            /* Configure Tx */
            flexioUartTxConfig.direction = FLEXIO_UART_DIRECTION_TX;
            flexioUartTxConfig.dmaChannel = config->txDMAChannel;
            flexioUartTxConfig.dataPin = ((extension_flexio_for_uart_t *)(config->extension))->dataPinTx;

            /* Configure Rx */
            flexioUartRxConfig.direction = FLEXIO_UART_DIRECTION_RX;
            flexioUartRxConfig.dmaChannel = config->rxDMAChannel;
            flexioUartRxConfig.dataPin = ((extension_flexio_for_uart_t *)(config->extension))->dataPinRx;

            /* Link Flexio Callbacks to the callbacks defined in PAL */
            flexioUartRxConfig.callback = config->rxCallback;
            flexioUartRxConfig.callbackParam = config->rxCallbackParam;
            flexioUartTxConfig.callback = config->txCallback;
            flexioUartTxConfig.callbackParam = config->txCallbackParam;

            status = FLEXIO_DRV_InitDevice(0U, &flexioDeviceState);
            if (status == STATUS_SUCCESS)
            {
                /* Allocate one of the Flexio state structure for this instance */
                index = UART_AllocateState(s_flexioUartStateIsAllocated,
                                           s_flexioUartStateInstanceMapping,
                                           instance,
                                           NO_OF_FLEXIO_INSTS_FOR_UART);
                /* Init FlexIO UART driver for Tx */
                status = FLEXIO_UART_DRV_Init(0U,
                                              &flexioUartTxConfig,
                                              (flexio_uart_state_t*)&s_flexioUartTxState[index]);
                /* Init FlexIO UART driver for Rx */
                status = FLEXIO_UART_DRV_Init(0U,
                                              &flexioUartRxConfig,
                                              (flexio_uart_state_t*)&s_flexioUartRxState[index]);
            }
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            linflexd_uart_user_config_t linFlexDConfig;
            linFlexDConfig.baudRate = config->baudRate;
            /* LinFlexD does not support 9 or 10 bits per character */
            DEV_ASSERT((config->bitCount != UART_9_BITS_PER_CHAR) &&
                       (config->bitCount != UART_10_BITS_PER_CHAR));
            switch (config->bitCount)
            {
                case UART_7_BITS_PER_CHAR:
                    linFlexDConfig.wordLength = LINFLEXD_UART_7_BITS;
                    break;
                case UART_8_BITS_PER_CHAR:
                    linFlexDConfig.wordLength = LINFLEXD_UART_8_BITS;
                    break;
                case UART_15_BITS_PER_CHAR:
                    linFlexDConfig.wordLength = LINFLEXD_UART_15_BITS;
                    break;
                case UART_16_BITS_PER_CHAR:
                    linFlexDConfig.wordLength = LINFLEXD_UART_16_BITS;
                    break;
                default:
                    /* Impossible type - do nothing */
                    break;
            }

            switch (config->parityMode)
            {
                case UART_PARITY_DISABLED:
                    linFlexDConfig.parityCheck = false;
                    break;
                case UART_PARITY_EVEN:
                    linFlexDConfig.parityCheck = true;
                    linFlexDConfig.parityType = LINFLEXD_UART_PARITY_EVEN;
                    break;
                case UART_PARITY_ODD:
                    linFlexDConfig.parityCheck = true;
                    linFlexDConfig.parityType = LINFLEXD_UART_PARITY_ODD;
                    break;
                default:
                    /* Impossible type - do nothing */
                    break;
            }

            linFlexDConfig.stopBitsCount = (linflexd_uart_stop_bits_count_t)config->stopBitCount;
            linFlexDConfig.transferType = (linflexd_uart_transfer_type_t)config->transferType;
            linFlexDConfig.txDMAChannel = config->txDMAChannel;
            linFlexDConfig.rxDMAChannel = config->rxDMAChannel;

            /* Allocate one of the LinFlexD state structures for this instance */
            index = UART_AllocateState(s_linFlexDStateIsAllocated,
                                       s_linFlexDStateInstanceMapping,
                                       instance,
                                       NO_OF_LINFLEXD_INSTS_FOR_UART);
            /* Initialize LinFlexD instance */
            status = LINFLEXD_UART_DRV_Init(uartIdx, &s_linFlexDState[index], &linFlexDConfig);

            /* Install Rx callback */
            if (config->rxCallback != NULL)
            {
                (void)LINFLEXD_UART_DRV_InstallRxCallback(uartIdx,
                                                          config->rxCallback,
                                                          config->rxCallbackParam);
            }

            /* Install Tx callback */
            if (config->txCallback != NULL)
            {
                (void)LINFLEXD_UART_DRV_InstallTxCallback(uartIdx,
                                                          config->txCallback,
                                                          config->txCallbackParam);
            }
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_Deinit
 * Description   : De-initializes the UART module
 *
 * Implements    : UART_Deinit_Activity
 *END**************************************************************************/
status_t UART_Deinit(const uart_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;
    status_t status = STATUS_ERROR;
#if (defined(UART_OVER_FLEXIO))
    uint8_t index = 0;
#endif

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_Deinit(uartIdx);
            if (status == STATUS_SUCCESS)
            {
                UART_FreeState(s_lpuartStateIsAllocated,
                               s_lpuartStateInstanceMapping,
                               instance,
                               NO_OF_LPUART_INSTS_FOR_UART);
            }
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        /* Define UART PAL over FLEXIO */
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            index = UART_FindFlexioState(instance);
            status = FLEXIO_UART_DRV_Deinit(&(s_flexioUartTxState[index]));
            if (status == STATUS_SUCCESS)
            {
                UART_FreeState(s_flexioUartStateIsAllocated,
                               s_flexioUartStateInstanceMapping,
                               instance,
                               NO_OF_FLEXIO_INSTS_FOR_UART);
            }
            status = FLEXIO_UART_DRV_Deinit(&(s_flexioUartRxState[index]));
            if (status == STATUS_SUCCESS)
            {
                UART_FreeState(s_flexioUartStateIsAllocated,
                               s_flexioUartStateInstanceMapping,
                               instance,
                               NO_OF_FLEXIO_INSTS_FOR_UART);
            }
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        /* Define UART PAL over LinFlexD */
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_Deinit(uartIdx);
            if (status == STATUS_SUCCESS)
            {
                UART_FreeState(s_linFlexDStateIsAllocated,
                               s_linFlexDStateInstanceMapping,
                               instance,
                               NO_OF_LINFLEXD_INSTS_FOR_UART);
            }
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetBaudRate
 * Description   : Configures the UART baud rate.
 *
 * Implements    : UART_SetBaudRate_Activity
 *END**************************************************************************/
status_t UART_SetBaudRate(const uart_instance_t * const instance, uint32_t desiredBaudRate)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
#if (defined(UART_OVER_FLEXIO))
    uint8_t index = 0;
    uint8_t bitCount = 0;
#endif
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_SetBaudRate(uartIdx, desiredBaudRate);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            index = UART_FindFlexioState(instance);
            bitCount = (s_flexioUartTxState[index]).bitCount;
            status = FLEXIO_UART_DRV_SetConfig(&(s_flexioUartTxState[index]),
                                               desiredBaudRate,
                                               bitCount);
            status = FLEXIO_UART_DRV_SetConfig(&(s_flexioUartRxState[index]),
                                               desiredBaudRate,
                                               bitCount);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_SetBaudRate(uartIdx, desiredBaudRate);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetBaudRate
 * Description   : Returns the UART configured baud rate.
 *
 * Implements    : UART_GetBaudRate_Activity
 *END**************************************************************************/
status_t UART_GetBaudRate(const uart_instance_t * const instance, uint32_t * configuredBaudRate)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            LPUART_DRV_GetBaudRate(uartIdx, configuredBaudRate);
            status = STATUS_SUCCESS;
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_GetBaudRate(&(s_flexioUartTxState[UART_FindFlexioState(instance)]),
                                                 configuredBaudRate);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_GetBaudRate(uartIdx, configuredBaudRate);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SendDataBlocking
 * Description   : Perform a blocking UART transmission
 *
 * Implements    : UART_SendDataBlocking_Activity
 *END**************************************************************************/
status_t UART_SendDataBlocking(
        const uart_instance_t * const instance,
        const uint8_t * txBuff,
        uint32_t txSize,
        uint32_t timeout)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(txBuff != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_SendDataBlocking(uartIdx, txBuff, txSize, timeout);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_SendDataBlocking(
                            &(s_flexioUartTxState[UART_FindFlexioState(instance)]),
                            txBuff,
                            txSize,
                            timeout);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_SendDataBlocking(uartIdx, txBuff, txSize, timeout);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SendData
 * Description   : Perform a non-blocking UART transmission
 *
 * Implements    : UART_SendData_Activity
 *END**************************************************************************/
status_t UART_SendData(const uart_instance_t * const instance, const uint8_t * txBuff, uint32_t txSize)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(txBuff != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_SendData(uartIdx, txBuff, txSize);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_SendData(
                            &(s_flexioUartTxState[UART_FindFlexioState(instance)]),
                            txBuff,
                            txSize);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_SendData(uartIdx, txBuff, txSize);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_AbortSendingData
 * Description   : This function terminates an non-blocking transmission early.
 *
 * Implements    : UART_AbortSendingData_Activity
 *END**************************************************************************/
status_t UART_AbortSendingData(const uart_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_AbortSendingData(uartIdx);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_TransferAbort(&(s_flexioUartTxState[UART_FindFlexioState(instance)]));
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_AbortSendingData(uartIdx);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetTransmitStatus
 * Description   : This function returns whether the previous transmission has
 * finished
 *
 * Implements    : UART_GetTransmitStatus_Activity
 *END**************************************************************************/
status_t UART_GetTransmitStatus(const uart_instance_t * const instance, uint32_t * bytesRemaining)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(bytesRemaining != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_GetTransmitStatus(uartIdx, bytesRemaining);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_GetStatus(
                                &(s_flexioUartTxState[UART_FindFlexioState(instance)]),
                                bytesRemaining);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_GetTransmitStatus(uartIdx, bytesRemaining);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_ReceiveDataBlocking
 * Description   : Perform a blocking UART reception
 *
 * Implements    : UART_ReceiveDataBlocking_Activity
 *END**************************************************************************/
status_t UART_ReceiveDataBlocking(
        const uart_instance_t * const instance,
        uint8_t * rxBuff,
        uint32_t rxSize,
        uint32_t timeout)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch(instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_ReceiveDataBlocking(uartIdx, rxBuff, rxSize, timeout);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_ReceiveDataBlocking(
                            &(s_flexioUartRxState[UART_FindFlexioState(instance)]),
                            rxBuff,
                            rxSize,
                            timeout);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_ReceiveDataBlocking(uartIdx, rxBuff, rxSize, timeout);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_ReceiveData
 * Description   : Perform a non-blocking UART reception
 *
 * Implements    : UART_ReceiveData_Activity
 *END**************************************************************************/
status_t UART_ReceiveData(const uart_instance_t * const instance, uint8_t * rxBuff, uint32_t rxSize)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_ReceiveData(uartIdx, rxBuff, rxSize);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_ReceiveData(
                            &(s_flexioUartRxState[UART_FindFlexioState(instance)]),
                            rxBuff,
                            rxSize);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_ReceiveData(uartIdx, rxBuff, rxSize);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_AbortReceivingData
 * Description   : Terminates a non-blocking receive early.
 *
 * Implements    : UART_AbortReceivingData_Activity
 *END**************************************************************************/
status_t UART_AbortReceivingData(const uart_instance_t * const instance)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_AbortReceivingData(uartIdx);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_TransferAbort(&(s_flexioUartRxState[UART_FindFlexioState(instance)]));
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_AbortReceivingData(uartIdx);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_GetReceiveStatus
 * Description   : This function returns whether the previous reception has
 * finished
 *
 * Implements    : UART_GetReceiveStatus_Activity
 *END**************************************************************************/
status_t UART_GetReceiveStatus(const uart_instance_t * const instance, uint32_t * bytesRemaining)
{
    DEV_ASSERT(instance != NULL);
    DEV_ASSERT(bytesRemaining != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_GetReceiveStatus(uartIdx, bytesRemaining);
            break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_GetStatus(&(s_flexioUartRxState[UART_FindFlexioState(instance)]),
                                               bytesRemaining);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_GetReceiveStatus(uartIdx, bytesRemaining);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetRxBuffer
 * Description   : This function provide a buffer for receiving data.
 *
 *
 * Implements    : UART_SetRxBuffer_Activity
 *END**************************************************************************/
status_t UART_SetRxBuffer(const uart_instance_t * const instance, uint8_t * rxBuff, uint32_t rxSize)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
       case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_SetRxBuffer(uartIdx, rxBuff, rxSize);
           break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_SetRxBuffer(&(s_flexioUartRxState[UART_FindFlexioState(instance)]), rxBuff, rxSize);
            break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_SetRxBuffer(uartIdx, rxBuff, rxSize);
            break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : UART_SetTxBuffer
 * Description   : This function provide a buffer for transmitting data.
 *
 *
 * Implements    : UART_SetTxBuffer_Activity
 *END**************************************************************************/
status_t UART_SetTxBuffer(const uart_instance_t * const instance, const uint8_t * txBuff, uint32_t txSize)
{
    DEV_ASSERT(instance != NULL);
    status_t status = STATUS_ERROR;
    uint32_t uartIdx = 0;
    uartIdx = instance->instIdx;

    switch (instance->instType)
    {
    /* Define UART PAL over LPUART */
    #if (defined(UART_OVER_LPUART))
        case UART_INST_TYPE_LPUART:
            DEV_ASSERT(uartIdx < LPUART_INSTANCE_COUNT);
            status = LPUART_DRV_SetTxBuffer(uartIdx, txBuff, txSize);
        break;
    #endif
    /* Define UART PAL over FLEXIO */
    #if (defined(UART_OVER_FLEXIO))
        case UART_INST_TYPE_FLEXIO_UART:
            DEV_ASSERT(uartIdx < FLEXIO_UART_INSTANCE_COUNT);
            status = FLEXIO_UART_DRV_SetTxBuffer(&(s_flexioUartTxState[UART_FindFlexioState(instance)]), txBuff, txSize);
        break;
    #endif
    /* Define UART PAL over LinFlexD */
    #if (defined(UART_OVER_LINFLEXD))
        case UART_INST_TYPE_LINFLEXD:
            DEV_ASSERT(uartIdx < LINFlexD_INSTANCE_COUNT);
            status = LINFLEXD_UART_DRV_SetTxBuffer(uartIdx, txBuff, txSize);
        break;
    #endif
        default:
            /* Impossible type - do nothing */
            break;
    }
    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
