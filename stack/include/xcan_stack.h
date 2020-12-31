#ifndef XCAN_STACK_H
#define XCAN_STACK_H

#include "xcan_config.h"

#include "xcan_device.h"

/* -------- Stack Entry --------- */
int xcan_stack_recv(struct xcan_device *dev, uint8_t buffer, uint32_t len);

/* ------- Initialisation ------- */
int xcan_stack_init(void);

/* ------- Loop Function -------- */
void xcan_stack_tick(void);

#endif