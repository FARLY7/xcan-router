#ifndef PORTMACRO_H
#define PORTMACRO_H

/*==================================================================================================
*
*   (c) Copyright 2015 Freescale Semiconductor Inc.
*   Copyright 2018 NXP.
*
*   This program is free software; you can redistribute it and/or modify it under
*   the terms of the GNU General Public License (version 2) as published by the
*   Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.
*
*   ***************************************************************************
*   >>!   NOTE: The modification to the GPL is included to allow you to     !<<
*   >>!   distribute a combined work that includes FreeRTOS without being   !<<
*   >>!   obliged to provide the source code for proprietary components     !<<
*   >>!   outside of the FreeRTOS kernel.                                   !<<
*   ***************************************************************************
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu_defines.h"
/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR             char
#define portFLOAT            float
#define portDOUBLE           double
#define portLONG             long
#define portSHORT            short
#define portSTACK_TYPE       unsigned portLONG
#define portBASE_TYPE        portLONG

#ifndef configASSERT
    #define configASSERT(a)			{ if (!(a)) { while (1) {} } }
#endif

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

enum portSYSCALL_e
{
    portSYSCALL_YIELD = 0,
    portSYSCALL_RAISE_PRIVILEGE = 1,
    portSYSCALL_LOWER_PRIVILEGE = 2,
    portSYSCALL_NUM_SYSCALLS
};

#if( configUSE_16_BIT_TICKS == 1 )
    typedef uint16_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xFFFFU
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xFFFFFFFFUL

    /* 32-bit tick type on a 32-bit architecture, so reads of the tick count do
    not need to be guarded with a critical section. */
    #define portTICK_TYPE_IS_ATOMIC 1
#endif

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH            ( -1 )
#define portTICK_PERIOD_MS          ((TickType_t)1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT          32
#define portNOP()                   __asm__ volatile ( "se_nop" )

#define portINTC_CPR  (*(volatile unsigned int *)(INTC_CPR_ADDR))
/*-----------------------------------------------------------*/

#define COMPILER_BARRIER()          __asm__ volatile ( "" : : : "memory" )

/*-----------------------------------------------------------*/

/* Scheduler utilities. */

#define portYIELD()                 ((void)xPortSyscall(portSYSCALL_YIELD))

void vPortTickISR( void );

//lint -emacro( {717}, portYIELD_FROM_ISR )
#define portYIELD_FROM_ISR(x)   do                                                                          \
                                {                                                                           \
                                    if ( (x) == pdTRUE )                                                    \
                                    {                                                                       \
                                        UBaseType_t uxSavedInterruptStatus = ulPortMaskInterruptsFromISR(); \
                                        vTaskSwitchContext();                                               \
                                        vPortUnmaskInterrupts(uxSavedInterruptStatus);                      \
                                    }                                                                       \
                                }                                                                           \
                                while (0) //lint -e9036 "Conditional expression should have essentially Boolean type"

/*-----------------------------------------------------------*/

/* Interrupt control macros - disable interrupts and system call */
#define portDISABLE_INTERRUPTS()                vPortMaskInterrupts()
#define portENABLE_INTERRUPTS()                 vPortUnmaskInterrupts(0)
#define portENTER_CRITICAL()                    vPortTaskEnterCritical()
#define portEXIT_CRITICAL()                     vPortTaskExitCritical()
#define portSET_INTERRUPT_MASK_FROM_ISR()       ulPortMaskInterruptsFromISR()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)    vPortUnmaskInterrupts(x)

/*-----------------------------------------------------------*/

/* SPR access macros */
#define portGetSPR(dest, spr)    __asm__ volatile ("mfspr %0, %1" : "=r" (dest) : "i" (spr))
#define portSetSPR(spr, src)     __asm__ volatile ("mtspr %0, %1" : : "i" (spr), "r" (src))
#define portGetMSR(dest)         __asm__ volatile ("mfmsr %0"     : "=r" (dest))
#define portSetMSR(src)          __asm__ volatile ("mtmsr %0"     : : "r" (src))

/*-----------------------------------------------------------*/

/* Critical section functions */
void vPortTaskEnterCritical(void);
void vPortTaskExitCritical(void);

/*-----------------------------------------------------------*/

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE inline __attribute__(( always_inline ))
#endif

/* Architecture specific optimizations. */
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
#if ( configMAX_PRIORITIES <= 32 )
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#else
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#endif
#endif

#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 1 )

    /* Generic helper function. */
    static portFORCE_INLINE uint32_t ucPortCountLeadingZeros( const uint32_t ulBitmap )
    {
        uint32_t ucReturn;

        // cntlzw Count Leading Zeros Word
        // A count of the number of consecutive zero bits starting
        // at bit 32 of register RS is placed into register RA.
        // This number ranges from 0 to 32, inclusive.
        // 1 clock latency, 1 clock cycle per MPC5746C Reference Manual section 60.4 Table 60-1
        __asm__ volatile ("cntlzw %0, %1" : "=r" (ucReturn) : "r" (ulBitmap));

        return ucReturn;
    }

    /* Check the configuration. */
    #if ( configMAX_PRIORITIES > 32 )
        #error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
    #endif

    /* Store/clear the ready priorities in a bit map. */
    #define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( ( uxReadyPriorities ) |=  ( 1UL << ( uxPriority ) ) )
    #define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities )  ( ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) ) )

    /*-----------------------------------------------------------*/

    #define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities ) uxTopPriority = ( 31UL - ucPortCountLeadingZeros( ( uxReadyPriorities ) ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

