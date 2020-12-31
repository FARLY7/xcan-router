#ifndef XCAN_DEV_SOCKETCAN_H
#define XCAN_DEV_SOCKETCAN_H

#include "xcan_device.h"

void xcan_socketcan_destroy(struct xcan_device *dev);

struct xcan_device* xcan_socketcan_create(uint8_t id, char *name);

#endif /* XCAN_DEV_SOCKETXCAN_H */
