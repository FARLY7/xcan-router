;#* ###################################################################
;#*  @file:    core0_intc_sw_handlers.s
;#*  @purpose: creates prolog, epilog for C ISR and enables nested interrupts.
;#*
;#*     Copyright 2017 NXP
;#*     All rights reserved.
;#*
;#*     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
;#*     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
;#*     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
;#*     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
;#*     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
;#*     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;#*     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;#*     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
;#*     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
;#*     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
;#*     THE POSSIBILITY OF SUCH DAMAGE.
;#* ###################################################################
    .globl   IVOR1_Handler
    .globl   IVOR4_Handler
    .globl   IVOR8_Handler
    .extern  IVOR1_Exception_Handler
    .extern  IVOR8_Exception_Handler

#ifdef __ghs__
    .section    .vletext, axv
    .vle
#endif

    .equ  INTC_IACKR, 0xfc040020  ;# Interrupt Acknowledge Register address for core 0
    .equ  INTC_EOIR,  0xfc040030   ;# End Of Interrupt Register address for core 0

    .align 4
IVOR8_Handler:
prolog_IVOR8:
    e_stwu   r1, -0x50(r1)            # Create stack frame and store back chain
    se_stw   r0,  0x20(r1)            # Store GPR0 (working register)
    mfmsr    r0                       # Move value from machine state register to R0
    se_stw   r0,  0x10(r1)            # Store MSR
    mfesr    r0
    se_stw   r0,  0x14(r1)            # Store ESR
    se_mflr  r0
    se_stw   r0,  0x18(r1)            # Store LR
    se_stw   r3,  0x1C(r1)            # Store GPR3
    se_stw   r4,  0x24(r1)            # Store GPR4
    se_stw   r5,  0x28(r1)            # Store GPR5
    se_stw   r6,  0x2C(r1)            # Store GPR6
    se_stw   r7,  0x30(r1)            # Store GPR7
    e_stw    r8,  0x34(r1)            # Store GPR8
    e_stw    r9,  0x38(r1)            # Store GPR9
    e_stw    r10, 0x3C(r1)            # Store GPR10
    e_stw    r11, 0x40(r1)            # Store GPR11
    e_stw    r12, 0x44(r1)            # Store GPR12

    e_bl    IVOR8_Exception_Handler

epilog_IVOR8:
    se_lwz   r3,  0x1C(r1)            # Restore GPR3
    se_lwz   r4,  0x24(r1)            # Restore GPR4
    se_lwz   r5,  0x28(r1)            # Restore GPR5
    se_lwz   r6,  0x2C(r1)            # Restore GPR6
    se_lwz   r7,  0x30(r1)            # Restore GPR7
    e_lwz    r8,  0x34(r1)            # Restore GPR8
    e_lwz    r9,  0x38(r1)            # Restore GPR9
    e_lwz    r10, 0x3C(r1)            # Restore GPR10
    e_lwz    r11, 0x40(r1)            # Restore GPR11
    e_lwz    r12, 0x44(r1)            # Restore GPR12
    se_lwz   r0,  0x10(r1)
    mtmsr r0                          # Restore MSR
    se_lwz   r0,  0x14(r1)
    mtesr r0                          # Restore ESR
    se_lwz   r0,  0x18(r1)
    se_mtlr  r0                       # Restore LR

    se_lwz   r0,  0x20(r1)            # Restore GPR0 (working register)
    e_add16i r1, r1, 0x50             # Clean up stack     r0
    se_rfi

IVOR4_Handler:
prologue:
    e_stwu      r1,-0x50 (r1)           ;# Create stack frame and store back chain
    e_stmvsrrw      0x0c (r1)           ;# Save SRR[0-1] (must be done before enabling MSR[EE])
    se_stw      r3, 0x08 (r1)           ;# Save working register (r3)
    e_lis       r3, INTC_IACKR@ha       ;# Save address  of INTC_IACKR in r3
    e_lwz       r3, INTC_IACKR@l(r3)    ;# Save contents of INTC_IACKR in r3 (this is vector table address)
    wrteei      1                       ;# Set MSR[EE] (must wait a couple clocks after reading IACKR)
    se_lwz      r3, 0x0(r3)             ;# Read ISR address from Interrupt Vector Table using pointer
    e_stmvsprw      0x14 (r1)           ;# Save CR, LR, CTR, XER
    se_mtLR     r3                      ;# Copy ISR address (from IACKR) to LR for next branch
    e_stmvgprw      0x24 (r1)           ;# Save GPRs, r[0,3-12]
    se_blrl                             ;# Branch to ISR, with return to next instruction (epilogue)

