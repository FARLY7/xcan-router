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
 * @file usdhc_driver.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The macros are public and used by application.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, global typedef not referenced
 * This structure is used by user.
 *
 */

#ifndef USDHC_DRIVER_H
#define USDHC_DRIVER_H

#include "device_registers.h"
#include "status.h"
#include <stdlib.h>
#include "clock_manager.h"
#include "interrupt_manager.h"
#include "osif.h"


/******************************************************************************
 * Definitions.
 *****************************************************************************/

/*! @brief Definitions some macros to check signal level in DATx pins */
#define uSDHC_DATA0_LINE_LEVEL      (1U << 24U)     /*!< Data0 line signal level */
#define uSDHC_DATA1_LINE_LEVEL      (1U << 25U)     /*!< Data1 line signal level */
#define uSDHC_DATA2_LINE_LEVEL      (1U << 26U)     /*!< Data2 line signal level */
#define uSDHC_DATA3_LINE_LEVEL      (1U << 27U)     /*!< Data3 line signal level */
#define uSDHC_DATA4_LINE_LEVEL      (1U << 28U)     /*!< Data4 line signal level */
#define uSDHC_DATA5_LINE_LEVEL      (1U << 29U)     /*!< Data5 line signal level */
#define uSDHC_DATA6_LINE_LEVEL      (1U << 30U)     /*!< Data6 line signal level */
#define uSDHC_DATA7_LINE_LEVEL      (1U << 31U)     /*!< Data7 line signal level */

/*! @brief Flags to check if uSDHC supports 4 bit or 8 bit mode */
#define uSDHC_HOST_CTRL_CAP_SUPPORT_4BIT    (uSDHC_HOST_CTRL_CAP_MBL_SHIFT << 0U)
#define uSDHC_HOST_CTRL_CAP_SUPPORT_8BIT    (uSDHC_HOST_CTRL_CAP_MBL_SHIFT << 1U)

/*! @brief Mask and shift macro to calculate the command response in case response type is R2 */
#define uSDHC_RESPONSE_TYPE_R2_MASK         (0xFF000000U)
#define uSDHC_RESPONSE_TYPE_R2_SHIFT        (24U)

/*! @brief Reset type masks
 * Implements : usdhc_reset_mask_t_Class
 */
typedef enum
{
    uSDHC_RESET_CMD = uSDHC_SYS_CTRL_RSTC_MASK,     /*!< Reset command line */
    uSDHC_RESET_DATA = uSDHC_SYS_CTRL_RSTD_MASK,    /*!< Reset data line */
    uSDHC_RESET_ALL = uSDHC_SYS_CTRL_RSTA_MASK      /*!< Reset all except card detection */
} usdhc_reset_mask_t;

/*! @brief Present status flag masks
 * Implements : usdhc_present_status_flag_t_Class
 */
typedef enum
{
    uSDHC_COMMAND_INHIBIT = uSDHC_PRES_STATE_CIHB_MASK,      /*!< If this status bit is 0, it indicates that the CMD line is not in use
                                                                  and the uSDHC can issue a SD/MMC Command using the CMD line */
    uSDHC_DATA_INHIBIT = uSDHC_PRES_STATE_CDIHB_MASK,        /*!< If this status bit is 0, it indicates that the DAT line is not in use
                                                                  and the uSDHC can issue a SD/MMC Command using the DAT line */
    uSDHC_DATA_LINE_ACTIVE = uSDHC_PRES_STATE_DLA_MASK,      /*!< If this status bit is 0, it indicates that all of the
                                                                  DAT lines on the SD Bus is not in use */
    uSDHC_SD_CLOCK_STABLE = uSDHC_PRES_STATE_SDSTB_MASK,     /*!< SD bus clock stable */
    uSDHC_WRITE_TRANSFER_ACTIVE = uSDHC_PRES_STATE_WTA_MASK, /*!< Write transfer active */
    uSDHC_READ_TRANSFER_ACTIVE = uSDHC_PRES_STATE_RTA_MASK,  /*!< Read transfer active */
    uSDHC_BUFFER_WRITE_ENABLE = uSDHC_PRES_STATE_BWEN_MASK,  /*!< Buffer write enable */
    uSDHC_BUFFER_READ_ENABLE = uSDHC_PRES_STATE_BREN_MASK,   /*!< Buffer read enable */
    uSDHC_CARD_INSERTED = uSDHC_PRES_STATE_CINST_MASK,       /*!< Card inserted */
    uSDHC_CMD_LINE_LEVEL = uSDHC_PRES_STATE_CLSL_MASK,       /*!< CMD line signal level */
    uSDHC_WRITE_PROTECTED = uSDHC_PRES_STATE_WPSPL_MASK      /*!< Card has been protected for write operation */
} usdhc_present_status_flag_t;

