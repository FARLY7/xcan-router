#ifndef CPU_DEFINES_H
#define CPU_DEFINES_H

#if (defined(CPU_MPC5747C) || defined(CPU_MPC5748C) || defined(CPU_MPC5746G) || defined(CPU_MPC5747G) || defined(CPU_MPC5748G) || \
     defined(CPU_MPC5744B) || defined(CPU_MPC5745B) || defined(CPU_MPC5746B) || defined(CPU_MPC5744C) || defined(CPU_MPC5745C)  || defined(CPU_MPC5746C) || \
     defined(CPU_MPC5741P) || defined(CPU_MPC5742P) || defined(CPU_MPC5743P) || defined(CPU_MPC5744P))
    #define INTC_CPR_ADDR                      0xFC040010
    #define INTC_IACKR_PRC_ADDR                0xFC040020
    #define INTC_EOIR_PRC_ADDR                 0xFC040030
    #define INTC_IACKR_INTVEC_BITWIDTH_NUM      10
#else
    #error not define platform
#endif

#endif/* CPU_DEFINES_H */
