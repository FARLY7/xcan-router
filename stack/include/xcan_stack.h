#ifndef XCAN_STACK_H
#define XCAN_STACK_H

#include "xcan_config.h"
#include "xcan_device.h"

/*******************************************************************************
 *  DATALINK LAYER
 ******************************************************************************/

int xcan_datalink_receive(struct xcan_frame *f);

int xcan_datalink_send(struct xcan_frame *f);

/*******************************************************************************
 *  PHYSICAL LAYER
 ******************************************************************************/

int xcan_stack_recv(struct xcan_device * dev,
                    uint32_t             can_id,
                    uint8_t              flags,
                    uint8_t      const * data,
                    uint8_t              len);



int xcan_sendto_dev()


/* ------- Initialisation ------- */
int xcan_stack_init(struct xcan_routing_table *routing_table);

/* ------- Loop Function -------- */
void xcan_stack_tick(void);

#endif