/*! @brief Host controller capabilities flag mask
 * Implements : usdhc_capability_flag_t_Class
 */
typedef enum
{
    uSDHC_SUPPORT_ADMA = uSDHC_HOST_CTRL_CAP_ADMAS_MASK,            /*!< Support ADMA */
    uSDHC_SUPPORT_HIGH_SPEED = uSDHC_HOST_CTRL_CAP_HSS_MASK,        /*!< Support high-speed */
    uSDHC_SUPPORT_DMA = uSDHC_HOST_CTRL_CAP_DMAS_MASK,              /*!< Support DMA */
    uSDHC_SUPPORT_SUSPEND_RESUME = uSDHC_HOST_CTRL_CAP_SRS_MASK,    /*!< Support suspend/resume */
    uSDHC_SUPPORT_1V8 = uSDHC_HOST_CTRL_CAP_VS18_MASK,              /*!< Support voltage 1.8V */
    uSDHC_SUPPORT_3V0 = uSDHC_HOST_CTRL_CAP_VS30_MASK,              /*!< Support voltage 3.0V */
    uSDHC_SUPPORT_3V3 = uSDHC_HOST_CTRL_CAP_VS33_MASK,              /*!< Support voltage 3.3V */
    uSDHC_SUPPORT_4BIT = uSDHC_HOST_CTRL_CAP_SUPPORT_4BIT,          /*!< Support 4 bit mode */
    uSDHC_SUPPORT_8BIT = uSDHC_HOST_CTRL_CAP_SUPPORT_8BIT           /*!< Support 8 bit mode */
} usdhc_capability_flag_t;

/*! @brief Interrupt status flag mask
 * Implements : usdhc_interrupt_t_Class
 */
typedef enum
{
    uSDHC_COMMAND_COMPLETE_INT = uSDHC_INT_STATUS_CC_MASK,          /*!< Command complete */
    uSDHC_DATA_COMPLETE_INT = uSDHC_INT_STATUS_TC_MASK,             /*!< Data complete */
    uSDHC_BLOCK_GAP_INT = uSDHC_INT_STATUS_BGE_MASK,                /*!< Block gap event */
    uSDHC_DMA_COMPLETE_INT = uSDHC_INT_STATUS_DINT_MASK,            /*!< DMA interrupt */
    uSDHC_BUFFER_WRITE_READY_INT = uSDHC_INT_STATUS_BWR_MASK,       /*!< Buffer write ready */
    uSDHC_BUFFER_READ_READY_INT = uSDHC_INT_STATUS_BRR_MASK,        /*!< Buffer read ready */
    uSDHC_CARD_INSERT_INT = uSDHC_INT_STATUS_CINS_MASK,             /*!< Card inserted */
    uSDHC_CARD_REMOVE_INT = uSDHC_INT_STATUS_CRM_MASK,              /*!< Card removed */
    uSDHC_CARD_INTERRUPT_INT = uSDHC_INT_STATUS_CINT_MASK,          /*!< Card interrupt */
    uSDHC_COMMAND_TIMEOUT_INT = uSDHC_INT_STATUS_CTOE_MASK,         /*!< Command timeout error */
    uSDHC_COMMAND_CRC_ERROR_INT = uSDHC_INT_STATUS_CCE_MASK,        /*!< Command CRC error */
    uSDHC_COMMAND_ENDBIT_ERROR_INT = uSDHC_INT_STATUS_CEBE_MASK,    /*!< Command end bit error */
    uSDHC_COMMAND_INDEX_ERROR_INT = uSDHC_INT_STATUS_CIE_MASK,      /*!< Command index error */
    uSDHC_DATA_TIMEOUT_INT = uSDHC_INT_STATUS_DTOE_MASK,            /*!< Data timeout error */
    uSDHC_DATA_CRC_ERROR_INT = uSDHC_INT_STATUS_DCE_MASK,           /*!< Data CRC error */
    uSDHC_DATA_ENDBIT_ERROR_INT = uSDHC_INT_STATUS_DEBE_MASK,       /*!< Data end bit error */
    uSDHC_AUTO_CMD12_ERROR_INT = uSDHC_INT_STATUS_AC12E_MASK,       /*!< Auto CMD12 error */
    uSDHC_DMA_ERROR_INT = uSDHC_INT_STATUS_DMAE_MASK                /*!< DMA error */
} usdhc_interrupt_t;

