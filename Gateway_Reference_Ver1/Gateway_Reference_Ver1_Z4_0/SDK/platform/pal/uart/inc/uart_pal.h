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

#ifndef UART_PAL_H
#define UART_PAL_H

#include "uart_pal_cfg.h"
#include "uart_pal_mapping.h"
#include "status.h"
#include "callbacks.h"

/*!
 * @defgroup uart_pal UART PAL
 * @ingroup uart_pal
 * @addtogroup uart_pal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Defines the number of bits in a character
 *
 * Implements : uart_bit_count_per_char_t_Class
 */
typedef enum
{
    UART_7_BITS_PER_CHAR  = 0x0U, /*!< 7-bit data characters */
    UART_8_BITS_PER_CHAR  = 0x1U, /*!< 8-bit data characters */
    UART_9_BITS_PER_CHAR  = 0x2U, /*!< 9-bit data characters */
    UART_10_BITS_PER_CHAR = 0x3U, /*!< 10-bit data characters */
    UART_15_BITS_PER_CHAR = 0x4U, /*!< 15-bit data characters */
    UART_16_BITS_PER_CHAR = 0x5U, /*!< 16-bit data characters */
} uart_bit_count_per_char_t;

/*!
 * @brief Defines the transfer type
 *
 * Implements : uart_transfer_type_t_Class
 */
typedef enum
{
    UART_USING_DMA         = 0U,    /*!< Driver uses DMA for data transfers */
    UART_USING_INTERRUPTS  = 1U,    /*!< Driver uses interrupts for data transfers */
} uart_transfer_type_t;

/*!
 * @brief Defines the parity mode
 *
 * Implements : uart_parity_mode_t_Class
 */
typedef enum
{
    UART_PARITY_DISABLED = 0x0U, /*!< parity disabled */
    UART_PARITY_EVEN     = 0x2U, /*!< parity enabled, type even */
    UART_PARITY_ODD      = 0x3U  /*!< parity enabled, type odd */
} uart_parity_mode_t;

/*!
 * @brief Defines the number of stop bits
 *
 * Implements : uart_stop_bit_count_t_Class
 */
typedef enum
{
    UART_ONE_STOP_BIT = 0x0U, /*!< one stop bit */
    UART_TWO_STOP_BIT = 0x1U  /*!< two stop bits */
} uart_stop_bit_count_t;

/*!
 * @brief Defines the UART configuration structure
 *
 * Implements : uart_user_config_t_Class
 */
typedef struct
{
    uint32_t baudRate;                           /*!< Baud rate */
    uart_bit_count_per_char_t bitCount;          /*!< Number of bits in a character */
    uart_parity_mode_t parityMode;               /*!< Parity mode, disabled (default), even, odd */
    uart_stop_bit_count_t stopBitCount;          /*!< number of stop bits, 1 stop bit (default) or 2 stop bits */
    uart_transfer_type_t transferType;           /*!< Type of the transfer (interrupt/dma based) */
    uint8_t rxDMAChannel;                        /*!< Channel number for DMA rx channel. */
    uint8_t txDMAChannel;                        /*!< Channel number for DMA tx channel. */
    uart_callback_t rxCallback;                  /*!< Callback to invoke for data receive */
    void *rxCallbackParam;                       /*!< Receive callback parameter */
    uart_callback_t txCallback;                  /*!< Callback to invoke for data send */
    void *txCallbackParam;                       /*!< Transmit callback parameter */
    void *extension;                             /*!< This field will be used to add extra settings to the basic configuration like FlexIO data pins */
} uart_user_config_t;

#if (defined (UART_OVER_FLEXIO))
/*!
 * @brief Defines the extension structure for the UART over FLEXIO
 *
 * Implements : extension_flexio_for_uart_t_Class
 */
typedef struct
{
    uint8_t dataPinTx;    /*!< Flexio pin to use as Tx pin */
    uint8_t dataPinRx;    /*!< Flexio pin to use as Rx pin */
} extension_flexio_for_uart_t;
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Initializes the UART module
 *
 * This function initializes and enables the requested UART module,
 * configuring the bus parameters.
 *
 * @param[in] instance The instance structure
 * @param[in] config The configuration structure
 * @return    Error or success status returned by API
 */
status_t UART_Init(const uart_instance_t * const instance, const uart_user_config_t *config);

/*!
 * @brief De-initializes the UART module
 *
 * This function de-initializes the UART module.
 *
 * @param[in] instance The instance structure
 * @return    Error or success status returned by API
 */
status_t UART_Deinit(const uart_instance_t * const instance);

/*!
 * @brief Configures the UART baud rate.
 *
 * This function configures the UART baud rate.
 * Note that due to module limitation not any baud rate can be achieved. The driver
 * will set a baud rate as close as possible to the requested baud rate, but there may
 * still be substantial differences. The application should call
 * UART_GetBaudRate() after UART_SetBaudRate() to check
 * what baud rate was actually set.
 *
 * @param instance The instance structure
 * @param desiredBaudRate desired baud rate.
 * @return STATUS_SUCCESS
 */
status_t UART_SetBaudRate(const uart_instance_t * const instance, uint32_t desiredBaudRate);

/*!
 * @brief Returns the UART baud rate.
 *
 * This function returns the UART configured baud rate.
 *
 * @param instance  Instance number.
 * @param[out] configuredBaudRate configured baud rate.
 * @return STATUS_SUCCESS
 */
status_t UART_GetBaudRate(const uart_instance_t * const instance, uint32_t * configuredBaudRate);

