#include "xcan_device.h"
#include "xcan_stack.h"

struct xcan_device *devices[XCAN_MAX_DEVICES];

static int devloop_in(struct xcan_device *dev, int loop_score)
{
    struct xcan_frame *f;

    if(!dev)
        return loop_score;

    while(loop_score > 0)
    {
        if(dev->q_in->frames == 0)
            break;

        f = xcan_dequeue(dev->q_in);
        if(f) {
            xcan_datalink_receive(f);
            loop_score--;
        }
    }

    return loop_score;
}

static int devloop_out(struct xcan_device *dev, int loop_score)
{
    struct xcan_frame *f;

    if(!dev)
        return loop_score;

    while(loop_score > 0)
    {
        if(dev->q_out->frames == 0)
            break;

        /* We just peek incase device is unable to send frame,
           then we retain it */
        f = xcan_queue_peek(dev->q_out);
        if(!f)
            break;

        if(dev->send(dev, f->id, f->flags, f->data, f->len) == 0) {
            /* Frame successfully sent */
            f = xcan_dequeue(dev->q_out);
            xcan_frame_discard(f);
            loop_score--;
        } else {
            /* Failed to send frame, try again next time round */
            break;
        }
    }

    return loop_score;
}


int xcan_device_init(struct xcan_device *dev, uint8_t id, const char *name)
{
    if(id >= XCAN_MAX_DEVICES)
        return -1;

    dev->id = id;
    strncpy(dev->name, name, XCAN_MAX_DEVICE_NAME);

    dev->q_in = XCAN_ZALLOC(sizeof(struct xcan_queue));
    if(!dev->q_in)
        return -1;

    dev->q_out = XCAN_ZALLOC(sizeof(struct xcan_queue));
    if(!dev->q_out) {
        XCAN_FREE(dev->q_in);
        return -1;
    }

    /* Register device with device pool */
    devices[id] = dev;
    return 0;
}


void xcan_device_destroy(struct xcan_device *dev)
{
    /* Unregister device with device pool */
    devices[dev->id] = NULL;

    /* Call device specific destroyer */
    dev->destroy(dev);
}



int xcan_devices_loop(int loop_score, int direction)
{
    /** @todo */

    while(loop_score > 0)
    {
        if(direction == 0)
        {
            /* Receiving frames into the stack */
            for(int i = 0 ; i < XCAN_MAX_DEVICES ; i++) {
                loop_score = devloop_in(devices[i], loop_score);
            }
        }
        else if(direction == 1)
        {
            /* Sending frames out of the stack */
            for(int i = 0 ; i < XCAN_MAX_DEVICES ; i++) {
                loop_score = devloop_out(devices[i], loop_score);
            }
        }

        loop_score--;
    }

    return loop_score;
}

struct xcan_device* xcan_get_device(uint8_t id)
{
    for(int i = 0 ; i < XCAN_MAX_DEVICES ; i++) {
        if(devices[i]->id == id) {
            return devices[i];
        }        
    }
    return NULL;
}

int xcan_device_link_state(struct xcan_device *dev)
{
    return dev->link_state(dev);
}