/*! @brief Data transfer width
 * Implements : usdhc_data_bus_width_t_Class
 */
typedef enum
{
    uSDHC_DATA_BUS_WIDTH_1BIT = 0U,     /*!< 1-bit mode */
    uSDHC_DATA_BUS_WIDTH_4BIT = 1U,     /*!< 4-bit mode */
    uSDHC_DATA_BUS_WIDTH_8BIT = 2U      /*!< 8-bit mode */
} usdhc_data_bus_width_t;

/*! @brief Endian mode
 * Implements : usdhc_endian_mode_t_Class
 */
typedef enum
{
    uSDHC_ENDIAN_MODE_BIG = 0U,             /*!< Big endian mode */
    uSDHC_ENDIAN_MODE_HALF_WORD_BIG = 1U,   /*!< Half word big endian mode */
    uSDHC_ENDIAN_MODE_LITTLE = 2U           /*!< Little endian mode */
} usdhc_endian_mode_t;

/*! @brief DMA mode
 * Implements : usdhc_dma_mode_t_Class
 */
typedef enum
{
    uSDHC_DMA_MODE_NO = 0U,         /*!< No DMA, use interrupt or polling method for transferring */
    uSDHC_DMA_MODE_ADMA1 = 1U,      /*!< ADMA1 is selected for transferring */
    uSDHC_DMA_MODE_ADMA2 = 2U       /*!< ADMA2 is selected for transferring */
} usdhc_dma_mode_t;

/*! @brief The command type
 * Implements : usdhc_command_type_t_Class
 */
typedef enum
{
    uSDHC_COMMAND_TYPE_NORMAL = 0U,     /*!< Normal command */
    uSDHC_COMMAND_TYPE_SUSPEND = 1U,    /*!< Suspend command */
    uSDHC_COMMAND_TYPE_RESUME = 2U,     /*!< Resume command */
    uSDHC_COMMAND_TYPE_ABORT = 3U       /*!< Abort command */
} usdhc_command_type_t;

/*! @brief Card control masks
 * Implements : usdhc_card_control_t_Class
 */
typedef enum
{
    uSDHC_WAKEUP_ON_CARD_INT = uSDHC_PROT_CTRL_WECINT_MASK,     /*!< Wakeup on card interrupt */
    uSDHC_WAKEUP_ON_CARD_INSERT = uSDHC_PROT_CTRL_WECINS_MASK,  /*!< Wakeup on card insertion */
    uSDHC_WAKEUP_ON_CARD_REMOVE = uSDHC_PROT_CTRL_WECRM_MASK,   /*!< Wakeup on card removal */
    uSDHC_STOP_AT_BLOCK_GAP = uSDHC_PROT_CTRL_SABGREQ_MASK,     /*!< Stop at block gap request */
    uSDHC_READ_WAIT_CONTROL = uSDHC_PROT_CTRL_RWCTL_MASK,       /*!< Read wait control */
    uSDHC_INT_AT_BLOCK_GAP = uSDHC_PROT_CTRL_IABG_MASK,         /*!< Interrupt at block gap */
    uSDHC_NON_EXACT_BLOCK_READ = uSDHC_PROT_CTRL_NON_EXACT_BLK_RD_MASK,      /*!< Non exact block read */
    uSDHC_CONTINUE_REQUEST = uSDHC_PROT_CTRL_CREQ_MASK          /*!< Continue request */
} usdhc_card_control_t;

/*!
 * @brief The command response type.
 *
 * Define the command response type from card to host controller.
 * Implements : usdhc_response_type_t_Class
 */