/*!
 * @brief Perform a blocking UART transmission
 *
 * This function sends a block of data and
 * only returns when the transmission is complete.
 *
 * @param[in] instance The instance structure
 * @param[in] txBuffer pointer to the data to be transferred
 * @param[in] txSize length in bytes of the data to be transferred
 * @param[in] timeout timeout value in milliseconds
 * @return    Error or success status returned by API
 */
status_t UART_SendDataBlocking(
        const uart_instance_t * const instance,
        const uint8_t * txBuff,
        uint32_t txSize,
        uint32_t timeout);

/*!
 * @brief Perform a non-blocking UART transmission
 *
 * This function sends a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode).
 *
 * @param[in] instance The instance structure
 * @param[in] txBuffer pointer to the data to be transferred
 * @param[in] txSize length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t UART_SendData(const uart_instance_t * const instance, const uint8_t * txBuff, uint32_t txSize);

/*!
 * @brief Terminates a non-blocking transmission early.
 *
 * @param instance The instance structure
 * @return Whether the aborting is successful or not.
 */
status_t UART_AbortSendingData(const uart_instance_t * const instance);

/*!
 * @brief Get the status of the current non-blocking UART transmission
 *
 * @param instance The instance structure
 * @param bytesRemaining Pointer to value that is populated with the number of bytes that
 *        have been sent in the active transfer
 *        @note In DMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the DMA transfer descriptor.
 * @return The transmit status.
 * @retval STATUS_SUCCESS The transmit has completed successfully.
 * @retval STATUS_BUSY The transmit is still in progress. @a bytesTransmitted will be
 *     filled with the number of bytes that have been transmitted so far.
 * @retval STATUS_UART_ABORTED The transmit was aborted.
 * @retval STATUS_TIMEOUT A timeout was reached.
 * @retval STATUS_ERROR An error occurred.
 */
status_t UART_GetTransmitStatus(const uart_instance_t * const instance, uint32_t * bytesRemaining);

/*!
 * @brief Perform a blocking UART reception
 *
 * This function receives a block of data and
 * only returns when the transmission is complete.
 *
 * @param[in] instance The instance structure
 * @param rxBuff    pointer to the receive buffer
 * @param rxSize    length in bytes of the data to be received
 * @param timeout  timeout for the transfer in milliseconds
 * @return    Error or success status returned by API
 */
status_t UART_ReceiveDataBlocking(
        const uart_instance_t * const instance,
        uint8_t * rxBuff,
        uint32_t rxSize,
        uint32_t timeout);

/*!
 * @brief Perform a non-blocking UART reception
 *
 * This function receives a block of data and returns immediately.
 * The rest of the transmission is handled by the interrupt service routine (if the driver
 * is initialized in interrupt mode).
 *
 * @param[in] instance The instance structure
 * @param[in] rxBuff pointer to the data to be transferred
 * @param[in] rxSize length in bytes of the data to be transferred
 * @return    Error or success status returned by API
 */
status_t UART_ReceiveData(const uart_instance_t * const instance, uint8_t * rxBuff, uint32_t rxSize);

/*!
 * @brief Terminates a non-blocking receive early.
 *
 * @param instance  Instance number
 *
 * @return Whether the receiving was successful or not.
 */
status_t UART_AbortReceivingData(const uart_instance_t * const instance);

/*!
 * @brief Get the status of the current non-blocking UART reception
 *
 * @param instance  Instance number
 * @param bytesRemaining Pointer to value that is filled  with the number of bytes that
 *        still need to be received in the active transfer
 *        @note In DMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the DMA transfer descriptor.
 * @return The transmit status.
 * @retval STATUS_SUCCESS The transmit has completed successfully.
 * @retval STATUS_BUSY The transmit is still in progress. @a bytesTransmitted will be
 *     filled with the number of bytes that have been transmitted so far.
 * @retval STATUS_UART_ABORTED The transmit was aborted.
 * @retval STATUS_TIMEOUT A timeout was reached.
 * @retval STATUS_ERROR An error occurred.
 */
status_t UART_GetReceiveStatus(const uart_instance_t * const instance, uint32_t * bytesRemaining);

/*!
 * @brief Provide a buffer for receiving data.
 *
 * The function can be used to provide a new buffer for receiving data to the driver.
 * Beside, It can be called from rx callback to provide a new buffer for continuous reception.
 *
 * @param instance The instance structure.
 * @param rxBuff Pointer to buffer containing received data.
 * @param rxSize The number of bytes to receive.
 * @return Executed status.
 * @retVal STATUS_ERROR Wrong instance
 * @retVal STATUS_SUCCESS Provide completed.
*/
status_t UART_SetRxBuffer(const uart_instance_t * const instance, uint8_t * rxBuff, uint32_t rxSize);

/*!
 * @brief Provide a buffer for transmitting data.
 *
 * The function can be used to provide a new buffer for transmitting data to the driver.
 * Beside, It can be called from tx callback to provide a new buffer for continuous transmission.
 *
 * @param instance The instance structure.
 * @param txBuff Pointer to buffer containing transmitted data.
 * @param txSize The number of bytes to transmit.
 * @return Executed status.
 * @retVal STATUS_ERROR Wrong instance
 * @retVal STATUS_SUCCESS Provide completed.
*/
status_t UART_SetTxBuffer(const uart_instance_t * const instance, const uint8_t * txBuff, uint32_t txSize);

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* UART_PAL_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
