/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2107 NXP
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

#if !defined(CLOCK_MPC57XX_H)
#define CLOCK_MPC57XX_H

/*!
 * @file clock_MPC57XX.h
 */


#include "status.h"
#include <stdint.h>
#include <stdbool.h>
#include "device_registers.h"

/*! @file clock_MPC5748G.h */


/*!
 * @file clock_MPC57xx.h
 *
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * This is required to implement enumeration entries as macro.
 * Enumeration entries (macros) are referenced from application code.
 */

/*!
 * @ingroup clock_manager
 * @defgroup clock_MPC57xx
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @brief Key and inverted key values so write access to MCTL register is permitted */
#define FEATURE_MC_ME_KEY           (0x5AF0U)
#define FEATURE_MC_ME_KEY_INV       (0xA50FU)

/* @brief EXTAL0 clock frequency. */
extern uint32_t g_xtal0ClkFreq;

/* @brief EXTAL1 clock frequency. */
extern uint32_t g_xtal1ClkFreq;

/*!
 * @brief CGM auxiliary clock selector.
 * Implements cgm_sys_clk_src_t_Class
 */
typedef uint8_t cgm_sys_clk_src_t;
#define CGM_SYSTEM_CLOCK_SRC_FIRC       0U       /*!< FIRC. */
#define CGM_SYSTEM_CLOCK_SRC_IRCOSC     0U       /*!< IRCOSC. */
#define CGM_SYSTEM_CLOCK_SRC_FXOSC      1U       /*!< FXOSC. */
#define CGM_SYSTEM_CLOCK_SRC_XOSC       1U       /*!< XOSC. */
#define CGM_SYSTEM_CLOCK_SRC_PLL_PHI0   2U       /*!< PLL PHI_0. */
#define CGM_SYSTEM_CLOCK_SRC_PLL0_PHI0  2U       /*!< Primary PLL PHI_0. */
#define CGM_SYSTEM_CLOCK_SRC_PLL1_PHI0  4U       /*!< Secondary PLL PHI_0. */
#define CGM_SYSTEM_CLOCK_SRC_NONE       15U      /*!< NONE. */

/*!
 * @brief Enabled clock sources structure. Indicates what clock sources are enabled for a particular power mode.
 * Implements mc_me_clk_srcs_t_Class
 */
typedef struct
{
    bool irc0;                                      /*!< Internal RC oscillator 0 is switched on */
    bool irc1;                                      /*!< Internal RC oscillator 1 is switched on */
    bool xosc0;                                     /*!< External RC oscillator 0 is switched on */
    bool xosc1;                                     /*!< External RC oscillator 1 is switched on */
    bool pll0;                                      /*!< PLL0 is switched on */
    bool pll1;                                      /*!< PLL1 is switched on */

} mc_me_clk_srcs_t;


/*!
 * @brief Peripheral configuration enum. Used to specify one of the available peripheral configurations.
 * Implements mc_me_periph_config_t_Class
 */
typedef enum
{
    MC_ME_PERIPH_CONFIG_0,              /*!< Peripheral configuration 0 */
    MC_ME_PERIPH_CONFIG_1,              /*!< Peripheral configuration 1 */
    MC_ME_PERIPH_CONFIG_2,              /*!< Peripheral configuration 2 */
    MC_ME_PERIPH_CONFIG_3,              /*!< Peripheral configuration 3 */
    MC_ME_PERIPH_CONFIG_4,              /*!< Peripheral configuration 4 */
    MC_ME_PERIPH_CONFIG_5,              /*!< Peripheral configuration 5 */
    MC_ME_PERIPH_CONFIG_6,              /*!< Peripheral configuration 6 */
    MC_ME_PERIPH_CONFIG_7,              /*!< Peripheral configuration 7 */
    MC_ME_PERIPH_CONFIG_COUNT           /*!< Peripheral configuration count */
} mc_me_periph_config_t;


/*! @brief MC_ME peripheral instance clock configuration.
 *  Implements mc_me_peripheral_clock_config_t_Class
 */
typedef struct
{
    /* clockName   is the name of the peripheral clock
     *    must be one of the following values
     *    DMA0_CLK
     *    MPU0_CLK
     *    ...
     *    LPUART3_CLK
     */
    clock_names_t clockName;
    mc_me_periph_config_t mc_me_RunPeriphConfig;               /*!< Peripheral clock gating in running mode. */
    mc_me_periph_config_t mc_me_LowPowerPeriphConfig;          /*!< Peripheral clock gating in low-power mode. */

}mc_me_peripheral_clock_config_t;


/*!
 * @brief Mode configuration structure. System clock source and enabled clock sources for a mode.
 * Implements mc_me_mode_config_t_Class
 */
typedef struct
{
    cgm_sys_clk_src_t          sysclk;         /*!< System clock source */
    mc_me_clk_srcs_t           clocksEnabled;  /*!< Enabled clock sources (struct) */
} mc_me_mode_config_t;

/*!
 * @brief Running modes peripheral configuration
 * Implements mc_me_run_periph_config_t_Class
 */
typedef struct
{
    bool reset;
    bool test;
    bool safe;
    bool drun;
    bool run0;
    bool run1;
    bool run2;
    bool run3;

} mc_me_run_periph_config_t;

/*!
 * @brief Low power modes peripheral configuration
 * Implements mc_me_low_periph_config_t_Class
 */
typedef struct
{
    bool stop0;
    bool standby0;
    bool halt0;

} mc_me_low_periph_config_t;


/*! @brief MC_ME configuration.
 *  Implements mc_me_config_t_Class
 */
typedef struct
{
    mc_me_mode_config_t reset;               /*!< MC_ME_RESET.   */
    mc_me_mode_config_t test;                /*!< MC_ME_TEST.    */
    mc_me_mode_config_t safe;                /*!< MC_ME_SAFE.    */
    mc_me_mode_config_t drun;                /*!< MC_ME_DRUN.    */
    mc_me_mode_config_t run0;                /*!< MC_ME_RUN0.    */
    mc_me_mode_config_t run1;                /*!< MC_ME_RUN1.    */
    mc_me_mode_config_t run2;                /*!< MC_ME_RUN2.    */
    mc_me_mode_config_t run3;                /*!< MC_ME_RUN3.    */
    mc_me_mode_config_t stop0;               /*!< MC_ME_STOP.    */
    mc_me_mode_config_t standby0;            /*!< MC_ME_STANDBY. */
    mc_me_mode_config_t halt0;               /*!< MC_ME_HALT.    */
    mc_me_mode_config_t lpurun;              /*!< LPU_RUN.       */
    mc_me_mode_config_t lpustop;             /*!< LPU_STOP.      */
    mc_me_mode_config_t lpustandby;          /*!< LPU_STANDBY.   */

    mc_me_run_periph_config_t periphRunConfig[MC_ME_PERIPH_CONFIG_COUNT];      /*!< Run peripheral configurations          */
    mc_me_low_periph_config_t periphLowPowerConfig[MC_ME_PERIPH_CONFIG_COUNT]; /*!< Low power peripheral configurations    */

    uint32_t count;                                          /*!< Number of peripherals to be configured.               */
    mc_me_peripheral_clock_config_t *peripherals;            /*!< Pointer to the mc_me peripheral clock configurations array. */

} mc_me_config_t;

/*!
 * @brief CGM clock divider
 * Implements cgm_clk_div_t_Class
 */