typedef enum
{
    uSDHC_RESPONSE_TYPE_NONE = 0U, /*!< Response type: none */
    uSDHC_RESPONSE_TYPE_R1 = 1U,   /*!< Response type: R1 */
    uSDHC_RESPONSE_TYPE_R1b = 2U,  /*!< Response type: R1b */
    uSDHC_RESPONSE_TYPE_R2 = 3U,   /*!< Response type: R2 */
    uSDHC_RESPONSE_TYPE_R3 = 4U,   /*!< Response type: R3 */
    uSDHC_RESPONSE_TYPE_R4 = 5U,   /*!< Response type: R4 */
    uSDHC_RESPONSE_TYPE_R5 = 6U,   /*!< Response type: R5 */
    uSDHC_RESPONSE_TYPE_R5b = 7U,  /*!< Response type: R5b */
    uSDHC_RESPONSE_TYPE_R6 = 8U,   /*!< Response type: R6 */
    uSDHC_RESPONSE_TYPE_R7 = 9U    /*!< Response type: R7 */
} usdhc_response_type_t;

/*!
 * @brief The event for uSDHC. This enumeration is used to register the callback function.
 * Implements : usdhc_event_t_Class
 */
typedef enum
{
    uSDHC_EVENT_TRANSFER_COMPLETE,      /* Transfer completion event */
    uSDHC_EVENT_CARD_INSERT,            /* Card insertion event */
    uSDHC_EVENT_CARD_REMOVE,            /* Card removal event */
    uSDHC_EVENT_BLOCK_GAP,              /* Block gap event */
    uSDHC_EVENT_CARD_INTERRUPT          /* Card interrupt event */
} usdhc_event_t;

/*!
 * @brief Callback function for uSDHC
 *
 * @param instance  uSDHC number instance invoking this function
 * @param status    This parameter is used for transfer complete callback function. It indicates the status of a transfer.
 *                  In case of success, value of status is STATUS_SUCCESS and in case of failure, this value will contain the interrupt flag
 *                  of uSDHC module. User can do a logical OR of this value with "usdhc_interrupt_t" enumeration to know what error happened
 * @param param     The user data that registered before by InstallCallback functions
 * Implements : usdhc_callback_t_Class
 */
typedef void (*usdhc_callback_t)(uint32_t instance, uint32_t status, void * param);

/*! @brief Data structure to initialize the uSDHC
 * Implements : usdhc_config_t_Class
 */
typedef struct
{
    bool                    cardDetectDat3;     /*!< Set this field to true if user want to use DAT3 pin as a card detection pin */
    usdhc_endian_mode_t     endianMode;         /*!< Endian mode */
    usdhc_dma_mode_t        dmaMode;            /*!< DMA mode */
    void *                  admaTable;          /*!< ADMA table for transferring with ADMA1 and ADMA2 mode. */
    uint32_t                admaTableSize;      /*!< The size of ADMA table in bytes */
} usdhc_config_t;

/*!
 * @brief Card data descriptor
 *
 * Define structure to contain data-related attribute.
 * Implements : usdhc_data_t_Class
 */
typedef struct
{
    bool             enableAutoCMD12;    /*!< Enable auto CMD12. If this field is false, user have to send a "CMD12" command to card after
                                              each time send/receive data to stop the data transfer. If this field is true, uSDHC will send
                                              "CMD12" command automatically after data transfer is end */
    uint32_t         blockSize;          /*!< Block size for each data transfer */
    uint32_t         blockCount;         /*!< Block count for each data transfer */
    uint32_t *       rxData;             /*!< Buffer to store the read data */
    const uint32_t * txData;             /*!< Data buffer to write */
} usdhc_data_t;

/*!
 * @brief Card command descriptor
 *
 * Define card command-related attribute.
 * Implements : usdhc_command_t_Class
 */
typedef struct
{
    uint32_t                index;          /*!< Command index */
    uint32_t                argument;       /*!< Command argument */
    usdhc_command_type_t    type;           /*!< Command type */
    usdhc_response_type_t   responseType;   /*!< Command response type */
    uint32_t                response[4U];   /*!< Response for this command */
} usdhc_command_t;

/*! @brief Transfer state
 * Implements : usdhc_transfer_t_Class
 */
typedef struct
{
    usdhc_data_t      *data;      /*!< Data to transfer */
    usdhc_command_t   *command;   /*!< Command to send */
} usdhc_transfer_t;

/*!
 * @brief uSDHC capability information.
 *
 * Define structure to save the capability information of uSDHC.
 * Implements : usdhc_capability_t_Class
 */
