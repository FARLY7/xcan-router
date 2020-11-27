#include "can_dev_softdevice.h"

#include <stdio.h>

struct can_device_softdevice {	
	struct can_device dev;
	// Anything else specific to softdevice
};


/* ================================================================= */
/* =======================      PRIVATE      ======================= */
/* ================================================================= */

int link_state(struct can_device *dev)
{
	return 1;
}

int send(struct can_device *self, void *buf, int len)
{
	printf("CAN SD: Sending %d bytes\n", len);
	return 0;
}

int poll(struct can_device *self, int loop_score)
{

}

/* ================================================================= */
/* =======================      PUBLIC      ======================== */
/* ================================================================= */


void can_softdevice_destroy(struct can_device *dev)
{
	/* Destroy device */
}


struct can_device* create_can_soft_device(uint8_t id, char *name)
{
	struct can_device_softdevice *sd = malloc(sizeof(struct can_device_softdevice));

	if(!sd)
		return NULL;

	if( 0 != can_device_init((struct can_device *) sd, id, name)) {
	
		printf("CAN SoftDevice init failed.\n");
		can_softdevice_destroy(sd);
		return NULL;
	}

	sd->link_state = link_state;
	sd->send = send;
	sd->poll = poll;
	sd->destory = can_device_destroy;
	printf("Device %s created\n", name);
	return (struct can_device *)dev;
}
