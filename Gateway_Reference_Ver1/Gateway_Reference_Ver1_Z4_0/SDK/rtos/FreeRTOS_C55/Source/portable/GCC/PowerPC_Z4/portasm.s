/*==================================================================================================
*
*   (c) Copyright 2015 Freescale Semiconductor Inc.
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

#include "FreeRTOSConfig.h"
#include "cpu_defines.h"
/*
   STACK FRAME DESIGN: Depth: (0x98, or 152 bytes modulo 8 bytes = 19)
              ************* ______________
     0x94     *  GPR31    *    ^
     0x90     *  GPR30    *    |
     0x8C     *  GPR29    *    |
     0x88     *  GPR28    *    |
     0x84     *  GPR27    *    |
     0x80     *  GPR26    *    |
     0x7C     *  GPR25    *    |
     0x78     *  GPR24    *    |
     0x74     *  GPR23    *    |
     0x70     *  GPR22    *    |
     0x6C     *  GPR21    *    |
     0x68     *  GPR20    *    |
     0x64     *  GPR19    *    |
     0x60     *  GPR18    *    |
     0x5C     *  GPR17    *    |
     0x58     *  GPR16    *    |
     0x54     *  GPR15    *    |
     0x50     *  GPR14    *    |
     0x4C     *  GPR12    *    |
     0x48     *  GPR11    *    |
     0x44     *  GPR10    *    |
     0x40     *  GPR9     *    |
     0x3C     *  GPR8     *    |
     0x38     *  GPR7     *  GPRs (32 bit)
     0x34     *  GPR6     *    |
     0x30     *  GPR5     *    |
     0x2C     *  GPR4     *    |
     0x28     *  GPR3     *    |
     0x24     *  GPR0     * ___v__________
     0x20     *  XER      *    ^
     0x1C     *  CTR      *    |
     0x18     *  LR       *    |
     0x14     *  CR       * locals
     0x10     *  SRR1     *    |
     0x0C     *  SRR0     * ___v__________
     0x08     *  nest cnt * Nested critical section count
     0x04     *  LR save  * Reserved for calling function
     0x00     *  SP       * Backchain (same as gpr1 in GPRs)
              *************
*/
    # Declarations
    .global vPortStartFirstTask
    .global vPortISRHandler
    .global xPortSyscall
    .global vPortDataStorage
    .global vPortNMI

    .extern pxCurrentTCB
    .extern pxSystemStackPointer
    .extern vTaskSwitchContext

    # Address of the INTC_CPR0 register
    .equ    INTC_CPR0_ADDR,  INTC_CPR_ADDR
    # CORE0 Interrupt Acknowledge Register address
    .equ    INTC_IACKR_PRC0, INTC_IACKR_PRC_ADDR
    # CORE0 End Of Interrupt Register address
    .equ    INTC_EOIR_PRC0,  INTC_EOIR_PRC_ADDR
    # Number of bits in the INTVEC field of IACKR
    .equ    INTC_IACKR_INTVEC_BITWIDTH, INTC_IACKR_INTVEC_BITWIDTH_NUM
    # Saves context to task stack
    # Relies on HID0.ICR being set so outstanding reservations are cleared whenever an external interrupt occurs,
    # which handles clearing reservations in task->ISR and ISR->ISR context switches. ISR->task and task->task
    # context switches are handled by clearing reservation when restoring context.
    # See e200z4 Core Reference Manual Rev 2 section 2.4.9 "Hardware Implementation Dependent Register 0 (HID0)" Table 8
    # See e200z0 Core Reference Manual Rev 0 section 2.3.9 "Hardware Implementation Dependent Register 0 (HID0)" Table 2-8
    .macro portSAVE_CONTEXT
      e_stw         r1, -0x98 (r1)          # Store backchain
      e_sub16i      r1, r1, 0x98            # Allocate stack

      e_stmvsrrw    0x0c (r1)               # Save SRR[0-1]
      e_stmvsprw    0x14 (r1)               # Save CR, LR, CTR, XER
      e_stmvgprw    0x24 (r1)               # Save GPRs, r[0, 3-12]
      e_stmw        r14, 0x50 (r1)          # Save GPRs, r[14-31]

      mfsprg        r4, 1
      se_stw        r4, 0x08 (r1)           # Save nested critical section count from SPRG1
    .endm

    # Restores registers from task stack and clears any outstanding reservation
    # Does not restore nested critical section count: must also call portRESTORE_CRITICAL_NESTING_COUNT when restoring context
    .macro portRESTORE_CONTEXT
      stwcx.        r1, 0, r1               # Clear any outstanding reservation. Scribbles on stack before reclaiming
                                            # See Power ISA Version 2.06B Revision B (July 23, 2010) section 1.7.3.1 "Reservations",
                                            # specifically the programming note in the bottom right corner of page 662

      e_lmvsrrw     0x0c (r1)               # Restore SRR[0-1]
      e_lmvsprw     0x14 (r1)               # Restore CR, LR, CTR, XER
      e_lmvgprw     0x24 (r1)               # Restore GPRs, r[0, 3-12]
      e_lmw         r14, 0x50 (r1)          # Restore GPRs, r[14-31]

      e_add16i      r1, r1, 0x98            # Reclaim stack space
    .endm

    # Restores critical nesting count from stack (returns value in r4)
    .macro portRESTORE_CRITICAL_NESTING_COUNT
      se_lwz        r4, 0x08 (r1)
      mtsprg        1, r4                   # Restore nested critical section count to SPRG1
    .endm

    # Macro to put task stack pointer into the TCB (r3 used as scratch register)
    .macro portPUSH_TASK
      e_lis     r3, pxCurrentTCB@ha
      e_lwz     r3, pxCurrentTCB@l(r3)
      se_stw    r1, 0x00 (r3)
    .endm

    # Macro to get the task stack pointer from the TCB (r3 used as scratch register)
    .macro portPOP_TASK
      e_lis     r3, pxCurrentTCB@ha
      e_lwz     r3, pxCurrentTCB@l(r3)
      se_lwz    r1, 0x00 (r3)
    .endm

    # Macro to update the system stack pointer (r6 used as scratch register)
    .macro portUPDATE_SYSTEM_STACK_POINTER
      e_lis     r6, pxSystemStackPointer@ha
      e_stw     r1, pxSystemStackPointer@l(r6)
    .endm

    # Macro to load the system stack pointer (switch to the system stack) (r6 used as scratch register)
    .macro portLOAD_SYSTEM_STACK_POINTER
      e_lis     r6, pxSystemStackPointer@ha
      e_lwz     r1, pxSystemStackPointer@l(r6)
    .endm

    # Macro to load the nested interrupt count (r4 used for value)
    # Can only be run with global interrupts disabled
    .macro portLOAD_NESTED_INTERRUPT_COUNT
      mfsprg    r4, 0                       # SPRG0 holds nested interrupt count
    .endm

    # Macro to increment the nested interrupt count (r4 used for value)
    # Can only be run with global interrupts disabled
    .macro portINCREMENT_NESTED_INTERRUPT_COUNT
      se_addi   r4, 1                       # Increment the nested interrupt count
      mtsprg    0, r4                       # save the new value to SPRG0
    .endm

    # Macro to decrement the nested interrupt count (r4 used for value)
    # Can only be run with global interrupts disabled
    .macro portDECREMENT_NESTED_INTERRUPT_COUNT
      se_subi   r4, 1                       # Decrement the nested interrupt count
      mtsprg    0, r4                       # save the new value to SPRG0
    .endm

    # Macro to set current interrupt priority to max API call priority, allowing other non-API ISRs to preempt (r7,r8 used as scratch registers)
    .macro portSET_INTERRUPT_PRIORITY_CEILING
      e_lis     r7, INTC_CPR0_ADDR@ha                       # Load upper half of address of INTC_CPR0 register
      e_li      r8, configMAX_API_CALL_INTERRUPT_PRIORITY   # Load new current priority value
      e_stw     r8, INTC_CPR0_ADDR@l(r7)                    # Write INTC0_CPR to raise current interrupt priority
    .endm

    # Macro to set current interrupt priority to 0 (r7,r8 used as scratch registers)
    .macro portCLEAR_INTERRUPT_PRIORITY_CEILING
      e_lis     r7, INTC_CPR0_ADDR@ha                       # Load upper half of address of INTC_CPR0 register
      e_li      r8, 0                                       # Load new current priority value
      e_stw     r8, INTC_CPR0_ADDR@l(r7)                    # Write INTC0_CPR to reset current interrupt priority
    .endm

    # PowerISA V2.06B Book III-E chapter 12 "Synchronization Requirements for Context Alterations" says
    # "No software synchronization is required before or after a context-altering instruction that
    # is also context synchronizing (e.g., rfi, etc.) or when altering the MSR in most cases (see the tables)."
    # Table 12 says none/none for wrteei with footnote 4 "The effect of changing MSR EE or MSR CE is immediate."
    .macro portDISABLE_GLOBAL_INTERRUPTS
      wrteei    0                           # Disable interrupts
    .endm

    .macro portENABLE_GLOBAL_INTERRUPTS
      wrteei    1                           # Enable interrupts
    .endm

    .align  4
	.func vPortStartFirstTask, vPortStartFirstTask
