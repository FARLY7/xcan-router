#include "xcan_dev_socketcan.h"
#include "xcan_stack.h"

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
    struct xcan_device_socketcan *sc = (struct xcan_device_socketcan *) dev;
    (void)sc;

    return 1;
}

int prv_send(struct xcan_device *self, uint32_t id, uint8_t flags, uint8_t *data, uint8_t len)
{
    struct xcan_device_socketcan *sc = (struct xcan_device_socketcan *) self;

    struct canfd_frame frame;
    frame.can_id = id;
    frame.flags  = flags;
    frame.len    = len;
    memcpy(frame.data, data, len);
    
    int frame_len = 0;
    int nbytes;

    if(len > 8) {
        frame_len = sizeof(struct canfd_frame);
    } else{
        frame_len = sizeof(struct can_frame);
    }

    nbytes = write(sc->fd, &frame, frame_len);
    dbg("SocketCAN (%s): Sent %d bytes\n", sc->dev.name, nbytes);
    return nbytes;
}

int prv_poll(struct xcan_device *self, int loop_score)
{
    struct xcan_device_socketcan *sc = (struct xcan_device_socketcan *) self;

    struct canfd_frame f;
    int nbytes;

    nbytes = read(sc->fd, &f, sizeof(struct can_frame));

    if(nbytes == CAN_MTU) {
        dbg("SocketCAN (%s): Received CAN frame\n", sc->dev.name);
    } else if(nbytes == CANFD_MTU) {
        dbg("SocketCAN (%s): Received CAN-FD frame\n", sc->dev.name);
    } else {
        dbg("SocketCAN (%s): Received unknown frame\n", sc->dev.name);
    }

    xcan_stack_recv(self, f.can_id, f.flags, f.data, f.len);

    return 0;
}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void xcan_socketcan_destroy(struct xcan_device *dev)
{
    struct xcan_device_socketcan *sc = (struct xcan_device_socketcan *) dev;

    if(close(sc->fd) < 0) {
        dbg("SocketCAN (%s): Failed to close socket\n", sc->dev.name);
    }

    free(sc);
    dbg("SocketCAN (%s): Destroyed.\n", sc->dev.name);
}


struct xcan_device* xcan_socketcan_create(uint8_t id, char *name)
{
    struct xcan_device_socketcan *sc = XCAN_ZALLOC(sizeof(struct xcan_device_socketcan));

    struct sockaddr_can addr;
    struct ifreq ifr;

    if(!sc)
        return NULL;

    /* Initialise SocketCAN interface */
    if((sc->fd = socket(PF_CAN, SOCK_RAW, CAN_RAW_FD_FRAMES)) < 0) {
        dbg("SocketCAN (%s): Failed to open socket", name);
        return NULL;
    }
    
    /* Retrieve the correct interface name */
    strcpy(ifr.ifr_name, name);
    ioctl(sc->fd, SIOCGIFINDEX, &ifr);

    /* Bind socket */
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if(bind(sc->fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("SocketCAN: Failed to bind socket");
		return NULL;
	}

    /* Register SocketCAN interface as XCAN device */
    if( 0 != xcan_device_init((struct xcan_device *) sc, id, name)) {
        dbg("SocketCAN (%s): Failed to init XCAN device\n", name);
        xcan_socketcan_destroy((struct xcan_device*) sc);
        return NULL;
    }

    /* Fill in vtable */
    sc->dev.link_state  = prv_link_state;
    sc->dev.send        = prv_send;
    sc->dev.poll        = prv_poll;
    sc->dev.destroy     = xcan_socketcan_destroy;
    dbg("SocketCAN (%s): XCAN Device created\n", name);
    return (struct xcan_device *) sc;
}