typedef enum {

    CGM_CLOCK_DIV_BY_1  = 0U,           /*!< Divided by 1  */
    CGM_CLOCK_DIV_BY_2  = 1U,           /*!< Divided by 2  */
    CGM_CLOCK_DIV_BY_3  = 2U,           /*!< Divided by 3  */
    CGM_CLOCK_DIV_BY_4  = 3U,           /*!< Divided by 4  */
    CGM_CLOCK_DIV_BY_5  = 4U,           /*!< Divided by 5  */
    CGM_CLOCK_DIV_BY_6  = 5U,           /*!< Divided by 6  */
    CGM_CLOCK_DIV_BY_7  = 6U,           /*!< Divided by 7  */
    CGM_CLOCK_DIV_BY_8  = 7U,           /*!< Divided by 8  */
    CGM_CLOCK_DIV_BY_9  = 8U,           /*!< Divided by 9  */
    CGM_CLOCK_DIV_BY_10 = 9U,           /*!< Divided by 10 */
    CGM_CLOCK_DIV_BY_11 = 10U,          /*!< Divided by 11 */
    CGM_CLOCK_DIV_BY_12 = 11U,          /*!< Divided by 12 */
    CGM_CLOCK_DIV_BY_13 = 12U,          /*!< Divided by 13 */
    CGM_CLOCK_DIV_BY_14 = 13U,          /*!< Divided by 14 */
    CGM_CLOCK_DIV_BY_15 = 14U,          /*!< Divided by 15 */
    CGM_CLOCK_DIV_BY_16 = 15U,          /*!< Divided by 16 */
    CGM_CLOCK_DIV_BY_17 = 16U,          /*!< Divided by 17 */
    CGM_CLOCK_DIV_BY_18 = 17U,          /*!< Divided by 18 */
    CGM_CLOCK_DIV_BY_19 = 18U,          /*!< Divided by 19 */
    CGM_CLOCK_DIV_BY_20 = 19U,          /*!< Divided by 20 */
    CGM_CLOCK_DIV_BY_21 = 20U,          /*!< Divided by 21 */
    CGM_CLOCK_DIV_BY_22 = 21U,          /*!< Divided by 22 */
    CGM_CLOCK_DIV_BY_23 = 22U,          /*!< Divided by 23 */
    CGM_CLOCK_DIV_BY_24 = 23U,          /*!< Divided by 24 */
    CGM_CLOCK_DIV_BY_25 = 24U,          /*!< Divided by 25 */
    CGM_CLOCK_DIV_BY_26 = 25U,          /*!< Divided by 26 */
    CGM_CLOCK_DIV_BY_27 = 26U,          /*!< Divided by 27 */
    CGM_CLOCK_DIV_BY_28 = 27U,          /*!< Divided by 28 */
    CGM_CLOCK_DIV_BY_29 = 28U,          /*!< Divided by 29 */
    CGM_CLOCK_DIV_BY_30 = 29U,          /*!< Divided by 30 */
    CGM_CLOCK_DIV_BY_31 = 30U,          /*!< Divided by 31 */
    CGM_CLOCK_DIV_BY_32 = 31U,          /*!< Divided by 32 */
    CGM_CLOCK_DIV_BY_33 = 32U,          /*!< Divided by 33 */
    CGM_CLOCK_DIV_BY_34 = 33U,          /*!< Divided by 34 */
    CGM_CLOCK_DIV_BY_35 = 34U,          /*!< Divided by 35 */
    CGM_CLOCK_DIV_BY_36 = 35U,          /*!< Divided by 36 */
    CGM_CLOCK_DIV_BY_37 = 36U,          /*!< Divided by 37 */
    CGM_CLOCK_DIV_BY_38 = 37U,          /*!< Divided by 38 */
    CGM_CLOCK_DIV_BY_39 = 38U,          /*!< Divided by 39 */
    CGM_CLOCK_DIV_BY_40 = 39U,          /*!< Divided by 40 */
    CGM_CLOCK_DIV_BY_41 = 40U,          /*!< Divided by 41 */
    CGM_CLOCK_DIV_BY_42 = 41U,          /*!< Divided by 42 */
    CGM_CLOCK_DIV_BY_43 = 42U,          /*!< Divided by 43 */
    CGM_CLOCK_DIV_BY_44 = 43U,          /*!< Divided by 44 */
    CGM_CLOCK_DIV_BY_45 = 44U,          /*!< Divided by 45 */
    CGM_CLOCK_DIV_BY_46 = 45U,          /*!< Divided by 46 */
    CGM_CLOCK_DIV_BY_47 = 46U,          /*!< Divided by 47 */
    CGM_CLOCK_DIV_BY_48 = 47U,          /*!< Divided by 48 */
    CGM_CLOCK_DIV_BY_49 = 48U,          /*!< Divided by 49 */
    CGM_CLOCK_DIV_BY_50 = 49U,          /*!< Divided by 50 */
    CGM_CLOCK_DIV_BY_51 = 50U,          /*!< Divided by 51 */
    CGM_CLOCK_DIV_BY_52 = 51U,          /*!< Divided by 52 */
    CGM_CLOCK_DIV_BY_53 = 52U,          /*!< Divided by 53 */
    CGM_CLOCK_DIV_BY_54 = 53U,          /*!< Divided by 54 */
    CGM_CLOCK_DIV_BY_55 = 54U,          /*!< Divided by 55 */
    CGM_CLOCK_DIV_BY_56 = 55U,          /*!< Divided by 56 */
    CGM_CLOCK_DIV_BY_57 = 56U,          /*!< Divided by 57 */
    CGM_CLOCK_DIV_BY_58 = 57U,          /*!< Divided by 58 */
    CGM_CLOCK_DIV_BY_59 = 58U,          /*!< Divided by 59 */
    CGM_CLOCK_DIV_BY_60 = 59U,          /*!< Divided by 60 */
    CGM_CLOCK_DIV_BY_61 = 60U,          /*!< Divided by 61 */
    CGM_CLOCK_DIV_BY_62 = 61U,          /*!< Divided by 62 */
    CGM_CLOCK_DIV_BY_63 = 62U,          /*!< Divided by 63 */
    CGM_CLOCK_DIV_BY_64 = 63U,          /*!< Divided by 64 */
    CGM_CLOCK_DIV_BY_65 = 64U,          /*!< Divided by 65 */
    CGM_CLOCK_DIV_BY_66 = 65U,          /*!< Divided by 66 */
    CGM_CLOCK_DIV_BY_67 = 66U,          /*!< Divided by 67 */
    CGM_CLOCK_DIV_BY_68 = 67U,          /*!< Divided by 68 */
    CGM_CLOCK_DIV_BY_69 = 68U,          /*!< Divided by 69 */
    CGM_CLOCK_DIV_BY_70 = 69U,          /*!< Divided by 70 */
    CGM_CLOCK_DIV_BY_71 = 70U,          /*!< Divided by 71 */
    CGM_CLOCK_DIV_BY_72 = 71U,          /*!< Divided by 72 */
    CGM_CLOCK_DIV_BY_73 = 72U,          /*!< Divided by 73 */
    CGM_CLOCK_DIV_BY_74 = 73U,          /*!< Divided by 74 */
    CGM_CLOCK_DIV_BY_75 = 74U,          /*!< Divided by 75 */
    CGM_CLOCK_DIV_BY_76 = 75U,          /*!< Divided by 76 */
    CGM_CLOCK_DIV_BY_77 = 76U,          /*!< Divided by 77 */
    CGM_CLOCK_DIV_BY_78 = 77U,          /*!< Divided by 78 */
    CGM_CLOCK_DIV_BY_79 = 78U,          /*!< Divided by 79 */
    CGM_CLOCK_DIV_BY_80 = 79U,          /*!< Divided by 80 */
    CGM_CLOCK_DIV_BY_81 = 80U,          /*!< Divided by 81 */
    CGM_CLOCK_DIV_BY_82 = 81U,          /*!< Divided by 82 */
    CGM_CLOCK_DIV_BY_83 = 82U,          /*!< Divided by 83 */
    CGM_CLOCK_DIV_BY_84 = 83U,          /*!< Divided by 84 */
    CGM_CLOCK_DIV_BY_85 = 84U,          /*!< Divided by 85 */
    CGM_CLOCK_DIV_BY_86 = 85U,          /*!< Divided by 86 */
    CGM_CLOCK_DIV_BY_87 = 86U,          /*!< Divided by 87 */
    CGM_CLOCK_DIV_BY_88 = 87U,          /*!< Divided by 88 */
    CGM_CLOCK_DIV_BY_89 = 88U,          /*!< Divided by 89 */
    CGM_CLOCK_DIV_BY_90 = 89U,          /*!< Divided by 90 */
    CGM_CLOCK_DIV_BY_91 = 90U,          /*!< Divided by 91 */
    CGM_CLOCK_DIV_BY_92 = 91U,          /*!< Divided by 92 */
    CGM_CLOCK_DIV_BY_93 = 92U,          /*!< Divided by 93 */
    CGM_CLOCK_DIV_BY_94 = 93U,          /*!< Divided by 94 */
    CGM_CLOCK_DIV_BY_95 = 94U,          /*!< Divided by 95 */
    CGM_CLOCK_DIV_BY_96 = 95U,          /*!< Divided by 96 */
    CGM_CLOCK_DIV_BY_97 = 96U,          /*!< Divided by 97 */
    CGM_CLOCK_DIV_BY_98 = 97U,          /*!< Divided by 98 */
    CGM_CLOCK_DIV_BY_99 = 98U,          /*!< Divided by 99 */
    CGM_CLOCK_DIV_BY_100 = 99U,         /*!< Divided by 100 */
    CGM_CLOCK_DIV_BY_101 = 100U,        /*!< Divided by 101 */
    CGM_CLOCK_DIV_BY_102 = 101U,        /*!< Divided by 102 */
    CGM_CLOCK_DIV_BY_103 = 102U,        /*!< Divided by 103 */
    CGM_CLOCK_DIV_BY_104 = 103U,        /*!< Divided by 104 */
    CGM_CLOCK_DIV_BY_105 = 104U,        /*!< Divided by 105 */
    CGM_CLOCK_DIV_BY_106 = 105U,        /*!< Divided by 106 */
    CGM_CLOCK_DIV_BY_107 = 106U,        /*!< Divided by 107 */
    CGM_CLOCK_DIV_BY_108 = 107U,        /*!< Divided by 108 */
    CGM_CLOCK_DIV_BY_109 = 108U,        /*!< Divided by 109 */
    CGM_CLOCK_DIV_BY_110 = 109U,        /*!< Divided by 110 */
    CGM_CLOCK_DIV_BY_111 = 110U,        /*!< Divided by 111 */
    CGM_CLOCK_DIV_BY_112 = 111U,        /*!< Divided by 112 */
    CGM_CLOCK_DIV_BY_113 = 112U,        /*!< Divided by 113 */
    CGM_CLOCK_DIV_BY_114 = 113U,        /*!< Divided by 114 */
    CGM_CLOCK_DIV_BY_115 = 114U,        /*!< Divided by 115 */
    CGM_CLOCK_DIV_BY_116 = 115U,        /*!< Divided by 116 */
    CGM_CLOCK_DIV_BY_117 = 116U,        /*!< Divided by 117 */
    CGM_CLOCK_DIV_BY_118 = 117U,        /*!< Divided by 118 */
    CGM_CLOCK_DIV_BY_119 = 118U,        /*!< Divided by 119 */
    CGM_CLOCK_DIV_BY_120 = 119U,        /*!< Divided by 120 */
    CGM_CLOCK_DIV_BY_121 = 120U,        /*!< Divided by 121 */
    CGM_CLOCK_DIV_BY_122 = 121U,        /*!< Divided by 122 */
    CGM_CLOCK_DIV_BY_123 = 122U,        /*!< Divided by 123 */
    CGM_CLOCK_DIV_BY_124 = 123U,        /*!< Divided by 124 */
    CGM_CLOCK_DIV_BY_125 = 124U,        /*!< Divided by 125 */
    CGM_CLOCK_DIV_BY_126 = 125U,        /*!< Divided by 126 */
    CGM_CLOCK_DIV_BY_127 = 126U,        /*!< Divided by 127 */
    CGM_CLOCK_DIV_BY_128 = 127U,        /*!< Divided by 128 */
    CGM_CLOCK_DIV_BY_129 = 128U,        /*!< Divided by 129 */
    CGM_CLOCK_DIV_BY_130 = 129U,        /*!< Divided by 130 */
    CGM_CLOCK_DIV_BY_131 = 130U,        /*!< Divided by 131 */
    CGM_CLOCK_DIV_BY_132 = 131U,        /*!< Divided by 132 */
    CGM_CLOCK_DIV_BY_133 = 132U,        /*!< Divided by 133 */
    CGM_CLOCK_DIV_BY_134 = 133U,        /*!< Divided by 134 */
    CGM_CLOCK_DIV_BY_135 = 134U,        /*!< Divided by 135 */
    CGM_CLOCK_DIV_BY_136 = 135U,        /*!< Divided by 136 */
    CGM_CLOCK_DIV_BY_137 = 136U,        /*!< Divided by 137 */
    CGM_CLOCK_DIV_BY_138 = 137U,        /*!< Divided by 138 */
    CGM_CLOCK_DIV_BY_139 = 138U,        /*!< Divided by 139 */
    CGM_CLOCK_DIV_BY_140 = 139U,        /*!< Divided by 140 */
    CGM_CLOCK_DIV_BY_141 = 140U,        /*!< Divided by 141 */
    CGM_CLOCK_DIV_BY_142 = 141U,        /*!< Divided by 142 */
    CGM_CLOCK_DIV_BY_143 = 142U,        /*!< Divided by 143 */
    CGM_CLOCK_DIV_BY_144 = 143U,        /*!< Divided by 144 */
    CGM_CLOCK_DIV_BY_145 = 144U,        /*!< Divided by 145 */
    CGM_CLOCK_DIV_BY_146 = 145U,        /*!< Divided by 146 */
    CGM_CLOCK_DIV_BY_147 = 146U,        /*!< Divided by 147 */
    CGM_CLOCK_DIV_BY_148 = 147U,        /*!< Divided by 148 */
    CGM_CLOCK_DIV_BY_149 = 148U,        /*!< Divided by 149 */
    CGM_CLOCK_DIV_BY_150 = 149U,        /*!< Divided by 150 */
    CGM_CLOCK_DIV_BY_151 = 150U,        /*!< Divided by 151 */
    CGM_CLOCK_DIV_BY_152 = 151U,        /*!< Divided by 152 */
    CGM_CLOCK_DIV_BY_153 = 152U,        /*!< Divided by 153 */
    CGM_CLOCK_DIV_BY_154 = 153U,        /*!< Divided by 154 */
    CGM_CLOCK_DIV_BY_155 = 154U,        /*!< Divided by 155 */
    CGM_CLOCK_DIV_BY_156 = 155U,        /*!< Divided by 156 */
    CGM_CLOCK_DIV_BY_157 = 156U,        /*!< Divided by 157 */
    CGM_CLOCK_DIV_BY_158 = 157U,        /*!< Divided by 158 */
    CGM_CLOCK_DIV_BY_159 = 158U,        /*!< Divided by 159 */
    CGM_CLOCK_DIV_BY_160 = 159U,        /*!< Divided by 160 */
    CGM_CLOCK_DIV_BY_161 = 160U,        /*!< Divided by 161 */
    CGM_CLOCK_DIV_BY_162 = 161U,        /*!< Divided by 162 */
    CGM_CLOCK_DIV_BY_163 = 162U,        /*!< Divided by 163 */
    CGM_CLOCK_DIV_BY_164 = 163U,        /*!< Divided by 164 */
    CGM_CLOCK_DIV_BY_165 = 164U,        /*!< Divided by 165 */
    CGM_CLOCK_DIV_BY_166 = 165U,        /*!< Divided by 166 */
    CGM_CLOCK_DIV_BY_167 = 166U,        /*!< Divided by 167 */
    CGM_CLOCK_DIV_BY_168 = 167U,        /*!< Divided by 168 */
    CGM_CLOCK_DIV_BY_169 = 168U,        /*!< Divided by 169 */
    CGM_CLOCK_DIV_BY_170 = 169U,        /*!< Divided by 170 */
    CGM_CLOCK_DIV_BY_171 = 170U,        /*!< Divided by 171 */
    CGM_CLOCK_DIV_BY_172 = 171U,        /*!< Divided by 172 */
    CGM_CLOCK_DIV_BY_173 = 172U,        /*!< Divided by 173 */
    CGM_CLOCK_DIV_BY_174 = 173U,        /*!< Divided by 174 */
    CGM_CLOCK_DIV_BY_175 = 174U,        /*!< Divided by 175 */
    CGM_CLOCK_DIV_BY_176 = 175U,        /*!< Divided by 176 */
    CGM_CLOCK_DIV_BY_177 = 176U,        /*!< Divided by 177 */
    CGM_CLOCK_DIV_BY_178 = 177U,        /*!< Divided by 178 */
    CGM_CLOCK_DIV_BY_179 = 178U,        /*!< Divided by 179 */
    CGM_CLOCK_DIV_BY_180 = 179U,        /*!< Divided by 180 */
    CGM_CLOCK_DIV_BY_181 = 180U,        /*!< Divided by 181 */
    CGM_CLOCK_DIV_BY_182 = 181U,        /*!< Divided by 182 */
    CGM_CLOCK_DIV_BY_183 = 182U,        /*!< Divided by 183 */
    CGM_CLOCK_DIV_BY_184 = 183U,        /*!< Divided by 184 */
    CGM_CLOCK_DIV_BY_185 = 184U,        /*!< Divided by 185 */
    CGM_CLOCK_DIV_BY_186 = 185U,        /*!< Divided by 186 */
    CGM_CLOCK_DIV_BY_187 = 186U,        /*!< Divided by 187 */
    CGM_CLOCK_DIV_BY_188 = 187U,        /*!< Divided by 188 */
    CGM_CLOCK_DIV_BY_189 = 188U,        /*!< Divided by 189 */
    CGM_CLOCK_DIV_BY_190 = 189U,        /*!< Divided by 190 */
    CGM_CLOCK_DIV_BY_191 = 190U,        /*!< Divided by 191 */
    CGM_CLOCK_DIV_BY_192 = 191U,        /*!< Divided by 192 */
    CGM_CLOCK_DIV_BY_193 = 192U,        /*!< Divided by 193 */
    CGM_CLOCK_DIV_BY_194 = 193U,        /*!< Divided by 194 */
    CGM_CLOCK_DIV_BY_195 = 194U,        /*!< Divided by 195 */
    CGM_CLOCK_DIV_BY_196 = 195U,        /*!< Divided by 196 */
    CGM_CLOCK_DIV_BY_197 = 196U,        /*!< Divided by 197 */
    CGM_CLOCK_DIV_BY_198 = 197U,        /*!< Divided by 198 */
    CGM_CLOCK_DIV_BY_199 = 198U,        /*!< Divided by 199 */
    CGM_CLOCK_DIV_BY_200 = 199U,        /*!< Divided by 200 */
    CGM_CLOCK_DIV_BY_201 = 200U,        /*!< Divided by 201 */
    CGM_CLOCK_DIV_BY_202 = 201U,        /*!< Divided by 202 */
    CGM_CLOCK_DIV_BY_203 = 202U,        /*!< Divided by 203 */
    CGM_CLOCK_DIV_BY_204 = 203U,        /*!< Divided by 204 */
    CGM_CLOCK_DIV_BY_205 = 204U,        /*!< Divided by 205 */
    CGM_CLOCK_DIV_BY_206 = 205U,        /*!< Divided by 206 */
    CGM_CLOCK_DIV_BY_207 = 206U,        /*!< Divided by 207 */
    CGM_CLOCK_DIV_BY_208 = 207U,        /*!< Divided by 208 */
    CGM_CLOCK_DIV_BY_209 = 208U,        /*!< Divided by 209 */
    CGM_CLOCK_DIV_BY_210 = 209U,        /*!< Divided by 210 */
    CGM_CLOCK_DIV_BY_211 = 210U,        /*!< Divided by 211 */
    CGM_CLOCK_DIV_BY_212 = 211U,        /*!< Divided by 212 */
    CGM_CLOCK_DIV_BY_213 = 212U,        /*!< Divided by 213 */
    CGM_CLOCK_DIV_BY_214 = 213U,        /*!< Divided by 214 */
    CGM_CLOCK_DIV_BY_215 = 214U,        /*!< Divided by 215 */
    CGM_CLOCK_DIV_BY_216 = 215U,        /*!< Divided by 216 */
    CGM_CLOCK_DIV_BY_217 = 216U,        /*!< Divided by 217 */
    CGM_CLOCK_DIV_BY_218 = 217U,        /*!< Divided by 218 */
    CGM_CLOCK_DIV_BY_219 = 218U,        /*!< Divided by 219 */
    CGM_CLOCK_DIV_BY_220 = 219U,        /*!< Divided by 220 */
    CGM_CLOCK_DIV_BY_221 = 220U,        /*!< Divided by 221 */
    CGM_CLOCK_DIV_BY_222 = 221U,        /*!< Divided by 222 */
    CGM_CLOCK_DIV_BY_223 = 222U,        /*!< Divided by 223 */
    CGM_CLOCK_DIV_BY_224 = 223U,        /*!< Divided by 224 */
    CGM_CLOCK_DIV_BY_225 = 224U,        /*!< Divided by 225 */
    CGM_CLOCK_DIV_BY_226 = 225U,        /*!< Divided by 226 */
    CGM_CLOCK_DIV_BY_227 = 226U,        /*!< Divided by 227 */
    CGM_CLOCK_DIV_BY_228 = 227U,        /*!< Divided by 228 */
    CGM_CLOCK_DIV_BY_229 = 228U,        /*!< Divided by 229 */
    CGM_CLOCK_DIV_BY_230 = 229U,        /*!< Divided by 230 */
    CGM_CLOCK_DIV_BY_231 = 230U,        /*!< Divided by 231 */
    CGM_CLOCK_DIV_BY_232 = 231U,        /*!< Divided by 232 */
    CGM_CLOCK_DIV_BY_233 = 232U,        /*!< Divided by 233 */
    CGM_CLOCK_DIV_BY_234 = 233U,        /*!< Divided by 234 */
    CGM_CLOCK_DIV_BY_235 = 234U,        /*!< Divided by 235 */
    CGM_CLOCK_DIV_BY_236 = 235U,        /*!< Divided by 236 */
    CGM_CLOCK_DIV_BY_237 = 236U,        /*!< Divided by 237 */
    CGM_CLOCK_DIV_BY_238 = 237U,        /*!< Divided by 238 */
    CGM_CLOCK_DIV_BY_239 = 238U,        /*!< Divided by 239 */
    CGM_CLOCK_DIV_BY_240 = 239U,        /*!< Divided by 240 */
    CGM_CLOCK_DIV_BY_241 = 240U,        /*!< Divided by 241 */
    CGM_CLOCK_DIV_BY_242 = 241U,        /*!< Divided by 242 */
    CGM_CLOCK_DIV_BY_243 = 242U,        /*!< Divided by 243 */
    CGM_CLOCK_DIV_BY_244 = 243U,        /*!< Divided by 244 */
    CGM_CLOCK_DIV_BY_245 = 244U,        /*!< Divided by 245 */
    CGM_CLOCK_DIV_BY_246 = 245U,        /*!< Divided by 246 */
    CGM_CLOCK_DIV_BY_247 = 246U,        /*!< Divided by 247 */
    CGM_CLOCK_DIV_BY_248 = 247U,        /*!< Divided by 248 */
    CGM_CLOCK_DIV_BY_249 = 248U,        /*!< Divided by 249 */
    CGM_CLOCK_DIV_BY_250 = 249U,        /*!< Divided by 250 */
    CGM_CLOCK_DIV_BY_251 = 250U,        /*!< Divided by 251 */
    CGM_CLOCK_DIV_BY_252 = 251U,        /*!< Divided by 252 */
    CGM_CLOCK_DIV_BY_253 = 252U,        /*!< Divided by 253 */
    CGM_CLOCK_DIV_BY_254 = 253U,        /*!< Divided by 254 */
    CGM_CLOCK_DIV_BY_255 = 254U,        /*!< Divided by 255 */
    CGM_CLOCK_DISABLE    = 255U,        /*!< Disable system clock divider */

}cgm_clk_div_t;


