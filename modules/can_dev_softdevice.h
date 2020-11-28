#ifndef CAN_DEV_SOFTDEVICE_H
#define CAN_DEV_SOFTDEVICE_H

#include "can_device.h"

void can_softdevice_destroy(struct can_device *softdevice);

struct can_device* can_softdevice_create(uint8_t id, char *name);

#endif /* CAN_DEV_SOFTDEVICE_H */
