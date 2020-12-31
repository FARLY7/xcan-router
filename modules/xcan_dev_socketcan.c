#include "xcan_dev_socketcan.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

    struct sockaddr_can addr;
    struct ifreq ifr;

    if(!sc)
        return NULL;

    /* Initialise SocketCAN interface */
    if((sc->fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        dbg("Failed to open socketXCAN interface");
        return NULL;
    }
    
    /* Retrieve the correct interface name */
    strcpy(ifr.ifr_name, name);
    ioctl(sc->fd, SIOCGIFINDEX, &ifr);
    //dbg("SocketCAN: %s at index %d\n", ifr.ifr_name, ifr.ifr_ifindex);

    /* Bind socket */
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if(bind(sc->fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("SocketCAN: Error in socket bind");
		return NULL;
	}

    /* Register SocketXCAN interface as XXCAN device */
    if( 0 != xcan_device_init((struct xcan_device *) sc, id, name)) {
    
        dbg("XCAN socketxcan init failed.\n");
        xcan_socketcan_destroy((struct xcan_device*) sc);
        return NULL;
    }

	// struct can_frame frame;
	// frame.can_id  = 0x123;
	// frame.can_dlc = 2;
	// frame.data[0] = 0x11;
	// frame.data[1] = 0x22;
	// int nbytes = write(sc->fd, &frame, sizeof(struct can_frame));
    // dbg("SocketCAN: Sent %dB\n", nbytes);

    /* Just immediately close for now */
    close(sc->fd);

    /* Fill in vtable */
    sc->dev.link_state  = prv_link_state;
    sc->dev.send        = prv_send;
    sc->dev.poll        = prv_poll;
    sc->dev.destroy     = xcan_socketcan_destroy;
    dbg("Device %s created\n", name);
    return (struct xcan_device *) sc;
}