/*!
 * @brief CGM auxiliary clock selector.
 * Implements cgm_clk_sel_t_Class
 */
typedef uint8_t cgm_clk_sel_t;
#define CGM_ADCx_SOURCE_IRCOSC           0U
#define CGM_ADCx_SOURCE_XOSC             1U
#define CGM_ADCx_SOURCE_PLL0_PHI0        2U
#define CGM_ADCx_SOURCE_PLL1_PHI0        4U
#define CGM_SPI0_SOURCE_F40              0U
#define CGM_SPI0_SOURCE_FXOSC            1U
#define CGM_FLEXCANx_SOURCE_FS80         0U
#define CGM_FLEXCANx_SOURCE_FXOSC        1U
#define CGM_FLEXCANx_SOURCE_PLL0_PHI0    0U
#define CGM_FLEXRAYx_SOURCE_XOSC         0U
#define CGM_FLEXRAYx_SOURCE_PLL0_PHI0    0U
#define CGM_LFASTx_SOURCE_XOSC           1U
#define CGM_LFASTx_SOURCE_PLL0_PHI0      2U
#define CGM_LFASTx_SOURCE_SYS_PIN        5U
#define CGM_USDHCx_SOURCE_F40            0U
#define CGM_USDHCx_SOURCE_FXOSC          1U
#define CGM_ENETx_SOURCE_F40             0U
#define CGM_ENETx_SOURCE_IRCOSC          0U
#define CGM_ENETx_SOURCE_FXOSC           1U
#define CGM_ENETx_SOURCE_XOSC            1U
#define CGM_ENETx_SOURCE_PLL0_PHI0       2U
#define CGM_ENETx_SOURCE_PLL0_PHI1       3U
#define CGM_ENETx_SOURCE_PLL1_PHI0       4U
#define CGM_SENTx_SOURCE_PLL0_PHI0       0U
#define CGM_SGENx_SOURCE_IRCOSC          0U
#define CGM_SGENx_SOURCE_XOSC            1U
#define CGM_SGENx_SOURCE_PLL0_PHI0       2U
#define CGM_SGENx_SOURCE_PLL1_PHI0       4U
#define CGM_MOTC_SOURCE_IRCOSC           0U
#define CGM_MOTC_SOURCE_XOSC             1U
#define CGM_MOTC_SOURCE_PLL0_PHI0        2U
#define CGM_MOTC_SOURCE_PLL1_PHI0        4U
#define CGM_PLL_REFERENCE_FIRC           0U
#define CGM_PLL_REFERENCE_IRCOSC         0U
#define CGM_PLL_REFERENCE_FXOSC          1U
#define CGM_PLL_REFERENCE_XOSC           1U
#define CGM_PLL_REFERENCE_PLL0_PHI1      3U
#define CGM_CLOCKOUT0_SRC_FXOSC          0U
#define CGM_CLOCKOUT0_SRC_IRCOSC         0U
#define CGM_CLOCKOUT0_SRC_XOSC           1U
#define CGM_CLOCKOUT0_SRC_FIRC           1U
#define CGM_CLOCKOUT0_SRC_PLL0_PHI0      2U
#define CGM_CLOCKOUT0_SRC_PLL_PHI0       2U
#define CGM_CLOCKOUT0_SRC_S160           3U
#define CGM_CLOCKOUT0_SRC_PLL1_PHI0      4U
#define CGM_CLOCKOUT0_SRC_RTC            4U
#define CGM_CLOCKOUT0_SRC_PLL_PHI1       5U
#define CGM_CLOCKOUT0_SRC_SXOSC          6U
#define CGM_CLOCKOUT0_SRC_SIRC           7U
#define CGM_CLOCKOUT0_SRC_Z4_CLK         8U
#define CGM_CLOCKOUT0_SRC_Z4A_CLK        8U
#define CGM_CLOCKOUT0_SRC_Z4B_CLK        9U
#define CGM_CLOCKOUT0_SRC_HSM_CLK        10U
#define CGM_CLOCKOUT0_SRC_S40_CLK        12U
#define CGM_CLOCKOUT0_SRC_F40_CLK        13U
#define CGM_CLOCKOUT0_SRC_F80_CLK        14U
#define CGM_CLOCKOUT0_SRC_FS80_CLK       15U
#define CGM_CLOCKOUT1_SRC_FXOSC_DIVIDED  0U
#define CGM_CLOCKOUT1_SRC_FIRC_DIVIDED   1U
#define CGM_CLOCKOUT1_SRC_SXOSC          2U
#define CGM_CLOCKOUT1_SRC_SIRC           3U
#define CGM_CLOCKOUT1_SRC_RTC            4U
#define CGM_CLOCKOUT1_SRC_LPU            5U
#define CGM_CLOCKOUT1_Z2_CORE            6U
#define CGM_CLOCKOUT1_FXOSC_ANA          7U
#define CGM_CLOCKOUT1_FXOSC              8U
#define CGM_CLOCKOUT1_FIRC               9U
#define CGM_CLOCKOUT1_SRC_SXOSC_DIVIDED  10U
#define CGM_CLOCKOUT1_SRC_SIRC_DIVIDED   11U
#define CGM_CLOCKOUT1_SRC_CAN0_CHI       12U
#define CGM_CLOCKOUT1_SRC_CAN0_PE        13U
#define CGM_CLOCKOUT1_SRC_PLL_PHI0       14U

