/*****************************************************************************/
/* FILE NAME: intc_SW_mode_isr_vectors_MPC5748G.c COPYRIGHT(c) Freescale 2013*/
/*                                                      All Rights Reserved  */
/* DESCRIPTION: MPC5748G ISR vectors for INTC in software vector mode        */
/*              Based on MPC5748G ref manual rev 1 Table 20-1.               */
/* USAGE:  For desired vector #, replace "dummy" with ISR name and declare   */
/*         your isr name extern like the example below:                      */
/*                       extern void MyPeripheralISR(void);                  */
/*                                                                           */
/*****************************************************************************/	
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  S Mihalik     10 Sep 2013   Initial version                        */
/* 1.1    D Haro        12 Feb 2014   Added SW IRQ 1                         */
/*****************************************************************************/

#include "typedefs.h"
/*========================================================================*/
/*		PROTOTYPES						                                  */
/*========================================================================*/
void dummy 						  (void);

/*========================================================================*/
/*	GLOBAL VARIABLES						                              */
/*========================================================================*/

const uint32_t __attribute__ ((section (".intc_vector_table"))) IntcIsrVectorTable[] = {
    
(uint32_t) &dummy, /* Vector #   0 Software setable flag 0 SSCIR0[CLR0] */
(uint32_t) &dummy, /* Vector #   1 Software setable flag 1 SSCIR0[CLR1] */
(uint32_t) &dummy, /* Vector #   2 Software setable flag 2 SSCIR0[CLR2] */
(uint32_t) &dummy, /* Vector #   3 Software setable flag 3 SSCIR0[CLR3] */
(uint32_t) &dummy, /* Vector #   4 Software setable flag 4 SSCIR0[CLR4] */
(uint32_t) &dummy, /* Vector #   5 Software setable flag 5 SSCIR0[CLR5] */
(uint32_t) &dummy, /* Vector #   6 Software setable flag 6 SSCIR0[CLR6] */
(uint32_t) &dummy, /* Vector #   7 Software setable flag 7 SSCIR0[CLR7] */
(uint32_t) &dummy, /* Vector #   8 Software setable flag 8 SSCIR0[CLR8] */
(uint32_t) &dummy, /* Vector #   9 Software setable flag 9 SSCIR0[CLR9] */
(uint32_t) &dummy, /* Vector #  10 Software setable flag 10 SSCIR0[CLR10] */
(uint32_t) &dummy, /* Vector #  11 Software setable flag 11 SSCIR0[CLR11] */
(uint32_t) &dummy, /* Vector #  12 Software setable flag 12 SSCIR0[CLR12] */
(uint32_t) &dummy, /* Vector #  13 Software setable flag 13 SSCIR0[CLR13] */
(uint32_t) &dummy, /* Vector #  14 Software setable flag 14 SSCIR0[CLR14] */
(uint32_t) &dummy, /* Vector #  15 Software setable flag 15 SSCIR0[CLR15] */
(uint32_t) &dummy, /* Vector #  16 Software setable flag 16 SSCIR0[CLR16] */
(uint32_t) &dummy, /* Vector #  17 Software setable flag 17 SSCIR0[CLR17] */
(uint32_t) &dummy, /* Vector #  18 Software setable flag 18 SSCIR0[CLR18] */
(uint32_t) &dummy, /* Vector #  19 Software setable flag 19 SSCIR0[CLR19] */
(uint32_t) &dummy, /* Vector #  20 Software setable flag 20 SSCIR0[CLR20] */
(uint32_t) &dummy, /* Vector #  21 Software setable flag 21 SSCIR0[CLR21] */
(uint32_t) &dummy, /* Vector #  22 Software setable flag 22 SSCIR0[CLR22] */
(uint32_t) &dummy, /* Vector #  23 Software setable flag 23 SSCIR0[CLR23] */
(uint32_t) &dummy, /* Vector #  24 */
(uint32_t) &dummy, /* Vector #  25 */
(uint32_t) &dummy, /* Vector #  26 */
(uint32_t) &dummy, /* Vector #  27 */
(uint32_t) &dummy, /* Vector #  28 */
(uint32_t) &dummy, /* Vector #  29 */
(uint32_t) &dummy, /* Vector #  30 */
(uint32_t) &dummy, /* Vector #  31 */
(uint32_t) &dummy, /* Vector #  32 Platform watchdog timer0 SWT_0_IR[TIF] */
(uint32_t) &dummy, /* Vector #  33 Platform watchdog timer1 SWT_1_IR[TIF] */
(uint32_t) &dummy, /* Vector #  34 Platform watchdog timer2 SWT_2_IR[TIF] */
(uint32_t) &dummy, /* Vector #  35 */
(uint32_t) &dummy, /* Vector #  36 On-Platform periodic timer 0_0 (STM) STM_0_CIR0[CIF] */
(uint32_t) &dummy, /* Vector #  37 On-Platform periodic timer 0_1 (STM) STM_0_CIR1[CIF] */
(uint32_t) &dummy, /* Vector #  38 On-Platform periodic timer 0_2 (STM) STM_0_CIR2[CIF] */
(uint32_t) &dummy, /* Vector #  39 On-Platform periodic timer 0_3 (STM) STM_0_CIR3[CIF] */
(uint32_t) &dummy, /* Vector #  40 On-Platform periodic timer 1_0 (STM) STM_1_CIR0[CIF] */
(uint32_t) &dummy, /* Vector #  41 On-Platform periodic timer 1_1 (STM) STM_1_CIR1[CIF] */
(uint32_t) &dummy, /* Vector #  42 On-Platform periodic timer 1_2 (STM) STM_1_CIR2[CIF] */
(uint32_t) &dummy, /* Vector #  43 On-Platform periodic timer 1_3 (STM) STM_1_CIR3[CIF] */
(uint32_t) &dummy, /* Vector #  44 On-Platform periodic timer 2_0 (STM) STM_2_CIR0[CIF] */
(uint32_t) &dummy, /* Vector #  45 On-Platform periodic timer 2_1 (STM) STM_2_CIR1[CIF] */
(uint32_t) &dummy, /* Vector #  46 On-Platform periodic timer 2_2 (STM) STM_2_CIR2[CIF] */
(uint32_t) &dummy, /* Vector #  47 On-Platform periodic timer 2_3 (STM) STM_2_CIR3[CIF] */
(uint32_t) &dummy, /* Vector #  48 */
(uint32_t) &dummy, /* Vector #  49 */
(uint32_t) &dummy, /* Vector #  50 */
(uint32_t) &dummy, /* Vector #  51 */
(uint32_t) &dummy, /* Vector #  52 eDMA Combined Error eDMA Channel Error Flags 63-0 */
(uint32_t) &dummy, /* Vector #  53 eDMA Channel 0 DMA_INTL[INT0] */
(uint32_t) &dummy, /* Vector #  54 eDMA Channel 1 DMA_INTL[INT1] */
(uint32_t) &dummy, /* Vector #  55 eDMA Channel 2 DMA_INTL[INT2] */
(uint32_t) &dummy, /* Vector #  56 eDMA Channel 3 DMA_INTL[INT3] */
(uint32_t) &dummy, /* Vector #  57 eDMA Channel 4 DMA_INTL[INT4] */
(uint32_t) &dummy, /* Vector #  58 eDMA Channel 5 DMA_INTL[INT5] */
(uint32_t) &dummy, /* Vector #  59 eDMA Channel 6 DMA_INTL[INT6] */
(uint32_t) &dummy, /* Vector #  60 eDMA Channel 7 DMA_INTL[INT7] */
(uint32_t) &dummy, /* Vector #  61 eDMA Channel 8 DMA_INTL[INT8] */
(uint32_t) &dummy, /* Vector #  62 eDMA Channel 9 DMA_INTL[INT9] */
(uint32_t) &dummy, /* Vector #  63 eDMA Channel 10 DMA_INTL[INT10] */
(uint32_t) &dummy, /* Vector #  64 eDMA Channel 11 DMA_INTL[INT11] */
(uint32_t) &dummy, /* Vector #  65 eDMA Channel 12 DMA_INTL[INT12] */
(uint32_t) &dummy, /* Vector #  66 eDMA Channel 13 DMA_INTL[INT13] */
(uint32_t) &dummy, /* Vector #  67 eDMA Channel 14 DMA_INTL[INT14] */
(uint32_t) &dummy, /* Vector #  68 eDMA Channel 15 DMA_INTL[INT15] */
(uint32_t) &dummy, /* Vector #  69 eDMA Channel 16 DMA_INTL[INT16] */
(uint32_t) &dummy, /* Vector #  70 eDMA Channel 17 DMA_INTL[INT17] */
(uint32_t) &dummy, /* Vector #  71 eDMA Channel 18 DMA_INTL[INT18] */
(uint32_t) &dummy, /* Vector #  72 eDMA Channel 19 DMA_INTL[INT19] */
(uint32_t) &dummy, /* Vector #  73 eDMA Channel 20 DMA_INTL[INT20] */
(uint32_t) &dummy, /* Vector #  74 eDMA Channel 21 DMA_INTL[INT21] */
(uint32_t) &dummy, /* Vector #  75 eDMA Channel 22 DMA_INTL[INT22] */
(uint32_t) &dummy, /* Vector #  76 eDMA Channel 23 DMA_INTL[INT23] */
(uint32_t) &dummy, /* Vector #  77 eDMA Channel 24 DMA_INTL[INT24] */
(uint32_t) &dummy, /* Vector #  78 eDMA Channel 25 DMA_INTL[INT25] */
(uint32_t) &dummy, /* Vector #  79 eDMA Channel 26 DMA_INTL[INT26] */
(uint32_t) &dummy, /* Vector #  80 eDMA Channel 27 DMA_INTL[INT27] */
(uint32_t) &dummy, /* Vector #  81 eDMA Channel 28 DMA_INTL[INT28] */
(uint32_t) &dummy, /* Vector #  82 eDMA Channel 29 DMA_INTL[INT29] */
(uint32_t) &dummy, /* Vector #  83 eDMA Channel 30 DMA_INTL[INT30] */
(uint32_t) &dummy, /* Vector #  84 eDMA Channel 31 DMA_INTL[INT31] */
(uint32_t) &dummy, /* Vector #  85 */
(uint32_t) &dummy, /* Vector #  86 */
(uint32_t) &dummy, /* Vector #  87 */
(uint32_t) &dummy, /* Vector #  88 */
(uint32_t) &dummy, /* Vector #  89 */
(uint32_t) &dummy, /* Vector #  90 */
(uint32_t) &dummy, /* Vector #  91 */
(uint32_t) &dummy, /* Vector #  92 */
(uint32_t) &dummy, /* Vector #  93 */
(uint32_t) &dummy, /* Vector #  94 */
(uint32_t) &dummy, /* Vector #  95 */
(uint32_t) &dummy, /* Vector #  96 */
(uint32_t) &dummy, /* Vector #  97 */
(uint32_t) &dummy, /* Vector #  98 */
(uint32_t) &dummy, /* Vector #  99 */
(uint32_t) &dummy, /* Vector # 100 */
(uint32_t) &dummy, /* Vector # 101 */
(uint32_t) &dummy, /* Vector # 102 */
(uint32_t) &dummy, /* Vector # 103 */
(uint32_t) &dummy, /* Vector # 104 */
(uint32_t) &dummy, /* Vector # 105 */
(uint32_t) &dummy, /* Vector # 106 */
(uint32_t) &dummy, /* Vector # 107 */
(uint32_t) &dummy, /* Vector # 108 */
(uint32_t) &dummy, /* Vector # 109 */
(uint32_t) &dummy, /* Vector # 110 */
(uint32_t) &dummy, /* Vector # 111 */
(uint32_t) &dummy, /* Vector # 112 */
(uint32_t) &dummy, /* Vector # 113 */
(uint32_t) &dummy, /* Vector # 114 */
(uint32_t) &dummy, /* Vector # 115 */
(uint32_t) &dummy, /* Vector # 116 */
(uint32_t) &dummy, /* Vector # 117 */
(uint32_t) &dummy, /* Vector # 118 */
(uint32_t) &dummy, /* Vector # 119 */
(uint32_t) &dummy, /* Vector # 120 */
(uint32_t) &dummy, /* Vector # 121 */
(uint32_t) &dummy, /* Vector # 122 */
(uint32_t) &dummy, /* Vector # 123 */
(uint32_t) &dummy, /* Vector # 124 */
(uint32_t) &dummy, /* Vector # 125 */
(uint32_t) &dummy, /* Vector # 126 */
(uint32_t) &dummy, /* Vector # 127 */
(uint32_t) &dummy, /* Vector # 128 */
(uint32_t) &dummy, /* Vector # 129 */
(uint32_t) &dummy, /* Vector # 130 */
(uint32_t) &dummy, /* Vector # 131 */
(uint32_t) &dummy, /* Vector # 132 */
(uint32_t) &dummy, /* Vector # 133 */
(uint32_t) &dummy, /* Vector # 134 */
(uint32_t) &dummy, /* Vector # 135 */
(uint32_t) &dummy, /* Vector # 136 */
(uint32_t) &dummy, /* Vector # 137 */
(uint32_t) &dummy, /* Vector # 138 */
(uint32_t) &dummy, /* Vector # 139 */
(uint32_t) &dummy, /* Vector # 140 */
(uint32_t) &dummy, /* Vector # 141 */
(uint32_t) &dummy, /* Vector # 142 */
(uint32_t) &dummy, /* Vector # 143 */
(uint32_t) &dummy, /* Vector # 144 */
(uint32_t) &dummy, /* Vector # 145 */
(uint32_t) &dummy, /* Vector # 146 */
(uint32_t) &dummy, /* Vector # 147 */
(uint32_t) &dummy, /* Vector # 148 */
(uint32_t) &dummy, /* Vector # 149 */
(uint32_t) &dummy, /* Vector # 150 */
(uint32_t) &dummy, /* Vector # 151 */
(uint32_t) &dummy, /* Vector # 152 */
(uint32_t) &dummy, /* Vector # 153 */
(uint32_t) &dummy, /* Vector # 154 */
(uint32_t) &dummy, /* Vector # 155 */
(uint32_t) &dummy, /* Vector # 156 */
(uint32_t) &dummy, /* Vector # 157 */
(uint32_t) &dummy, /* Vector # 158 */
(uint32_t) &dummy, /* Vector # 159 */
(uint32_t) &dummy, /* Vector # 160 */
(uint32_t) &dummy, /* Vector # 161 */
(uint32_t) &dummy, /* Vector # 162 */
(uint32_t) &dummy, /* Vector # 163 */
(uint32_t) &dummy, /* Vector # 164 */
(uint32_t) &dummy, /* Vector # 165 */
(uint32_t) &dummy, /* Vector # 166 */
(uint32_t) &dummy, /* Vector # 167 */
(uint32_t) &dummy, /* Vector # 168 */
(uint32_t) &dummy, /* Vector # 169 */
(uint32_t) &dummy, /* Vector # 170 */
(uint32_t) &dummy, /* Vector # 171 */
(uint32_t) &dummy, /* Vector # 172 */
(uint32_t) &dummy, /* Vector # 173 */
(uint32_t) &dummy, /* Vector # 174 */
(uint32_t) &dummy, /* Vector # 175 */
(uint32_t) &dummy, /* Vector # 176 */
(uint32_t) &dummy, /* Vector # 177 */
(uint32_t) &dummy, /* Vector # 178 */
(uint32_t) &dummy, /* Vector # 179 */
(uint32_t) &dummy, /* Vector # 180 */
(uint32_t) &dummy, /* Vector # 181 */
(uint32_t) &dummy, /* Vector # 182 */
(uint32_t) &dummy, /* Vector # 183 */
(uint32_t) &dummy, /* Vector # 184 */
(uint32_t) &dummy, /* Vector # 185 Flash controller Prog/Erase/Suspend IRQ_0  MCR[DONE] */
(uint32_t) &dummy, /* Vector # 186 */
(uint32_t) &dummy, /* Vector # 187 */
(uint32_t) &dummy, /* Vector # 188 */
(uint32_t) &dummy, /* Vector # 189 */
(uint32_t) &dummy, /* Vector # 190 */
(uint32_t) &dummy, /* Vector # 191 */
(uint32_t) &dummy, /* Vector # 192 */
(uint32_t) &dummy, /* Vector # 193 */
(uint32_t) &dummy, /* Vector # 194 */
(uint32_t) &dummy, /* Vector # 195 */
(uint32_t) &dummy, /* Vector # 196 */
(uint32_t) &dummy, /* Vector # 197 */
(uint32_t) &dummy, /* Vector # 198 */
(uint32_t) &dummy, /* Vector # 199 */
(uint32_t) &dummy, /* Vector # 200 */
(uint32_t) &dummy, /* Vector # 201 */
(uint32_t) &dummy, /* Vector # 202 */
(uint32_t) &dummy, /* Vector # 203 */
(uint32_t) &dummy, /* Vector # 204 */
(uint32_t) &dummy, /* Vector # 205 */
(uint32_t) &dummy, /* Vector # 206 */
(uint32_t) &dummy, /* Vector # 207 */
(uint32_t) &dummy, /* Vector # 208 */
(uint32_t) &dummy, /* Vector # 209 */
(uint32_t) &dummy, /* Vector # 210 ENET Interrupt Group0 ENET_0 */
(uint32_t) &dummy, /* Vector # 211 ENET Interrupt Group1 ENET_1 */
(uint32_t) &dummy, /* Vector # 212 ENET Interrupt Group2 ENET_2 */
(uint32_t) &dummy, /* Vector # 213 */
(uint32_t) &dummy, /* Vector # 214 ENET Interrupt Group4 ENET_4 */
(uint32_t) &dummy, /* Vector # 215 ENET Interrupt Group5 ENET_5 */
(uint32_t) &dummy, /* Vector # 216 ENET Interrupt Group6 ENET_6 */
(uint32_t) &dummy, /* Vector # 217 ENET Interrupt Group7 ENET_7 */
(uint32_t) &dummy, /* Vector # 218 ENET Interrupt Group8 ENET_8 */
(uint32_t) &dummy, /* Vector # 219 */
(uint32_t) &dummy, /* Vector # 220 */
(uint32_t) &dummy, /* Vector # 221 */
(uint32_t) &dummy, /* Vector # 222 */
(uint32_t) &dummy, /* Vector # 223 */
(uint32_t) &dummy, /* Vector # 224 Real Time Counter (RTC) RTC */
(uint32_t) &dummy, /* Vector # 225 Autonomous Periodic Interrupt (API) API */
(uint32_t) &dummy, /* Vector # 226 Periodic Interrupt Timer (PIT0) PIT_1_TFLG0[TIF] */
(uint32_t) &dummy, /* Vector # 227 Periodic Interrupt Timer (PIT1) PIT_1_TFLG1[TIF] */
(uint32_t) &dummy, /* Vector # 228 Periodic Interrupt Timer (PIT2) PIT_1_TFLG2[TIF] */
(uint32_t) &dummy, /* Vector # 229 Periodic Interrupt Timer (PIT3) PIT_1_TFLG3[TIF] */
(uint32_t) &dummy, /* Vector # 230 Periodic Interrupt Timer (PIT4) PIT_1_TFLG4[TIF] */
(uint32_t) &dummy, /* Vector # 231 Periodic Interrupt Timer (PIT5) PIT_1_TFLG5[TIF] */
(uint32_t) &dummy, /* Vector # 232 Periodic Interrupt Timer (PIT6) PIT_1_TFLG6[TIF] */
(uint32_t) &dummy, /* Vector # 233 Periodic Interrupt Timer (PIT7) PIT_1_TFLG7[TIF] */
(uint32_t) &dummy, /* Vector # 234 Periodic Interrupt Timer (PIT8) PIT_1_TFLG8[TIF] */
(uint32_t) &dummy, /* Vector # 235 Periodic Interrupt Timer (PIT9) PIT_1_TFLG9[TIF] */
(uint32_t) &dummy, /* Vector # 236 Periodic Interrupt Timer (PIT10) PIT_1_TFLG10[TIF] */
(uint32_t) &dummy, /* Vector # 237 Periodic Interrupt Timer (PIT11) PIT_1_TFLG11[TIF] */
(uint32_t) &dummy, /* Vector # 238 Periodic Interrupt Timer (PIT12) PIT_1_TFLG12[TIF] */
(uint32_t) &dummy, /* Vector # 239 Periodic Interrupt Timer (PIT13) PIT_1_TFLG13[TIF] */
(uint32_t) &dummy, /* Vector # 240 Periodic Interrupt Timer (PIT14) PIT_1_TFLG14[TIF] */
(uint32_t) &dummy, /* Vector # 241 Periodic Interrupt Timer (PIT15) PIT_1_TFLG15[TIF] */
(uint32_t) &dummy, /* Vector # 242 PIT_RTI PIT_RTI */
(uint32_t) &dummy, /* Vector # 243 SIUL EIRQ [0-7] - SIUL Combined External Interrupt */
(uint32_t) &dummy, /* Vector # 244 SIUL EIRQ [8-15] - SIUL Combined External Interrupt */
(uint32_t) &dummy, /* Vector # 245 SIUL EIRQ [16-23] - SIUL Combined External Interrupt */
(uint32_t) &dummy, /* Vector # 246 SIUL EIRQ [24-31] - SIUL Combined External Interrupt */
(uint32_t) &dummy, /* Vector # 247 */
(uint32_t) &dummy, /* Vector # 248 */
(uint32_t) &dummy, /* Vector # 249 */
(uint32_t) &dummy, /* Vector # 250 LPU_CTL o_interrupt */
(uint32_t) &dummy, /* Vector # 251 MC _ME ME_IS[I_SAFE] */
(uint32_t) &dummy, /* Vector # 252 MC _ME ME_IS[I_MTC] */
(uint32_t) &dummy, /* Vector # 253 MC _ME ME_IS[I_IMODE] */
(uint32_t) &dummy, /* Vector # 254 MC _ME ME_IS[I_ICONF] */
(uint32_t) &dummy, /* Vector # 255 MC_RGM MC_RGM Functional and destructive reset alternate event interrupt */
(uint32_t) &dummy, /* Vector # 256 */
(uint32_t) &dummy, /* Vector # 257 FXOSC Counter FXOSC Counter */
(uint32_t) &dummy, /* Vector # 258 SXOSC Counter SXOSC Counter */
(uint32_t) &dummy, /* Vector # 259 DSPI0_0 DSPI_0_SR[TFUF] | DSPI_0_SR[RFOF] | DSPI_0_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 260 DSPI0_1 DSPI_0_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 261 DSPI0_2 DSPI_0_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 262 DSPI0_3 DSPI_0_SR[TCF] */
(uint32_t) &dummy, /* Vector # 263 DSPI0_4 DSPI_0_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 264 DSPI0_5 DSPI_0_SR[SPITCF] | DSPI_0_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 265 DSPI0_6 DSPI_0_SR[DSITCF] | DSPI_0_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 266 DSPI0_7 DSPI_0_SR[SPEF] | DSPI_0_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 267 DSPI0_8 DSPI_0_SR[DDIF] */
(uint32_t) &dummy, /* Vector # 268 DSPI1_0 DSPI_1_SR[TFUF] | DSPI_1_SR[RFOF]| DSPI_1_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 269 DSPI1_1 DSPI_1_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 270 DSPI1_2 DSPI_1_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 271 DSPI1_3 DSPI_1_SR[TCF] */
(uint32_t) &dummy, /* Vector # 272 DSPI1_4 DSPI_1_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 273 DSPI1_5 DSPI_1_SR[SPITCF] | DSPI_1_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 274 DSPI1_6 DSPI_1_SR[DSITCF] | DSPI_1_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 275 DSPI1_7 DSPI_1_SR[SPEF] | DSPI_1_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 276 DSPI1_8 DSPI_1_SR[DDIF] */
(uint32_t) &dummy, /* Vector # 277 DSPI2_0 DSPI_2_SR[TFUF] | DSPI_2_SR[RFOF]| DSPI_2_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 278 DSPI2_1 DSPI_2_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 279 DSPI2_2 DSPI_2_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 280 DSPI2_3 DSPI_2_SR[TCF] */
(uint32_t) &dummy, /* Vector # 281 DSPI2_4 DSPI_2_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 282 DSPI2_5 DSPI_2_SR[SPITCF] | DSPI_2_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 283 DSPI2_6 DSPI_2_SR[DSITCF] | DSPI_2_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 284 DSPI2_7 DSPI_2_SR[SPEF] | DSPI_2_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 285 DSPI2_8 DSPI_2_SR[DDIF] */
(uint32_t) &dummy, /* Vector # 286 DSPI3_0 DSPI_3_SR[TFUF] | DSPI_3_SR[RFOF]| DSPI_3_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 287 DSPI3_1 DSPI_3_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 288 DSPI3_2 DSPI_3_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 289 DSPI3_3 DSPI_3_SR[TCF] */
(uint32_t) &dummy, /* Vector # 290 DSPI3_4 DSPI_3_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 291 DSPI3_5 DSPI_3_SR[SPITCF] | DSPI_3_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 292 DSPI3_6 DSPI_3_SR[DSITCF] | DSPI_3_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 293 DSPI3_7 DSPI_3_SR[SPEF] | DSPI_3_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 294 DSPI3_8 DSPI_3_SR[DDIF] */
(uint32_t) &dummy, /* Vector # 295 DSPI4_0 DSPI_4_SR[TFUF] | DSPI_4_SR[RFOF]| DSPI_4_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 296 DSPI4_1 DSPI_4_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 297 DSPI4_2 DSPI_4_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 298 DSPI4_3 DSPI_4_SR[TCF] */
(uint32_t) &dummy, /* Vector # 299 DSPI4_4 DSPI_4_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 300 DSPI4_5 DSPI_4_SR[SPITCF] | DSPI_4_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 301 DSPI4_6 DSPI_4_SR[DSITCF] | DSPI_4_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 302 DSPI4_7 DSPI_4_SR[SPEF] | DSPI_4_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 303 */
(uint32_t) &dummy, /* Vector # 304 DSPI5_0 DSPI_5_SR[TFUF] | DSPI_5_SR[RFOF]| DSPI_5_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 305 DSPI5_1 DSPI_5_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 306 DSPI5_2 DSPI_5_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 307 DSPI5_3 DSPI_5_SR[TCF] */
(uint32_t) &dummy, /* Vector # 308 DSPI5_4 DSPI_5_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 309 DSPI5_5 DSPI_5_SR[SPITCF] | DSPI_5_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 310 DSPI5_6 DSPI_5_SR[DSITCF] | DSPI_5_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 311 DSPI5_7 DSPI_5_SR[SPEF] | DSPI_5_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 312 */
(uint32_t) &dummy, /* Vector # 313 DSPI6_0 DSPI_6_SR[TFUF] | DSPI_6_SR[RFOF]| DSPI_6_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 314 DSPI6_1 DSPI_6_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 315 DSPI6_2 DSPI_6_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 316 DSPI6_3 DSPI_6_SR[TCF] */
(uint32_t) &dummy, /* Vector # 317 DSPI6_4 DSPI_6_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 318 DSPI6_5 DSPI_6_SR[SPITCF] | DSPI_6_SR[CMD_TCF] */
(uint32_t) &dummy, /* Vector # 319 DSPI6_6 DSPI_6_SR[DSITCF] | DSPI_6_SR[CMDFFF] */
(uint32_t) &dummy, /* Vector # 320 DSPI6_7 DSPI_6_SR[SPEF] | DSPI_6_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 321 */
(uint32_t) &dummy, /* Vector # 322 DSPI7_0 DSPI_7_SR[TFUF] | DSPI_7_SR[RFOF]| DSPI_7_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 323 DSPI7_1 DSPI_7_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 324 DSPI7_2 DSPI_7_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 325 DSPI7_3 DSPI_7_SR[TCF] */
(uint32_t) &dummy, /* Vector # 326 DSPI7_4 DSPI_7_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 327 DSPI7_5 DSPI_7_SR[SPITCF] */
(uint32_t) &dummy, /* Vector # 328 DSPI7_6 DSPI_7_SR[DSITCF] */
(uint32_t) &dummy, /* Vector # 329 DSPI7_7 DSPI_7_SR[SPEF] | DSPI_7_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 330 */
(uint32_t) &dummy, /* Vector # 331 DSPI8_0 DSPI_8_SR[TFUF] | DSPI_8_SR[RFOF]| DSPI_8_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 332 DSPI8_1 DSPI_8_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 333 DSPI8_2 DSPI_8_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 334 DSPI8_3 DSPI_8_SR[TCF] */
(uint32_t) &dummy, /* Vector # 335 DSPI8_4 DSPI_8_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 336 DSPI8_5 DSPI_8_SR[SPITCF] */
(uint32_t) &dummy, /* Vector # 337 DSPI8_6 DSPI_8_SR[DSITCF] */
(uint32_t) &dummy, /* Vector # 338 DSPI8_7 DSPI_8_SR[SPEF] | DSPI_8_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 339 */
(uint32_t) &dummy, /* Vector # 340 DSPI9_0 DSPI_9_SR[TFUF] | DSPI_9_SR[RFOF]| DSPI_9_SR[TFIWF] */
(uint32_t) &dummy, /* Vector # 341 DSPI9_1 DSPI_9_SR[EOQF] */
(uint32_t) &dummy, /* Vector # 342 DSPI9_2 DSPI_9_SR[TFFF] */
(uint32_t) &dummy, /* Vector # 343 DSPI9_3 DSPI_9_SR[TCF] */
(uint32_t) &dummy, /* Vector # 344 DSPI9_4 DSPI_9_SR[RFDF] */
(uint32_t) &dummy, /* Vector # 345 DSPI9_5 DSPI_9_SR[SPITCF] */
(uint32_t) &dummy, /* Vector # 346 DSPI9_6 DSPI_9_SR[DSITCF] */
(uint32_t) &dummy, /* Vector # 347 DSPI9_7 DSPI_9_SR[SPEF] | DSPI_9_SR[DPEF] */
(uint32_t) &dummy, /* Vector # 348 */
(uint32_t) &dummy, /* Vector # 349 */
(uint32_t) &dummy, /* Vector # 350 */
(uint32_t) &dummy, /* Vector # 351 */
(uint32_t) &dummy, /* Vector # 352 */
(uint32_t) &dummy, /* Vector # 353 */
(uint32_t) &dummy, /* Vector # 354 */
(uint32_t) &dummy, /* Vector # 355 */
(uint32_t) &dummy, /* Vector # 356 */
(uint32_t) &dummy, /* Vector # 357 */
(uint32_t) &dummy, /* Vector # 358 */
(uint32_t) &dummy, /* Vector # 359 */
(uint32_t) &dummy, /* Vector # 360 */
(uint32_t) &dummy, /* Vector # 361 */
(uint32_t) &dummy, /* Vector # 362 */
(uint32_t) &dummy, /* Vector # 363 */
(uint32_t) &dummy, /* Vector # 364 */
(uint32_t) &dummy, /* Vector # 365 */
(uint32_t) &dummy, /* Vector # 366 */
(uint32_t) &dummy, /* Vector # 367 */
(uint32_t) &dummy, /* Vector # 368 */
(uint32_t) &dummy, /* Vector # 369 */
(uint32_t) &dummy, /* Vector # 370 */
(uint32_t) &dummy, /* Vector # 371 */
(uint32_t) &dummy, /* Vector # 372 BCTU LIST0_LAST | LIST1_LAST */
(uint32_t) &dummy, /* Vector # 373 BCTU NDATA0 | NDATA1 | DATA_OVR0 | DATA_OVR1 */
(uint32_t) &dummy, /* Vector # 374 */
(uint32_t) &dummy, /* Vector # 375 */
(uint32_t) &dummy, /* Vector # 376 LinFLEX0_0 LinFlex_0_RXI */
(uint32_t) &dummy, /* Vector # 377 LinFlex0_1 LinFlex_0_TXI */
(uint32_t) &dummy, /* Vector # 378 LinFlex0_2 LinFlex_0_ERR */
(uint32_t) &dummy, /* Vector # 379 LinFlex1_0 LinFlex_1_RXI */
(uint32_t) &dummy, /* Vector # 380 LinFlex1_1 LinFlex_1_TXI */
(uint32_t) &dummy, /* Vector # 381 LinFlex1_2 LinFlex_1_ERR */
(uint32_t) &dummy, /* Vector # 382 LinFlex2_0 LinFlex_2_RXI */
(uint32_t) &dummy, /* Vector # 383 LinFlex2_1 LinFlex_2_TXI */
(uint32_t) &dummy, /* Vector # 384 LinFlex2_2 LinFlex_2_ERR */
(uint32_t) &dummy, /* Vector # 385 LinFlex3_0 LinFlex_3_RXI */
(uint32_t) &dummy, /* Vector # 386 LinFlex3_1 LinFlex_3_TXI */
(uint32_t) &dummy, /* Vector # 387 LinFlex3_2 LinFlex_3_ERR */
(uint32_t) &dummy, /* Vector # 388 LinFlex4_0 LinFlex_4_RXI */
(uint32_t) &dummy, /* Vector # 389 LinFlex4_1 LinFlex_4_TXI */
(uint32_t) &dummy, /* Vector # 390 LinFlex4_2 LinFlex_4_ERR */
(uint32_t) &dummy, /* Vector # 391 LinFlex5_0 LinFlex_5_RXI */
(uint32_t) &dummy, /* Vector # 392 LinFlex5_1 LinFlex_5_TXI */
(uint32_t) &dummy, /* Vector # 393 LinFlex5_2 LinFlex_5_ERR */
(uint32_t) &dummy, /* Vector # 394 LinFlex6_0 LinFlex_6_RXI */
(uint32_t) &dummy, /* Vector # 395 LinFlex6_1 LinFlex_6_TXI */
(uint32_t) &dummy, /* Vector # 396 LinFlex6_2 LinFlex_6_ERR */
(uint32_t) &dummy, /* Vector # 397 LinFlex7_0 LinFlex_7_RXI */
(uint32_t) &dummy, /* Vector # 398 LinFlex7_1 LinFlex_7_TXI */
(uint32_t) &dummy, /* Vector # 399 LinFlex7_2 LinFlex_7_ERR */
(uint32_t) &dummy, /* Vector # 400 LinFlex8_0 LinFlex_8_RXI */
(uint32_t) &dummy, /* Vector # 401 LinFlex8_1 LinFlex_8_TXI */
(uint32_t) &dummy, /* Vector # 402 LinFlex8_2 LinFlex_8_ERR */
(uint32_t) &dummy, /* Vector # 403 LinFlex9_0 LinFlex_9_RXI */
(uint32_t) &dummy, /* Vector # 404 LinFlex9_1 LinFlex_9_TXI */
(uint32_t) &dummy, /* Vector # 405 LinFlex9_2 LinFlex_9_ERR */
(uint32_t) &dummy, /* Vector # 406 LinFlex10_0 LinFlex_10_RXI */
(uint32_t) &dummy, /* Vector # 407 LinFlex10_1 LinFlex_10_TXI */
(uint32_t) &dummy, /* Vector # 408 LinFlex10_2 LinFlex_10_ERR */
(uint32_t) &dummy, /* Vector # 409 LinFlex11_0 LinFlex_11_RXI */
(uint32_t) &dummy, /* Vector # 410 LinFlex11_1 LinFlex_11_TXI */
(uint32_t) &dummy, /* Vector # 411 LinFlex11_2 LinFlex_11_ERR */
(uint32_t) &dummy, /* Vector # 412 LinFlex12_0 LinFlex_12_RXI */
(uint32_t) &dummy, /* Vector # 413 LinFlex12_1 LinFlex_12_TXI */
(uint32_t) &dummy, /* Vector # 414 LinFlex12_2 LinFlex_12_ERR */
(uint32_t) &dummy, /* Vector # 415 LinFlex13_0 LinFlex_13_RXI */
(uint32_t) &dummy, /* Vector # 416 LinFlex13_1 LinFlex_13_TXI */
(uint32_t) &dummy, /* Vector # 417 LinFlex13_2 LinFlex_13_ERR */
(uint32_t) &dummy, /* Vector # 418 LinFlex14_0 LinFlex_14_RXI */
(uint32_t) &dummy, /* Vector # 419 LinFlex14_1 LinFlex_14_TXI */
(uint32_t) &dummy, /* Vector # 420 LinFlex14_2 LinFlex_14_ERR */
(uint32_t) &dummy, /* Vector # 421 LinFlex15_0 LinFlex_15_RXI */
(uint32_t) &dummy, /* Vector # 422 LinFlex15_1 LinFlex_15_TXI */
(uint32_t) &dummy, /* Vector # 423 LinFlex15_2 LinFlex_15_ERR */
(uint32_t) &dummy, /* Vector # 424 LinFlex16_0 LinFlex_16_RXI */
(uint32_t) &dummy, /* Vector # 425 LinFlex16_1 LinFlex_16_TXI */
(uint32_t) &dummy, /* Vector # 426 LinFlex16_2 LinFlex_16_ERR */
(uint32_t) &dummy, /* Vector # 427 LinFlex17_0 LinFlex_17_RXI */
(uint32_t) &dummy, /* Vector # 428 LinFlex17_1 LinFlex_17_TXI */
(uint32_t) &dummy, /* Vector # 429 LinFlex17_2 LinFlex_17_ERR */
(uint32_t) &dummy, /* Vector # 430 */
(uint32_t) &dummy, /* Vector # 431 */
(uint32_t) &dummy, /* Vector # 432 */
(uint32_t) &dummy, /* Vector # 433 */
(uint32_t) &dummy, /* Vector # 434 */
(uint32_t) &dummy, /* Vector # 435 */
(uint32_t) &dummy, /* Vector # 436 */
(uint32_t) &dummy, /* Vector # 437 */
(uint32_t) &dummy, /* Vector # 438 */
(uint32_t) &dummy, /* Vector # 439 */
(uint32_t) &dummy, /* Vector # 440 I2C_0_0 I2C0_SR[IBAL] | I2C0_SR[TCF] | I2C0_SR[IAAS] */
(uint32_t) &dummy, /* Vector # 441 */
(uint32_t) &dummy, /* Vector # 442 I2C_1_0 I2C1_SR[IBAL] | I2C1_SR[TCF] | I2C1_SR[IAAS] */
(uint32_t) &dummy, /* Vector # 443 */
(uint32_t) &dummy, /* Vector # 444 I2C_2_0 I2C2_SR[IBAL] | I2C2_SR[TCF] | I2C2_SR[IAAS] */
(uint32_t) &dummy, /* Vector # 445 */
(uint32_t) &dummy, /* Vector # 446 I2C_3_0 I2C3_SR[IBAL] | I2C3_SR[TCF] | I2C3_SR[IAAS] */
(uint32_t) &dummy, /* Vector # 447 */
(uint32_t) &dummy, /* Vector # 448 */
(uint32_t) &dummy, /* Vector # 449 */
(uint32_t) &dummy, /* Vector # 450 */
(uint32_t) &dummy, /* Vector # 451 */
(uint32_t) &dummy, /* Vector # 452 */
(uint32_t) &dummy, /* Vector # 453 FlexRay_0_0 LRNEIF | DRNEIF */
(uint32_t) &dummy, /* Vector # 454 FlexRay_0_1 LRCEIF | DRCEIF */
(uint32_t) &dummy, /* Vector # 455 FlexRay_0_2 FNEAIF */
(uint32_t) &dummy, /* Vector # 456 FlexRay_0_3 FNEBIF */
(uint32_t) &dummy, /* Vector # 457 FlexRay_0_4 WUPIF */
(uint32_t) &dummy, /* Vector # 458 FlexRay_0_5 PRIF */
(uint32_t) &dummy, /* Vector # 459 FlexRay_0_6 CHIF */
(uint32_t) &dummy, /* Vector # 460 FlexRay_0_7 TBIF */
(uint32_t) &dummy, /* Vector # 461 FlexRay_0_8 RBIF */
(uint32_t) &dummy, /* Vector # 462 FlexRay_0_9 MIF */
(uint32_t) &dummy, /* Vector # 463 */
(uint32_t) &dummy, /* Vector # 464 */
(uint32_t) &dummy, /* Vector # 465 */
(uint32_t) &dummy, /* Vector # 466 */
(uint32_t) &dummy, /* Vector # 467 */
(uint32_t) &dummy, /* Vector # 468 */
(uint32_t) &dummy, /* Vector # 469 */
(uint32_t) &dummy, /* Vector # 470 */
(uint32_t) &dummy, /* Vector # 471 */
(uint32_t) &dummy, /* Vector # 472 */
(uint32_t) &dummy, /* Vector # 473 */
(uint32_t) &dummy, /* Vector # 474 */
(uint32_t) &dummy, /* Vector # 475 */
(uint32_t) &dummy, /* Vector # 476 */
(uint32_t) &dummy, /* Vector # 477 Power Monitor Unit LVD/HVD self checking fault */
(uint32_t) &dummy, /* Vector # 478 */
(uint32_t) &dummy, /* Vector # 479 */
(uint32_t) &dummy, /* Vector # 480 PLL Loss-of-lock interrupt request for PLL */
(uint32_t) &dummy, /* Vector # 481 */
(uint32_t) &dummy, /* Vector # 482 */
(uint32_t) &dummy, /* Vector # 483 */
(uint32_t) &dummy, /* Vector # 484 PLL Indicates PLL Power down process completed by MC_ME */
(uint32_t) &dummy, /* Vector # 485 */
(uint32_t) &dummy, /* Vector # 486 */
(uint32_t) &dummy, /* Vector # 487 */
(uint32_t) &dummy, /* Vector # 488 FCCU Interrupt request (ALARM state) */
(uint32_t) &dummy, /* Vector # 489 FCCU Interrupt request (miscellaneous conditions) */
(uint32_t) &dummy, /* Vector # 490 FCCU EOUT interrupt */
(uint32_t) &dummy, /* Vector # 491 */
(uint32_t) &dummy, /* Vector # 492 */
(uint32_t) &dummy, /* Vector # 493 */
(uint32_t) &dummy, /* Vector # 494 STCU Interrupt request line related to online MBIST execution */
(uint32_t) &dummy, /* Vector # 495 STCU Interrupt request line related to online LBIST execution */
(uint32_t) &dummy, /* Vector # 496 Hardware Security Module 0 HSM2HTIE[0] */
(uint32_t) &dummy, /* Vector # 497 Hardware Security Module 1 HSM2HTIE[1] */
(uint32_t) &dummy, /* Vector # 498 Hardware Security Module 2 HSM2HTIE[2] */
(uint32_t) &dummy, /* Vector # 499 Hardware Security Module 3 HSM2HTIE[3] */
(uint32_t) &dummy, /* Vector # 500 Hardware Security Module 4 HSM2HTIE[4] */
(uint32_t) &dummy, /* Vector # 501 Hardware Security Module 5 HSM2HTIE[5] */
(uint32_t) &dummy, /* Vector # 502 Hardware Security Module 6 HSM2HTIE[6] */
(uint32_t) &dummy, /* Vector # 503 Hardware Security Module 7 HSM2HTIE[7] */
(uint32_t) &dummy, /* Vector # 504 Hardware Security Module 8 HSM2HTIE[8] */
(uint32_t) &dummy, /* Vector # 505 Hardware Security Module 9 HSM2HTIE[9] */
(uint32_t) &dummy, /* Vector # 506 Hardware Security Module 10 HSM2HTIE[10] */
(uint32_t) &dummy, /* Vector # 507 Hardware Security Module 11 HSM2HTIE[11] */
(uint32_t) &dummy, /* Vector # 508 Hardware Security Module 12 HSM2HTIE[12] */
(uint32_t) &dummy, /* Vector # 509 Hardware Security Module 13 HSM2HTIE[13] */
(uint32_t) &dummy, /* Vector # 510 Hardware Security Module 14 HSM2HTIE[14] */
(uint32_t) &dummy, /* Vector # 511 Hardware Security Module 15 HSM2HTIE[15] */
(uint32_t) &dummy, /* Vector # 512 Hardware Security Module 16 HSM2HTIE[16] */
(uint32_t) &dummy, /* Vector # 513 Hardware Security Module 17 HSM2HTIE[17] */
(uint32_t) &dummy, /* Vector # 514 Hardware Security Module 18 HSM2HTIE[18] */
(uint32_t) &dummy, /* Vector # 515 Hardware Security Module 19 HSM2HTIE[19] */
(uint32_t) &dummy, /* Vector # 516 Hardware Security Module 20 HSM2HTIE[20] */
(uint32_t) &dummy, /* Vector # 517 Hardware Security Module 21 HSM2HTIE[21] */
(uint32_t) &dummy, /* Vector # 518 Hardware Security Module 22 HSM2HTIE[22] */
(uint32_t) &dummy, /* Vector # 519 Hardware Security Module 23 HSM2HTIE[23] */
(uint32_t) &dummy, /* Vector # 520 Hardware Security Module 24 HSM2HTIE[24] */
(uint32_t) &dummy, /* Vector # 521 Hardware Security Module 25 HSM2HTIE[25] */
(uint32_t) &dummy, /* Vector # 522 Hardware Security Module 26 HSM2HTIE[26] */
(uint32_t) &dummy, /* Vector # 523 Hardware Security Module 27 HSM2HTIE[27] */
(uint32_t) &dummy, /* Vector # 524 Hardware Security Module 28 HSM2HTIE[28] */
(uint32_t) &dummy, /* Vector # 525 Hardware Security Module 29 HSM2HTIE[29] */
(uint32_t) &dummy, /* Vector # 526 Hardware Security Module 30 HSM2HTIE[30] */
(uint32_t) &dummy, /* Vector # 527 Hardware Security Module 31 HSM2HTIE[31] */
(uint32_t) &dummy, /* Vector # 528 */
(uint32_t) &dummy, /* Vector # 529 */
(uint32_t) &dummy, /* Vector # 530 */
(uint32_t) &dummy, /* Vector # 531 */
(uint32_t) &dummy, /* Vector # 532 */
(uint32_t) &dummy, /* Vector # 533 */
(uint32_t) &dummy, /* Vector # 534 */
(uint32_t) &dummy, /* Vector # 535 */
(uint32_t) &dummy, /* Vector # 536 */
(uint32_t) &dummy, /* Vector # 537 */
(uint32_t) &dummy, /* Vector # 538 */
(uint32_t) &dummy, /* Vector # 539 */
(uint32_t) &dummy, /* Vector # 540 */
(uint32_t) &dummy, /* Vector # 541 */
(uint32_t) &dummy, /* Vector # 542 */
(uint32_t) &dummy, /* Vector # 543 */
(uint32_t) &dummy, /* Vector # 544 */
(uint32_t) &dummy, /* Vector # 545 */
(uint32_t) &dummy, /* Vector # 546 */
(uint32_t) &dummy, /* Vector # 547 */
(uint32_t) &dummy, /* Vector # 548 ADC_0_EOC ADC_0_EOC */
(uint32_t) &dummy, /* Vector # 549 ADC_0_ER ADC_0_ER */
(uint32_t) &dummy, /* Vector # 550 ADC_0_WD ADC_0_WD */
(uint32_t) &dummy, /* Vector # 551 */
(uint32_t) &dummy, /* Vector # 552 */
(uint32_t) &dummy, /* Vector # 553 */
(uint32_t) &dummy, /* Vector # 554 ADC_1_EOC ADC_1_EOC */
(uint32_t) &dummy, /* Vector # 555 ADC_1_ER ADC_1_ER */
(uint32_t) &dummy, /* Vector # 556 ADC_1_WD ADC_1_WD */
(uint32_t) &dummy, /* Vector # 557 */
(uint32_t) &dummy, /* Vector # 558 */
(uint32_t) &dummy, /* Vector # 559 */
(uint32_t) &dummy, /* Vector # 560 Analogue_Comparator_Module_0 CMP0 */
(uint32_t) &dummy, /* Vector # 561 Analogue_Comparator_Module_1 CMP1 */
(uint32_t) &dummy, /* Vector # 562 Analogue_Comparator_Module_2 CMP2 */
(uint32_t) &dummy, /* Vector # 563 */
(uint32_t) &dummy, /* Vector # 564 */
(uint32_t) &dummy, /* Vector # 565 FlexCAN0_0 FlexCAN0_PN */
(uint32_t) &dummy, /* Vector # 566 FlexCAN0_1 FLEXCAN_0_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 567 FlexCAN0_2 FLEXCAN_0_ESR_BOFF | FLEXCAN_0_Transmit_Warning | FLEXCAN_0_Receive_Warning */
(uint32_t) &dummy, /* Vector # 568 FlexCAN0_3 FlexCAN0_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 569 FlexCAN0_4 FlexCAN0_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 570 FlexCAN0_5 FlexCAN0_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 571 FlexCAN0_6 FlexCAN0_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 572 FlexCAN0_7 FlexCAN0_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 573 FlexCAN0_8 FlexCAN0_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 574 */
(uint32_t) &dummy, /* Vector # 575 */
(uint32_t) &dummy, /* Vector # 576 */
(uint32_t) &dummy, /* Vector # 577 */
(uint32_t) &dummy, /* Vector # 578 FlexCAN1_0 FLEXCAN_1_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 579 FlexCAN1_1 FLEXCAN_1_ESR_BOFF | FLEXCAN_1_Transmit_Warning | FLEXCAN_1_Receive_Warning */
(uint32_t) &dummy, /* Vector # 580 FlexCAN1_2 FlexCAN1_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 581 FlexCAN1_3 FlexCAN1_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 582 FlexCAN1_4 FlexCAN1_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 583 FlexCAN1_5 FlexCAN1_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 584 FlexCAN1_6 FlexCAN1_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 585 FlexCAN1_7 FlexCAN1_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 586 */
(uint32_t) &dummy, /* Vector # 587 */
(uint32_t) &dummy, /* Vector # 588 */
(uint32_t) &dummy, /* Vector # 589 */
(uint32_t) &dummy, /* Vector # 590 FlexCAN2_0 FLEXCAN_2_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 591 FlexCAN2_1 FLEXCAN_2_ESR_BOFF | FLEXCAN_2_Transmit_Warning | FLEXCAN_2_Receive_Warning */
(uint32_t) &dummy, /* Vector # 592 FlexCAN2_2 FlexCAN2_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 593 FlexCAN2_3 FlexCAN2_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 594 FlexCAN2_4 FlexCAN2_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 595 FlexCAN2_5 FlexCAN2_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 596 FlexCAN2_6 FlexCAN2_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 597 FlexCAN2_7 FlexCAN2_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 598 */
(uint32_t) &dummy, /* Vector # 599 */
(uint32_t) &dummy, /* Vector # 600 */
(uint32_t) &dummy, /* Vector # 601 */
(uint32_t) &dummy, /* Vector # 602 FlexCAN3_0 FLEXCAN_3_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 603 FlexCAN3_1 FLEXCAN_3_ESR_BOFF | FLEXCAN_3_Transmit_Warning | FLEXCAN_3_Receive_Warning */
(uint32_t) &dummy, /* Vector # 604 FlexCAN3_2 FlexCAN3_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 605 FlexCAN3_3 FlexCAN3_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 606 FlexCAN3_4 FlexCAN3_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 607 FlexCAN3_5 FlexCAN3_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 608 FlexCAN3_6 FlexCAN3_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 609 FlexCAN3_7 FlexCAN3_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 610 */
(uint32_t) &dummy, /* Vector # 611 */
(uint32_t) &dummy, /* Vector # 612 */
(uint32_t) &dummy, /* Vector # 613 */
(uint32_t) &dummy, /* Vector # 614 FlexCAN4_0 FLEXCAN_4_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 615 FlexCAN4_1 FLEXCAN_4_ESR_BOFF | FLEXCAN_4_Transmit_Warning | FLEXCAN_4_Receive_Warning */
(uint32_t) &dummy, /* Vector # 616 FlexCAN4_2 FlexCAN4_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 617 FlexCAN4_3 FlexCAN4_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 618 FlexCAN4_4 FlexCAN4_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 619 FlexCAN4_5 FlexCAN4_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 620 FlexCAN4_6 FlexCAN4_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 621 FlexCAN4_7 FlexCAN4_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 622 */
(uint32_t) &dummy, /* Vector # 623 */
(uint32_t) &dummy, /* Vector # 624 */
(uint32_t) &dummy, /* Vector # 625 */
(uint32_t) &dummy, /* Vector # 626 FlexCAN5_0 FLEXCAN_5_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 627 FlexCAN5_1 FLEXCAN_5_ESR_BOFF | FLEXCAN_5_Transmit_Warning | FLEXCAN_5_Receive_Warning */
(uint32_t) &dummy, /* Vector # 628 FlexCAN5_2 FlexCAN5_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 629 FlexCAN5_3 FlexCAN5_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 630 FlexCAN5_4 FlexCAN5BUF_08_11 */
(uint32_t) &dummy, /* Vector # 631 FlexCAN5_5 FlexCAN5_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 632 FlexCAN5_6 FlexCAN5_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 633 FlexCAN5_7 FlexCAN5_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 634 */
(uint32_t) &dummy, /* Vector # 635 */
(uint32_t) &dummy, /* Vector # 636 */
(uint32_t) &dummy, /* Vector # 637 */
(uint32_t) &dummy, /* Vector # 638 FlexCAN6_0 FLEXCAN_6_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 639 FlexCAN6_1 FLEXCAN_6_ESR_BOFF | FLEXCAN_6_Transmit_Warning | FLEXCAN_6_Receive_Warning */
(uint32_t) &dummy, /* Vector # 640 FlexCAN6_2 FlexCAN6_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 641 FlexCAN6_3 FlexCAN6_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 642 FlexCAN6_4 FlexCAN6_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 643 FlexCAN6_5 FlexCAN6_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 644 FlexCAN6_6 FlexCAN6_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 645 FlexCAN6_7 FlexCAN6_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 646 */
(uint32_t) &dummy, /* Vector # 647 */
(uint32_t) &dummy, /* Vector # 648 */
(uint32_t) &dummy, /* Vector # 649 */
(uint32_t) &dummy, /* Vector # 650 FlexCAN7_0 FLEXCAN_7_ESR[ERR_INT] */
(uint32_t) &dummy, /* Vector # 651 FlexCAN7_1 FLEXCAN_7_ESR_BOFF | FLEXCAN_7_Transmit_Warning | FLEXCAN_7_Receive_Warning */
(uint32_t) &dummy, /* Vector # 652 FlexCAN7_2 FlexCAN7_BUF_00_03 */
(uint32_t) &dummy, /* Vector # 653 FlexCAN7_3 FlexCAN7_BUF_04_07 */
(uint32_t) &dummy, /* Vector # 654 FlexCAN7_4 FlexCAN7_BUF_08_11 */
(uint32_t) &dummy, /* Vector # 655 FlexCAN7_5 FlexCAN7_BUF_12_15 */
(uint32_t) &dummy, /* Vector # 656 FlexCAN7_6 FlexCAN7_BUF_16_31 */
(uint32_t) &dummy, /* Vector # 657 FlexCAN7_7 FlexCAN7_BUF_32_63 */
(uint32_t) &dummy, /* Vector # 658 */
(uint32_t) &dummy, /* Vector # 659 */
(uint32_t) &dummy, /* Vector # 660 */
(uint32_t) &dummy, /* Vector # 661 */
(uint32_t) &dummy, /* Vector # 662 */
(uint32_t) &dummy, /* Vector # 663 */
(uint32_t) &dummy, /* Vector # 664 */
(uint32_t) &dummy, /* Vector # 665 */
(uint32_t) &dummy, /* Vector # 666 */
(uint32_t) &dummy, /* Vector # 667 */
(uint32_t) &dummy, /* Vector # 668 WKUP_IRQ_0 WKPU_IRQ_0 */
(uint32_t) &dummy, /* Vector # 669 WKPU_IRQ_1 WKPU_IRQ_1 */
(uint32_t) &dummy, /* Vector # 670 WKPU_IRQ_2 WKPU_IRQ_2 */
(uint32_t) &dummy, /* Vector # 671 WKPU_IRQ_3 WKPU_IRQ_3 */
(uint32_t) &dummy, /* Vector # 672 */
(uint32_t) &dummy, /* Vector # 673 */
(uint32_t) &dummy, /* Vector # 674 */
(uint32_t) &dummy, /* Vector # 675 */
(uint32_t) &dummy, /* Vector # 676 */
(uint32_t) &dummy, /* Vector # 677 */
(uint32_t) &dummy, /* Vector # 678 */
(uint32_t) &dummy, /* Vector # 679 */
(uint32_t) &dummy, /* Vector # 680 SDIO (SDHC) 0 SDHC0 */
(uint32_t) &dummy, /* Vector # 681 */
(uint32_t) &dummy, /* Vector # 682 USB_OTG USB OTG */
(uint32_t) &dummy, /* Vector # 683 USB_SPH USB SPH */
(uint32_t) &dummy, /* Vector # 684 */
(uint32_t) &dummy, /* Vector # 685 */
(uint32_t) &dummy, /* Vector # 686 MLB MLB System Interrupt */
(uint32_t) &dummy, /* Vector # 687 MLB MLB Channel Interrupts 0-15 Combined */
(uint32_t) &dummy, /* Vector # 688 */
(uint32_t) &dummy, /* Vector # 689 */
(uint32_t) &dummy, /* Vector # 690 SAI0_TX_FIFO_IRQ SAI0_TX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 691 SAI0_RX_FIFO_IRQ SAI0_RX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 692 SAI1_TX_FIFO_IRQ SAI1_TX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 693 SAI1_RX_FIFO_IRQ SAI1_RX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 694 SAI2_TX_FIFO_IRQ SAI2_TX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 695 SAI2_RX_FIFO_IRQ SAI2_RX_FIFO_IRQ */
(uint32_t) &dummy, /* Vector # 696 */
(uint32_t) &dummy, /* Vector # 697 */
(uint32_t) &dummy, /* Vector # 698 */
(uint32_t) &dummy, /* Vector # 699 */
(uint32_t) &dummy, /* Vector # 700 */
(uint32_t) &dummy, /* Vector # 701 JDC JDC */
(uint32_t) &dummy, /* Vector # 702 MEMU_1 Correctable errors in LPU mode */
(uint32_t) &dummy, /* Vector # 703 */
(uint32_t) &dummy, /* Vector # 704 */
(uint32_t) &dummy, /* Vector # 705 */
(uint32_t) &dummy, /* Vector # 706 eMIOS channel 0,1 EMIOS_GFR[F0,F1] */
(uint32_t) &dummy, /* Vector # 707 eMIOS channel 2,3 EMIOS_GFR[F2,F3] */
(uint32_t) &dummy, /* Vector # 708 eMIOS channel 4,5 EMIOS_GFR[F4,F5] */
(uint32_t) &dummy, /* Vector # 709 eMIOS channel 6,7 EMIOS_GFR[F6,F7] */
(uint32_t) &dummy, /* Vector # 710 eMIOS channel 8,9 EMIOS_GFR[F8,F9] */
(uint32_t) &dummy, /* Vector # 711 eMIOS channel 10,11 EMIOS_GFR[F10,F11] */
(uint32_t) &dummy, /* Vector # 712 eMIOS channel 12,13 EMIOS_GFR[F12,F13] */
(uint32_t) &dummy, /* Vector # 713 eMIOS channel 14,15 EMIOS_GFR[F14,F15] */
(uint32_t) &dummy, /* Vector # 714 eMIOS channel 16,17 EMIOS_GFR[F16,F17] */
(uint32_t) &dummy, /* Vector # 715 eMIOS channel 18,19 EMIOS_GFR[F18,F19] */
(uint32_t) &dummy, /* Vector # 716 eMIOS channel 20,21 EMIOS_GFR[F20,F21] */
(uint32_t) &dummy, /* Vector # 717 eMIOS channel 22,23 EMIOS_GFR[F22,F23] */
(uint32_t) &dummy, /* Vector # 718 eMIOS channel 24,25 EMIOS_GFR[F24,F25] */
(uint32_t) &dummy, /* Vector # 719 eMIOS channel 26,27 EMIOS_GFR[F26,F27] */
(uint32_t) &dummy, /* Vector # 720 eMIOS channel 28,29 EMIOS_GFR[F28,F29] */
(uint32_t) &dummy, /* Vector # 721 eMIOS channel 30,31 EMIOS_GFR[F30,F31] */
(uint32_t) &dummy, /* Vector # 722 eMIOS channel 32,33 EMIOS_GFR[F32,F33] */
(uint32_t) &dummy, /* Vector # 723 eMIOS channel 34,35 EMIOS_GFR[F34,F35] */
(uint32_t) &dummy, /* Vector # 724 eMIOS channel 36,37 EMIOS_GFR[F36,F37] */
(uint32_t) &dummy, /* Vector # 725 eMIOS channel 38,39 EMIOS_GFR[F38,F39] */
(uint32_t) &dummy, /* Vector # 726 eMIOS channel 40,41 EMIOS_GFR[F40,F41] */
(uint32_t) &dummy, /* Vector # 727 eMIOS channel 42,43 EMIOS_GFR[F42,F43] */
(uint32_t) &dummy, /* Vector # 728 eMIOS channel 44,45 EMIOS_GFR[F44,F45] */
(uint32_t) &dummy, /* Vector # 729 eMIOS channel 46,47 EMIOS_GFR[F46,F47] */
(uint32_t) &dummy, /* Vector # 730 eMIOS channel 48,49 EMIOS_GFR[F48,F49] */
(uint32_t) &dummy, /* Vector # 731 eMIOS channel 50,51 EMIOS_GFR[F50,F51] */
(uint32_t) &dummy, /* Vector # 732 eMIOS channel 52,53 EMIOS_GFR[F52,F53] */
(uint32_t) &dummy, /* Vector # 733 eMIOS channel 54,55 EMIOS_GFR[F54,F55] */
(uint32_t) &dummy, /* Vector # 734 eMIOS channel 56,57 EMIOS_GFR[F56,F57] */
(uint32_t) &dummy, /* Vector # 735 eMIOS channel 58,59 EMIOS_GFR[F58,F59] */
(uint32_t) &dummy, /* Vector # 736 eMIOS channel 60,61 EMIOS_GFR[F60,F61] */
(uint32_t) &dummy, /* Vector # 737 eMIOS channel 62,63 EMIOS_GFR[F62,F63] */
(uint32_t) &dummy, /* Vector # 738 eMIOS channel 64,65 EMIOS_GFR[F64,F65] */
(uint32_t) &dummy, /* Vector # 739 eMIOS channel 66,67 EMIOS_GFR[F66,F67] */
(uint32_t) &dummy, /* Vector # 740 eMIOS channel 68,69 EMIOS_GFR[F68,F69] */
(uint32_t) &dummy, /* Vector # 741 eMIOS channel 70,71 EMIOS_GFR[F70,F71] */
(uint32_t) &dummy, /* Vector # 742 eMIOS channel 72,73 EMIOS_GFR[F72,F73] */
(uint32_t) &dummy, /* Vector # 743 eMIOS channel 74,75 EMIOS_GFR[F74,F75] */
(uint32_t) &dummy, /* Vector # 744 eMIOS channel 76,77 EMIOS_GFR[F76,F77] */
(uint32_t) &dummy, /* Vector # 745 eMIOS channel 78,79 EMIOS_GFR[F78,F79] */
(uint32_t) &dummy, /* Vector # 746 eMIOS channel 80,81 EMIOS_GFR[F80,F81] */
(uint32_t) &dummy, /* Vector # 747 eMIOS channel 82,83 EMIOS_GFR[F82,F83] */
(uint32_t) &dummy, /* Vector # 748 eMIOS channel 84,85 EMIOS_GFR[F84,F85] */
(uint32_t) &dummy, /* Vector # 749 eMIOS channel 86,87 EMIOS_GFR[F86,F87] */
(uint32_t) &dummy, /* Vector # 750 eMIOS channel 88,89 EMIOS_GFR[F88,F89] */
(uint32_t) &dummy, /* Vector # 751 eMIOS channel 90,91 EMIOS_GFR[F90,F91] */
(uint32_t) &dummy, /* Vector # 752 eMIOS channel 92,93 EMIOS_GFR[F92,F93] */
(uint32_t) &dummy, /* Vector # 753 eMIOS channel 94,95 EMIOS_GFR[F94,F95] */

};


void dummy (void) {   /* Dummy ISR vector to trap undefined ISRs */
    while (1) {};  /* Wait forever or for watchdog timeout */
}

