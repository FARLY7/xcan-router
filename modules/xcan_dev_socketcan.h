#ifndef XCAN_DEV_SOCKETXCAN_H
#define XCAN_DEV_SOCKETXCAN_H

#include "xcan_device.h"

void xcan_socketxcan_destroy(struct xcan_device *socketxcan);

struct xcan_device* xcan_socketxcan_create(uint8_t id, char *name);

#endif /* XCAN_DEV_SOCKETXCAN_H */