/*!
 * @brief CGM configuration structure.
 * Implements cgm_config_t_Class
 */
typedef struct
{
    cgm_clk_div_t        sc_dc0;              /*!< CGM system clock divider 0.  */
    cgm_clk_div_t        sc_dc1;              /*!< CGM system clock divider 1.  */
    cgm_clk_div_t        sc_dc2;              /*!< CGM system clock divider 2.  */
    cgm_clk_div_t        sc_dc3;              /*!< CGM system clock divider 3.  */
    cgm_clk_div_t        sc_dc4;              /*!< CGM system clock divider 4.  */
    cgm_clk_div_t        sc_dc5;              /*!< CGM system clock divider 5.  */
    cgm_clk_div_t        sc_dc6;              /*!< CGM system clock divider 6.  */

    cgm_clk_sel_t       ac0_sc;               /*!< CGM auxiliary clock selector 0.                 */
    cgm_clk_div_t       ac0_dc0;              /*!< CGM auxiliary clock selector 0, first  divider  */
    cgm_clk_div_t       ac0_dc1;              /*!< CGM auxiliary clock selector 0, second divider  */
    cgm_clk_div_t       ac0_dc2;              /*!< CGM auxiliary clock selector 0, third  divider  */

    cgm_clk_sel_t       ac1_sc;               /*!< CGM auxiliary clock selector 1.                 */
    cgm_clk_div_t       ac1_dc0;              /*!< CGM auxiliary clock selector 1, first  divider  */
    cgm_clk_div_t       ac1_dc1;              /*!< CGM auxiliary clock selector 1, second divider  */

    cgm_clk_sel_t       ac2_sc;               /*!< CGM auxiliary clock selector 2.                 */
    cgm_clk_div_t       ac2_dc0;              /*!< CGM auxiliary clock selector 2, first  divider  */

    cgm_clk_sel_t       ac3_sc;               /*!< CGM auxiliary clock selector 3.                 */

    cgm_clk_sel_t       ac4_sc;               /*!< CGM auxiliary clock selector 4.                 */

    cgm_clk_sel_t       ac5_sc;               /*!< CGM auxiliary clock selector 5.                 */
    cgm_clk_div_t       ac5_dc0;              /*!< CGM auxiliary clock selector 5, first  divider  */

    cgm_clk_sel_t       ac6_sc;               /*!< CGM auxiliary clock selector 6.                 */
    cgm_clk_div_t       ac6_dc0;              /*!< CGM auxiliary clock selector 6, first  divider  */

    cgm_clk_sel_t       ac8_sc;               /*!< CGM auxiliary clock selector 8.                 */
    cgm_clk_sel_t       ac9_sc;               /*!< CGM auxiliary clock selector 9.                 */

    cgm_clk_sel_t       ac10_sc;              /*!< CGM auxiliary clock selector 10.                */
    cgm_clk_div_t       ac10_dc0;             /*!< CGM auxiliary clock selector 10, first  divider */

    cgm_clk_sel_t       ac11_sc;              /*!< CGM auxiliary clock selector 11.                */
    cgm_clk_div_t       ac11_dc0;             /*!< CGM auxiliary clock selector 11, first  divider */

    cgm_clk_sel_t       clkout0_sc;           /*!< Clock out 0 selector.                           */
    cgm_clk_div_t       clkout0_dc;           /*!< Clock out 0 divider.                            */

    cgm_clk_sel_t       clkout1_sc;           /*!< Clock out 1 selector.                           */
    cgm_clk_div_t       clkout1_dc;           /*!< Clock out 1 divider.                            */


} cgm_config_t;

