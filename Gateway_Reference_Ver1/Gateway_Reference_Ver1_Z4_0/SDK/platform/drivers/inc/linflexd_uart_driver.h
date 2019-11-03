/*
 * Copyright (c) 2016 , Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
 * @file linflexd_uart_driver.h
 */

#ifndef LINFLEXD_UART_DRIVER_H
#define LINFLEXD_UART_DRIVER_H

#include <stddef.h>
#include <stdbool.h>
#include "clock_manager.h"
#include "interrupt_manager.h"
#include "osif.h"
#if defined(FEATURE_LINFLEXD_HAS_DMA_ENABLED)
#include "edma_driver.h"
#endif
#include "callbacks.h"

/*!
 * @addtogroup linflexd_uart
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Word length in UART mode.
 *
 * Implements : linflexd_uart_word_length_t_Class
 */
typedef enum
{
    LINFLEXD_UART_7_BITS = 0U,
    LINFLEXD_UART_8_BITS = 1U,
    LINFLEXD_UART_15_BITS = 2U,
    LINFLEXD_UART_16_BITS = 3U
} linflexd_uart_word_length_t;

/*! @brief Number of stop bits.
 *
 * Implements : linflexd_uart_stop_bits_count_t_Class
 */
typedef enum
{
    LINFLEXD_UART_ONE_STOP_BIT = 0U,
    LINFLEXD_UART_TWO_STOP_BIT = 1U
} linflexd_uart_stop_bits_count_t;

/*! @brief Parity type.
 *
 * Implements : linflexd_uart_parity_type_t_Class
 */
typedef enum
{
    LINFLEXD_UART_PARITY_EVEN = 0U,
    LINFLEXD_UART_PARITY_ODD = 1U,
    LINFLEXD_UART_PARITY_ZERO = 2U,
    LINFLEXD_UART_PARITY_ONE = 3U
} linflexd_uart_parity_type_t;

/*! @brief Type of UART transfer (based on interrupts or DMA).
 *
 * Implements : linflexd_uart_transfer_type_t_Class
 */
typedef enum
{
    LINFLEXD_UART_USING_DMA         = 0U,    /*!< The driver will use DMA to perform UART transfer */
    LINFLEXD_UART_USING_INTERRUPTS  = 1U     /*!< The driver will use interrupts to perform UART transfer */
} linflexd_uart_transfer_type_t;

/*! @brief UART state structure
 *
 * Implements : linflexd_uart_state_t_Class
 */
typedef struct
{
    const uint8_t * txBuff;                      /*!< The buffer of data being sent.*/
    uint8_t * rxBuff;                            /*!< The buffer of received data.*/
    volatile uint32_t txSize;                    /*!< The remaining number of bytes to be transmitted. */
    volatile uint32_t rxSize;                    /*!< The remaining number of bytes to be received. */
    volatile bool isTxBusy;                      /*!< True if there is an active transmit.*/
    volatile bool isRxBusy;                      /*!< True if there is an active receive.*/
    volatile bool isTxBlocking;                  /*!< True if transmit is blocking transaction. */
    volatile bool isRxBlocking;                  /*!< True if receive is blocking transaction. */
    uart_callback_t rxCallback;                  /*!< Callback to invoke for data receive */
    void * rxCallbackParam;                      /*!< Receive callback parameter pointer.*/
    uart_callback_t txCallback;                  /*!< Callback to invoke for data send */
    void * txCallbackParam;                      /*!< Transmit callback parameter pointer.*/
    uart_callback_t errorCallback;               /*!< Callback to invoke on error conditions */
    void * errorCallbackParam;                   /*!< Error callback parameter pointer */
    linflexd_uart_word_length_t wordLength;      /*!< Word length (8/16 bits) */
    semaphore_t rxComplete;                      /*!< Synchronization object for blocking Rx timeout condition */
    semaphore_t txComplete;                      /*!< Synchronization object for blocking Tx timeout condition */
    linflexd_uart_transfer_type_t transferType;  /*!< Type of UART transfer (interrupt/dma based) */
#if defined(FEATURE_LINFLEXD_HAS_DMA_ENABLED)
    uint8_t rxDMAChannel;                        /*!< DMA channel number for DMA-based rx. */
    uint8_t txDMAChannel;                        /*!< DMA channel number for DMA-based tx. */
#endif
    volatile status_t transmitStatus;            /*!< Status of last driver transmit operation */
    volatile status_t receiveStatus;             /*!< Status of last driver receive operation */
} linflexd_uart_state_t;

