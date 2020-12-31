#include "xcan_device.h"

struct xcan_device *devices[XCAN_MAX_DEVICES];


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
    /** @todo */
    
}

int xcan_devices_loop(int loop_score, int direction)
{
    /** @todo */
    return 0;
}

struct xcan_device* xcan_get_device(uint8_t id)
{
    /** @todo */
    return NULL;
}

int xcan_device_link_state(struct xcan_device *dev)
{
    return dev->link_state(dev);
}