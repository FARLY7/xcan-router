/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_MPC5748G.S                                                          */
/*  @purpose: GNU Compiler Collection Startup File                                        */
/*            MPC5748G                                                                    */
/*  @version: 1.0                                                                         */
/*  @date:    2017-2-9                                                                    */
/*  @build:   #                                                                           */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright 2017 NXP                                                                     */
/* All rights reserved.                                                                   */
/*                                                                                        */
/* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR                          */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES              */
/* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.                */
/* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,                    */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                     */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR                     */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,                    */
/* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING                  */
/* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF                         */
/* THE POSSIBILITY OF SUCH DAMAGE.                                                        */
/*****************************************************************************/
/* Version: GNU Compiler Collection                                          */

/* ----------------------------------------------------------------------------
   -- Boot header configuration
   ---------------------------------------------------------------------------- */
#if defined(TURN_ON_CPU0)
#define CPU0_ENABLED  0x00000002
#else
#define CPU0_ENABLED  0x0
#endif /* defined(TURN_ON_CPU0) */
#if defined(TURN_ON_CPU1)
#define CPU1_ENABLED  0x00000008
#else
#define CPU1_ENABLED  0x0
#endif /* defined(TURN_ON_CPU1) */
#if defined(TURN_ON_CPU2)
#define CPU2_ENABLED  0x00000001
#else
#define CPU2_ENABLED  0x0
#endif /* defined(TURN_ON_CPU2) */
#define MPC57xx_ID    0x005A0000
#define RCHW_VAL (MPC57xx_ID | CPU0_ENABLED | CPU1_ENABLED | CPU2_ENABLED)

#ifndef ICACHE_ENABLE
#define ICACHE_ENABLE 0
#endif
#ifndef DCACHE_ENABLE
#define DCACHE_ENABLE 0
#endif

#ifdef TURN_ON_CPU0
    .section .cpu0_reset_vector, "a"
    .long       __start

    .section .rchw, "a"
        .long RCHW_VAL

    .section .startup, "ax"

    .globl    _start
    .globl    Reset_Handler
#endif
#ifdef TURN_ON_CPU1
    .section .cpu1_reset_vector, "a"
    .long       __start
    .section .startup, "ax"

    .globl    _start
    .globl    Reset_Handler
#endif
#ifdef TURN_ON_CPU2
    .section .cpu2_reset_vector, "a"
    .long       __start

    .section .startup, "ax"

    .globl    _start
    .globl    Reset_Handler
#endif
Reset_Handler:
_start:
    wrteei 0    ;# Disable interrupts


;#**************************** Init Core Registers ****************************
;# The E200Z4 core needs its registers initialising before they are used
;# otherwise in Lock Step mode the two cores will contain different random data.
;# If this is stored to memory (e.g. stacked) it will cause a Lock Step error.

;# GPRs 0-31
    e_li    r0, 0
    e_li    r1, 0
    e_li    r2, 0
    e_li    r3, 0
    e_li    r4, 0
    e_li    r5, 0
    e_li    r6, 0
    e_li    r7, 0
    e_li    r8, 0
    e_li    r9, 0
    e_li    r10, 0
    e_li    r11, 0
    e_li    r12, 0
    e_li    r13, 0
    e_li    r14, 0
    e_li    r15, 0
    e_li    r16, 0
    e_li    r17, 0
    e_li    r18, 0
    e_li    r19, 0
    e_li    r20, 0
    e_li    r21, 0
    e_li    r22, 0
    e_li    r23, 0
    e_li    r24, 0
    e_li    r25, 0
    e_li    r26, 0
    e_li    r27, 0
    e_li    r28, 0
    e_li    r29, 0
    e_li    r30, 0
    e_li    r31, 0

;# Init any other CPU register which might be stacked (before being used).

    mtspr    1, r1          ;#XER
    mtcrf    0xFF, r1
    mtspr    CTR, r1
    mtspr    272, r1        ;#SPRG0
    mtspr    273, r1        ;#SPRG1
    mtspr    274, r1        ;#SPRG2
    mtspr    275, r1        ;#SPRG3
    mtspr    58, r1         ;#CSRR0
    mtspr    59, r1         ;#CSRR1
    mtspr    570, r1        ;#MCSRR0
    mtspr    571, r1        ;#MCSRR1
    mtspr    61, r1         ;#DEAR
    mtspr    63, r1         ;#IVPR
    mtspr    256, r1        ;#USPRG0
    mtspr    62, r1         ;#ESR
    mtspr    8, r31         ;#LR

;#***************************** Initialise SRAM ECC **************************
;# Store number of 128Byte (32GPRs) segments in Counter
    e_lis       r5, __SRAM_SIZE@h  # Initialize r5 to size of SRAM (Bytes)
    e_or2i      r5, __SRAM_SIZE@l
    e_srwi      r5, r5, 0x7         ;# Divide SRAM size by 128
    mtctr       r5                  ;# Move to counter for use with "bdnz"

;# Base Address of the internal SRAM
    e_lis       r5, __SRAM_BASE_ADDR@h
    e_or2i      r5, __SRAM_BASE_ADDR@l

;# Fill SRAM with writes of 32GPRs
sram_loop:
    e_stmw      r0, 0(r5)           ;# Write all 32 registers to SRAM
    e_addi      r5, r5, 128         ;# Increment the RAM pointer to next 128bytes
    e_bdnz      sram_loop           ;# Loop for all of SRAM


;#********************************* Enable BTB ********************************
;# Flush & Enable BTB - Set BBFI bit in BUCSR
    e_li      r3, 0x201
    mtspr     1013, r3
    se_isync

;#******************** Clear reservations on external interrupt *****************
;# Set ICR in HID0
    e_lis     r3, 0x2
    mtspr     1008, r3
    se_isync

;#*************************** Enable ME Bit in MSR *****************************
    mfmsr    r6
    e_or2i   r6, 0x1000
    mtmsr    r6

#if defined(I_CACHE) && (ICACHE_ENABLE == 1)
;#****************** Invalidate and Enable the Instruction cache **************
__icache_cfg:
    e_li     r5, 0x2
    mtspr    1011, r5

    e_li     r7, 0x4
    e_li     r8, 0x2
    e_lis    r11, 0xFFFF
    e_or2i   r11, 0xFFFB

__icache_inv:
    mfspr    r9, 1011
    and.     r10, r7, r9
    e_beq    __icache_no_abort
    and.     r10, r11, r9
    mtspr    1011, r10
    e_b      __icache_cfg

__icache_no_abort:
    and.     r10, r8, r9
    e_bne    __icache_inv

    mfspr    r5, 1011
    e_ori    r5, r5, 0x0001
    se_isync
    mtspr    1011, r5
#endif

#if defined(D_CACHE) && (DCACHE_ENABLE == 1)
;#****************** Invalidate and Enable the Data cache **************
__dcache_cfg:
    e_li r5, 0x2
    mtspr 1010, r5

    e_li r7, 0x4
    e_li r8, 0x2
    e_lis    r11, 0xFFFF
    e_or2i   r11, 0xFFFB

__dcache_inv:
    mfspr r9, 1010
    and.  r10, r7, r9
    e_beq   __dcache_no_abort
    and.  r10, r11, r9
    mtspr 1010, r10
    e_b __dcache_cfg

__dcache_no_abort:
    and.  r10, r8, r9
    e_bne __dcache_inv

    mfspr r5, 1010
    e_ori   r5, r5, 0x0001
    se_isync
    msync
    mtspr 1010, r5
#endif


;#****************************** Configure Stack *****************************
    e_lis       r1, __SP_INIT@h         ;# Initialize stack pointer r1 to
    e_or2i      r1, __SP_INIT@l         ;# value in linker command file.

    e_lis       r13, _SDA_BASE_@h       ;# Initialize r13 to sdata base
    e_or2i      r13, _SDA_BASE_@l       ;# (provided by linker).

    e_lis       r2, _SDA2_BASE_@h       ;# Initialize r2 to sdata2 base
    e_or2i      r2, _SDA2_BASE_@l       ;# (provided by linker).

    e_stwu      r0, -64(r1)             ;# Terminate stack.

#ifndef __NO_SYSTEM_INIT
;# Call the system init routine
    e_bl   SystemInit
#endif
;# Init .data and .bss sections
    e_bl   init_data_bss
    wrteei 1    ;# Enable interrupts