/*!
 * @brief IRC clock divider
 * Implements cgmcs_irc_clk_div_t_Class
 */
typedef enum {

    IRC_CLOCK_DIV_BY_1  = 0U,         /*!< Divided by 1  */
    IRC_CLOCK_DIV_BY_2  = 1U,         /*!< Divided by 2  */
    IRC_CLOCK_DIV_BY_3  = 2U,         /*!< Divided by 3  */
    IRC_CLOCK_DIV_BY_4  = 3U,         /*!< Divided by 4  */
    IRC_CLOCK_DIV_BY_5  = 4U,         /*!< Divided by 5  */
    IRC_CLOCK_DIV_BY_6  = 5U,         /*!< Divided by 6  */
    IRC_CLOCK_DIV_BY_7  = 6U,         /*!< Divided by 7  */
    IRC_CLOCK_DIV_BY_8  = 7U,         /*!< Divided by 8  */
    IRC_CLOCK_DIV_BY_9  = 8U,         /*!< Divided by 9  */
    IRC_CLOCK_DIV_BY_10 = 9U,         /*!< Divided by 10 */
    IRC_CLOCK_DIV_BY_11 = 10U,        /*!< Divided by 11 */
    IRC_CLOCK_DIV_BY_12 = 11U,        /*!< Divided by 12 */
    IRC_CLOCK_DIV_BY_13 = 12U,        /*!< Divided by 13 */
    IRC_CLOCK_DIV_BY_14 = 13U,        /*!< Divided by 14 */
    IRC_CLOCK_DIV_BY_15 = 14U,        /*!< Divided by 15 */
    IRC_CLOCK_DIV_BY_16 = 15U,        /*!< Divided by 16 */
    IRC_CLOCK_DIV_BY_17 = 16U,        /*!< Divided by 17 */
    IRC_CLOCK_DIV_BY_18 = 17U,        /*!< Divided by 18 */
    IRC_CLOCK_DIV_BY_19 = 18U,        /*!< Divided by 19 */
    IRC_CLOCK_DIV_BY_20 = 19U,        /*!< Divided by 20 */
    IRC_CLOCK_DIV_BY_21 = 20U,        /*!< Divided by 21 */
    IRC_CLOCK_DIV_BY_22 = 21U,        /*!< Divided by 22 */
    IRC_CLOCK_DIV_BY_23 = 22U,        /*!< Divided by 23 */
    IRC_CLOCK_DIV_BY_24 = 23U,        /*!< Divided by 24 */
    IRC_CLOCK_DIV_BY_25 = 24U,        /*!< Divided by 25 */
    IRC_CLOCK_DIV_BY_26 = 25U,        /*!< Divided by 26 */
    IRC_CLOCK_DIV_BY_27 = 26U,        /*!< Divided by 27 */
    IRC_CLOCK_DIV_BY_28 = 27U,        /*!< Divided by 28 */
    IRC_CLOCK_DIV_BY_29 = 28U,        /*!< Divided by 29 */
    IRC_CLOCK_DIV_BY_30 = 29U,        /*!< Divided by 30 */
    IRC_CLOCK_DIV_BY_31 = 30U,        /*!< Divided by 31 */
    IRC_CLOCK_DIV_BY_32 = 31U,        /*!< Divided by 32 */

}cgmcs_irc_clk_div_t;


