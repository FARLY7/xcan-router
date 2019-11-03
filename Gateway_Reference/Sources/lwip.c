/*
 * Copyright (c) 2001,2002 Florian Schulze.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of the contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * test.c - This file is part of lwIP test
 *
 */


/* C runtime includes */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* lwIP core includes */
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/api.h"
/* lwIP netif includes */
#include "lwip/etharp.h"
#include "netif/ethernet.h"
/* include the port-dependent configuration */
#include "enetif.h"

/* THE ethernet interface */
struct netif netif;

void LWIP_task(void *pvParameters);

/* This function initializes all network interfaces */
static void enetif_init(void)
{
	struct ip4_addr ipaddr;
    struct ip4_addr netmask;
    struct ip4_addr gw;

	LWIP_PORT_INIT_GW(&gw);           /* Setting gateway */
	LWIP_PORT_INIT_IPADDR(&ipaddr);   /* Setting network IP address */
	LWIP_PORT_INIT_NETMASK(&netmask); /* Setting network mask */

    /* Add a network interface to the list of lwIP netifs */
    netif_add(&netif, &ipaddr, &netmask, &gw, 0, enet_ethernetif_init, ethernet_input); // ethernet_input -> tcpip_input?

    netif_set_default(&netif);  /* Set a network interface as the default network interface */
    netif_set_up(&netif);       /* Bring an interface up, available for processing traffic  */
}


/* This function initializes this lwIP test. When NO_SYS=1, this is done in
 * the main_loop context (there is no other one), when NO_SYS=0, this is done
 * in the tcpip_thread context */
static void test_init(void * arg)
{ /* remove compiler warning */
	sys_sem_t *init_sem;
	LWIP_ASSERT("arg != NULL", arg != NULL);
	init_sem = (sys_sem_t*)arg;

	/* init network interfaces */
	enetif_init();

	/* Initialise TCP Socket Server */
	socket_init();

	//chargen_init();

	sys_sem_signal(init_sem);
}

void LWIP_task(void* pvParameters)
{
	err_t err;
	sys_sem_t init_sem;

	/* Initialise lwIP stack, network interfaces and applications */
	err = sys_sem_new(&init_sem, 0);
	LWIP_ASSERT("failed to create init_sem", err == ERR_OK);
	LWIP_UNUSED_ARG(err);

	/* **** Initialise TCP/IP Stack Thread **** */
	tcpip_init(test_init, &init_sem);

	/* we have to wait for initialization to finish before
	 * calling update_adapter()! */
	sys_sem_wait(&init_sem);
	sys_sem_free(&init_sem);

	/* MAIN LOOP for driver update (and timers if NO_SYS) */
	while (1) {
		sys_msleep(5000);
	}

	/* release the enet interface... */
	enet_ethernetif_shutdown(&netif);
}