static portFORCE_INLINE void vPortMaskInterrupts( void )
{
    BaseType_t msr;

    // See MPC5746C RM Rev 3   section 21.8.5.2  "Ensuring coherency" (most restrictive rules)
    // See MPC5646C RM Rev 5   section 19.10.4.2 "Ensuring coherency"
    // See MPC5607B RM Rev 7.2 section 18.7.5.2  "Ensuring coherency"
    // See MPC5604B RM Rev 8.2 section 16.7.5.2  "Ensuring coherency"
    __asm__ volatile
    (
        "mfmsr  %0 \n\t"
        "wrteei  0 \n\t" // disable interrupts
        : "=r" (msr)
    );

    /* Set current interrupt priority to max API priority */
    portINTC_CPR = configMAX_API_CALL_INTERRUPT_PRIORITY;

    __asm__ volatile
    (
        "mbar     \n\t" // ensure INTC_CPR write completes before re-enabling interrupts
        "wrtee %0 \n\t" // re-enable interrupts if they were previously enabled
        "se_isync \n\t" // re-fetch Processor pipeline
        : : "r" (msr)
    );
}

static portFORCE_INLINE UBaseType_t ulPortMaskInterruptsFromISR( void )
{
    UBaseType_t originalPriority = portINTC_CPR;

    vPortMaskInterrupts();

    return originalPriority;
}

static portFORCE_INLINE void vPortUnmaskInterrupts( UBaseType_t priority )
{
    BaseType_t msr;

    // See MPC5746C RM Rev 3   section 21.8.5.2  "Ensuring coherency" (most restrictive rules)
    // See MPC5646C RM Rev 5   section 19.10.4.2 "Ensuring coherency"
    // See MPC5607B RM Rev 7.2 section 18.7.5.2  "Ensuring coherency"
    // See MPC5604B RM Rev 8.2 section 16.7.5.2  "Ensuring coherency"
    __asm__ volatile
    (
        "mbar      \n\t" // flush out writes from store buffer
        "mfmsr  %0 \n\t"
        "wrteei  0 \n\t" // disable interrupts
        : "=r" (msr)
    );

    // Restore current interrupt priority
    portINTC_CPR = priority;

    __asm__ volatile
    (
        "wrtee %0 \n\t" // re-enable interrupts if they were previously enabled. no mbar needed: OK if INTC_CPR write takes a few cycles to show up.
        : : "r" (msr)
    );
}

static portFORCE_INLINE BaseType_t xPortSyscall( const BaseType_t number )
{
    register BaseType_t numberR3 __asm__ ("r3") = number;

    __asm__ volatile
    (
        "se_sc" : "+r" (numberR3) : : "cr0", "r4"
    );

    return numberR3;
}

static portFORCE_INLINE void portINCREMENT_CRITICAL_NESTING( void )
{
    UBaseType_t ulCriticalNesting;

    /* Increment critical nesting count */
    __asm__ volatile
    (
        "mfsprg  %0, 1  \n\t" // get critical nesting count from SPRG1
        "se_addi %0, 1  \n\t" // increment
        "mtsprg   1, %0 \n\t" // store to SPRG1
        : "=kregs" (ulCriticalNesting)
    );
}

static portFORCE_INLINE UBaseType_t portDECREMENT_CRITICAL_NESTING( void )
{
    UBaseType_t ulCriticalNesting;

    /* Decrement critical nesting count */
    __asm__ volatile
    (
        "mfsprg   %0, 1  \n\t" // get critical nesting count from SPRG1
        "se_subi  %0, 1  \n\t" // decrement
        "mtsprg    1, %0 \n\t" // store to SPRG1
        : "=kregs" (ulCriticalNesting)
    );

    /* Return new critical nesting count */
    return ulCriticalNesting;
}

/*-----------------------------------------------------------*/

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )       void vFunction( void *pvParameters )

/* Set the privilege level to user mode if xRunningPrivileged is false. */
static portFORCE_INLINE void vPortResetPrivilege( const BaseType_t xRunningPrivileged )
{
    if ( xRunningPrivileged == pdFALSE )
    {
        (void)xPortSyscall( portSYSCALL_LOWER_PRIVILEGE );
    }
}

/*
 * Checks to see if being called from the context of an unprivileged task, and
 * if so raises the privilege level and returns false - otherwise does nothing
 * other than return true.
 */
static portFORCE_INLINE BaseType_t xPortRaisePrivilege( void )
{
    // syscalls are context synchronizing on se_sc entry and rfi exit
    // no need for explicit context synchronization after changing privilege in handler.
    return xPortSyscall( portSYSCALL_RAISE_PRIVILEGE );
}

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