/*!
 * @brief IRC clock trimming value
 * Implements cgmcs_irc_clk_trim_t_Class
 */
typedef enum {

    IRC_CLOCK_TRIM_BY_MINUS_15 = 31U,         /*!< Trim by minus 15 */
    IRC_CLOCK_TRIM_BY_MINUS_14 = 30U,         /*!< Trim by minus 14 */
    IRC_CLOCK_TRIM_BY_MINUS_13 = 29U,         /*!< Trim by minus 13 */
    IRC_CLOCK_TRIM_BY_MINUS_12 = 28U,         /*!< Trim by minus 12 */
    IRC_CLOCK_TRIM_BY_MINUS_11 = 27U,         /*!< Trim by minus 11 */
    IRC_CLOCK_TRIM_BY_MINUS_10 = 26U,         /*!< Trim by minus 10 */
    IRC_CLOCK_TRIM_BY_MINUS_9  = 25U,         /*!< Trim by minus 9  */
    IRC_CLOCK_TRIM_BY_MINUS_8  = 24U,         /*!< Trim by minus 8  */
    IRC_CLOCK_TRIM_BY_MINUS_7  = 23U,         /*!< Trim by minus 7  */
    IRC_CLOCK_TRIM_BY_MINUS_6  = 22U,         /*!< Trim by minus 6  */
    IRC_CLOCK_TRIM_BY_MINUS_5  = 21U,         /*!< Trim by minus 5  */
    IRC_CLOCK_TRIM_BY_MINUS_4  = 20U,         /*!< Trim by minus 4  */
    IRC_CLOCK_TRIM_BY_MINUS_3  = 19U,         /*!< Trim by minus 3  */
    IRC_CLOCK_TRIM_BY_MINUS_2  = 18U,         /*!< Trim by minus 2  */
    IRC_CLOCK_TRIM_BY_MINUS_1  = 17U,         /*!< Trim by minus 1  */
    IRC_CLOCK_NO_TRIMMING      = 16U,         /*!< No trimming      */
    IRC_CLOCK_TRIM_BY_PLUS_1   = 1U,          /*!< Trim by plus 1   */
    IRC_CLOCK_TRIM_BY_PLUS_2   = 2U,          /*!< Trim by plus 2   */
    IRC_CLOCK_TRIM_BY_PLUS_3   = 3U,          /*!< Trim by plus 3   */
    IRC_CLOCK_TRIM_BY_PLUS_4   = 4U,          /*!< Trim by plus 4   */
    IRC_CLOCK_TRIM_BY_PLUS_5   = 5U,          /*!< Trim by plus 5   */
    IRC_CLOCK_TRIM_BY_PLUS_6   = 6U,          /*!< Trim by plus 6   */
    IRC_CLOCK_TRIM_BY_PLUS_7   = 7U,          /*!< Trim by plus 7   */
    IRC_CLOCK_TRIM_BY_PLUS_8   = 8U,          /*!< Trim by plus 8   */
    IRC_CLOCK_TRIM_BY_PLUS_9   = 9U,          /*!< Trim by plus 9   */
    IRC_CLOCK_TRIM_BY_PLUS_10  = 10U,         /*!< Trim by plus 10  */
    IRC_CLOCK_TRIM_BY_PLUS_11  = 11U,         /*!< Trim by plus 11  */
    IRC_CLOCK_TRIM_BY_PLUS_12  = 12U,         /*!< Trim by plus 12  */
    IRC_CLOCK_TRIM_BY_PLUS_13  = 13U,         /*!< Trim by plus 13  */
    IRC_CLOCK_TRIM_BY_PLUS_14  = 14U,         /*!< Trim by plus 14  */
    IRC_CLOCK_TRIM_BY_PLUS_15  = 15U,         /*!< Trim by plus 15  */

}cgmcs_irc_clk_trim_t;