vPortStartFirstTask:
    portUPDATE_SYSTEM_STACK_POINTER         # Init pxSystemStackPointer
    portPOP_TASK                            # pxCurrentTCB should already point to the highest priority task
    mbar                                    # Ensure MPU config writes complete before switching context

    portCLEAR_INTERRUPT_PRIORITY_CEILING    # Set current priority to 0

    portRESTORE_CONTEXT
    se_rfi
	.endfunc

    .align  4
	.func vPortISRHandler, vPortISRHandler
vPortISRHandler:
prologue:                                   # Interrupts implicitly disabled on entry, context synchronized by external interrupt
    portSAVE_CONTEXT                        # Save context state

    portLOAD_NESTED_INTERRUPT_COUNT
    se_cmpi     r4, 0                       # If the nested interrupt count is already above zero,
    se_bgt      1f                          # Skip updating the current TCB

    portPUSH_TASK                           # Save stack pointer to TCB
    portLOAD_SYSTEM_STACK_POINTER           # Switch to system stack

1:  portINCREMENT_NESTED_INTERRUPT_COUNT

    e_lis       r3, INTC_IACKR_PRC0@ha      # Save address of INTC_IACKR in r3
    e_lwz       r3, INTC_IACKR_PRC0@l(r3)   # Save contents (vector table address) of INTC_IACKR in r3

    portENABLE_GLOBAL_INTERRUPTS            # Set MSR[EE] (must wait a couple clocks after reading IACKR)

    se_lwz      r4, 0x0(r3)                 # Read ISR address from Interrupt Vector Table using pointer
    se_mtLR     r4                          # Copy ISR address to LR for next branch
    e_rlwinm    r3, r3, 30, 32-INTC_IACKR_INTVEC_BITWIDTH, 31
                                            # Rotate INTVEC into the lowest bits of the register, mask off non-INTVEC bits
                                            # NOTE: On MPC56xx, above instruction assumes INTC_MCR[VTES] = 0
    se_blrl                                 # Branch to ISR with return to next instruction (epilogue)

