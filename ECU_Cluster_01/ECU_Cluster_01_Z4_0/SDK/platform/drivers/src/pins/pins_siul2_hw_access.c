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

#include "pins_siul2_hw_access.h"
#include "pins_gpio_hw_access.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PINS_Init
 * Description   : This function configures the pin feature with the options
 * provided in the given structure.
 *
 *END**************************************************************************/
status_t PINS_Init(const pin_settings_config_t * config)
{
    uint32_t pinsValues = 0U;
    uint32_t pinsValuesInput = 0U;
    uint8_t inputMuxIterator;
    uint32_t pinIntValue;
    DEV_ASSERT(config->pinPortIdx < SIUL2_MSCR_COUNT);
#if FEATURE_SIUL2_HAS_DDR_PAD
    pinsValues |= SIUL2_MSCR_CRPOINT_TRIM(config->ddrConfiguration.crosspointAdjustment);
    pinsValues |= SIUL2_MSCR_DCYCLE_TRIM(config->ddrConfiguration.trimmingAdjustment);
    pinsValues |= SIUL2_MSCR_DDR_ODT(config->odtSelect);
    pinsValues |= SIUL2_MSCR_DDR_SEL(config->ddrConfiguration.ddrSelection);
    pinsValues |= SIUL2_MSCR_DDR_INPUT(config->inputMode);
    pinsValues |= SIUL2_MSCR_DDR_DO_TRIM(config->ddrConfiguration.trimmingDelay);
#endif
#if FEATURE_PINS_HAS_DRIVE_STRENGTH_CONTROL
    pinsValues |= SIUL2_MSCR_DSE(config->driveSelect);
#endif
#if FEATURE_SIUL2_HAS_PULL_KEEPER
    pinsValues |= SIUL2_MSCR_PKE(config->pullKeepEnable);
    pinsValues |= SIUL2_MSCR_PUE(config->pullKeepSelect);
    pinsValues |= SIUL2_MSCR_PUS(config->pullSelect);
#endif
#if FEATURE_PINS_HAS_PULL_SELECTION
    if (config->pullConfig != PORT_INTERNAL_PULL_NOT_ENABLED)
    {
        pinsValues |= SIUL2_MSCR_PUE(1);
        if (config->pullConfig == PORT_INTERNAL_PULL_UP_ENABLED)
        {
            pinsValues |= SIUL2_MSCR_PUS(1);
        }
        else
        {
            /* Pull down is default */
        }
    }
    else
    {
        /* default disable pull resistors */
    }

#endif /* FEATURE_PINS_HAS_PULL_SELECTION */

#if FEATURE_PINS_HAS_SLEW_RATE
    pinsValues |= SIUL2_MSCR_SRE(config->rateSelect);
#endif
#if FEATURE_SIUL2_HAS_SLEW_RATE_CONTROL
    pinsValues |= SIUL2_MSCR_SRC(config->slewRateCtrlSel);
#endif
#if FEATURE_SIUL2_HAS_INVERT_DATA_OUTPUT
    pinsValues |= SIUL2_MSCR_INV(config->invertOutput);
#endif
#if FEATURE_SIUL2_HAS_SAFE_MODE_CONTROL
    pinsValues |= SIUL2_MSCR_SMC(config->safeMode);
#endif
#if FEATURE_SIUL2_HAS_HYSTERESIS
    pinsValues |= SIUL2_MSCR_HYS(config->hysteresisSelect);
#endif
#if FEATURE_PINS_HAS_OPEN_DRAIN
    pinsValues |= SIUL2_MSCR_ODE(config->openDrain);
#endif
#if FEATURE_SIUL2_HAS_ANALOG_PAD
    pinsValues |= SIUL2_MSCR_APC(config->analogPadCtrlSel);
#endif
    pinsValues |= SIUL2_MSCR_IBE(config->inputBuffer);
    pinsValues |= SIUL2_MSCR_OBE(config->outputBuffer);

    pinsValues |= SIUL2_OUTPUT_SOURCE_SELECT(config->mux);

    /* Write to Multiplexed Signal Configuration Register register */
    config->base->MSCR[config->pinPortIdx] = pinsValues;

    if (PORT_INPUT_BUFFER_ENABLED == config->inputBuffer)
    {
        for (inputMuxIterator = 0U; inputMuxIterator < FEATURE_SIUL2_INPUT_MUX_WIDTH; inputMuxIterator++)
        {
            /* Check if input mux information needs to be configured */
            if (PORT_INPUT_MUX_NO_INIT != config->inputMux[inputMuxIterator])
            {
                /* Input mux value */
                pinsValuesInput = SIUL2_INPUT_SOURCE_SELECT(config->inputMux[inputMuxIterator]);

                /* Input inversion feature */
            #if FEATURE_SIUL2_HAS_INVERT_DATA_INPUT
                pinsValuesInput |= SIUL2_IMCR_INV(config->inputInvert[inputMuxIterator]);
            #endif
                /* Write to Input Mux register */
                config->base->IMCR[config->inputMuxReg[inputMuxIterator]] = (uint32_t)pinsValuesInput;
            }
        }

        DEV_ASSERT(config->intConfig.eirqPinIdx < SIUL2_INTERRUPT_COUNT);
        /* Get the appropriate value follow index of pin interrupt */
        pinIntValue = (1UL << config->intConfig.eirqPinIdx);
        /* External interrupt initialization */
        if (config->intConfig.intEdgeSel != SIUL2_INT_DISABLE)
        {
            /* Setting the appropriate IREEnbits in IREER0 */
            if ((config->intConfig.intEdgeSel == SIUL2_INT_RISING_EDGE) || (config->intConfig.intEdgeSel == SIUL2_INT_EITHER_EDGE))
            {
                config->base->IREER0 |= pinIntValue;
            }
            else
            {
                config->base->IREER0 &= ~pinIntValue;
            }
            /* Setting the appropriate IREEnbits in IFEER0 */
            if ((config->intConfig.intEdgeSel == SIUL2_INT_FALLING_EDGE) || (config->intConfig.intEdgeSel == SIUL2_INT_EITHER_EDGE))
            {
                config->base->IFEER0 |= pinIntValue;
            }
            else
            {
                config->base->IFEER0 &= ~pinIntValue;
            }

        #if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
            /* Select the request desired between DMA or Interrupt */
            if (SIUL2_INT_USING_DMA == config->intConfig.intExeSel)
            {
                config->base->DIRSR0 |= pinIntValue;
            }
            else
            {
                config->base->DIRSR0 &= ~pinIntValue;
            }
            /* Write to EIFn bits in DISR0  to clear any flags */
            config->base->DISR0  |= pinIntValue;
            /* Enable the interrupt pins */
            config->base->DIRER0 |= pinIntValue;
        #else  /* if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA */
            /* Select the request desired Interrupt */
            config->base->IRSR0 &= ~pinIntValue;
            /* Write to EIFn bits in ISR0  to clear any flags */
            config->base->ISR0 = pinIntValue;
            /* Enable the interrupt pins */
            config->base->IRER0 |= pinIntValue;
        #endif /* if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA */
        }
        else
        {
        #if FEATURE_SIUL2_EXTERNAL_INT_SUPPORT_DMA
            /* Mask interrupts by clearing the EIREn bits in DIRER0 */
            config->base->DIRER0 &= ~pinIntValue;
        #else
            /* Mask interrupts by clearing the EIREn bits in IRER0 */
            config->base->IRER0 &= ~pinIntValue;
        #endif
        }
    }

    /* Configure initial value for GPIO pin in GPIO mux */
    if ((PORT_MUX_AS_GPIO == config->mux) && (PORT_OUTPUT_BUFFER_ENABLED == config->outputBuffer))
    {
        PINS_GPIO_WritePin(config->gpioBase, (pins_channel_type_t)(config->pinPortIdx % 16u), config->initValue);
    }

    return STATUS_SUCCESS;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
