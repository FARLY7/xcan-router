/*
 * Copyright 2017-2018 NXP
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

#include "rtc_c55_hw_access.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_Configure
 * Description   : Configure the RTC module.
 * Return        : None
 *END**************************************************************************/
void RTC_Configure(RTC_Type * const base,
                   bool freezeEnable,
                   rtc_clk_select_t clockSelect,
                   bool divBy512,
                   bool divBy32,
                   bool rolloverIntEnable,
                   bool globalIntEnable
                   )
{
    uint32_t configReg = 0UL;

    if(freezeEnable)
    {
        configReg |= RTC_RTCC_FRZEN_MASK;
    }
    if(divBy512)
    {
        configReg |= RTC_RTCC_DIV512EN_MASK;
    }
    if(divBy32)
    {
        configReg |= RTC_RTCC_DIV32EN_MASK;
    }
    if(rolloverIntEnable)
    {
        configReg |= RTC_RTCC_ROVREN_MASK;
    }
    if(globalIntEnable)
    {
        configReg |= RTC_RTCC_RTCIE_MASK;
    }

    switch(clockSelect)
    {
    case RTC_CLOCK_SOURCE_XOSC:
        configReg |= (0UL << RTC_RTCC_CLKSEL_SHIFT);
        break;
    case RTC_CLOCK_SOURCE_SIRC:
        configReg |= (1UL << RTC_RTCC_CLKSEL_SHIFT);
        break;
    case RTC_CLOCK_SOURCE_FIRC:
        configReg |= (2UL << RTC_RTCC_CLKSEL_SHIFT);
        break;
    case RTC_CLOCK_SOURCE_FOSC:
        configReg |= (3UL << RTC_RTCC_CLKSEL_SHIFT);
        break;
    default:
        /* This case should not be entered */
        break;
    }

    base->RTCC = configReg;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_ConfigureAPI
 * Description   : Configures the API function of the RTC module. Before the
 *                 configuration it waits for the synchronization of the register
 *                 to be completed.
 * Return        : Operation status
 *END**************************************************************************/
status_t RTC_ConfigureAPI(RTC_Type * const base,
                          uint32_t value,
                          bool enable,
                          bool interruptEnable)
{
    status_t returnCode;
    returnCode = STATUS_SUCCESS;
    /* Check if timer is synchronized or not */
    if((base->RTCS & RTC_RTCS_INV_API_MASK) != 0UL)
    {
    	returnCode = STATUS_BUSY;
    }
	/* Report ERROR in case of value less then FEATURE_RTC_MINIMUM_API_VALUE */
	if((value < FEATURE_RTC_MINIMUM_API_VALUE) || (returnCode != STATUS_SUCCESS))
	{
        returnCode = STATUS_ERROR;
	}
    else
    {
        base->APIVAL = value;
        base->RTCC &= ~(RTC_RTCC_APIEN(1) | RTC_RTCC_APIIE(1));
        base->RTCC |= RTC_RTCC_APIEN(enable ? 1UL : 0UL) | RTC_RTCC_APIIE(interruptEnable ? 1UL : 0UL);
    }

    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_ConfigureCMPTrigger
 * Description   : Configures the Analog Comparator trigger feature of the RTC
 *                 module. Before the configuration it waits for the
 *                 synchronization of the register to be completed.
 * Return        : Operation status
 *END**************************************************************************/
status_t RTC_ConfigureCMPTrigger(RTC_Type * const base,
                                 uint32_t value,
                                 bool enable)
{
    status_t returnCode;
    returnCode = STATUS_SUCCESS;
    /* Check if timer is synchronized or not */
    if((base->RTCS & RTC_RTCS_INV_ANL_MASK) != 0UL)
    {
    	returnCode = STATUS_BUSY;
    }
    else
    {
        base->ANLCMP_CNT = value;
        base->RTCC &= ~RTC_RTCC_TRIG_EN(1);
        base->RTCC |= RTC_RTCC_TRIG_EN(enable ? 1UL : 0UL);
    }
    return returnCode;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : RTC_SetCompareRegister
 * Description   : Configures the Compare function of the RTC
 *                 module. Before the configuration reports the synchronization
 *                 of the register is completed or not.
 * Return        : Operation status
 *END**************************************************************************/
status_t RTC_SetCompareRegister(RTC_Type * const base,
                                uint32_t value)
{
    status_t returnCode;
    returnCode = STATUS_SUCCESS;
    /* Check if timer is synchronized or not */
    if((base->RTCS & RTC_RTCS_INV_RTC_MASK) != 0UL)
    {
    	returnCode = STATUS_BUSY;
    }
    else
    {
        base->RTCVAL = value;
    }

    return returnCode;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