/*!
 * @brief CGM Clock Source IRC configuration structure.
 * Implements cgmcs_irc_config_t_Class
 */
typedef struct
{
    cgmcs_irc_clk_div_t      divider;          /*!< SIRC divider value.  */
    cgmcs_irc_clk_trim_t     trimmingValue;    /*!< User trim value.  */

} cgmcs_irc_config_t;

/*!
 * @brief XOSC clock divider
 * Implements cgmcs_xosc_clk_div_t_Class
 */
typedef enum {

    XOSC_CLOCK_DIV_BY_1  = 0U,         /*!< Divided by 1  */
    XOSC_CLOCK_DIV_BY_2  = 1U,         /*!< Divided by 2  */
    XOSC_CLOCK_DIV_BY_3  = 2U,         /*!< Divided by 3  */
    XOSC_CLOCK_DIV_BY_4  = 3U,         /*!< Divided by 4  */
    XOSC_CLOCK_DIV_BY_5  = 4U,         /*!< Divided by 5  */
    XOSC_CLOCK_DIV_BY_6  = 5U,         /*!< Divided by 6  */
    XOSC_CLOCK_DIV_BY_7  = 6U,         /*!< Divided by 7  */
    XOSC_CLOCK_DIV_BY_8  = 7U,         /*!< Divided by 8  */
    XOSC_CLOCK_DIV_BY_9  = 8U,         /*!< Divided by 9  */
    XOSC_CLOCK_DIV_BY_10 = 9U,         /*!< Divided by 10 */
    XOSC_CLOCK_DIV_BY_11 = 10U,        /*!< Divided by 11 */
    XOSC_CLOCK_DIV_BY_12 = 11U,        /*!< Divided by 12 */
    XOSC_CLOCK_DIV_BY_13 = 12U,        /*!< Divided by 13 */
    XOSC_CLOCK_DIV_BY_14 = 13U,        /*!< Divided by 14 */
    XOSC_CLOCK_DIV_BY_15 = 14U,        /*!< Divided by 15 */
    XOSC_CLOCK_DIV_BY_16 = 15U,        /*!< Divided by 16 */
    XOSC_CLOCK_DIV_BY_17 = 16U,        /*!< Divided by 17 */
    XOSC_CLOCK_DIV_BY_18 = 17U,        /*!< Divided by 18 */
    XOSC_CLOCK_DIV_BY_19 = 18U,        /*!< Divided by 19 */
    XOSC_CLOCK_DIV_BY_20 = 19U,        /*!< Divided by 20 */
    XOSC_CLOCK_DIV_BY_21 = 20U,        /*!< Divided by 21 */
    XOSC_CLOCK_DIV_BY_22 = 21U,        /*!< Divided by 22 */
    XOSC_CLOCK_DIV_BY_23 = 22U,        /*!< Divided by 23 */
    XOSC_CLOCK_DIV_BY_24 = 23U,        /*!< Divided by 24 */
    XOSC_CLOCK_DIV_BY_25 = 24U,        /*!< Divided by 25 */
    XOSC_CLOCK_DIV_BY_26 = 25U,        /*!< Divided by 26 */
    XOSC_CLOCK_DIV_BY_27 = 26U,        /*!< Divided by 27 */
    XOSC_CLOCK_DIV_BY_28 = 27U,        /*!< Divided by 28 */
    XOSC_CLOCK_DIV_BY_29 = 28U,        /*!< Divided by 29 */
    XOSC_CLOCK_DIV_BY_30 = 29U,        /*!< Divided by 30 */
    XOSC_CLOCK_DIV_BY_31 = 30U,        /*!< Divided by 31 */
    XOSC_CLOCK_DIV_BY_32 = 31U,        /*!< Divided by 32 */

}cgmcs_xosc_clk_div_t;


/*!
 * @brief XOSC bypass type.
 * Implements cgmcs_xosc_bypass_t_Class
 */
typedef enum
{
    XOSC_USE_CRYSTAL     = 0U,
    XOSC_USE_EXTAL       = 1U,

}cgmcs_xosc_bypass_t;

/*!
 * @brief XOSC mode.
 * Implements cgmcs_xosc_mode_t_Class
 */
typedef enum
{
    XOSC_FULL_SWING_PIERCE_MODE       = 0U,
    XOSC_LOOP_CONTROLLED_PIERCE_MODE  = 1U,

}cgmcs_xosc_mode_t;

/*!
 * @brief CGM Clock Source XOSC configuration structure.
 * Implements cgmcs_xosc_config_t_Class
 */
typedef struct
{
    uint32_t                  freq;               /*!< External oscillator frequency. */

    bool                      autoLevelControl;   /*!< Auto level control. */
    bool                      monitor;            /*!< Enable/disable monitor.  */
    uint16_t                  startupDelay;       /*!< Startup stabilization time. */
    cgmcs_xosc_clk_div_t      divider;            /*!< XOSC divider value. */
    cgmcs_xosc_bypass_t       bypassOption;       /*!< XOSC option */
    cgmcs_xosc_mode_t         mode;               /*!< XOSC mode */

} cgmcs_xosc_config_t;

/*!
 * @brief plldig clock divider
 * Implements cgmcs_plldig_clk_prediv_t_Class
 */
typedef enum {

	PLLDIG_CLOCK_PREDIV_DISABLE  = 0U,     /*!< Disable predivider        */
	PLLDIG_CLOCK_PREDIV_BY_1     = 1U,     /*!< Input clock divided by 1  */
    PLLDIG_CLOCK_PREDIV_BY_2     = 2U,     /*!< Input clock divided by 2  */
    PLLDIG_CLOCK_PREDIV_BY_3     = 3U,     /*!< Input clock divided by 3  */
    PLLDIG_CLOCK_PREDIV_BY_4     = 4U,     /*!< Input clock divided by 4  */
    PLLDIG_CLOCK_PREDIV_BY_5     = 5U,     /*!< Input clock divided by 5  */
    PLLDIG_CLOCK_PREDIV_BY_6     = 6U,     /*!< Input clock divided by 6  */
    PLLDIG_CLOCK_PREDIV_BY_7     = 7U,     /*!< Input clock divided by 7  */

}cgmcs_plldig_clk_prediv_t;


/*!
 * @brief plldig clockout divider
 * Implements cgmcs_plldig_clkout_div_t_Class
 */