;# Jump to Main
    e_bl        main

    .section .intc_vector_table, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   DefaultISR                       /* Vector #   0 Software setable flag 0 SSCIR0[CLR0] */
    .long   DefaultISR                       /* Vector #   1 Software setable flag 1 SSCIR0[CLR1] */
    .long   DefaultISR                       /* Vector #   2 Software setable flag 2 SSCIR0[CLR2] */
    .long   DefaultISR                       /* Vector #   3 Software setable flag 3 SSCIR0[CLR3] */
    .long   DefaultISR                       /* Vector #   4 Software setable flag 4 SSCIR0[CLR4] */
    .long   DefaultISR                       /* Vector #   5 Software setable flag 5 SSCIR0[CLR5] */
    .long   DefaultISR                       /* Vector #   6 Software setable flag 6 SSCIR0[CLR6] */
    .long   DefaultISR                       /* Vector #   7 Software setable flag 7 SSCIR0[CLR7] */
    .long   DefaultISR                       /* Vector #   8 Software setable flag 8 SSCIR0[CLR8] */
    .long   DefaultISR                       /* Vector #   9 Software setable flag 9 SSCIR0[CLR9] */
    .long   DefaultISR                       /* Vector #  10 Software setable flag 10 SSCIR0[CLR10] */
    .long   DefaultISR                       /* Vector #  11 Software setable flag 11 SSCIR0[CLR11] */
    .long   DefaultISR                       /* Vector #  12 Software setable flag 12 SSCIR0[CLR12] */
    .long   DefaultISR                       /* Vector #  13 Software setable flag 13 SSCIR0[CLR13] */
    .long   DefaultISR                       /* Vector #  14 Software setable flag 14 SSCIR0[CLR14] */
    .long   DefaultISR                       /* Vector #  15 Software setable flag 15 SSCIR0[CLR15] */
    .long   DefaultISR                       /* Vector #  16 Software setable flag 16 SSCIR0[CLR16] */
    .long   DefaultISR                       /* Vector #  17 Software setable flag 17 SSCIR0[CLR17] */
    .long   DefaultISR                       /* Vector #  18 Software setable flag 18 SSCIR0[CLR18] */
    .long   DefaultISR                       /* Vector #  19 Software setable flag 19 SSCIR0[CLR19] */
    .long   DefaultISR                       /* Vector #  20 Software setable flag 20 SSCIR0[CLR20] */
    .long   DefaultISR                       /* Vector #  21 Software setable flag 21 SSCIR0[CLR21] */
    .long   DefaultISR                       /* Vector #  22 Software setable flag 22 SSCIR0[CLR22] */
    .long   DefaultISR                       /* Vector #  23 Software setable flag 23 SSCIR0[CLR23] */
    .long   DefaultISR                       /* Vector #  24 */
    .long   DefaultISR                       /* Vector #  25 */
    .long   DefaultISR                       /* Vector #  26 */
    .long   DefaultISR                       /* Vector #  27 */
    .long   DefaultISR                       /* Vector #  28 */
    .long   DefaultISR                       /* Vector #  29 */
    .long   DefaultISR                       /* Vector #  30 */
    .long   DefaultISR                       /* Vector #  31 */
    .long   SWT0_IRQHandler                  /* Vector #  32 Platform watchdog timer0 SWT_0_IR[TIF] */
    .long   SWT1_IRQHandler                  /* Vector #  33 Platform watchdog timer1 SWT_1_IR[TIF] */
    .long   SWT2_IRQHandler                  /* Vector #  34 Platform watchdog timer2 SWT_2_IR[TIF] */
    .long   DefaultISR                       /* Vector #  35 */
    .long   STM0_Ch0_IRQHandler              /* Vector #  36 On-Platform periodic timer 0_0 (STM) STM_0_CIR0[CIF] */
    .long   STM0_Ch1_IRQHandler              /* Vector #  37 On-Platform periodic timer 0_1 (STM) STM_0_CIR1[CIF] */
    .long   STM0_Ch2_IRQHandler              /* Vector #  38 On-Platform periodic timer 0_2 (STM) STM_0_CIR2[CIF] */
    .long   STM0_Ch3_IRQHandler              /* Vector #  39 On-Platform periodic timer 0_3 (STM) STM_0_CIR3[CIF] */
    .long   STM1_Ch0_IRQHandler              /* Vector #  40 On-Platform periodic timer 1_0 (STM) STM_1_CIR0[CIF] */
    .long   STM1_Ch1_IRQHandler              /* Vector #  41 On-Platform periodic timer 1_1 (STM) STM_1_CIR1[CIF] */
    .long   STM1_Ch2_IRQHandler              /* Vector #  42 On-Platform periodic timer 1_2 (STM) STM_1_CIR2[CIF] */
    .long   STM1_Ch3_IRQHandler              /* Vector #  43 On-Platform periodic timer 1_3 (STM) STM_1_CIR3[CIF] */
    .long   STM2_Ch0_IRQHandler              /* Vector #  44 On-Platform periodic timer 2_0 (STM) STM_2_CIR0[CIF] */
    .long   STM2_Ch1_IRQHandler              /* Vector #  45 On-Platform periodic timer 2_1 (STM) STM_2_CIR1[CIF] */
    .long   STM2_Ch2_IRQHandler              /* Vector #  46 On-Platform periodic timer 2_2 (STM) STM_2_CIR2[CIF] */
    .long   STM2_Ch3_IRQHandler              /* Vector #  47 On-Platform periodic timer 2_3 (STM) STM_2_CIR3[CIF] */
    .long   DefaultISR                       /* Vector #  48 */
    .long   DefaultISR                       /* Vector #  49 */
    .long   DefaultISR                       /* Vector #  50 */
    .long   DefaultISR                       /* Vector #  51 */
    .long   DMA_Error_IRQHandler             /* Vector #  52 eDMA Combined Error eDMA Channel Error Flags 63-0 */
    .long   DMA0_IRQHandler                  /* Vector #  53 eDMA Channel 0 DMA_INTL[INT0] */
    .long   DMA1_IRQHandler                  /* Vector #  54 eDMA Channel 1 DMA_INTL[INT1] */
    .long   DMA2_IRQHandler                  /* Vector #  55 eDMA Channel 2 DMA_INTL[INT2] */
    .long   DMA3_IRQHandler                  /* Vector #  56 eDMA Channel 3 DMA_INTL[INT3] */
    .long   DMA4_IRQHandler                  /* Vector #  57 eDMA Channel 4 DMA_INTL[INT4] */
    .long   DMA5_IRQHandler                  /* Vector #  58 eDMA Channel 5 DMA_INTL[INT5] */
    .long   DMA6_IRQHandler                  /* Vector #  59 eDMA Channel 6 DMA_INTL[INT6] */
    .long   DMA7_IRQHandler                  /* Vector #  60 eDMA Channel 7 DMA_INTL[INT7] */
    .long   DMA8_IRQHandler                  /* Vector #  61 eDMA Channel 8 DMA_INTL[INT8] */
    .long   DMA9_IRQHandler                  /* Vector #  62 eDMA Channel 9 DMA_INTL[INT9] */
    .long   DMA10_IRQHandler                 /* Vector #  63 eDMA Channel 10 DMA_INTL[INT10] */
    .long   DMA11_IRQHandler                 /* Vector #  64 eDMA Channel 11 DMA_INTL[INT11] */
    .long   DMA12_IRQHandler                 /* Vector #  65 eDMA Channel 12 DMA_INTL[INT12] */
    .long   DMA13_IRQHandler                 /* Vector #  66 eDMA Channel 13 DMA_INTL[INT13] */
    .long   DMA14_IRQHandler                 /* Vector #  67 eDMA Channel 14 DMA_INTL[INT14] */
    .long   DMA15_IRQHandler                 /* Vector #  68 eDMA Channel 15 DMA_INTL[INT15] */
    .long   DMA16_IRQHandler                 /* Vector #  69 eDMA Channel 16 DMA_INTL[INT16] */
    .long   DMA17_IRQHandler                 /* Vector #  70 eDMA Channel 17 DMA_INTL[INT17] */
    .long   DMA18_IRQHandler                 /* Vector #  71 eDMA Channel 18 DMA_INTL[INT18] */
    .long   DMA19_IRQHandler                 /* Vector #  72 eDMA Channel 19 DMA_INTL[INT19] */
    .long   DMA20_IRQHandler                 /* Vector #  73 eDMA Channel 20 DMA_INTL[INT20] */
    .long   DMA21_IRQHandler                 /* Vector #  74 eDMA Channel 21 DMA_INTL[INT21] */
    .long   DMA22_IRQHandler                 /* Vector #  75 eDMA Channel 22 DMA_INTL[INT22] */
    .long   DMA23_IRQHandler                 /* Vector #  76 eDMA Channel 23 DMA_INTL[INT23] */
    .long   DMA24_IRQHandler                 /* Vector #  77 eDMA Channel 24 DMA_INTL[INT24] */
    .long   DMA25_IRQHandler                 /* Vector #  78 eDMA Channel 25 DMA_INTL[INT25] */
    .long   DMA26_IRQHandler                 /* Vector #  79 eDMA Channel 26 DMA_INTL[INT26] */
    .long   DMA27_IRQHandler                 /* Vector #  80 eDMA Channel 27 DMA_INTL[INT27] */
    .long   DMA28_IRQHandler                 /* Vector #  81 eDMA Channel 28 DMA_INTL[INT28] */
    .long   DMA29_IRQHandler                 /* Vector #  82 eDMA Channel 29 DMA_INTL[INT29] */
    .long   DMA30_IRQHandler                 /* Vector #  83 eDMA Channel 30 DMA_INTL[INT30] */
    .long   DMA31_IRQHandler                 /* Vector #  84 eDMA Channel 31 DMA_INTL[INT31] */
    .long   DefaultISR                       /* Vector #  85 */
    .long   DefaultISR                       /* Vector #  86 */
    .long   DefaultISR                       /* Vector #  87 */
    .long   DefaultISR                       /* Vector #  88 */
    .long   DefaultISR                       /* Vector #  89 */
    .long   DefaultISR                       /* Vector #  90 */
    .long   DefaultISR                       /* Vector #  91 */
    .long   DefaultISR                       /* Vector #  92 */
    .long   DefaultISR                       /* Vector #  93 */
    .long   DefaultISR                       /* Vector #  94 */
    .long   DefaultISR                       /* Vector #  95 */
    .long   DefaultISR                       /* Vector #  96 */
    .long   DefaultISR                       /* Vector #  97 */
    .long   DefaultISR                       /* Vector #  98 */
    .long   DefaultISR                       /* Vector #  99 */
    .long   DefaultISR                       /* Vector # 100 */
    .long   DefaultISR                       /* Vector # 101 */
    .long   DefaultISR                       /* Vector # 102 */
    .long   DefaultISR                       /* Vector # 103 */
    .long   DefaultISR                       /* Vector # 104 */
    .long   DefaultISR                       /* Vector # 105 */
    .long   DefaultISR                       /* Vector # 106 */
    .long   DefaultISR                       /* Vector # 107 */
    .long   DefaultISR                       /* Vector # 108 */
    .long   DefaultISR                       /* Vector # 109 */
    .long   DefaultISR                       /* Vector # 110 */
    .long   DefaultISR                       /* Vector # 111 */
    .long   DefaultISR                       /* Vector # 112 */
    .long   DefaultISR                       /* Vector # 113 */
    .long   DefaultISR                       /* Vector # 114 */
    .long   DefaultISR                       /* Vector # 115 */
    .long   DefaultISR                       /* Vector # 116 */
    .long   DefaultISR                       /* Vector # 117 */
    .long   DefaultISR                       /* Vector # 118 */
    .long   DefaultISR                       /* Vector # 119 */
    .long   DefaultISR                       /* Vector # 120 */
    .long   DefaultISR                       /* Vector # 121 */
    .long   DefaultISR                       /* Vector # 122 */
    .long   DefaultISR                       /* Vector # 123 */
    .long   DefaultISR                       /* Vector # 124 */
    .long   DefaultISR                       /* Vector # 125 */
    .long   DefaultISR                       /* Vector # 126 */
    .long   DefaultISR                       /* Vector # 127 */
    .long   DefaultISR                       /* Vector # 128 */
    .long   DefaultISR                       /* Vector # 129 */
    .long   DefaultISR                       /* Vector # 130 */
    .long   DefaultISR                       /* Vector # 131 */
    .long   DefaultISR                       /* Vector # 132 */
    .long   DefaultISR                       /* Vector # 133 */
    .long   DefaultISR                       /* Vector # 134 */
    .long   DefaultISR                       /* Vector # 135 */
    .long   DefaultISR                       /* Vector # 136 */
    .long   DefaultISR                       /* Vector # 137 */
    .long   DefaultISR                       /* Vector # 138 */
    .long   DefaultISR                       /* Vector # 139 */
    .long   DefaultISR                       /* Vector # 140 */
    .long   DefaultISR                       /* Vector # 141 */
    .long   DefaultISR                       /* Vector # 142 */
    .long   DefaultISR                       /* Vector # 143 */
    .long   DefaultISR                       /* Vector # 144 */
    .long   DefaultISR                       /* Vector # 145 */
    .long   DefaultISR                       /* Vector # 146 */
    .long   DefaultISR                       /* Vector # 147 */
    .long   DefaultISR                       /* Vector # 148 */
    .long   DefaultISR                       /* Vector # 149 */
    .long   DefaultISR                       /* Vector # 150 */
    .long   DefaultISR                       /* Vector # 151 */
    .long   DefaultISR                       /* Vector # 152 */
    .long   DefaultISR                       /* Vector # 153 */
    .long   DefaultISR                       /* Vector # 154 */
    .long   DefaultISR                       /* Vector # 155 */
    .long   DefaultISR                       /* Vector # 156 */
    .long   DefaultISR                       /* Vector # 157 */
    .long   DefaultISR                       /* Vector # 158 */
    .long   DefaultISR                       /* Vector # 159 */
    .long   DefaultISR                       /* Vector # 160 */
    .long   DefaultISR                       /* Vector # 161 */
    .long   DefaultISR                       /* Vector # 162 */
    .long   DefaultISR                       /* Vector # 163 */
    .long   DefaultISR                       /* Vector # 164 */
    .long   DefaultISR                       /* Vector # 165 */
    .long   DefaultISR                       /* Vector # 166 */
    .long   DefaultISR                       /* Vector # 167 */
    .long   DefaultISR                       /* Vector # 168 */
    .long   DefaultISR                       /* Vector # 169 */
    .long   DefaultISR                       /* Vector # 170 */
    .long   DefaultISR                       /* Vector # 171 */
    .long   DefaultISR                       /* Vector # 172 */
    .long   DefaultISR                       /* Vector # 173 */
    .long   DefaultISR                       /* Vector # 174 */
    .long   DefaultISR                       /* Vector # 175 */
    .long   DefaultISR                       /* Vector # 176 */
    .long   DefaultISR                       /* Vector # 177 */
    .long   DefaultISR                       /* Vector # 178 */
    .long   DefaultISR                       /* Vector # 179 */
    .long   DefaultISR                       /* Vector # 180 */
    .long   DefaultISR                       /* Vector # 181 */
    .long   DefaultISR                       /* Vector # 182 */
    .long   DefaultISR                       /* Vector # 183 */
    .long   DefaultISR                       /* Vector # 184 */
    .long   FLASH_Done_IRQHandler            /* Vector # 185 Flash controller Prog/Erase/Suspend IRQ_0 MCR[DONE] */
    .long   DefaultISR                       /* Vector # 186 */
    .long   DefaultISR                       /* Vector # 187 */
    .long   DefaultISR                       /* Vector # 188 */
    .long   DefaultISR                       /* Vector # 189 */
    .long   DefaultISR                       /* Vector # 190 */
    .long   DefaultISR                       /* Vector # 191 */
    .long   DefaultISR                       /* Vector # 192 */
    .long   DefaultISR                       /* Vector # 193 */
    .long   DefaultISR                       /* Vector # 194 */
    .long   DefaultISR                       /* Vector # 195 */
    .long   DefaultISR                       /* Vector # 196 */
    .long   DefaultISR                       /* Vector # 197 */
    .long   DefaultISR                       /* Vector # 198 */
    .long   DefaultISR                       /* Vector # 199 */
    .long   DefaultISR                       /* Vector # 200 */
    .long   ENET1_Err_IRQHandler             /* Vector # 201 ENET1 Interrupt Group0 ENET_0 */
    .long   ENET1_Rx_IRQHandler              /* Vector # 202 ENET1 Interrupt Group1 ENET_1 */
    .long   ENET1_Tx_IRQHandler              /* Vector # 203 ENET1 Interrupt Group2 ENET_2 */
    .long   ENET1_Parser_IRQHandler          /* Vector # 204 ENET1 Interrupt Group3 ENET_3 */
    .long   ENET1_Timer_IRQHandler           /* Vector # 205 ENET1 Interrupt Group4 ENET_4 */
    .long   ENET1_Rx_1_IRQHandler            /* Vector # 206 ENET1 Interrupt Group5 ENET_5 */
    .long   ENET1_Tx_1_IRQHandler            /* Vector # 207 ENET1 Interrupt Group6 ENET_6 */
    .long   ENET1_Rx_2_IRQHandler            /* Vector # 208 ENET1 Interrupt Group7 ENET_7 */
    .long   ENET1_Tx_2_IRQHandler            /* Vector # 209 ENET1 Interrupt Group8 ENET_8 */
    .long   ENET0_Err_IRQHandler             /* Vector # 210 ENET0 Interrupt Group0 ENET_0 */
    .long   ENET0_Rx_IRQHandler              /* Vector # 211 ENET0 Interrupt Group1 ENET_1 */
    .long   ENET0_Tx_IRQHandler              /* Vector # 212 ENET0 Interrupt Group2 ENET_2 */
    .long   ENET0_Parser_IRQHandler          /* Vector # 213 ENET0 Interrupt Group3 ENET_3 */
    .long   ENET0_Timer_IRQHandler           /* Vector # 214 ENET0 Interrupt Group4 ENET_4 */
    .long   ENET0_Rx_1_IRQHandler            /* Vector # 215 ENET0 Interrupt Group5 ENET_5 */
    .long   ENET0_Tx_1_IRQHandler            /* Vector # 216 ENET0 Interrupt Group6 ENET_6 */
    .long   ENET0_Rx_2_IRQHandler            /* Vector # 217 ENET0 Interrupt Group7 ENET_7 */
    .long   ENET0_Tx_2_IRQHandler            /* Vector # 218 ENET0 Interrupt Group8 ENET_8 */
    .long   DefaultISR                       /* Vector # 219 */
    .long   DefaultISR                       /* Vector # 220 */
    .long   DefaultISR                       /* Vector # 221 */
    .long   DefaultISR                       /* Vector # 222 */
    .long   DefaultISR                       /* Vector # 223 */
    .long   RTC0_IRQHandler                  /* Vector # 224 Real Time Counter (RTC) RTC */
    .long   API0_IRQHandler                  /* Vector # 225 Autonomous Periodic Interrupt (API) API */
    .long   PIT_Ch0_IRQHandler               /* Vector # 226 Periodic Interrupt Timer (PIT0) PIT_1_TFLG0[TIF] */
    .long   PIT_Ch1_IRQHandler               /* Vector # 227 Periodic Interrupt Timer (PIT1) PIT_1_TFLG1[TIF] */
    .long   PIT_Ch2_IRQHandler               /* Vector # 228 Periodic Interrupt Timer (PIT2) PIT_1_TFLG2[TIF] */
    .long   PIT_Ch3_IRQHandler               /* Vector # 229 Periodic Interrupt Timer (PIT3) PIT_1_TFLG3[TIF] */
    .long   PIT_Ch4_IRQHandler               /* Vector # 230 Periodic Interrupt Timer (PIT4) PIT_1_TFLG4[TIF] */
    .long   PIT_Ch5_IRQHandler               /* Vector # 231 Periodic Interrupt Timer (PIT5) PIT_1_TFLG5[TIF] */
    .long   PIT_Ch6_IRQHandler               /* Vector # 232 Periodic Interrupt Timer (PIT6) PIT_1_TFLG6[TIF] */
    .long   PIT_Ch7_IRQHandler               /* Vector # 233 Periodic Interrupt Timer (PIT7) PIT_1_TFLG7[TIF] */
    .long   PIT_Ch8_IRQHandler               /* Vector # 234 Periodic Interrupt Timer (PIT8) PIT_1_TFLG8[TIF] */
    .long   PIT_Ch9_IRQHandler               /* Vector # 235 Periodic Interrupt Timer (PIT9) PIT_1_TFLG9[TIF] */
    .long   PIT_Ch10_IRQHandler              /* Vector # 236 Periodic Interrupt Timer (PIT10) PIT_1_TFLG10[TIF] */
    .long   PIT_Ch11_IRQHandler              /* Vector # 237 Periodic Interrupt Timer (PIT11) PIT_1_TFLG11[TIF] */
    .long   PIT_Ch12_IRQHandler              /* Vector # 238 Periodic Interrupt Timer (PIT12) PIT_1_TFLG12[TIF] */
    .long   PIT_Ch13_IRQHandler              /* Vector # 239 Periodic Interrupt Timer (PIT13) PIT_1_TFLG13[TIF] */
    .long   PIT_Ch14_IRQHandler              /* Vector # 240 Periodic Interrupt Timer (PIT14) PIT_1_TFLG14[TIF] */
    .long   PIT_Ch15_IRQHandler              /* Vector # 241 Periodic Interrupt Timer (PIT15) PIT_1_TFLG15[TIF] */
    .long   PIT_RTI_IRQHandler               /* Vector # 242 PIT_RTI PIT_RTI */
    .long   SIUL_EIRQ_00_07_IRQHandler       /* Vector # 243 SIUL EIRQ [0-7] - SIUL Combined External Interrupt */
    .long   SIUL_EIRQ_08_15_IRQHandler       /* Vector # 244 SIUL EIRQ [8-15] - SIUL Combined External Interrupt */
    .long   SIUL_EIRQ_16_23_IRQHandler       /* Vector # 245 SIUL EIRQ [16-23] - SIUL Combined External Interrupt */
    .long   SIUL_EIRQ_24_31_IRQHandler       /* Vector # 246 SIUL EIRQ [24-31] - SIUL Combined External Interrupt */
    .long   DefaultISR                       /* Vector # 247 */
    .long   DefaultISR                       /* Vector # 248 */
    .long   DefaultISR                       /* Vector # 249 */
    .long   DefaultISR                       /* Vector # 250 LPU_CTL o_interrupt */
    .long   DefaultISR                       /* Vector # 251 MC _ME ME_IS[I_SAFE] */
    .long   DefaultISR                       /* Vector # 252 MC _ME ME_IS[I_MTC] */
    .long   DefaultISR                       /* Vector # 253 MC _ME ME_IS[I_IMODE] */
    .long   DefaultISR                       /* Vector # 254 MC _ME ME_IS[I_ICONF] */
    .long   DefaultISR                       /* Vector # 255 MC_RGM MC_RGM Functional and destructive reset alternate event interrupt */
    .long   DefaultISR                       /* Vector # 256 */
    .long   DefaultISR                       /* Vector # 257 FXOSC Counter FXOSC Counter */
    .long   DefaultISR                       /* Vector # 258 SXOSC Counter SXOSC Counter */
    .long   DSPI0_FIFO_Error_IRQHandler      /* Vector # 259 DSPI0_0 DSPI_0_SR[TFUF] | DSPI_0_SR[RFOF] | DSPI_0_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 260 DSPI0_1 DSPI_0_SR[EOQF] */
    .long   DSPI0_Send_IRQHandler            /* Vector # 261 DSPI0_2 DSPI_0_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 262 DSPI0_3 DSPI_0_SR[TCF] */
    .long   DSPI0_Receive_IRQHandler         /* Vector # 263 DSPI0_4 DSPI_0_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 264 DSPI0_5 DSPI_0_SR[SPITCF] | DSPI_0_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 265 DSPI0_6 DSPI_0_SR[DSITCF] | DSPI_0_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 266 DSPI0_7 DSPI_0_SR[SPEF] | DSPI_0_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 267 DSPI0_8 DSPI_0_SR[DDIF] */
    .long   DSPI1_FIFO_Error_IRQHandler      /* Vector # 268 DSPI1_0 DSPI_1_SR[TFUF] | DSPI_1_SR[RFOF]| DSPI_1_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 269 DSPI1_1 DSPI_1_SR[EOQF] */
    .long   DSPI1_Send_IRQHandler            /* Vector # 270 DSPI1_2 DSPI_1_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 271 DSPI1_3 DSPI_1_SR[TCF] */
    .long   DSPI1_Receive_IRQHandler         /* Vector # 272 DSPI1_4 DSPI_1_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 273 DSPI1_5 DSPI_1_SR[SPITCF] | DSPI_1_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 274 DSPI1_6 DSPI_1_SR[DSITCF] | DSPI_1_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 275 DSPI1_7 DSPI_1_SR[SPEF] | DSPI_1_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 276 DSPI1_8 DSPI_1_SR[DDIF] */
    .long   DSPI2_FIFO_Error_IRQHandler      /* Vector # 277 DSPI2_0 DSPI_2_SR[TFUF] | DSPI_2_SR[RFOF]| DSPI_2_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 278 DSPI2_1 DSPI_2_SR[EOQF] */
    .long   DSPI2_Send_IRQHandler            /* Vector # 279 DSPI2_2 DSPI_2_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 280 DSPI2_3 DSPI_2_SR[TCF] */
    .long   DSPI2_Receive_IRQHandler         /* Vector # 281 DSPI2_4 DSPI_2_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 282 DSPI2_5 DSPI_2_SR[SPITCF] | DSPI_2_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 283 DSPI2_6 DSPI_2_SR[DSITCF] | DSPI_2_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 284 DSPI2_7 DSPI_2_SR[SPEF] | DSPI_2_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 285 DSPI2_8 DSPI_2_SR[DDIF] */
    .long   DSPI3_FIFO_Error_IRQHandler      /* Vector # 286 DSPI3_0 DSPI_3_SR[TFUF] | DSPI_3_SR[RFOF]| DSPI_3_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 287 DSPI3_1 DSPI_3_SR[EOQF] */
    .long   DSPI3_Send_IRQHandler            /* Vector # 288 DSPI3_2 DSPI_3_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 289 DSPI3_3 DSPI_3_SR[TCF] */
    .long   DSPI3_Receive_IRQHandler         /* Vector # 290 DSPI3_4 DSPI_3_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 291 DSPI3_5 DSPI_3_SR[SPITCF] | DSPI_3_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 292 DSPI3_6 DSPI_3_SR[DSITCF] | DSPI_3_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 293 DSPI3_7 DSPI_3_SR[SPEF] | DSPI_3_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 294 DSPI3_8 DSPI_3_SR[DDIF] */
    .long   DSPI4_FIFO_Error_IRQHandler      /* Vector # 295 DSPI4_0 DSPI_4_SR[TFUF] | DSPI_4_SR[RFOF]| DSPI_4_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 296 DSPI4_1 DSPI_4_SR[EOQF] */
    .long   DSPI4_Send_IRQHandler            /* Vector # 297 DSPI4_2 DSPI_4_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 298 DSPI4_3 DSPI_4_SR[TCF] */
    .long   DSPI4_Receive_IRQHandler         /* Vector # 299 DSPI4_4 DSPI_4_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 300 DSPI4_5 DSPI_4_SR[SPITCF] | DSPI_4_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 301 DSPI4_6 DSPI_4_SR[DSITCF] | DSPI_4_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 302 DSPI4_7 DSPI_4_SR[SPEF] | DSPI_4_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 303 */
    .long   DSPI5_FIFO_Error_IRQHandler      /* Vector # 304 DSPI5_0 DSPI_5_SR[TFUF] | DSPI_5_SR[RFOF]| DSPI_5_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 305 DSPI5_1 DSPI_5_SR[EOQF] */
    .long   DSPI5_Send_IRQHandler            /* Vector # 306 DSPI5_2 DSPI_5_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 307 DSPI5_3 DSPI_5_SR[TCF] */
    .long   DSPI5_Receive_IRQHandler         /* Vector # 308 DSPI5_4 DSPI_5_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 309 DSPI5_5 DSPI_5_SR[SPITCF] | DSPI_5_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 310 DSPI5_6 DSPI_5_SR[DSITCF] | DSPI_5_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 311 DSPI5_7 DSPI_5_SR[SPEF] | DSPI_5_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 312 */
    .long   DSPI6_FIFO_Error_IRQHandler      /* Vector # 313 DSPI6_0 DSPI_6_SR[TFUF] | DSPI_6_SR[RFOF]| DSPI_6_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 314 DSPI6_1 DSPI_6_SR[EOQF] */
    .long   DSPI6_Send_IRQHandler            /* Vector # 315 DSPI6_2 DSPI_6_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 316 DSPI6_3 DSPI_6_SR[TCF] */
    .long   DSPI6_Receive_IRQHandler         /* Vector # 317 DSPI6_4 DSPI_6_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 318 DSPI6_5 DSPI_6_SR[SPITCF] | DSPI_6_SR[CMD_TCF] */
    .long   DefaultISR                       /* Vector # 319 DSPI6_6 DSPI_6_SR[DSITCF] | DSPI_6_SR[CMDFFF] */
    .long   DefaultISR                       /* Vector # 320 DSPI6_7 DSPI_6_SR[SPEF] | DSPI_6_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 321 */
    .long   DSPI7_FIFO_Error_IRQHandler      /* Vector # 322 DSPI7_0 DSPI_7_SR[TFUF] | DSPI_7_SR[RFOF]| DSPI_7_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 323 DSPI7_1 DSPI_7_SR[EOQF] */
    .long   DSPI7_Send_IRQHandler            /* Vector # 324 DSPI7_2 DSPI_7_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 325 DSPI7_3 DSPI_7_SR[TCF] */
    .long   DSPI7_Receive_IRQHandler         /* Vector # 326 DSPI7_4 DSPI_7_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 327 DSPI7_5 DSPI_7_SR[SPITCF] */
    .long   DefaultISR                       /* Vector # 328 DSPI7_6 DSPI_7_SR[DSITCF] */
    .long   DefaultISR                       /* Vector # 329 DSPI7_7 DSPI_7_SR[SPEF] | DSPI_7_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 330 */
    .long   DSPI8_FIFO_Error_IRQHandler      /* Vector # 331 DSPI8_0 DSPI_8_SR[TFUF] | DSPI_8_SR[RFOF]| DSPI_8_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 332 DSPI8_1 DSPI_8_SR[EOQF] */
    .long   DSPI8_Send_IRQHandler            /* Vector # 333 DSPI8_2 DSPI_8_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 334 DSPI8_3 DSPI_8_SR[TCF] */
    .long   DSPI8_Receive_IRQHandler         /* Vector # 335 DSPI8_4 DSPI_8_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 336 DSPI8_5 DSPI_8_SR[SPITCF] */
    .long   DefaultISR                       /* Vector # 337 DSPI8_6 DSPI_8_SR[DSITCF] */
    .long   DefaultISR                       /* Vector # 338 DSPI8_7 DSPI_8_SR[SPEF] | DSPI_8_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 339 */
    .long   DSPI9_FIFO_Error_IRQHandler      /* Vector # 340 DSPI9_0 DSPI_9_SR[TFUF] | DSPI_9_SR[RFOF]| DSPI_9_SR[TFIWF] */
    .long   DefaultISR                       /* Vector # 341 DSPI9_1 DSPI_9_SR[EOQF] */
    .long   DSPI9_Send_IRQHandler            /* Vector # 342 DSPI9_2 DSPI_9_SR[TFFF] */
    .long   DefaultISR                       /* Vector # 343 DSPI9_3 DSPI_9_SR[TCF] */
    .long   DSPI9_Receive_IRQHandler         /* Vector # 344 DSPI9_4 DSPI_9_SR[RFDF] */
    .long   DefaultISR                       /* Vector # 345 DSPI9_5 DSPI_9_SR[SPITCF] */
    .long   DefaultISR                       /* Vector # 346 DSPI9_6 DSPI_9_SR[DSITCF] */
    .long   DefaultISR                       /* Vector # 347 DSPI9_7 DSPI_9_SR[SPEF] | DSPI_9_SR[DPEF] */
    .long   DefaultISR                       /* Vector # 348 */
    .long   DefaultISR                       /* Vector # 349 */
    .long   DefaultISR                       /* Vector # 350 */
    .long   DefaultISR                       /* Vector # 351 */
    .long   DefaultISR                       /* Vector # 352 */
    .long   DefaultISR                       /* Vector # 353 */
    .long   DefaultISR                       /* Vector # 354 */
    .long   DefaultISR                       /* Vector # 355 */
    .long   DefaultISR                       /* Vector # 356 */
    .long   DefaultISR                       /* Vector # 357 */
    .long   DefaultISR                       /* Vector # 358 */
    .long   DefaultISR                       /* Vector # 359 */
    .long   DefaultISR                       /* Vector # 360 */
    .long   DefaultISR                       /* Vector # 361 */
    .long   DefaultISR                       /* Vector # 362 */
    .long   DefaultISR                       /* Vector # 363 */
    .long   DefaultISR                       /* Vector # 364 */
    .long   DefaultISR                       /* Vector # 365 */
    .long   DefaultISR                       /* Vector # 366 */
    .long   DefaultISR                       /* Vector # 367 */
    .long   DefaultISR                       /* Vector # 368 */
    .long   DefaultISR                       /* Vector # 369 */
    .long   DefaultISR                       /* Vector # 370 */
    .long   DefaultISR                       /* Vector # 371 */
    .long   BCTU_ListLast_IRQHandler         /* Vector # 372 BCTU LIST0_LAST | LIST1_LAST */
    .long   BCTU_ConvUpdate_IRQHandler       /* Vector # 373 BCTU NDATA0 | NDATA1 | DATA_OVR0 | DATA_OVR1 | TRGF */
    .long   DefaultISR                       /* Vector # 374 */
    .long   DefaultISR                       /* Vector # 375 */
    .long   LINFLEXD0_UART_RxIRQHandler      /* Vector # 376 LinFlex_0_RXI */
    .long   LINFLEXD0_UART_TxIRQHandler      /* Vector # 377 LinFlex_0_TXI */
    .long   LINFLEXD0_UART_ErrIRQHandler     /* Vector # 378 LinFlex_0_ERR */
    .long   LINFLEXD1_UART_RxIRQHandler      /* Vector # 379 LinFlex_1_RXI */
    .long   LINFLEXD1_UART_TxIRQHandler      /* Vector # 380 LinFlex_1_TXI */
    .long   LINFLEXD1_UART_ErrIRQHandler     /* Vector # 381 LinFlex_1_ERR */
    .long   LINFLEXD2_UART_RxIRQHandler      /* Vector # 382 LinFlex_2_RXI */
    .long   LINFLEXD2_UART_TxIRQHandler      /* Vector # 383 LinFlex_2_TXI */
    .long   LINFLEXD2_UART_ErrIRQHandler     /* Vector # 384 LinFlex_2_ERR */
    .long   LINFLEXD3_UART_RxIRQHandler      /* Vector # 385 LinFlex_3_RXI */
    .long   LINFLEXD3_UART_TxIRQHandler      /* Vector # 386 LinFlex_3_TXI */
    .long   LINFLEXD3_UART_ErrIRQHandler     /* Vector # 387 LinFlex_3_ERR */
    .long   LINFLEXD4_UART_RxIRQHandler      /* Vector # 388 LinFlex_4_RXI */
    .long   LINFLEXD4_UART_TxIRQHandler      /* Vector # 389 LinFlex_4_TXI */
    .long   LINFLEXD4_UART_ErrIRQHandler     /* Vector # 390 LinFlex_4_ERR */
    .long   LINFLEXD5_UART_RxIRQHandler      /* Vector # 391 LinFlex_5_RXI */
    .long   LINFLEXD5_UART_TxIRQHandler      /* Vector # 392 LinFlex_5_TXI */
    .long   LINFLEXD5_UART_ErrIRQHandler     /* Vector # 393 LinFlex_5_ERR */
    .long   LINFLEXD6_UART_RxIRQHandler      /* Vector # 394 LinFlex_6_RXI */
    .long   LINFLEXD6_UART_TxIRQHandler      /* Vector # 395 LinFlex_6_TXI */
    .long   LINFLEXD6_UART_ErrIRQHandler     /* Vector # 396 LinFlex_6_ERR */
    .long   LINFLEXD7_UART_RxIRQHandler      /* Vector # 397 LinFlex_7_RXI */
    .long   LINFLEXD7_UART_TxIRQHandler      /* Vector # 398 LinFlex_7_TXI */
    .long   LINFLEXD7_UART_ErrIRQHandler     /* Vector # 399 LinFlex_7_ERR */
    .long   LINFLEXD8_UART_RxIRQHandler      /* Vector # 400 LinFlex_8_RXI */
    .long   LINFLEXD8_UART_TxIRQHandler      /* Vector # 401 LinFlex_8_TXI */
    .long   LINFLEXD8_UART_ErrIRQHandler     /* Vector # 402 LinFlex_8_ERR */
    .long   LINFLEXD9_UART_RxIRQHandler      /* Vector # 403 LinFlex_9_RXI */
    .long   LINFLEXD9_UART_TxIRQHandler      /* Vector # 404 LinFlex_9_TXI */
    .long   LINFLEXD9_UART_ErrIRQHandler     /* Vector # 405 LinFlex_9_ERR */
    .long   LINFLEXD10_UART_RxIRQHandler     /* Vector # 406 LinFlex_10_RXI */
    .long   LINFLEXD10_UART_TxIRQHandler     /* Vector # 407 LinFlex_10_TXI */
    .long   LINFLEXD10_UART_ErrIRQHandler    /* Vector # 408 LinFlex_10_ERR */
    .long   LINFLEXD11_UART_RxIRQHandler     /* Vector # 409 LinFlex_11_RXI */
    .long   LINFLEXD11_UART_TxIRQHandler     /* Vector # 410 LinFlex_11_TXI */
    .long   LINFLEXD11_UART_ErrIRQHandler    /* Vector # 411 LinFlex_11_ERR */
    .long   LINFLEXD12_UART_RxIRQHandler     /* Vector # 412 LinFlex_12_RXI */
    .long   LINFLEXD12_UART_TxIRQHandler     /* Vector # 413 LinFlex_12_TXI */
    .long   LINFLEXD12_UART_ErrIRQHandler    /* Vector # 414 LinFlex_12_ERR */
    .long   LINFLEXD13_UART_RxIRQHandler     /* Vector # 415 LinFlex_13_RXI */
    .long   LINFLEXD13_UART_TxIRQHandler     /* Vector # 416 LinFlex_13_TXI */
    .long   LINFLEXD13_UART_ErrIRQHandler    /* Vector # 417 LinFlex_13_ERR */
    .long   LINFLEXD14_UART_RxIRQHandler     /* Vector # 418 LinFlex_14_RXI */
    .long   LINFLEXD14_UART_TxIRQHandler     /* Vector # 419 LinFlex_14_TXI */
    .long   LINFLEXD14_UART_ErrIRQHandler    /* Vector # 420 LinFlex_14_ERR */
    .long   LINFLEXD15_UART_RxIRQHandler     /* Vector # 421 LinFlex_15_RXI */
    .long   LINFLEXD15_UART_TxIRQHandler     /* Vector # 422 LinFlex_15_TXI */
    .long   LINFLEXD15_UART_ErrIRQHandler    /* Vector # 423 LinFlex_15_ERR */
    .long   LINFLEXD16_UART_RxIRQHandler     /* Vector # 424 LinFlex_16_RXI */
    .long   LINFLEXD16_UART_TxIRQHandler     /* Vector # 425 LinFlex_16_TXI */
    .long   LINFLEXD16_UART_ErrIRQHandler    /* Vector # 426 LinFlex_16_ERR */
    .long   LINFLEXD17_UART_RxIRQHandler     /* Vector # 427 LinFlex_17_RXI */
    .long   LINFLEXD17_UART_TxIRQHandler     /* Vector # 428 LinFlex_17_TXI */
    .long   LINFLEXD17_UART_ErrIRQHandler    /* Vector # 429 LinFlex_17_ERR */
    .long   DefaultISR                       /* Vector # 430 */
    .long   DefaultISR                       /* Vector # 431 */
    .long   DefaultISR                       /* Vector # 432 */
    .long   DefaultISR                       /* Vector # 433 */
    .long   DefaultISR                       /* Vector # 434 */
    .long   DefaultISR                       /* Vector # 435 */
    .long   DefaultISR                       /* Vector # 436 */
    .long   DefaultISR                       /* Vector # 437 */
    .long   DefaultISR                       /* Vector # 438 */
    .long   DefaultISR                       /* Vector # 439 */
    .long   I2C_DRV_Module0IRQHandler        /* Vector # 440 I2C_0_0 I2C0_SR[IBAL] | I2C0_SR[TCF] | I2C0_SR[IAAS] */
    .long   DefaultISR                       /* Vector # 441 */
    .long   I2C_DRV_Module1IRQHandler        /* Vector # 442 I2C_1_0 I2C1_SR[IBAL] | I2C1_SR[TCF] | I2C1_SR[IAAS] */
    .long   DefaultISR                       /* Vector # 443 */
    .long   I2C_DRV_Module2IRQHandler        /* Vector # 444 I2C_2_0 I2C2_SR[IBAL] | I2C2_SR[TCF] | I2C2_SR[IAAS] */
    .long   DefaultISR                       /* Vector # 445 */
    .long   I2C_DRV_Module3IRQHandler        /* Vector # 446 I2C_3_0 I2C3_SR[IBAL] | I2C3_SR[TCF] | I2C3_SR[IAAS] */
    .long   DefaultISR                       /* Vector # 447 */
    .long   DefaultISR                       /* Vector # 448 */
    .long   DefaultISR                       /* Vector # 449 */
    .long   DefaultISR                       /* Vector # 450 */
    .long   DefaultISR                       /* Vector # 451 */
    .long   DefaultISR                       /* Vector # 452 */
    .long   DefaultISR                       /* Vector # 453 FlexRay_0_0 LRNEIF | DRNEIF */
    .long   DefaultISR                       /* Vector # 454 FlexRay_0_1 LRCEIF | DRCEIF */
    .long   DefaultISR                       /* Vector # 455 FlexRay_0_2 FNEAIF */
    .long   DefaultISR                       /* Vector # 456 FlexRay_0_3 FNEBIF */
    .long   DefaultISR                       /* Vector # 457 FlexRay_0_4 WUPIF */
    .long   DefaultISR                       /* Vector # 458 FlexRay_0_5 PRIF */
    .long   DefaultISR                       /* Vector # 459 FlexRay_0_6 CHIF */
    .long   DefaultISR                       /* Vector # 460 FlexRay_0_7 TBIF */
    .long   DefaultISR                       /* Vector # 461 FlexRay_0_8 RBIF */
    .long   DefaultISR                       /* Vector # 462 FlexRay_0_9 MIF */
    .long   DefaultISR                       /* Vector # 463 */
    .long   DefaultISR                       /* Vector # 464 */
    .long   DefaultISR                       /* Vector # 465 */
    .long   DefaultISR                       /* Vector # 466 */
    .long   DefaultISR                       /* Vector # 467 */
    .long   DefaultISR                       /* Vector # 468 */
    .long   DefaultISR                       /* Vector # 469 */
    .long   DefaultISR                       /* Vector # 470 */
    .long   DefaultISR                       /* Vector # 471 */
    .long   DefaultISR                       /* Vector # 472 */
    .long   DefaultISR                       /* Vector # 473 */
    .long   DefaultISR                       /* Vector # 474 */
    .long   DefaultISR                       /* Vector # 475 */
    .long   DefaultISR                       /* Vector # 476 */
    .long   DefaultISR                       /* Vector # 477 Power Monitor Unit LVD/HVD self checking fault */
    .long   DefaultISR                       /* Vector # 478 */
    .long   DefaultISR                       /* Vector # 479 */
    .long   DefaultISR                       /* Vector # 480 PLL Loss-of-lock interrupt request for PLL */
    .long   DefaultISR                       /* Vector # 481 */
    .long   DefaultISR                       /* Vector # 482 */
    .long   DefaultISR                       /* Vector # 483 */
    .long   DefaultISR                       /* Vector # 484 PLL Indicates PLL Power down process completed by MC_ME */
    .long   DefaultISR                       /* Vector # 485 */
    .long   DefaultISR                       /* Vector # 486 */
    .long   DefaultISR                       /* Vector # 487 */
    .long   FCCU0_IRQ_Handler                /* Vector # 488 FCCU Interrupt request (ALARM state) */
    .long   FCCU0_IRQ_Handler                /* Vector # 489 FCCU Interrupt request (miscellaneous conditions) */
    .long   FCCU0_IRQ_Handler                /* Vector # 490 FCCU EOUT interrupt */
    .long   DefaultISR                       /* Vector # 491 */
    .long   DefaultISR                       /* Vector # 492 */
    .long   DefaultISR                       /* Vector # 493 */
    .long   DefaultISR                       /* Vector # 494 STCU Interrupt request line related to online MBIST execution */
    .long   DefaultISR                       /* Vector # 495 STCU Interrupt request line related to online LBIST execution */
    .long   DefaultISR                       /* Vector # 496 Hardware Security Module 0 HSM2HTIE[0] */
    .long   HSM_DRV_IRQHandler               /* Vector # 497 Hardware Security Module 1 HSM2HTIE[1] */
    .long   DefaultISR                       /* Vector # 498 Hardware Security Module 2 HSM2HTIE[2] */
    .long   DefaultISR                       /* Vector # 499 Hardware Security Module 3 HSM2HTIE[3] */
    .long   DefaultISR                       /* Vector # 500 Hardware Security Module 4 HSM2HTIE[4] */
    .long   DefaultISR                       /* Vector # 501 Hardware Security Module 5 HSM2HTIE[5] */
    .long   DefaultISR                       /* Vector # 502 Hardware Security Module 6 HSM2HTIE[6] */
    .long   DefaultISR                       /* Vector # 503 Hardware Security Module 7 HSM2HTIE[7] */
    .long   DefaultISR                       /* Vector # 504 Hardware Security Module 8 HSM2HTIE[8] */
    .long   DefaultISR                       /* Vector # 505 Hardware Security Module 9 HSM2HTIE[9] */
    .long   DefaultISR                       /* Vector # 506 Hardware Security Module 10 HSM2HTIE[10] */
    .long   DefaultISR                       /* Vector # 507 Hardware Security Module 11 HSM2HTIE[11] */
    .long   DefaultISR                       /* Vector # 508 Hardware Security Module 12 HSM2HTIE[12] */
    .long   DefaultISR                       /* Vector # 509 Hardware Security Module 13 HSM2HTIE[13] */
    .long   DefaultISR                       /* Vector # 510 Hardware Security Module 14 HSM2HTIE[14] */
    .long   DefaultISR                       /* Vector # 511 Hardware Security Module 15 HSM2HTIE[15] */
    .long   DefaultISR                       /* Vector # 512 Hardware Security Module 16 HSM2HTIE[16] */
    .long   DefaultISR                       /* Vector # 513 Hardware Security Module 17 HSM2HTIE[17] */
    .long   DefaultISR                       /* Vector # 514 Hardware Security Module 18 HSM2HTIE[18] */
    .long   DefaultISR                       /* Vector # 515 Hardware Security Module 19 HSM2HTIE[19] */
    .long   DefaultISR                       /* Vector # 516 Hardware Security Module 20 HSM2HTIE[20] */
    .long   DefaultISR                       /* Vector # 517 Hardware Security Module 21 HSM2HTIE[21] */
    .long   DefaultISR                       /* Vector # 518 Hardware Security Module 22 HSM2HTIE[22] */
    .long   DefaultISR                       /* Vector # 519 Hardware Security Module 23 HSM2HTIE[23] */
    .long   DefaultISR                       /* Vector # 520 Hardware Security Module 24 HSM2HTIE[24] */
    .long   DefaultISR                       /* Vector # 521 Hardware Security Module 25 HSM2HTIE[25] */
    .long   DefaultISR                       /* Vector # 522 Hardware Security Module 26 HSM2HTIE[26] */
    .long   DefaultISR                       /* Vector # 523 Hardware Security Module 27 HSM2HTIE[27] */
    .long   DefaultISR                       /* Vector # 524 Hardware Security Module 28 HSM2HTIE[28] */
    .long   DefaultISR                       /* Vector # 525 Hardware Security Module 29 HSM2HTIE[29] */
    .long   DefaultISR                       /* Vector # 526 Hardware Security Module 30 HSM2HTIE[30] */
    .long   DefaultISR                       /* Vector # 527 Hardware Security Module 31 HSM2HTIE[31] */
    .long   DefaultISR                       /* Vector # 528 */
    .long   DefaultISR                       /* Vector # 529 */
    .long   DefaultISR                       /* Vector # 530 */
    .long   DefaultISR                       /* Vector # 531 */
    .long   DefaultISR                       /* Vector # 532 */
    .long   DefaultISR                       /* Vector # 533 */
    .long   DefaultISR                       /* Vector # 534 */
    .long   DefaultISR                       /* Vector # 535 */
    .long   DefaultISR                       /* Vector # 536 */
    .long   DefaultISR                       /* Vector # 537 */
    .long   DefaultISR                       /* Vector # 538 */
    .long   DefaultISR                       /* Vector # 539 */
    .long   DefaultISR                       /* Vector # 540 */
    .long   DefaultISR                       /* Vector # 541 */
    .long   DefaultISR                       /* Vector # 542 */
    .long   DefaultISR                       /* Vector # 543 */
    .long   DefaultISR                       /* Vector # 544 */
    .long   DefaultISR                       /* Vector # 545 */
    .long   DefaultISR                       /* Vector # 546 */
    .long   DefaultISR                       /* Vector # 547 */
    .long   ADC0_EOC_IRQHandler              /* Vector # 548 ADC_0_EOC ADC_0_EOC */
    .long   ADC0_ER_IRQHandler               /* Vector # 549 ADC_0_ER ADC_0_ER */
    .long   ADC0_WD_IRQHandler               /* Vector # 550 ADC_0_WD ADC_0_WD */
    .long   DefaultISR                       /* Vector # 551 */
    .long   DefaultISR                       /* Vector # 552 */
    .long   DefaultISR                       /* Vector # 553 */
    .long   ADC1_EOC_IRQHandler              /* Vector # 554 ADC_1_EOC ADC_1_EOC */
    .long   ADC1_ER_IRQHandler               /* Vector # 555 ADC_1_ER ADC_1_ER */
    .long   ADC1_WD_IRQHandler               /* Vector # 556 ADC_1_WD ADC_1_WD */
    .long   DefaultISR                       /* Vector # 557 */
    .long   DefaultISR                       /* Vector # 558 */
    .long   DefaultISR                       /* Vector # 559 */
    .long   CMP0_IRQHandler                  /* Vector # 560 Analogue_Comparator_Module_0 CMP0 */
    .long   CMP1_IRQHandler                  /* Vector # 561 Analogue_Comparator_Module_1 CMP1 */
    .long   CMP2_IRQHandler                  /* Vector # 562 Analogue_Comparator_Module_2 CMP2 */
    .long   DefaultISR                       /* Vector # 563 */
    .long   DefaultISR                       /* Vector # 564 */
    .long   CAN0_Wake_Up_IRQHandler          /* Vector # 565 FlexCAN0_0 FlexCAN0_PN */
    .long   CAN0_Error_IRQHandler            /* Vector # 566 FlexCAN0_1 FLEXCAN_0_ESR[ERR_INT] */
    .long   CAN0_ORed_IRQHandler             /* Vector # 567 FlexCAN0_2 FLEXCAN_0_ESR_BOFF | FLEXCAN_0_Transmit_Warning | FLEXCAN_0_Receive_Warning */
    .long   CAN0_ORed_00_03_MB_IRQHandler    /* Vector # 568 FlexCAN0_3 FlexCAN0_BUF_00_03 */
    .long   CAN0_ORed_04_07_MB_IRQHandler    /* Vector # 569 FlexCAN0_4 FlexCAN0_BUF_04_07 */
    .long   CAN0_ORed_08_11_MB_IRQHandler    /* Vector # 570 FlexCAN0_5 FlexCAN0_BUF_08_11 */
    .long   CAN0_ORed_12_15_MB_IRQHandler    /* Vector # 571 FlexCAN0_6 FlexCAN0_BUF_12_15 */
    .long   CAN0_ORed_16_31_MB_IRQHandler    /* Vector # 572 FlexCAN0_7 FlexCAN0_BUF_16_31 */
    .long   CAN0_ORed_32_63_MB_IRQHandler    /* Vector # 573 FlexCAN0_8 FlexCAN0_BUF_32_63 */
    .long   CAN0_ORed_64_95_MB_IRQHandler    /* Vector # 574 FlexCAN0_9 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 575 */
    .long   DefaultISR                       /* Vector # 576 */
    .long   DefaultISR                       /* Vector # 577 */
    .long   CAN1_Error_IRQHandler            /* Vector # 578 FlexCAN1_0 FLEXCAN_1_ESR[ERR_INT] */
    .long   CAN1_ORed_IRQHandler             /* Vector # 579 FlexCAN1_1 FLEXCAN_1_ESR_BOFF | FLEXCAN_1_Transmit_Warning | FLEXCAN_1_Receive_Warning */
    .long   CAN1_ORed_00_03_MB_IRQHandler    /* Vector # 580 FlexCAN1_2 FlexCAN1_BUF_00_03 */
    .long   CAN1_ORed_04_07_MB_IRQHandler    /* Vector # 581 FlexCAN1_3 FlexCAN1_BUF_04_07 */
    .long   CAN1_ORed_08_11_MB_IRQHandler    /* Vector # 582 FlexCAN1_4 FlexCAN1_BUF_08_11 */
    .long   CAN1_ORed_12_15_MB_IRQHandler    /* Vector # 583 FlexCAN1_5 FlexCAN1_BUF_12_15 */
    .long   CAN1_ORed_16_31_MB_IRQHandler    /* Vector # 584 FlexCAN1_6 FlexCAN1_BUF_16_31 */
    .long   CAN1_ORed_32_63_MB_IRQHandler    /* Vector # 585 FlexCAN1_7 FlexCAN1_BUF_32_63 */
    .long   CAN1_ORed_64_95_MB_IRQHandler    /* Vector # 586 FlexCAN1_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 587 */
    .long   DefaultISR                       /* Vector # 588 */
    .long   DefaultISR                       /* Vector # 589 */
    .long   CAN2_Error_IRQHandler            /* Vector # 590 FlexCAN2_0 FLEXCAN_2_ESR[ERR_INT] */
    .long   CAN2_ORed_IRQHandler             /* Vector # 591 FlexCAN2_1 FLEXCAN_2_ESR_BOFF | FLEXCAN_2_Transmit_Warning | FLEXCAN_2_Receive_Warning */
    .long   CAN2_ORed_00_03_MB_IRQHandler    /* Vector # 592 FlexCAN2_2 FlexCAN2_BUF_00_03 */
    .long   CAN2_ORed_04_07_MB_IRQHandler    /* Vector # 593 FlexCAN2_3 FlexCAN2_BUF_04_07 */
    .long   CAN2_ORed_08_11_MB_IRQHandler    /* Vector # 594 FlexCAN2_4 FlexCAN2_BUF_08_11 */
    .long   CAN2_ORed_12_15_MB_IRQHandler    /* Vector # 595 FlexCAN2_5 FlexCAN2_BUF_12_15 */
    .long   CAN2_ORed_16_31_MB_IRQHandler    /* Vector # 596 FlexCAN2_6 FlexCAN2_BUF_16_31 */
    .long   CAN2_ORed_32_63_MB_IRQHandler    /* Vector # 597 FlexCAN2_7 FlexCAN2_BUF_32_63 */
    .long   CAN2_ORed_64_95_MB_IRQHandler    /* Vector # 598 FlexCAN2_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 599 */
    .long   DefaultISR                       /* Vector # 600 */
    .long   DefaultISR                       /* Vector # 601 */
    .long   CAN3_Error_IRQHandler            /* Vector # 602 FlexCAN3_0 FLEXCAN_3_ESR[ERR_INT] */
    .long   CAN3_ORed_IRQHandler             /* Vector # 603 FlexCAN3_1 FLEXCAN_3_ESR_BOFF | FLEXCAN_3_Transmit_Warning | FLEXCAN_3_Receive_Warning */
    .long   CAN3_ORed_00_03_MB_IRQHandler    /* Vector # 604 FlexCAN3_2 FlexCAN3_BUF_00_03 */
    .long   CAN3_ORed_04_07_MB_IRQHandler    /* Vector # 605 FlexCAN3_3 FlexCAN3_BUF_04_07 */
    .long   CAN3_ORed_08_11_MB_IRQHandler    /* Vector # 606 FlexCAN3_4 FlexCAN3_BUF_08_11 */
    .long   CAN3_ORed_12_15_MB_IRQHandler    /* Vector # 607 FlexCAN3_5 FlexCAN3_BUF_12_15 */
    .long   CAN3_ORed_16_31_MB_IRQHandler    /* Vector # 608 FlexCAN3_6 FlexCAN3_BUF_16_31 */
    .long   CAN3_ORed_32_63_MB_IRQHandler    /* Vector # 609 FlexCAN3_7 FlexCAN3_BUF_32_63 */
    .long   CAN3_ORed_64_95_MB_IRQHandler    /* Vector # 610 FlexCAN3_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 611 */
    .long   DefaultISR                       /* Vector # 612 */
    .long   DefaultISR                       /* Vector # 613 */
    .long   CAN4_Error_IRQHandler            /* Vector # 614 FlexCAN4_0 FLEXCAN_4_ESR[ERR_INT] */
    .long   CAN4_ORed_IRQHandler             /* Vector # 615 FlexCAN4_1 FLEXCAN_4_ESR_BOFF | FLEXCAN_4_Transmit_Warning | FLEXCAN_4_Receive_Warning */
    .long   CAN4_ORed_00_03_MB_IRQHandler    /* Vector # 616 FlexCAN4_2 FlexCAN4_BUF_00_03 */
    .long   CAN4_ORed_04_07_MB_IRQHandler    /* Vector # 617 FlexCAN4_3 FlexCAN4_BUF_04_07 */
    .long   CAN4_ORed_08_11_MB_IRQHandler    /* Vector # 618 FlexCAN4_4 FlexCAN4_BUF_08_11 */
    .long   CAN4_ORed_12_15_MB_IRQHandler    /* Vector # 619 FlexCAN4_5 FlexCAN4_BUF_12_15 */
    .long   CAN4_ORed_16_31_MB_IRQHandler    /* Vector # 620 FlexCAN4_6 FlexCAN4_BUF_16_31 */
    .long   CAN4_ORed_32_63_MB_IRQHandler    /* Vector # 621 FlexCAN4_7 FlexCAN4_BUF_32_63 */
    .long   CAN4_ORed_64_95_MB_IRQHandler    /* Vector # 622 FlexCAN4_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 623 */
    .long   DefaultISR                       /* Vector # 624 */
    .long   DefaultISR                       /* Vector # 625 */
    .long   CAN5_Error_IRQHandler            /* Vector # 626 FlexCAN5_0 FLEXCAN_5_ESR[ERR_INT] */
    .long   CAN5_ORed_IRQHandler             /* Vector # 627 FlexCAN5_1 FLEXCAN_5_ESR_BOFF | FLEXCAN_5_Transmit_Warning | FLEXCAN_5_Receive_Warning */
    .long   CAN5_ORed_00_03_MB_IRQHandler    /* Vector # 628 FlexCAN5_2 FlexCAN5_BUF_00_03 */
    .long   CAN5_ORed_04_07_MB_IRQHandler    /* Vector # 629 FlexCAN5_3 FlexCAN5_BUF_04_07 */
    .long   CAN5_ORed_08_11_MB_IRQHandler    /* Vector # 630 FlexCAN5_4 FlexCAN5BUF_08_11 */
    .long   CAN5_ORed_12_15_MB_IRQHandler    /* Vector # 631 FlexCAN5_5 FlexCAN5_BUF_12_15 */
    .long   CAN5_ORed_16_31_MB_IRQHandler    /* Vector # 632 FlexCAN5_6 FlexCAN5_BUF_16_31 */
    .long   CAN5_ORed_32_63_MB_IRQHandler    /* Vector # 633 FlexCAN5_7 FlexCAN5_BUF_32_63 */
    .long   CAN5_ORed_64_95_MB_IRQHandler    /* Vector # 634 FlexCAN5_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 635 */
    .long   DefaultISR                       /* Vector # 636 */
    .long   DefaultISR                       /* Vector # 637 */
    .long   CAN6_Error_IRQHandler            /* Vector # 638 FlexCAN6_0 FLEXCAN_6_ESR[ERR_INT] */
    .long   CAN6_ORed_IRQHandler             /* Vector # 639 FlexCAN6_1 FLEXCAN_6_ESR_BOFF | FLEXCAN_6_Transmit_Warning | FLEXCAN_6_Receive_Warning */
    .long   CAN6_ORed_00_03_MB_IRQHandler    /* Vector # 640 FlexCAN6_2 FlexCAN6_BUF_00_03 */
    .long   CAN6_ORed_04_07_MB_IRQHandler    /* Vector # 641 FlexCAN6_3 FlexCAN6_BUF_04_07 */
    .long   CAN6_ORed_08_11_MB_IRQHandler    /* Vector # 642 FlexCAN6_4 FlexCAN6_BUF_08_11 */
    .long   CAN6_ORed_12_15_MB_IRQHandler    /* Vector # 643 FlexCAN6_5 FlexCAN6_BUF_12_15 */
    .long   CAN6_ORed_16_31_MB_IRQHandler    /* Vector # 644 FlexCAN6_6 FlexCAN6_BUF_16_31 */
    .long   CAN6_ORed_32_63_MB_IRQHandler    /* Vector # 645 FlexCAN6_7 FlexCAN6_BUF_32_63 */
    .long   CAN6_ORed_64_95_MB_IRQHandler    /* Vector # 646 FlexCAN6_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 647 */
    .long   DefaultISR                       /* Vector # 648 */
    .long   DefaultISR                       /* Vector # 649 */
    .long   CAN7_Error_IRQHandler            /* Vector # 650 FlexCAN7_0 FLEXCAN_7_ESR[ERR_INT] */
    .long   CAN7_ORed_IRQHandler             /* Vector # 651 FlexCAN7_1 FLEXCAN_7_ESR_BOFF | FLEXCAN_7_Transmit_Warning | FLEXCAN_7_Receive_Warning */
    .long   CAN7_ORed_00_03_MB_IRQHandler    /* Vector # 652 FlexCAN7_2 FlexCAN7_BUF_00_03 */
    .long   CAN7_ORed_04_07_MB_IRQHandler    /* Vector # 653 FlexCAN7_3 FlexCAN7_BUF_04_07 */
    .long   CAN7_ORed_08_11_MB_IRQHandler    /* Vector # 654 FlexCAN7_4 FlexCAN7_BUF_08_11 */
    .long   CAN7_ORed_12_15_MB_IRQHandler    /* Vector # 655 FlexCAN7_5 FlexCAN7_BUF_12_15 */
    .long   CAN7_ORed_16_31_MB_IRQHandler    /* Vector # 656 FlexCAN7_6 FlexCAN7_BUF_16_31 */
    .long   CAN7_ORed_32_63_MB_IRQHandler    /* Vector # 657 FlexCAN7_7 FlexCAN7_BUF_32_63 */
    .long   CAN7_ORed_64_95_MB_IRQHandler    /* Vector # 658 FlexCAN7_8 FlexCAN0_BUF_64_95 */
    .long   DefaultISR                       /* Vector # 659 */
    .long   DefaultISR                       /* Vector # 660 */
    .long   DefaultISR                       /* Vector # 661 */
    .long   DefaultISR                       /* Vector # 662 */
    .long   DefaultISR                       /* Vector # 663 */
    .long   DefaultISR                       /* Vector # 664 */
    .long   DefaultISR                       /* Vector # 665 */
    .long   DefaultISR                       /* Vector # 666 */
    .long   DefaultISR                       /* Vector # 667 */
    .long   WKPU_07_00_IRQHandler            /* Vector # 668 WKPU_IRQ_0 (0-7) */
    .long   WKPU_15_08_IRQHandler            /* Vector # 669 WKPU_IRQ_1 (8-15) */
    .long   WKPU_23_16_IRQHandler            /* Vector # 670 WKPU_IRQ_2 (16-23) */
    .long   WKPU_31_24_IRQHandler            /* Vector # 671 WKPU_IRQ_3 (24-31) */
    .long   DefaultISR                       /* Vector # 672 */
    .long   DefaultISR                       /* Vector # 673 */
    .long   DefaultISR                       /* Vector # 674 */
    .long   DefaultISR                       /* Vector # 675 */
    .long   DefaultISR                       /* Vector # 676 */
    .long   DefaultISR                       /* Vector # 677 */
    .long   DefaultISR                       /* Vector # 678 */
    .long   DefaultISR                       /* Vector # 679 */
    .long   uSDHC_DriverIRQHandler           /* Vector # 680 SDIO (uSDHC) 0 uSDHC0 */
    .long   DefaultISR                       /* Vector # 681 */
    .long   USBOTG_IRQHandler                /* Vector # 682 USB_OTG USB OTG */
    .long   USBHS_IRQHandler                 /* Vector # 683 USB_SPH USB SPH */
    .long   DefaultISR                       /* Vector # 684 */
    .long   DefaultISR                       /* Vector # 685 */
    .long   DefaultISR                       /* Vector # 686 MLB MLB System Interrupt */
    .long   DefaultISR                       /* Vector # 687 MLB MLB Channel Interrupts 0-15 Combined */
    .long   DefaultISR                       /* Vector # 688 */
    .long   DefaultISR                       /* Vector # 689 */
    .long   SAI0_Tx_IRQHandler               /* Vector # 690 SAI0_TX_FIFO_IRQ SAI0_TX_FIFO_IRQ */
    .long   SAI0_Rx_IRQHandler               /* Vector # 691 SAI0_RX_FIFO_IRQ SAI0_RX_FIFO_IRQ */
    .long   SAI1_Tx_IRQHandler               /* Vector # 692 SAI1_TX_FIFO_IRQ SAI1_TX_FIFO_IRQ */
    .long   SAI1_Rx_IRQHandler               /* Vector # 693 SAI1_RX_FIFO_IRQ SAI1_RX_FIFO_IRQ */
    .long   SAI2_Tx_IRQHandler               /* Vector # 694 SAI2_TX_FIFO_IRQ SAI2_TX_FIFO_IRQ */
    .long   SAI2_Rx_IRQHandler               /* Vector # 695 SAI2_RX_FIFO_IRQ SAI2_RX_FIFO_IRQ */
    .long   DefaultISR                       /* Vector # 696 */
    .long   DefaultISR                       /* Vector # 697 */
    .long   DefaultISR                       /* Vector # 698 */
    .long   DefaultISR                       /* Vector # 699 */
    .long   DefaultISR                       /* Vector # 700 */
    .long   DefaultISR                       /* Vector # 701 JDC JDC */
    .long   DefaultISR                       /* Vector # 702 MEMU_1 Correctable errors in LPU mode */
    .long   DefaultISR                       /* Vector # 703 */
    .long   DefaultISR                       /* Vector # 704 */
    .long   DefaultISR                       /* Vector # 705 */
    .long   EMIOS0_00_01_IRQHandler          /* Vector # 706 eMIOS channel 0,1 EMIOS_GFR[F0,F1] */
    .long   EMIOS0_02_03_IRQHandler          /* Vector # 707 eMIOS channel 2,3 EMIOS_GFR[F2,F3] */
    .long   EMIOS0_04_05_IRQHandler          /* Vector # 708 eMIOS channel 4,5 EMIOS_GFR[F4,F5] */
    .long   EMIOS0_06_07_IRQHandler          /* Vector # 709 eMIOS channel 6,7 EMIOS_GFR[F6,F7] */
    .long   EMIOS0_08_09_IRQHandler          /* Vector # 710 eMIOS channel 8,9 EMIOS_GFR[F8,F9] */
    .long   EMIOS0_10_11_IRQHandler          /* Vector # 711 eMIOS channel 10,11 EMIOS_GFR[F10,F11] */
    .long   EMIOS0_12_13_IRQHandler          /* Vector # 712 eMIOS channel 12,13 EMIOS_GFR[F12,F13] */
    .long   EMIOS0_14_15_IRQHandler          /* Vector # 713 eMIOS channel 14,15 EMIOS_GFR[F14,F15] */
    .long   EMIOS0_16_17_IRQHandler          /* Vector # 714 eMIOS channel 16,17 EMIOS_GFR[F16,F17] */
    .long   EMIOS0_18_19_IRQHandler          /* Vector # 715 eMIOS channel 18,19 EMIOS_GFR[F18,F19] */
    .long   EMIOS0_20_21_IRQHandler          /* Vector # 716 eMIOS channel 20,21 EMIOS_GFR[F20,F21] */
    .long   EMIOS0_22_23_IRQHandler          /* Vector # 717 eMIOS channel 22,23 EMIOS_GFR[F22,F23] */
    .long   EMIOS0_24_25_IRQHandler          /* Vector # 718 eMIOS channel 24,25 EMIOS_GFR[F24,F25] */
    .long   EMIOS0_26_27_IRQHandler          /* Vector # 719 eMIOS channel 26,27 EMIOS_GFR[F26,F27] */
    .long   EMIOS0_28_29_IRQHandler          /* Vector # 720 eMIOS channel 28,29 EMIOS_GFR[F28,F29] */
    .long   EMIOS0_30_31_IRQHandler          /* Vector # 721 eMIOS channel 30,31 EMIOS_GFR[F30,F31] */
    .long   EMIOS1_00_01_IRQHandler          /* Vector # 722 eMIOS channel 32,33 EMIOS_GFR[F32,F33] */
    .long   EMIOS1_02_03_IRQHandler          /* Vector # 723 eMIOS channel 34,35 EMIOS_GFR[F34,F35] */
    .long   EMIOS1_04_05_IRQHandler          /* Vector # 724 eMIOS channel 36,37 EMIOS_GFR[F36,F37] */
    .long   EMIOS1_06_07_IRQHandler          /* Vector # 725 eMIOS channel 38,39 EMIOS_GFR[F38,F39] */
    .long   EMIOS1_08_09_IRQHandler          /* Vector # 726 eMIOS channel 40,41 EMIOS_GFR[F40,F41] */
    .long   EMIOS1_10_11_IRQHandler          /* Vector # 727 eMIOS channel 42,43 EMIOS_GFR[F42,F43] */
    .long   EMIOS1_12_13_IRQHandler          /* Vector # 728 eMIOS channel 44,45 EMIOS_GFR[F44,F45] */
    .long   EMIOS1_14_15_IRQHandler          /* Vector # 729 eMIOS channel 46,47 EMIOS_GFR[F46,F47] */
    .long   EMIOS1_16_17_IRQHandler          /* Vector # 730 eMIOS channel 48,49 EMIOS_GFR[F48,F49] */
    .long   EMIOS1_18_19_IRQHandler          /* Vector # 731 eMIOS channel 50,51 EMIOS_GFR[F50,F51] */
    .long   EMIOS1_20_21_IRQHandler          /* Vector # 732 eMIOS channel 52,53 EMIOS_GFR[F52,F53] */
    .long   EMIOS1_22_23_IRQHandler          /* Vector # 733 eMIOS channel 54,55 EMIOS_GFR[F54,F55] */
    .long   EMIOS1_24_25_IRQHandler          /* Vector # 734 eMIOS channel 56,57 EMIOS_GFR[F56,F57] */
    .long   EMIOS1_26_27_IRQHandler          /* Vector # 735 eMIOS channel 58,59 EMIOS_GFR[F58,F59] */
    .long   EMIOS1_28_29_IRQHandler          /* Vector # 736 eMIOS channel 60,61 EMIOS_GFR[F60,F61] */
    .long   EMIOS1_30_31_IRQHandler          /* Vector # 737 eMIOS channel 62,63 EMIOS_GFR[F62,F63] */
    .long   EMIOS2_00_01_IRQHandler          /* Vector # 738 eMIOS channel 64,65 EMIOS_GFR[F64,F65] */
    .long   EMIOS2_02_03_IRQHandler          /* Vector # 739 eMIOS channel 66,67 EMIOS_GFR[F66,F67] */
    .long   EMIOS2_04_05_IRQHandler          /* Vector # 740 eMIOS channel 68,69 EMIOS_GFR[F68,F69] */
    .long   EMIOS2_06_07_IRQHandler          /* Vector # 741 eMIOS channel 70,71 EMIOS_GFR[F70,F71] */
    .long   EMIOS2_08_09_IRQHandler          /* Vector # 742 eMIOS channel 72,73 EMIOS_GFR[F72,F73] */
    .long   EMIOS2_10_11_IRQHandler          /* Vector # 743 eMIOS channel 74,75 EMIOS_GFR[F74,F75] */
    .long   EMIOS2_12_13_IRQHandler          /* Vector # 744 eMIOS channel 76,77 EMIOS_GFR[F76,F77] */
    .long   EMIOS2_14_15_IRQHandler          /* Vector # 745 eMIOS channel 78,79 EMIOS_GFR[F78,F79] */
    .long   EMIOS2_16_17_IRQHandler          /* Vector # 746 eMIOS channel 80,81 EMIOS_GFR[F80,F81] */
    .long   EMIOS2_18_19_IRQHandler          /* Vector # 747 eMIOS channel 82,83 EMIOS_GFR[F82,F83] */
    .long   EMIOS2_20_21_IRQHandler          /* Vector # 748 eMIOS channel 84,85 EMIOS_GFR[F84,F85] */
    .long   EMIOS2_22_23_IRQHandler          /* Vector # 749 eMIOS channel 86,87 EMIOS_GFR[F86,F87] */
    .long   EMIOS2_24_25_IRQHandler          /* Vector # 750 eMIOS channel 88,89 EMIOS_GFR[F88,F89] */
    .long   EMIOS2_26_27_IRQHandler          /* Vector # 751 eMIOS channel 90,91 EMIOS_GFR[F90,F91] */
    .long   EMIOS2_28_29_IRQHandler          /* Vector # 752 eMIOS channel 92,93 EMIOS_GFR[F92,F93] */
    .long   EMIOS2_30_31_IRQHandler          /* Vector # 753 eMIOS channel 94,95 EMIOS_GFR[F94,F95] */

    .align  2
    .weak IVOR0_Handler
    .type IVOR0_Handler, %function
