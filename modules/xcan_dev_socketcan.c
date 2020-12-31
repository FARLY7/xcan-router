#include "xcan_dev_socketcan.h"

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

struct xcan_device_socketcan {	
    struct xcan_device dev;
    // Anything else specific to SocketCAN
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
    return 0;
}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void xcan_socketcan_destroy(struct xcan_device *dev)
{
    struct xcan_device_socketcan *sc = (struct xcan_device_socketcan *) dev;

    free(sc);
    dbg("Device %s destroyed.\n", sc->dev.name);
}


struct xcan_device* xcan_socketcan_create(uint8_t id, char *name)
{
    struct xcan_device_socketcan *sc = malloc(sizeof(struct xcan_device_socketcan));

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
        xcan_socketcan_destroy((struct xcan_device*) sc);
        return NULL;
    }

    /* Fill in vtable */
    sc->dev.link_state  = prv_link_state;
    sc->dev.send        = prv_send;
    sc->dev.poll        = prv_poll;
    sc->dev.destroy     = xcan_socketcan_destroy;
    dbg("Device %s created\n", name);
    return (struct xcan_device *) sc;
}