typedef enum {

    PLLDIG_PHI_DIV_BY_1  = 0U,           /*!< Divided by 1  */
    PLLDIG_PHI_DIV_BY_2  = 1U,           /*!< Divided by 2  */
    PLLDIG_PHI_DIV_BY_3  = 2U,           /*!< Divided by 3  */
    PLLDIG_PHI_DIV_BY_4  = 3U,           /*!< Divided by 4  */
    PLLDIG_PHI_DIV_BY_5  = 4U,           /*!< Divided by 5  */
    PLLDIG_PHI_DIV_BY_6  = 5U,           /*!< Divided by 6  */
    PLLDIG_PHI_DIV_BY_7  = 6U,           /*!< Divided by 7  */
    PLLDIG_PHI_DIV_BY_8  = 7U,           /*!< Divided by 8  */
    PLLDIG_PHI_DIV_BY_9  = 8U,           /*!< Divided by 9  */
    PLLDIG_PHI_DIV_BY_10 = 9U,           /*!< Divided by 10 */
    PLLDIG_PHI_DIV_BY_11 = 10U,          /*!< Divided by 11 */
    PLLDIG_PHI_DIV_BY_12 = 11U,          /*!< Divided by 12 */
    PLLDIG_PHI_DIV_BY_13 = 12U,          /*!< Divided by 13 */
    PLLDIG_PHI_DIV_BY_14 = 13U,          /*!< Divided by 14 */
    PLLDIG_PHI_DIV_BY_15 = 14U,          /*!< Divided by 15 */
    PLLDIG_PHI_DIV_BY_16 = 15U,          /*!< Divided by 16 */
    PLLDIG_PHI_DIV_BY_17 = 16U,          /*!< Divided by 17 */
    PLLDIG_PHI_DIV_BY_18 = 17U,          /*!< Divided by 18 */
    PLLDIG_PHI_DIV_BY_19 = 18U,          /*!< Divided by 19 */
    PLLDIG_PHI_DIV_BY_20 = 19U,          /*!< Divided by 20 */
    PLLDIG_PHI_DIV_BY_21 = 20U,          /*!< Divided by 21 */
    PLLDIG_PHI_DIV_BY_22 = 21U,          /*!< Divided by 22 */
    PLLDIG_PHI_DIV_BY_23 = 22U,          /*!< Divided by 23 */
    PLLDIG_PHI_DIV_BY_24 = 23U,          /*!< Divided by 24 */
    PLLDIG_PHI_DIV_BY_25 = 24U,          /*!< Divided by 25 */
    PLLDIG_PHI_DIV_BY_26 = 25U,          /*!< Divided by 26 */
    PLLDIG_PHI_DIV_BY_27 = 26U,          /*!< Divided by 27 */
    PLLDIG_PHI_DIV_BY_28 = 27U,          /*!< Divided by 28 */
    PLLDIG_PHI_DIV_BY_29 = 28U,          /*!< Divided by 29 */
    PLLDIG_PHI_DIV_BY_30 = 29U,          /*!< Divided by 30 */
    PLLDIG_PHI_DIV_BY_31 = 30U,          /*!< Divided by 31 */
    PLLDIG_PHI_DIV_BY_32 = 31U,          /*!< Divided by 32 */
    PLLDIG_PHI_DIV_BY_33 = 32U,          /*!< Divided by 33 */
    PLLDIG_PHI_DIV_BY_34 = 33U,          /*!< Divided by 34 */
    PLLDIG_PHI_DIV_BY_35 = 34U,          /*!< Divided by 35 */
    PLLDIG_PHI_DIV_BY_36 = 35U,          /*!< Divided by 36 */
    PLLDIG_PHI_DIV_BY_37 = 36U,          /*!< Divided by 37 */
    PLLDIG_PHI_DIV_BY_38 = 37U,          /*!< Divided by 38 */
    PLLDIG_PHI_DIV_BY_39 = 38U,          /*!< Divided by 39 */
    PLLDIG_PHI_DIV_BY_40 = 39U,          /*!< Divided by 40 */
    PLLDIG_PHI_DIV_BY_41 = 40U,          /*!< Divided by 41 */
    PLLDIG_PHI_DIV_BY_42 = 41U,          /*!< Divided by 42 */
    PLLDIG_PHI_DIV_BY_43 = 42U,          /*!< Divided by 43 */
    PLLDIG_PHI_DIV_BY_44 = 43U,          /*!< Divided by 44 */
    PLLDIG_PHI_DIV_BY_45 = 44U,          /*!< Divided by 45 */
    PLLDIG_PHI_DIV_BY_46 = 45U,          /*!< Divided by 46 */
    PLLDIG_PHI_DIV_BY_47 = 46U,          /*!< Divided by 47 */
    PLLDIG_PHI_DIV_BY_48 = 47U,          /*!< Divided by 48 */
    PLLDIG_PHI_DIV_BY_49 = 48U,          /*!< Divided by 49 */
    PLLDIG_PHI_DIV_BY_50 = 49U,          /*!< Divided by 50 */
    PLLDIG_PHI_DIV_BY_51 = 50U,          /*!< Divided by 51 */
    PLLDIG_PHI_DIV_BY_52 = 51U,          /*!< Divided by 52 */
    PLLDIG_PHI_DIV_BY_53 = 52U,          /*!< Divided by 53 */
    PLLDIG_PHI_DIV_BY_54 = 53U,          /*!< Divided by 54 */
    PLLDIG_PHI_DIV_BY_55 = 54U,          /*!< Divided by 55 */
    PLLDIG_PHI_DIV_BY_56 = 55U,          /*!< Divided by 56 */
    PLLDIG_PHI_DIV_BY_57 = 56U,          /*!< Divided by 57 */
    PLLDIG_PHI_DIV_BY_58 = 57U,          /*!< Divided by 58 */
    PLLDIG_PHI_DIV_BY_59 = 58U,          /*!< Divided by 59 */
    PLLDIG_PHI_DIV_BY_60 = 59U,          /*!< Divided by 60 */
    PLLDIG_PHI_DIV_BY_61 = 60U,          /*!< Divided by 61 */
    PLLDIG_PHI_DIV_BY_62 = 61U,          /*!< Divided by 62 */
    PLLDIG_PHI_DIV_BY_63 = 62U,          /*!< Divided by 63 */
    PLLDIG_PHI_DIV_BY_64 = 63U,          /*!< Divided by 64 */

}cgmcs_plldig_clkout_div_t;


/*!
 * @brief Modulation mode.
 * Implements cgmcs_plldig_mod_type_t_Class
 */
typedef enum
{
    CENTRE_SPREAD_MODULATION       = 0U,
    DOWN_SPREAD_MODULATION         = 1U,

}cgmcs_plldig_mod_type_t;



/*!
 * @brief CGM Clock Source PLLDIG configuration structure.
 * Implements cgmcs_plldig_config_t_Class
 */
typedef struct
{
    cgmcs_plldig_clk_prediv_t  predivider;                     /*!< Input clock predivider. (PREDIV) */
    uint16_t                   denominatorFracLoopDiv;         /*!< Denominator of fractional loop division factor. (MFDEN) */
    uint16_t                   numeratorFracLoopDiv;           /*!< Numerator of fractional loop division factor (MFN) */
    uint8_t                    mulFactorDiv;                   /*!< Multiplication factor divider (MFD) */

    bool                       fracDivider;                    /*!< Enable fractional divider */
    uint16_t                   fracDividerValue;               /*!< Fractional divider value */

    cgmcs_plldig_clkout_div_t  phi0Divider;                    /*!< Divider for clockout0 - PHI (RFDPHI) */
    cgmcs_plldig_clkout_div_t  phi1Divider;                    /*!< Divider for clockout1 - PHI1 (RFDPHI1) */

    bool                       modulation;                     /*!< Enable/disable modulation */
    cgmcs_plldig_mod_type_t    modulationType;                 /*!< Modulation type */
    uint16_t                   modulationPeriod;               /*!< Stepsize - modulation period */
    uint16_t                   incrementStep;                  /*!< Stepno  - step no */

    bool                       sigmaDelta;                     /*!< Sigma Delta Modulation Enable */
    bool                       secondOrderSigmaDelta;          /*!< Second Order Sigma Delta Modulation Enable */
    bool                       thirdOrderSigmaDelta;           /*!< Third Order Sigma Delta Modulation Enable */

    bool                       ditherControl;                  /*!< Dither control enable */
    uint8_t                    ditherControlValue;             /*!< Dither control value */

} cgmcs_plldig_config_t;

/*!
 * @brief CGM Clock Sources configuration structure.
 * Implements cgmcs_config_t_Class
 */
typedef struct
{
    cgmcs_irc_config_t                irc0Config;
    cgmcs_irc_config_t                irc1Config;

    cgmcs_xosc_config_t               xosc0Config;
    cgmcs_xosc_config_t               xosc1Config;

    cgmcs_plldig_config_t             pll0Config;
    cgmcs_plldig_config_t             pll1Config;

} cgmcs_config_t;

/*!
 * @brief Clock configuration structure.
 * Implements clock_manager_user_config_t_Class
 */
typedef struct
{
    mc_me_config_t              mcmeConfig;     /*!< MC_ME configuration.                             */
    cgm_config_t                cgmConfig;      /*!< CGM configuration.                               */
    cgmcs_config_t              cgmcsConfig;    /*!< CGM Clock Sources configuration.                 */
} clock_manager_user_config_t;


/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*! @}*/

#endif /* CLOCK_MPC5748G_H */
/*******************************************************************************
 * EOF
 ******************************************************************************/