epilogue:
    mbar                                    # Ensure all memory operations from ISR have completed
                                            # before setting INTC_EOIR, e.g. writes to clear interrupt flags
                                            # See MPC5746C RM Rev 3 section 21.7.3.1.2

    portDISABLE_GLOBAL_INTERRUPTS           # Clear MSR[EE]

    e_lis       r3, INTC_EOIR_PRC0@ha       # Load upper half of INTC_EOIR address to r3
    e_stw       r3, INTC_EOIR_PRC0@l(r3)    # Load lower half of INTC_EOIR address to r3 and write contents to INTC_EOIR

    portLOAD_NESTED_INTERRUPT_COUNT
    portDECREMENT_NESTED_INTERRUPT_COUNT
    se_cmpi     r4, 0                       # If the nested interrupt count is above zero,
    se_bgt      2f                          # Do not switch stacks

    portUPDATE_SYSTEM_STACK_POINTER
    portPOP_TASK
                                            # Relies on memory accesses from previous context having completed by now.
    mbar                                    # Ensure MPU config writes complete before switching context

2:  portRESTORE_CRITICAL_NESTING_COUNT      # Load new context's critical nesting count
    portRESTORE_CONTEXT                     # Restore context state
    se_rfi                                  # End of ISR Handler, re-enables interrupts, synchronizes context
	.endfunc

# Calls appropriate handler per syscall number passed in R3
# Passes result, if any, back in R3
# se_sc can ONLY be invoked from xPortSyscall() - do not call se_sc directly.
# Can ONLY use R3 and R4 here - if more are needed,
# update input/output/clobber constraints in xPortSyscall() in portmacro.h
# and this comment block
    .align  4
	.func xPortSyscall, xPortSyscall
xPortSyscall:
    se_cmpi     r3, 0 # portSYSCALL_YIELD
    se_beq      vPortYield
    se_cmpi     r3, 1 # portSYSCALL_RAISE_PRIVILEGE
    se_beq      xPortRaisePrivilege
    se_cmpi     r3, 2 # portSYSCALL_LOWER_PRIVILEGE
    se_beq      vPortLowerPrivilege
xPortSyscallEnd:
    se_b        xPortSyscall                # infinite loop for unhandled syscall numbers
	.endfunc

# Checks if called from unprivileged context.
# If so, raises privilege level and returns false.
# If not, does nothing and returns true.
    .align  4
	.func xPortRaisePrivilege, xPortRaisePrivilege
xPortRaisePrivilege:                        # Interrupts implicitly disabled on entry, context synchronized by syscall
    se_li       r3, 1                       # set default return value
    mfsrr1      r4                          # load stored MSR from SRR1
    se_btsti    r4, 17                      # extract Problem State: 0 = supervisor mode, 1 = user mode. Updates CR0
    se_beq      1f                          # skip if already in supervisor mode
    mfsrr1      r4                          # otherwise reload stored MSR from SRR1
    se_bclri    r4, 17                      # set supervisor mode
    mtsrr1      r4                          # store back to SRR1
    se_li       r3, 0                       # set return to false
