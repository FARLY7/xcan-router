/*****************************************************************************/
/* FILE NAME: common.h NXP Semiconductors 2017   							 */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: SD implementation                                            */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR              DATE        DESCRIPTION OF CHANGE            */
/* ---   -----------          ----------    ---------------------            */
/*****************************************************************************/

#ifndef SD_COMMON_H
#define SD_COMMON_H

#include <status.h>

/*!
 * @addtogroup SD_COMMON
 * @ingroup sdhc_mw
 * @{
 */

/*!
 * @sd.c
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 *
   @section [global]
 * Violates MISRA 2012 Advisory Directive 4.9, A function should be used in preference
 * to a function-like macro where they are interchangeable.
 * The macro is used to define static or dynamic implementation of API functions.
 * This way is more efficient.
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))

/*! @brief Construct the version number for drivers. */
#define MAKE_VERSION(major, minor, bugfix) (((major) << 16) | ((minor) << 8) | (bugfix))

/*! @brief Status group numbers. */
enum _status_groups
{
	kStatusGroup_Generic = 0,                 /*!< Group number for generic status codes. */
	kStatusGroup_SDSPI = 22
};

/*! @brief Generic status return codes. */
enum _generic_status
{
    aStatus_Success = MAKE_STATUS((int32_t)kStatusGroup_Generic, 0),
    aStatus_Fail = MAKE_STATUS((int32_t)kStatusGroup_Generic, 1),
    aStatus_ReadOnly = MAKE_STATUS((int32_t)kStatusGroup_Generic, 2),
    aStatus_OutOfRange = MAKE_STATUS((int32_t)kStatusGroup_Generic, 3),
    aStatus_InvalidArgument = MAKE_STATUS((int32_t)kStatusGroup_Generic, 4),
    aStatus_Timeout = MAKE_STATUS((int32_t)kStatusGroup_Generic, 5),
    aStatus_NoTransferInProgress = MAKE_STATUS((int32_t)kStatusGroup_Generic, 6),
};

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*! @brief Return reversed uint8_t data */
uint8_t load_hword(const volatile uint8_t *dstAddr, uintptr_t offset);
/*! @brief Return reversed uint16_t data */
uint16_t load_word(const volatile uint16_t *dstAddr, uintptr_t offset);
/*! @brief Return reversed uint32_t data */
uint32_t load_dword(const volatile uint32_t *dstAddr, uintptr_t offset);
/*! @brief Return reversed uint64_t data */
uint64_t load_qword(const volatile uint64_t *dstAddr, uintptr_t offset);
/*! @brief Reverse byte sequence in uint8_t */
void swap_hword(volatile uint8_t *dstAddr, uintptr_t offset, volatile uint8_t srcData);
/*! @brief Reverse byte sequence in uint16_t */
void swap_word(volatile uint16_t *dstAddr, uintptr_t offset, volatile uint16_t srcData);
/*! @brief Reverse byte sequence in uint32_t */
void swap_dword(volatile uint32_t *dstAddr, uintptr_t offset, volatile uint32_t srcData);
/*! @brief Reverse byte sequence in uint64_t */
void swap_qword(volatile uint64_t *dstAddr, uintptr_t offset, volatile uint64_t srcData);
/*! @brief Reverse byte sequence in uint32_t buffer */
void swap_dwords(uint8_t *buff, uint32_t size);

#if defined(__cplusplus)
}
#endif
/* @} */

#endif /* SD_COMMON_H */