typedef struct
{
    uint32_t specVersion;    /*!< Specification version */
    uint32_t vendorVersion;  /*!< Vendor version */
    uint32_t maxBlockLength; /*!< Maximum block length united as byte */
    uint32_t maxBlockCount;  /*!< Maximum block count can be set one time */
    uint32_t flags;          /*!< Capability flags to indicate the support information (a logical OR of "usdhc_capability_flag_t") */
} usdhc_capability_t;

/*!
 * @brief uSDHC current state.
 *
 * Define structure to save the current state of uSDHC.
 * Implements : usdhc_state_t_Class
 */
typedef struct
{
    volatile bool     isBlocking;
    void *            admaTable;            /*!< ADMA table is used to transfer the data */
    uint32_t          admaTableSize;        /*!< The size of ADMA table in bytes */

    /* Transfer parameters */
    usdhc_data_t * volatile    data;        /*!< Data to transfer */
    usdhc_command_t * volatile command;     /*!< Command to send */

    /* Transfer status */
    volatile uint32_t transferredWords;     /*!< Words transferred by DATAPORT way */

    /* Callback functions */
    usdhc_callback_t cardInsertCallback;    /*!< Card insertion callback function */
    void * cardInsertCallbackParam;         /*!< Parameter for Card insertion callback function */
    usdhc_callback_t cardRemoveCallback;    /*!< Card removal callback function */
    void * cardRemoveCallbackParam;         /*!< Parameter for Card removal callback function */
    usdhc_callback_t transferCallback;      /*!< Transfer complete callback function */
    void * transferCallbackParam;           /*!< Parameter for transfer complete callback function */
    usdhc_callback_t blockGapCallback;      /*!< Block gap callback function */
    void * blockGapCallbackParam;           /*!< Parameter for block gap callback function */
    usdhc_callback_t cardIntCallback;       /*!< Card interrupt callback function */
    void * cardIntCallbackParam;            /*!< Parameter for card interrupt callback function */

    semaphore_t semaTransferComplete;       /*!< Synchronization object for blocking transfer timeout condition */
} usdhc_state_t;

/*************************************************************************************************
 * API
 ************************************************************************************************/
/*! @brief uSDHC base pointer array */
extern uSDHC_Type * const g_usdhcBases[uSDHC_INSTANCE_COUNT];

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Gets the default configuration structure.
 *
 * This function gets the default configuration structure, with the following settings:
 * - card detection by DAT3/CD pin is disabled
 * - transfer method: ADMA2
 * - endian mode: little endian
 *
 * @param[out] config   The configuration structure buffer to store the got value
 *
 * @return     NONE
 */
void uSDHC_DRV_GetDefaultConfig(usdhc_config_t * config);

/*!
 * @brief Initializes a uSDHC module.
 *
 * This function initializes the uSDHC module, it must be called before using other functions.
 * The user must enable the uSDHC clock source in the application before calling this function.
 *
 * @param[in] instance          uSDHC instance number
 * @param[in] state             pointer to the uSDHC driver state structure
 * @param[in] config            user configuration structure of type #usdhc_config_t
 * @return    STATUS_SUCCESS   if successful;
 *            STATUS_ERROR     if failed
 */
status_t uSDHC_DRV_Init(uint32_t instance,
                        usdhc_state_t * state,
                        const usdhc_config_t *config);

/*!
 * @brief Shuts down the uSDHC module.
 *
 * @param[in] instance         uSDHC instance number
 *
 * @return    STATUS_SUCCESS   if successful;
 *            STATUS_ERROR     if failed
 */
status_t uSDHC_DRV_Deinit(uint32_t instance);

/*!
 * @brief Reset the uSDHC module.
 *
 * @param[in] instance         uSDHC instance number
 * @param[in] mask             The reset type mask (a logical OR of "usdhc_reset_mask_t").
 * @param[in] timeoutMs        Timeout value for resetting in milliseconds.
 *
 * @return    STATUS_SUCCESS   if successful;
 *            STATUS_TIMEOUT   if timeout occurs
 */
status_t uSDHC_DRV_Reset(uint32_t instance,
                         uint32_t mask,
                         uint32_t timeoutMs);