/*! @brief UART configuration structure
 *
 * Implements : linflexd_uart_user_config_t_Class
 */
typedef struct
{
    uint32_t baudRate;                             /*!< baud rate */
    bool parityCheck;                              /*!< parity control - enabled/disabled
                                                        NOTE: this parameter is valid only for 8/16 bits words;
                                                              for 7/15 word length parity is always enabled */
    linflexd_uart_parity_type_t parityType;        /*!< always 0/always 1/even/odd */
    linflexd_uart_stop_bits_count_t stopBitsCount; /*!< number of stop bits, 1 stop bit (default) or 2 stop bits */
    linflexd_uart_word_length_t wordLength;        /*!< number of bits per transmitted/received word */
    linflexd_uart_transfer_type_t transferType;    /*!< Type of UART transfer (interrupt/dma based) */
    uint8_t rxDMAChannel;                          /*!< Channel number for DMA rx channel.
                                                        If DMA mode is not used this field will be ignored. */
    uint8_t txDMAChannel;                          /*!< Channel number for DMA tx channel.
                                                        If DMA mode is not used this field will be ignored. */
} linflexd_uart_user_config_t;


/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Sets the baud rate for UART communication.
 *
 * This function computes the fractional and integer parts of the baud rate divisor
 * to obtain the desired baud rate using the current protocol clock.
 *
 * @param instance LINFlexD instance number
 * @param baudrate desired baud rate
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_SetBaudRate(uint32_t instance, uint32_t baudrate);

/*!
 * @brief Gets the baud rate for UART communication.
 *
 * This function returns the current UART baud rate, according to register values
 * and the protocol clock frequency.
 *
 * @param instance LINFlexD instance number
 * @param baudrate current baud rate
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_GetBaudRate(uint32_t instance, uint32_t * baudrate);

/*!
 * @brief Installs callback function for the UART receive.
 *
 * After a callback is installed, it bypasses part of the UART IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance The LINFlexD instance number.
 * @param function The UART receive callback function.
 * @param callbackParam The UART receive callback parameter pointer.
 * @return Former UART receive callback function pointer.
 */
uart_callback_t LINFLEXD_UART_DRV_InstallRxCallback(uint32_t instance,
                                                    uart_callback_t function,
                                                    void * callbackParam);

/*!
 * @brief Installs callback function for the UART transmit.
 *
 * After a callback is installed, it bypasses part of the UART IRQHandler logic.
 * Therefore, the callback needs to handle the indexes of txBuff and txSize.
 *
 * @param instance The LINFlexD instance number.
 * @param function The UART transmit callback function.
 * @param callbackParam The UART transmit callback parameter pointer.
 * @return Former UART transmit callback function pointer.
 */
uart_callback_t LINFLEXD_UART_DRV_InstallTxCallback(uint32_t instance,
                                                    uart_callback_t function,
                                                    void * callbackParam);

/*!
 * @brief Installs callback function for the UART error cases.
 *
 * After a callback is installed, the user must handle the error cases inside the
 * callback function.
 *
 * @param instance The LINFlexD instance number.
 * @param function The UART error callback function.
 * @param callbackParam The UART error callback parameter pointer.
 * @return Former UART error callback function pointer.
 */
uart_callback_t LINFLEXD_UART_DRV_InstallErrorCallback(uint32_t instance,
                                                       uart_callback_t function,
                                                       void * callbackParam);

/*!
 * @brief Initializes a LINFlexD instance for UART operation.
 *
 * The caller provides memory for the driver state structures during initialization.
 * The user must enable the LINFlexD clock source in the application to initialize the module.
 *
 * @param instance LINFlexD instance number
 * @param uartStatePtr pointer to the UART driver state structure
 * @param uartUserConfig user configuration structure of type #linflexd_uart_user_config_t
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_Init(uint32_t instance, linflexd_uart_state_t * uartStatePtr,
                                const linflexd_uart_user_config_t * uartUserConfig);

/*!
 * @brief Shuts down the UART functionality of the LINFlexD module
 * by disabling interrupts and transmitter/receiver.
 *
 * @param instance LINFlexD instance number
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_Deinit(uint32_t instance);

/*!
 * @brief Sends data using LINFlexD module in UART mode with blocking method.
 *
 * Blocking means that the function does not return until the transmission is complete.
 *
 * @param instance LINFlexD instance number
 * @param txBuff source buffer containing 8-bit data chars to send
 * @param txSize the number of bytes to send
 * @param timeout timeout value in milliseconds
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_SendDataBlocking(uint32_t instance,
                                            const uint8_t * txBuff,
                                            uint32_t txSize,
                                            uint32_t timeout);
/*!
 * @brief Sends data using LINFlexD module in UART mode with non-blocking method.
 *
 * This enables an a-sync method for transmitting data. When used with
 * a non-blocking receive, the UART driver can perform a full duplex operation.
 * Non-blocking  means that the function returns immediately.
 * The application has to get the transmit status to know when the transmission is complete.
 *
 * @param instance LINFlexD instance number
 * @param txBuff source buffer containing 8-bit data chars to send
 * @param txSize the number of bytes to send
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_SendData(uint32_t instance,
                                    const uint8_t * txBuff,
                                    uint32_t txSize);

/*!
 * @brief Returns whether the previous transmit is complete.
 *
 * @param instance LINFlexD instance number
 * @param bytesRemaining Pointer to value that is populated with the number of bytes that
 *        have been sent in the active transfer
 *        @note In DMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the DMA transfer descriptor.
 * @return The transmit status - STATUS_SUCCESS if the transmit has completed successfully,
 *         STATUS_BUSY otherwise.
 */
