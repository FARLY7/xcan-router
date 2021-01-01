#include "xcan_stack.h"
#include "xcan_router.h"

#define XCAN_LOOP_DIR_IN    0
#define XCAN_LOOP_DIR_OUT   1


/*******************************************************************************
 *  DATALINK LAYER
 ******************************************************************************/

int xcan_datalink_receive(struct xcan_frame *f)
{
    return xcan_router_receive(f);
}

int xcan_datalink_send(struct xcan_frame *f)
{
    return 0;
}

/*******************************************************************************
 *  PHYSICAL LAYER
 ******************************************************************************/

int xcan_stack_recv(struct xcan_device * dev,
                    uint32_t             can_id,
                    uint8_t              flags,
                    uint8_t      const * data,
                    uint8_t              len)
{
    struct xcan_frame *f = xcan_frame_alloc(len);

    if(!f)
        return 1;

    f->dev = dev;
    f->id = can_id;
    memcpy(f->data, data, len);
    
    if(xcan_enqueue(dev->q_in, f) != 0) {
        xcan_frame_discard(f);
        return 1;
    }

    return 0;
}


/* ------- Initialisation ------- */
int xcan_stack_init(struct xcan_routing_table *routing_table)
{
    /* Initialise XCAN Router */
    if(!xcan_router_init(routing_table))
        return 1;

    return 0;
}

/* ------- Loop Function -------- */
void xcan_stack_tick(void)
{
    int loop_score = 20;

    /* Receive up to 10 CAN frames into the stack */
    loop_score = xcan_devices_loop(loop_score, XCAN_LOOP_DIR_IN);

    /* Send up to 10 CAN frames out of the stack */
    loop_score = xcan_devices_loop(loop_score, XCAN_LOOP_DIR_OUT);
}