#include "can_dev_softdevice.h"

#include <stdio.h>

struct can_device_softdevice {	
    struct can_device dev;
    // Anything else specific to softdevice
};


/* ================================================================= */
/* =======================      PRIVATE      ======================= */
/* ================================================================= */

int prv_link_state(struct can_device *dev)
{
    return 1;
}

int prv_send(struct can_device *self, void *buf, int len)
{
    dbg("CAN SD: Sending %d bytes\n", len);
    return 0;
}

int prv_poll(struct can_device *self, int loop_score)
{

}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void can_softdevice_destroy(struct can_device *dev)
{
    struct can_device_softdevice *sd = (struct can_device_softdevice *) dev;

    free(sd);
    dbg("Device %s destroyed.\n", sd->dev.name);
}


struct can_device* can_softdevice_create(uint8_t id, char *name)
{
    struct can_device_softdevice *sd = malloc(sizeof(struct can_device_softdevice));

    if(!sd)
        return NULL;

    if( 0 != can_device_init((struct can_device *) sd, id, name)) {
    
        dbg("CAN SoftDevice init failed.\n");
        can_softdevice_destroy((struct can_device*) sd);
        return NULL;
    }

    sd->dev.link_state = prv_link_state;
    sd->dev.send       = prv_send;
    sd->dev.poll       = prv_poll;
    sd->dev.destroy    = can_softdevice_destroy;
    dbg("Device %s created\n", name);
    return (struct can_device *) sd;
}