/*!
 * @brief Register the callback functions for uSDHC events
 *
 * @param[in] instance        uSDHC instance number
 * @param[in] event           The event that need to be installed
 * @param[in] function        The callback function to be registered
 * @param[in] param           The user data used as the callback parameter
 *
 * @return    NONE
 */
void uSDHC_DRV_InstallCallback(uint32_t instance,
                               usdhc_event_t event,
                               usdhc_callback_t function,
                               void * param);

/*!
 * @brief Get supported capability of uSDHC module.
 *
 * @param[in]   instance    uSDHC instance number
 * @param[out]  capability  The usdhc_capability_t structure used to store the supported capability
 *
 * @return      NONE
 */
void uSDHC_DRV_GetCapability(uint32_t instance,
                             usdhc_capability_t * capability);

/*!
 * @brief Set bus clock frequency.
 *
 * @param[in] instance      uSDHC instance number.
 * @param[in] busClock      bus clock frequency in Hz.
 *
 * @return    The nearest frequency of busClock configured to SD bus.
 */
uint32_t uSDHC_DRV_SetBusClock(uint32_t instance,
                               uint32_t busClock);

/*!
 * @brief Send 80 clock cycles to the card to activate the card
 *
 * @param[in] instance         uSDHC instance number.
 * @param[in] timeoutMs        Timeout value in millisecond
 *
 * @return    STATUS_SUCCESS   if successful;
 *            STATUS_TIMEOUT   if timeout occurs
 */
status_t uSDHC_DRV_SetCardActive(uint32_t instance,
                                 uint32_t timeoutMs);

/*!
 * @brief Set data bus width for uSDHC module
 *
 * @param[in] instance         uSDHC instance number.
 * @param[in] width            Data bus width (One of the value in usdhc_data_bus_width_t)
 *
 * @return    NONE
 */
void uSDHC_DRV_SetDataBusWidth(uint32_t instance,
                               usdhc_data_bus_width_t width);

/*!
 * @brief Transfer command/data using blocking mode
 *
 * @param[in] instance         uSDHC instance number.
 * @param[in] transfer         Transfer content including the command or data
 * @param[in] timeoutMs        Timeout value
 *
 * @return    STATUS_SUCCESS   if transfer is successful
 *            STATUS_TIMEOUT   if timeout occurs
 *            STATUS_BUSY      if previous transfer is in progress
 *            STATUS_USDHC_PREPARE_ADMA_FAILED      if fail to configure the ADMA table
 *            STATUS_ERROR     The other errors
 */
status_t uSDHC_DRV_TransferBlocking(uint32_t instance,
                                    const usdhc_transfer_t * transfer,
                                    uint32_t timeoutMs);

/*!
 * @brief Transfer command/data using non-blocking mode
 *
 * @param[in] instance         uSDHC instance number.
 * @param[in] transfer         Transfer content including the command or data
 *
 * @return    STATUS_SUCCESS   if transfer is successful
 *            STATUS_BUSY      if previous transfer is in progress
 *            STATUS_USDHC_PREPARE_ADMA_FAILED      if fail to configure the ADMA table
 *            STATUS_ERROR     The other errors
 */
status_t uSDHC_DRV_Transfer(uint32_t instance,
                            const usdhc_transfer_t * transfer);

/*!
 * @brief Enable or disable some features for uSDHC.
 *
 * @param[in] instance  uSDHC instance number.
 * @param[in] mask      Control mask (a logical OR of "usdhc_card_control_t").
 * @param[in] enable    TRUE to enable, FALSE to disable.
 *
 * @return    NONE
 */
void uSDHC_DRV_EnableCardControl(uint32_t instance,
                                 uint32_t mask,
                                 bool enable);

/*!
 * @brief This function is used to get the status of uSDHC such as logic level of physical pins, card has been inserted or not,
 *        buffer read/write is enabled or not, etc.
 *
 * @param[in] instance   Instance number of uSDHC
 *
 * @return    Present uSDHC's status flags mask(usdhc_present_status_flag_t)
 * Implements : uSDHC_DRV_GetPresentStatusFlags_Activity
 */
static inline uint32_t uSDHC_DRV_GetPresentStatusFlags(uint32_t instance)
{
    DEV_ASSERT(instance < uSDHC_INSTANCE_COUNT);
    return (g_usdhcBases[instance]->PRES_STATE);
}


#if defined(__cplusplus)
}
#endif

#endif /* USDHC_DRIVER_H*/