epilogue:
    e_lmvsprw       0x14 (r1)           ;# Restore CR, LR, CTR, XER
    e_lmvgprw       0x24 (r1)           ;# Restore GPRs, r[0,3-12]
    e_lis       r3, INTC_EOIR@ha        ;# Load upper half of INTC_EOIR address to r3
    mbar                                ;# Ensure prior clearing of interrupt flag conmpleted.
    wrteei      0                       ;# Disable interrupts
    e_stw       r3, INTC_EOIR@l(r3)     ;# Load lower half of INTC_EOIR address to r3 and
                                        ;# write contents of r3 to INTC_EOIR
    se_lwz      r3, 0x08 (r1)           ;# Restore working register (r3) (original value)
    e_lmvsrrw       0x0c (r1)           ;# Restore SRR[0-1]
    e_add16i    r1, r1, 0x50            ;# Reclaim stack space
    se_rfi                              ;# End of Interrupt Handler - re-enables interrupts

IVOR1_Handler:
	prolog_IVOR1:
    e_stwu   r1, -0x50(r1)            # Create stack frame and store back chain
    se_stw   r0,  0x20(r1)            # Store GPR0 (working register)
    se_mfctr r0
    se_stw   r0,  0x10(r1)            # Store CTR
    mfxer    r0
    se_stw   r0,  0x14(r1)            # Store XER
    mfcr     r0
    se_stw   r0,  0x18(r1)            # Store CR
    se_mflr  r0
    se_stw   r0,  0x1C(r1)            # Store LR
    se_stw   r3,  0x24(r1)            # Store GPR3
    se_stw   r4,  0x28(r1)            # Store GPR4
    se_stw   r5,  0x2C(r1)            # Store GPR5
    se_stw   r6,  0x30(r1)            # Store GPR6
    se_stw   r7,  0x34(r1)            # Store GPR7
    e_stw    r8,  0x38(r1)            # Store GPR8
    e_stw    r9,  0x3C(r1)            # Store GPR9
    e_stw    r10, 0x40(r1)            # Store GPR10
    e_stw    r11, 0x44(r1)            # Store GPR11
    e_stw    r12, 0x48(r1)            # Store GPR12

    mfmsr    r0                       # As function prologs/epilogs may contain
    e_or2is  r0, 0x0200               # SPE instruction, enable SPE, as MSR
    mtmsr    r0                       # register is cleared at this point

    e_lis    r0,  IVOR1_Exception_Handler@h
    e_or2i   r0,  IVOR1_Exception_Handler@l

    mtlr     r0                       # Store LR
    se_blrl                           # Branch to ISR, return here

epilog_IVOR1:
    se_lwz   r3,  0x24(r1)            # Restore GPR3
    se_lwz   r4,  0x28(r1)            # Restore GPR4
    se_lwz   r5,  0x2C(r1)            # Restore GPR5
    se_lwz   r6,  0x30(r1)            # Restore GPR6
    se_lwz   r7,  0x34(r1)            # Restore GPR7
    e_lwz    r8,  0x38(r1)            # Restore GPR8
    e_lwz    r9,  0x3C(r1)            # Restore GPR9
    e_lwz    r10, 0x40(r1)            # Restore GPR10
    e_lwz    r11, 0x44(r1)            # Restore GPR11
    e_lwz    r12, 0x48(r1)            # Restore GPR12
    se_lwz   r0,  0x10(r1)
    se_mtctr r0                       # Restore CTR
    se_lwz   r0,  0x14(r1)
    mtxer r0                          # Restore XER
    se_lwz   r0,  0x18(r1)
    mtcr  r0                          # Restore CR
    se_lwz   r0,  0x1C(r1)
    se_mtlr  r0                       # Restore LR

    se_lwz   r0,  0x20(r1)            # Restore GPR0 (working register)
    e_add16i r1, r1, 0x50             # Clean up stack
    se_rfmci                          # Return from machine check
