#include "can_device.h"

struct can_device *devices[CAN_MAX_DEVICES];


int can_device_init(struct can_device *dev, uint8_t id, const char *name)
{
    if(id >= CAN_MAX_DEVICES)
        return -1;

    dev->id = id;
    strncpy(dev->name, name, CAN_MAX_DEVICE_NAME);

    dev->q_in = CAN_ZALLOC(sizeof(struct can_queue));
    if(!dev->q_in)
        return -1;

    dev->q_out = CAN_ZALLOC(sizeof(struct can_queue));
    if(!dev->q_out) {
        CAN_FREE(dev->q_in);
        return -1;
    }

    /* Register device with device pool */
    devices[id] = dev;
}


void can_device_destroy(struct can_device *dev)
{
    /** @todo */
    
}

int can_devices_loop(int loop_score, int direction)
{
    /** @todo */
}

struct can_device* can_get_device(uint8_t id)
{
    /** @todo */
    return NULL;
}

int can_device_link_state(struct can_device *dev)
{
    return dev->link_state(dev);
}