1:  se_rfi                                  # End of system call handler, restores MSR from SRR1, synchronizes context
	.endfunc

# Lowers privilege level to user mode
    .align  4
	.func vPortLowerPrivilege, vPortLowerPrivilege
vPortLowerPrivilege:                        # Interrupts implicitly disabled on entry, context synchronized by syscall
    mfsrr1      r4                          # load stored MSR from SRR1
    se_bseti    r4, 17                      # set Problem State: 1 = user mode
    mtsrr1      r4                          # store back to SRR1
    se_rfi                                  # End of system call handler, restores MSR from SRR1, synchronizes context
	.endfunc

# The system call handler is invoked from task context via portYIELD or portYIELD_WITHIN_API.  This
# handler can not be invoked from ISR context.  Therefore, the only valid priorites entering and exiting
# this handler are priority 0 (task priority) or configMAX_API_CALL_INTERRUPT_PRIORITY (if called from
# critical section). This cannot be preempted by ISRs utilizing RTOS APIs, but it can be preempted by
# ISRs that do not use RTOS APIs.
    .align  4
	.func vPortYield, vPortYield
vPortYield:                                 # Interrupts implicitly disabled on entry, context synchronized by syscall
    portSAVE_CONTEXT                        # Save current task's context

    portPUSH_TASK                           # Save current task's stack pointer
    portLOAD_SYSTEM_STACK_POINTER           # Switch to system stack

    portLOAD_NESTED_INTERRUPT_COUNT
    portINCREMENT_NESTED_INTERRUPT_COUNT

    portSET_INTERRUPT_PRIORITY_CEILING      # Set current interrupt priority to configMAX_API_CALL_INTERRUPT_PRIORITY
                                            # to allow hardware, non-API interrupts to continue to run while
                                            # vTaskSwitchContext runs

    mbar                                    # Ensure write to raise INTC_CPR completes before re-enabling interrupts

    portENABLE_GLOBAL_INTERRUPTS            # Set MSR[EE]

    se_isync                                # re-fetch Processor pipeline
                                            # See MPC5746C RM Rev 3   section 21.8.5.2  "Ensuring coherency" (most restrictive rules)
                                            # See MPC5646C RM Rev 5   section 19.10.4.2 "Ensuring coherency"
                                            # See MPC5607B RM Rev 7.2 section 18.7.5.2  "Ensuring coherency"
                                            # See MPC5604B RM Rev 8.2 section 16.7.5.2  "Ensuring coherency"

    e_bl vTaskSwitchContext                 # Schedule a new task to run

    mbar                                    # flush out writes from store buffer
                                            # See "Ensuring coherency" references above

    portDISABLE_GLOBAL_INTERRUPTS           # Clear MSR[EE]

    portLOAD_NESTED_INTERRUPT_COUNT
    portDECREMENT_NESTED_INTERRUPT_COUNT

    portUPDATE_SYSTEM_STACK_POINTER         # Save system stack pointer
    portPOP_TASK                            # Load new task's stack pointer
                                            # Relies on memory accesses from previous context having completed by now.
    mbar                                    # Ensure MPU config writes complete before switching context

    portRESTORE_CRITICAL_NESTING_COUNT
    se_cmpi       r4, 0                     # Check if the nested critical section count is above zero
    se_bgt        1f                        # If it is, skip setting CPR to 0

    portCLEAR_INTERRUPT_PRIORITY_CEILING    # Else set CPR to 0

1:  portRESTORE_CONTEXT                     # Restore new task's context
    se_rfi                                  # End of system call handler, re-enables interrupts, synchronizes context
	.endfunc

# Crude debug helper for NMIs
    .align 4
vPortNMI:
    mfspr       r3, 570                     # oops - let's see what's in MCSRR0 (saved address of offending instruction)
    mfspr       r4, 571                     # oops - let's see what's in MCSRR1 (saved MSR)
    mfspr       r5, 572                     # oops - let's see what's in MCSR (machine check syndrome)
    mfspr       r6, 573                     # oops - let's see what's in MCAR (machine check address - valid if MCSR.MAV is set)
    se_b        vPortNMI                    # infinite loop

# Crude debug helper for data storage exceptions
    .align 4
vPortDataStorage:
    mfspr       r3, 570                     # oops - let's see what's in MCSRR0 (Effective address of the excepting load/store instruction)
    mfspr       r4, 571                     # oops - let's see what's in MCSRR1 (Contents of the MSR at the time of the interrupt)
    mfspr       r5, 62                      # oops - let's see what's in ESR (Exception Syndrome Register) - ST, SPV, VLEMI are valid
    mfspr       r6, 61                      # oops - let's see what's in DEAR (For Access Control exceptions, effective address of the access which caused the violation)
    se_b        vPortDataStorage            # infinite loop
    .end
