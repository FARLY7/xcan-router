#include "xcan_dev_socketcan.h"

#include "sys/socket.h"
#include <linux/can.h>
#include <linux/can/raw.h>

struct xcan_device_socketxcan {	
    struct xcan_device dev;
    // Anything else specific to socketxcan
    int fd;
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
    dbg("XCAN sc: Sending %d bytes\n", len);
    return 0;
}

int prv_poll(struct xcan_device *self, int loop_score)
{

}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void xcan_socketxcan_destroy(struct xcan_device *dev)
{
    struct xcan_device_socketxcan *sc = (struct xcan_device_socketxcan *) dev;

    free(sc);
    dbg("Device %s destroyed.\n", sc->dev.name);
}


struct xcan_device* xcan_socketxcan_create(uint8_t id, char *name)
{
    struct xcan_device_socketxcan *sc = malloc(sizeof(struct xcan_device_socketxcan));

    if(!sc)
        return NULL;

    /* Initialise SocketXCAN interface */
    if((sc->fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        dbg("Failed to open socketXCAN interface");
        return NULL;
    }
    
    /* Retrieve the correct interface name */
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");


    /* Register SocketXCAN interface as XXCAN device */
    if( 0 != xcan_device_init((struct xcan_device *) sc, id, name)) {
    
        dbg("XCAN socketxcan init failed.\n");
        xcan_socketxcan_destroy((struct xcan_device*) sc);
        return NULL;
    }

    /* Fill in vtable */
    sc->dev.link_state  = prv_link_state;
    sc->dev.send        = prv_send;
    sc->dev.poll        = prv_poll;
    sc->dev.destroy     = xcan_socketxcan_destroy;
    dbg("Device %s created\n", name);
    return (struct xcan_device *) sc;
}