IVOR0_Handler:
    e_b       IVOR0_Handler
    .size IVOR0_Handler, . - IVOR0_Handler

    .align  2
    .weak IVOR1_Handler
    .type IVOR1_Handler, %function
IVOR1_Handler:
    e_b       IVOR1_Handler
    .size IVOR1_Handler, . - IVOR1_Handler

    .align  2
    .weak IVOR2_Handler
    .type IVOR2_Handler, %function
IVOR2_Handler:
    e_b       IVOR2_Handler
    .size IVOR2_Handler, . - IVOR2_Handler

    .align  2
    .weak IVOR3_Handler
    .type IVOR3_Handler, %function
IVOR3_Handler:
    e_b       IVOR3_Handler
    .size IVOR3_Handler, . - IVOR3_Handler

    .align  2
    .weak IVOR4_Handler
    .type IVOR4_Handler, %function
IVOR4_Handler:
    e_b       IVOR4_Handler
    .size IVOR4_Handler, . - IVOR4_Handler

    .align  2
    .weak IVOR5_Handler
    .type IVOR5_Handler, %function
IVOR5_Handler:
    e_b       IVOR5_Handler
    .size IVOR5_Handler, . - IVOR5_Handler

    .align  2
    .weak IVOR6_Handler
    .type IVOR6_Handler, %function
