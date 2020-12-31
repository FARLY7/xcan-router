#include "xcan_dev_softdevice.h"

#include <stdio.h>

struct xcan_device_softdevice {	
    struct xcan_device dev;
    // Anything else specific to softdevice
};


/* ================================================================= */
/* =======================      PRIVATE      ======================= */
/* ================================================================= */

int prv_link_state(struct xcan_device *dev)
{
    return 1;
}

int prv_send(struct xcan_device *self, void *buf, int len)
{
    dbg("XCAN SD: Sending %d bytes\n", len);
    return 0;
}

int prv_poll(struct xcan_device *self, int loop_score)
{

}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void xcan_softdevice_destroy(struct xcan_device *dev)
{
    struct xcan_device_softdevice *sd = (struct xcan_device_softdevice *) dev;

    free(sd);
    dbg("Device %s destroyed.\n", sd->dev.name);
}


struct xcan_device* xcan_softdevice_create(uint8_t id, char *name)
{
    struct xcan_device_softdevice *sd = malloc(sizeof(struct xcan_device_softdevice));

    if(!sd)
        return NULL;

    if( 0 != xcan_device_init((struct xcan_device *) sd, id, name)) {
    
        dbg("XCAN SoftDevice init failed.\n");
        xcan_softdevice_destroy((struct xcan_device*) sd);
        return NULL;
    }

    sd->dev.link_state = prv_link_state;
    sd->dev.send       = prv_send;
    sd->dev.poll       = prv_poll;
    sd->dev.destroy    = xcan_softdevice_destroy;
    dbg("Device %s created\n", name);
    return (struct xcan_device *) sd;
}
