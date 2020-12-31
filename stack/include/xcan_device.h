#ifndef XCAN_DEVICE_H
#define XCAN_DEVICE_H

#include "xcan_config.h"
#include "xcan_queue.h"
#include "xcan_frame.h"

#define XCAN_MAX_DEVICE_NAME 16

#define XCAN_MAX_DEVICES 4

struct xcan_device {
    uint8_t id;
    char name[XCAN_MAX_DEVICE_NAME];
    struct xcan_queue *q_in;
    struct xcan_queue *q_out;
    int (*link_state)(struct xcan_device *self);
    int (*send)(struct xcan_device *self, uint32_t id, uint8_t flags, uint8_t *data, uint8_t len);
    int (*poll)(struct xcan_device *self, int loop_score);
    void (*destroy)(struct xcan_device *self);
};

int xcan_device_init(struct xcan_device *dev, uint8_t id, const char *name);

void xcan_device_destroy(struct xcan_device *dev);

int xcan_devices_loop(int loop_score, int direction);

struct xcan_device* xcan_get_device(uint8_t id);

int xcan_device_link_state(struct xcan_device *dev);

#endif /* XCAN_DEVICE_H */