IVOR6_Handler:
    e_b       IVOR6_Handler
    .size IVOR6_Handler, . - IVOR6_Handler

    .align  2
    .weak IVOR7_Handler
    .type IVOR7_Handler, %function
IVOR7_Handler:
    e_b       IVOR7_Handler
    .size IVOR7_Handler, . - IVOR7_Handler

    .align  2
    .weak IVOR8_Handler
    .type IVOR8_Handler, %function
IVOR8_Handler:
    e_b       IVOR8_Handler
    .size IVOR8_Handler, . - IVOR8_Handler

    .align  2
    .weak IVOR9_Handler
    .type IVOR9_Handler, %function
IVOR9_Handler:
    e_b       IVOR9_Handler
    .size IVOR9_Handler, . - IVOR9_Handler

    .align  2
    .weak IVOR10_Handler
    .type IVOR10_Handler, %function
IVOR10_Handler:
    e_b       IVOR10_Handler
    .size IVOR10_Handler, . - IVOR10_Handler

    .align  2
    .weak IVOR11_Handler
    .type IVOR11_Handler, %function
IVOR11_Handler:
    e_b       IVOR11_Handler
    .size IVOR11_Handler, . - IVOR11_Handler

    .align  2
    .weak IVOR12_Handler
    .type IVOR12_Handler, %function