status_t LINFLEXD_UART_DRV_GetTransmitStatus(uint32_t instance, uint32_t * bytesRemaining);

/*!
 * @brief Terminates a non-blocking transmission early.
 *
 * @param instance LINFlexD instance number
 * @return STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_AbortSendingData(uint32_t instance);

/*!
 * @brief Retrieves data from the LINFlexD module in UART mode with blocking method.
 *
 * Blocking means that the function does not return until the receive is complete.
 *
 * @param instance LINFlexD instance number
 * @param rxBuff buffer containing 8-bit read data chars received
 * @param rxSize the number of bytes to receive
 * @param timeout timeout value in milliseconds
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_ReceiveDataBlocking(uint32_t instance,
                                               uint8_t * rxBuff,
                                               uint32_t rxSize,
                                               uint32_t timeout);

/*!
 * @brief Retrieves data from the LINFlexD module in UART mode with non-blocking method.
 * This enables an a-sync method for receiving data. When used with
 * a non-blocking transmission, the UART driver can perform a full duplex operation.
 * Non-blocking means that the function returns immediately.
 * The application has to get the receive status to know when the receive is complete.
 *
 * @param instance LINFlexD instance number
 * @param rxBuff buffer containing 8-bit read data chars received
 * @param rxSize the number of bytes to receive
 * @return An error code or STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_ReceiveData(uint32_t instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize);

/*!
 * @brief Returns whether the previous receive is complete.
 *
 * @param instance LINFlexD instance number
 * @param bytesRemaining pointer to value that is filled with the number of bytes that
 *        still need to be received in the active transfer.
 *        @note In DMA mode, this parameter may not be accurate, in case the transfer completes
 *              right after calling this function; in this edge-case, the parameter will reflect
 *              the initial transfer size, due to automatic reloading of the major loop count
 *              in the DMA transfer descriptor.
 * @return The receive status - STATUS_SUCCESS if receive operation has completed
 *         successfully, STATUS_BUSY otherwise.
 */
status_t LINFLEXD_UART_DRV_GetReceiveStatus(uint32_t instance, uint32_t * bytesRemaining);

/*!
 * @brief Terminates a non-blocking receive early.
 *
 * @param instance LINFlexD instance number
 *
 * @return STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_AbortReceivingData(uint32_t instance);


/*!
 * @brief Sets the internal driver reference to the tx buffer.
 *
 * This function can be called from the tx callback to provide the driver
 * with a new buffer, for continuous transmission.
 *
 * @param instance  LINFLEXD instance number
 * @param txBuff  source buffer containing 8-bit data chars to send
 * @param txSize  the number of bytes to send
 * @return STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_SetTxBuffer(uint32_t instance,
                                       const uint8_t * txBuff,
                                       uint32_t txSize);

/*!
 * @brief Sets the internal driver reference to the rx buffer.
 *
 * This function can be called from the rx callback to provide the driver
 * with a new buffer, for continuous reception.
 *
 * @param instance  LINFLEXD instance number
 * @param rxBuff  destination buffer containing 8-bit data chars to receive
 * @param rxSize  the number of bytes to receive
 * @return STATUS_SUCCESS
 */
status_t LINFLEXD_UART_DRV_SetRxBuffer(uint32_t instance,
                                       uint8_t * rxBuff,
                                       uint32_t rxSize);

/*! @}*/

#endif /* LINFLEXD_UART_DRIVER_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/
