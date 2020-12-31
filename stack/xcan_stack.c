#include "xcan_stack.h"
#include "xcan_router.h"


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
int xcan_stack_init(void)
{

    return 0;
}

/* ------- Loop Function -------- */
void xcan_stack_tick(void)
{
    int frames;

    /* Receive up to 10 CAN frames into the stack */
    frames = xcan_devices_loop(10, 0);
    dbg("XCAN: %d frames received\n", 10-frames);

    /* Send up to 10 CAN frames out of the stack */
    frames = xcan_devices_loop(10, 1);
    dbg("XCAN: %d frames sent\n", 10-frames);
}