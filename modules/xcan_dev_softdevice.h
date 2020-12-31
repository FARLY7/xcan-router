#ifndef XCAN_DEV_SOFTDEVICE_H
#define XCAN_DEV_SOFTDEVICE_H

#include "xcan_device.h"

void xcan_softdevice_destroy(struct xcan_device *softdevice);

struct xcan_device* xcan_softdevice_create(uint8_t id, char *name);

#endif /* XCAN_DEV_SOFTDEVICE_H */
