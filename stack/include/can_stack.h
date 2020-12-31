#ifndef CAN_STACK_H
#define CAN_STACK_H

#include "can_config.h"

#include "can_device.h"

/* -------- Stack Entry --------- */
int can_stack_recv(struct can_device *dev, uint8_t buffer, uint32_t len);

/* ------- Initialisation ------- */
int can_stack_init(void);

/* ------- Loop Function -------- */
void can_stack_tick(void);

#endif