IVOR12_Handler:
    e_b       IVOR12_Handler
    .size IVOR12_Handler, . - IVOR12_Handler

    .align  2
    .weak IVOR13_Handler
    .type IVOR13_Handler, %function
IVOR13_Handler:
    e_b       IVOR13_Handler
    .size IVOR13_Handler, . - IVOR13_Handler

    .align  2
    .weak IVOR14_Handler
    .type IVOR14_Handler, %function
IVOR14_Handler:
    e_b       IVOR14_Handler
    .size IVOR14_Handler, . - IVOR14_Handler

    .align  2
    .weak IVOR15_Handler
    .type IVOR15_Handler, %function
IVOR15_Handler:
    e_b       IVOR15_Handler
    .size IVOR15_Handler, . - IVOR15_Handler

    .align  2
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    e_b       DefaultISR
    .size DefaultISR, . - DefaultISR

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* NMI Exception Handlers */
    def_irq_handler    IVOR1_Exception_Handler

/* System call Handler */
    def_irq_handler    IVOR8_Exception_Handler

/* Exception Handlers */
    def_irq_handler    DMA_Error_IRQHandler
    def_irq_handler    DMA0_IRQHandler
    def_irq_handler    DMA1_IRQHandler
    def_irq_handler    DMA2_IRQHandler
    def_irq_handler    DMA3_IRQHandler
    def_irq_handler    DMA4_IRQHandler
    def_irq_handler    DMA5_IRQHandler
    def_irq_handler    DMA6_IRQHandler
    def_irq_handler    DMA7_IRQHandler
    def_irq_handler    DMA8_IRQHandler
    def_irq_handler    DMA9_IRQHandler
    def_irq_handler    DMA10_IRQHandler
    def_irq_handler    DMA11_IRQHandler
    def_irq_handler    DMA12_IRQHandler
    def_irq_handler    DMA13_IRQHandler
    def_irq_handler    DMA14_IRQHandler
    def_irq_handler    DMA15_IRQHandler
    def_irq_handler    DMA16_IRQHandler
    def_irq_handler    DMA17_IRQHandler
    def_irq_handler    DMA18_IRQHandler
    def_irq_handler    DMA19_IRQHandler
    def_irq_handler    DMA20_IRQHandler
    def_irq_handler    DMA21_IRQHandler
    def_irq_handler    DMA22_IRQHandler
    def_irq_handler    DMA23_IRQHandler
    def_irq_handler    DMA24_IRQHandler
    def_irq_handler    DMA25_IRQHandler
    def_irq_handler    DMA26_IRQHandler
    def_irq_handler    DMA27_IRQHandler
    def_irq_handler    DMA28_IRQHandler
    def_irq_handler    DMA29_IRQHandler
    def_irq_handler    DMA30_IRQHandler
    def_irq_handler    DMA31_IRQHandler
    def_irq_handler    FLASH_Done_IRQHandler
    def_irq_handler    ENET1_Err_IRQHandler
    def_irq_handler    ENET1_Rx_IRQHandler
    def_irq_handler    ENET1_Tx_IRQHandler
    def_irq_handler    ENET1_Parser_IRQHandler
    def_irq_handler    ENET1_Timer_IRQHandler
    def_irq_handler    ENET1_Rx_1_IRQHandler
    def_irq_handler    ENET1_Tx_1_IRQHandler
    def_irq_handler    ENET1_Rx_2_IRQHandler
    def_irq_handler    ENET1_Tx_2_IRQHandler
    def_irq_handler    ENET0_Err_IRQHandler
    def_irq_handler    ENET0_Rx_IRQHandler
    def_irq_handler    ENET0_Tx_IRQHandler
    def_irq_handler    ENET0_Parser_IRQHandler
    def_irq_handler    ENET0_Timer_IRQHandler
    def_irq_handler    ENET0_Rx_1_IRQHandler
    def_irq_handler    ENET0_Tx_1_IRQHandler
    def_irq_handler    ENET0_Rx_2_IRQHandler
    def_irq_handler    ENET0_Tx_2_IRQHandler
    def_irq_handler    RTC0_IRQHandler
    def_irq_handler    API0_IRQHandler
    def_irq_handler    I2C_DRV_Module0IRQHandler
    def_irq_handler    I2C_DRV_Module1IRQHandler
    def_irq_handler    I2C_DRV_Module2IRQHandler
    def_irq_handler    I2C_DRV_Module3IRQHandler
    def_irq_handler    CAN0_Wake_Up_IRQHandler
    def_irq_handler    CAN0_Error_IRQHandler
    def_irq_handler    CAN0_ORed_IRQHandler
    def_irq_handler    CAN0_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN0_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN0_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN0_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN0_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN0_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN0_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN1_Error_IRQHandler
    def_irq_handler    CAN1_ORed_IRQHandler
    def_irq_handler    CAN1_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN1_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN1_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN1_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN1_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN1_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN1_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN2_Error_IRQHandler
    def_irq_handler    CAN2_ORed_IRQHandler
    def_irq_handler    CAN2_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN2_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN2_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN2_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN2_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN2_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN2_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN3_Error_IRQHandler
    def_irq_handler    CAN3_ORed_IRQHandler
    def_irq_handler    CAN3_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN3_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN3_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN3_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN3_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN3_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN3_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN4_Error_IRQHandler
    def_irq_handler    CAN4_ORed_IRQHandler
    def_irq_handler    CAN4_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN4_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN4_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN4_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN4_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN4_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN4_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN5_Error_IRQHandler
    def_irq_handler    CAN5_ORed_IRQHandler
    def_irq_handler    CAN5_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN5_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN5_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN5_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN5_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN5_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN5_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN6_Error_IRQHandler
    def_irq_handler    CAN6_ORed_IRQHandler
    def_irq_handler    CAN6_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN6_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN6_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN6_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN6_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN6_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN6_ORed_64_95_MB_IRQHandler
    def_irq_handler    CAN7_Error_IRQHandler
    def_irq_handler    CAN7_ORed_IRQHandler
    def_irq_handler    CAN7_ORed_00_03_MB_IRQHandler
    def_irq_handler    CAN7_ORed_04_07_MB_IRQHandler
    def_irq_handler    CAN7_ORed_08_11_MB_IRQHandler
    def_irq_handler    CAN7_ORed_12_15_MB_IRQHandler
    def_irq_handler    CAN7_ORed_16_31_MB_IRQHandler
    def_irq_handler    CAN7_ORed_32_63_MB_IRQHandler
    def_irq_handler    CAN7_ORed_64_95_MB_IRQHandler
    def_irq_handler    SWT0_IRQHandler
    def_irq_handler    SWT1_IRQHandler
    def_irq_handler    SWT2_IRQHandler
    def_irq_handler    WKPU_07_00_IRQHandler
    def_irq_handler    WKPU_15_08_IRQHandler
    def_irq_handler    WKPU_23_16_IRQHandler
    def_irq_handler    WKPU_31_24_IRQHandler
    def_irq_handler    STM0_Ch0_IRQHandler
    def_irq_handler    STM0_Ch1_IRQHandler
    def_irq_handler    STM0_Ch2_IRQHandler
    def_irq_handler    STM0_Ch3_IRQHandler
    def_irq_handler    STM1_Ch0_IRQHandler
    def_irq_handler    STM1_Ch1_IRQHandler
    def_irq_handler    STM1_Ch2_IRQHandler
    def_irq_handler    STM1_Ch3_IRQHandler
    def_irq_handler    STM2_Ch0_IRQHandler
    def_irq_handler    STM2_Ch1_IRQHandler
    def_irq_handler    STM2_Ch2_IRQHandler
    def_irq_handler    STM2_Ch3_IRQHandler
    def_irq_handler    PIT_Ch0_IRQHandler
    def_irq_handler    PIT_Ch1_IRQHandler
    def_irq_handler    PIT_Ch2_IRQHandler
    def_irq_handler    PIT_Ch3_IRQHandler
    def_irq_handler    PIT_Ch4_IRQHandler
    def_irq_handler    PIT_Ch5_IRQHandler
    def_irq_handler    PIT_Ch6_IRQHandler
    def_irq_handler    PIT_Ch7_IRQHandler
    def_irq_handler    PIT_Ch8_IRQHandler
    def_irq_handler    PIT_Ch9_IRQHandler
    def_irq_handler    PIT_Ch10_IRQHandler
    def_irq_handler    PIT_Ch11_IRQHandler
    def_irq_handler    PIT_Ch12_IRQHandler
    def_irq_handler    PIT_Ch13_IRQHandler
    def_irq_handler    PIT_Ch14_IRQHandler
    def_irq_handler    PIT_Ch15_IRQHandler
    def_irq_handler    PIT_RTI_IRQHandler
    def_irq_handler    BCTU_ListLast_IRQHandler
    def_irq_handler    BCTU_ConvUpdate_IRQHandler
    def_irq_handler    LINFLEXD0_UART_RxIRQHandler
    def_irq_handler    LINFLEXD0_UART_TxIRQHandler
    def_irq_handler    LINFLEXD0_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD1_UART_RxIRQHandler
    def_irq_handler    LINFLEXD1_UART_TxIRQHandler
    def_irq_handler    LINFLEXD1_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD2_UART_RxIRQHandler
    def_irq_handler    LINFLEXD2_UART_TxIRQHandler
    def_irq_handler    LINFLEXD2_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD3_UART_RxIRQHandler
    def_irq_handler    LINFLEXD3_UART_TxIRQHandler
    def_irq_handler    LINFLEXD3_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD4_UART_RxIRQHandler
    def_irq_handler    LINFLEXD4_UART_TxIRQHandler
    def_irq_handler    LINFLEXD4_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD5_UART_RxIRQHandler
    def_irq_handler    LINFLEXD5_UART_TxIRQHandler
    def_irq_handler    LINFLEXD5_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD6_UART_RxIRQHandler
    def_irq_handler    LINFLEXD6_UART_TxIRQHandler
    def_irq_handler    LINFLEXD6_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD7_UART_RxIRQHandler
    def_irq_handler    LINFLEXD7_UART_TxIRQHandler
    def_irq_handler    LINFLEXD7_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD8_UART_RxIRQHandler
    def_irq_handler    LINFLEXD8_UART_TxIRQHandler
    def_irq_handler    LINFLEXD8_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD9_UART_RxIRQHandler
    def_irq_handler    LINFLEXD9_UART_TxIRQHandler
    def_irq_handler    LINFLEXD9_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD10_UART_RxIRQHandler
    def_irq_handler    LINFLEXD10_UART_TxIRQHandler
    def_irq_handler    LINFLEXD10_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD11_UART_RxIRQHandler
    def_irq_handler    LINFLEXD11_UART_TxIRQHandler
    def_irq_handler    LINFLEXD11_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD12_UART_RxIRQHandler
    def_irq_handler    LINFLEXD12_UART_TxIRQHandler
    def_irq_handler    LINFLEXD12_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD13_UART_RxIRQHandler
    def_irq_handler    LINFLEXD13_UART_TxIRQHandler
    def_irq_handler    LINFLEXD13_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD14_UART_RxIRQHandler
    def_irq_handler    LINFLEXD14_UART_TxIRQHandler
    def_irq_handler    LINFLEXD14_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD15_UART_RxIRQHandler
    def_irq_handler    LINFLEXD15_UART_TxIRQHandler
    def_irq_handler    LINFLEXD15_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD16_UART_RxIRQHandler
    def_irq_handler    LINFLEXD16_UART_TxIRQHandler
    def_irq_handler    LINFLEXD16_UART_ErrIRQHandler
    def_irq_handler    LINFLEXD17_UART_RxIRQHandler
    def_irq_handler    LINFLEXD17_UART_TxIRQHandler
    def_irq_handler    LINFLEXD17_UART_ErrIRQHandler
    def_irq_handler    DSPI0_FIFO_Error_IRQHandler
    def_irq_handler    DSPI0_Send_IRQHandler
    def_irq_handler    DSPI0_Receive_IRQHandler
    def_irq_handler    DSPI1_FIFO_Error_IRQHandler
    def_irq_handler    DSPI1_Send_IRQHandler
    def_irq_handler    DSPI1_Receive_IRQHandler
    def_irq_handler    DSPI2_FIFO_Error_IRQHandler
    def_irq_handler    DSPI2_Send_IRQHandler
    def_irq_handler    DSPI2_Receive_IRQHandler
    def_irq_handler    DSPI3_FIFO_Error_IRQHandler
    def_irq_handler    DSPI3_Send_IRQHandler
    def_irq_handler    DSPI3_Receive_IRQHandler
    def_irq_handler    DSPI4_FIFO_Error_IRQHandler
    def_irq_handler    DSPI4_Send_IRQHandler
    def_irq_handler    DSPI4_Receive_IRQHandler
    def_irq_handler    DSPI5_FIFO_Error_IRQHandler
    def_irq_handler    DSPI5_Send_IRQHandler
    def_irq_handler    DSPI5_Receive_IRQHandler
    def_irq_handler    DSPI6_FIFO_Error_IRQHandler
    def_irq_handler    DSPI6_Send_IRQHandler
    def_irq_handler    DSPI6_Receive_IRQHandler
    def_irq_handler    DSPI7_FIFO_Error_IRQHandler
    def_irq_handler    DSPI7_Send_IRQHandler
    def_irq_handler    DSPI7_Receive_IRQHandler
    def_irq_handler    DSPI8_FIFO_Error_IRQHandler
    def_irq_handler    DSPI8_Send_IRQHandler
    def_irq_handler    DSPI8_Receive_IRQHandler
    def_irq_handler    DSPI9_FIFO_Error_IRQHandler
    def_irq_handler    DSPI9_Send_IRQHandler
    def_irq_handler    DSPI9_Receive_IRQHandler
    def_irq_handler    ADC0_EOC_IRQHandler
    def_irq_handler    ADC0_ER_IRQHandler
    def_irq_handler    ADC0_WD_IRQHandler
    def_irq_handler    ADC1_EOC_IRQHandler
    def_irq_handler    ADC1_ER_IRQHandler
    def_irq_handler    ADC1_WD_IRQHandler
    def_irq_handler    uSDHC_DriverIRQHandler
    def_irq_handler    FCCU0_IRQ_Handler
    def_irq_handler    SAI0_Tx_IRQHandler
    def_irq_handler    SAI0_Rx_IRQHandler
    def_irq_handler    SAI1_Tx_IRQHandler
    def_irq_handler    SAI1_Rx_IRQHandler
    def_irq_handler    SAI2_Tx_IRQHandler
    def_irq_handler    SAI2_Rx_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    CMP2_IRQHandler
    def_irq_handler    HSM_DRV_IRQHandler
    def_irq_handler    SIUL_EIRQ_00_07_IRQHandler
    def_irq_handler    SIUL_EIRQ_08_15_IRQHandler
    def_irq_handler    SIUL_EIRQ_16_23_IRQHandler
    def_irq_handler    SIUL_EIRQ_24_31_IRQHandler
    def_irq_handler    EMIOS0_00_01_IRQHandler
    def_irq_handler    EMIOS0_02_03_IRQHandler
    def_irq_handler    EMIOS0_04_05_IRQHandler
    def_irq_handler    EMIOS0_06_07_IRQHandler
    def_irq_handler    EMIOS0_08_09_IRQHandler
    def_irq_handler    EMIOS0_10_11_IRQHandler
    def_irq_handler    EMIOS0_12_13_IRQHandler
    def_irq_handler    EMIOS0_14_15_IRQHandler
    def_irq_handler    EMIOS0_16_17_IRQHandler
    def_irq_handler    EMIOS0_18_19_IRQHandler
    def_irq_handler    EMIOS0_20_21_IRQHandler
    def_irq_handler    EMIOS0_22_23_IRQHandler
    def_irq_handler    EMIOS0_24_25_IRQHandler
    def_irq_handler    EMIOS0_26_27_IRQHandler
    def_irq_handler    EMIOS0_28_29_IRQHandler
    def_irq_handler    EMIOS0_30_31_IRQHandler
    def_irq_handler    EMIOS1_00_01_IRQHandler
    def_irq_handler    EMIOS1_02_03_IRQHandler
    def_irq_handler    EMIOS1_04_05_IRQHandler
    def_irq_handler    EMIOS1_06_07_IRQHandler
    def_irq_handler    EMIOS1_08_09_IRQHandler
    def_irq_handler    EMIOS1_10_11_IRQHandler
    def_irq_handler    EMIOS1_12_13_IRQHandler
    def_irq_handler    EMIOS1_14_15_IRQHandler
    def_irq_handler    EMIOS1_16_17_IRQHandler
    def_irq_handler    EMIOS1_18_19_IRQHandler
    def_irq_handler    EMIOS1_20_21_IRQHandler
    def_irq_handler    EMIOS1_22_23_IRQHandler
    def_irq_handler    EMIOS1_24_25_IRQHandler
    def_irq_handler    EMIOS1_26_27_IRQHandler
    def_irq_handler    EMIOS1_28_29_IRQHandler
    def_irq_handler    EMIOS1_30_31_IRQHandler
    def_irq_handler    EMIOS2_00_01_IRQHandler
    def_irq_handler    EMIOS2_02_03_IRQHandler
    def_irq_handler    EMIOS2_04_05_IRQHandler
    def_irq_handler    EMIOS2_06_07_IRQHandler
    def_irq_handler    EMIOS2_08_09_IRQHandler
    def_irq_handler    EMIOS2_10_11_IRQHandler
    def_irq_handler    EMIOS2_12_13_IRQHandler
    def_irq_handler    EMIOS2_14_15_IRQHandler
    def_irq_handler    EMIOS2_16_17_IRQHandler
    def_irq_handler    EMIOS2_18_19_IRQHandler
    def_irq_handler    EMIOS2_20_21_IRQHandler
    def_irq_handler    EMIOS2_22_23_IRQHandler
    def_irq_handler    EMIOS2_24_25_IRQHandler
    def_irq_handler    EMIOS2_26_27_IRQHandler
    def_irq_handler    EMIOS2_28_29_IRQHandler
    def_irq_handler    EMIOS2_30_31_IRQHandler
    def_irq_handler    USBHS_IRQHandler
    def_irq_handler    USBOTG_IRQHandler
