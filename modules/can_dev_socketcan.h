#ifndef CAN_DEV_SOCKETCAN_H
#define CAN_DEV_SOCKETCAN_H

#include "can_device.h"

void can_socketcan_destroy(struct can_device *socketcan);

struct can_device* can_socketcan_create(uint8_t id, char *name);

#endif /* CAN_DEV_SOCKETCAN_H */
