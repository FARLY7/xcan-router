// ---------------------------------------------------------------------------------------
//  @file:    interrupt_vectors.s
//  @purpose: GNU Compiler Collection Startup File
//            MPC5748G
//  @version: 1.0
//  @date:    2017-3-22
//  @build:   #
// ---------------------------------------------------------------------------------------
//
// Copyright 2017 NXP
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//****************************************************************************



.globl VTABLE
.globl IVOR0_Vector
.globl IVOR1_Vector
.globl IVOR2_Vector
.globl IVOR3_Vector
.globl IVOR4_Vector
.globl IVOR5_Vector
.globl IVOR6_Vector
.globl IVOR7_Vector
.globl IVOR8_Vector
.globl IVOR9_Vector
.globl IVOR10_Vector
.globl IVOR11_Vector
.globl IVOR12_Vector
.globl IVOR13_Vector
.globl IVOR14_Vector
.globl IVOR15_Vector

/*
 * The USING_OS_FREERTOS must be defined in defined symbol list in case FreeRTOS is used
 * Note: In Design Studio the Symbol List for Assembly files is separate from C files
 */
#ifdef USING_OS_FREERTOS
.extern vPortISRHandler
.extern xPortSyscall
#else
.extern     IVOR4_Handler
#endif
.section    .core_exceptions_table, "axv"

.align 4
VTABLE:
IVOR0_Vector:
e_b   IVOR0_Handler

.align 4
IVOR1_Vector:
e_b   IVOR1_Handler

.align 4
IVOR2_Vector:
e_b   IVOR2_Handler

.align 4
IVOR3_Vector:
e_b   IVOR3_Handler

.align 4
IVOR4_Vector:
#ifdef USING_OS_FREERTOS
e_b vPortISRHandler
#else
e_b   IVOR4_Handler
#endif

.align 4
IVOR5_Vector:
e_b   IVOR5_Handler

.align 4
IVOR6_Vector:
e_b   IVOR6_Handler

.align 4
IVOR7_Vector:
e_b   IVOR7_Handler

.align 4
IVOR8_Vector:
#ifdef USING_OS_FREERTOS
e_b xPortSyscall
#else
#ifndef SEMIHOSTING
e_b IVOR8_Handler
#else
se_illegal  /* for semihosting support  */
se_rfi
#endif
#endif

.align 4
IVOR9_Vector:
e_b   IVOR9_Handler

.align 4
IVOR10_Vector:
e_b   IVOR10_Handler

.align 4
IVOR11_Vector:
e_b   IVOR11_Handler

.align 4
IVOR12_Vector:
e_b   IVOR12_Handler

.align 4
IVOR13_Vector:
e_b   IVOR13_Handler

.align 4
IVOR14_Vector:
e_b   IVOR14_Handler

.align 4
IVOR15_Vector:
e_b   IVOR15_Handler
