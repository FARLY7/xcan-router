/*
 * Copyright 2018 NXP
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

#include <stddef.h>

#include "device_registers.h"
#include "FreeRTOSConfig.h"
#include "interrupt_manager.h"

/* functions required by port.c */
extern void prvPortTimerSetup(void* paramF, uint32_t tick_interval);
extern void prvPortTimerReset(void);

/* Workaround for MPC574xP platforms where PIT is PIT_0 defined in header */
#if defined(PIT_0)
#define PIT PIT_0
#endif

void prvPortTimerSetup(void* paramF, uint32_t tick_interval)
{
	DEV_ASSERT(configUSE_PIT_CHANNEL < PIT_TIMER_COUNT);
	DEV_ASSERT(paramF != NULL);

	static const IRQn_Type pitIrqId[] = PIT_IRQS;
	INT_SYS_InstallHandler(pitIrqId[configUSE_PIT_CHANNEL], (isr_t)paramF, NULL);
	INT_SYS_EnableIRQ(pitIrqId[configUSE_PIT_CHANNEL]);
	INT_SYS_SetPriority(pitIrqId[configUSE_PIT_CHANNEL], 1);

	PIT->MCR |= PIT_MCR_FRZ(1u); /* stop the timer in debug */
	PIT->MCR &= ~PIT_MCR_MDIS(1u); /* enable the timer */
	/* PIT_LDVAL : tick period */
	PIT->TIMER[configUSE_PIT_CHANNEL].LDVAL = tick_interval;
	/* PIT_RTI_TCTRL: start channel, enable IRQ */
	PIT->TIMER[configUSE_PIT_CHANNEL].TCTRL = PIT_TCTRL_TEN(1u) | PIT_TCTRL_TIE(1u);
}

void prvPortTimerReset(void)
{
	/* clear PIT channel IRQ flag */
	PIT->TIMER[configUSE_PIT_CHANNEL].TFLG = PIT_TFLG_TIF(1u);
}
