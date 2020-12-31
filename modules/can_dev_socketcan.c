#include "can_dev_socketcan.h"

#include "sys/socket.h"
#include <linux/can.h>
#include <linux/can/raw.h>

struct can_device_socketcan {	
    struct can_device dev;
    // Anything else specific to socketcan
    int fd;
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
    dbg("CAN sc: Sending %d bytes\n", len);
    return 0;
}

int prv_poll(struct can_device *self, int loop_score)
{

}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void can_socketcan_destroy(struct can_device *dev)
{
    struct can_device_socketcan *sc = (struct can_device_socketcan *) dev;

    free(sc);
    dbg("Device %s destroyed.\n", sc->dev.name);
}


struct can_device* can_socketcan_create(uint8_t id, char *name)
{
    struct can_device_socketcan *sc = malloc(sizeof(struct can_device_socketcan));

    if(!sc)
        return NULL;

    /* Initialise SocketCAN interface */
    if((sc->fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        dbg("Failed to open socketCAN interface");
        return NULL;
    }
    
    /* Retrieve the correct interface name */
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "vcan0");


    /* Register SocketCAN interface as XCAN device */
    if( 0 != can_device_init((struct can_device *) sc, id, name)) {
    
        dbg("CAN socketcan init failed.\n");
        can_socketcan_destroy((struct can_device*) sc);
        return NULL;
    }

    /* Fill in vtable */
    sc->dev.link_state  = prv_link_state;
    sc->dev.send        = prv_send;
    sc->dev.poll        = prv_poll;
    sc->dev.destroy     = can_socketcan_destroy;
    dbg("Device %s created\n", name);
    return (struct can_device *) sc;
}
