#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include "can_config.h"
#include "can_queue.h"
#include "can_frame.h"

#define CAN_MAX_DEVICE_NAME 16

#define CAN_MAX_DEVICES 4

struct can_device {
    uint8_t id;
    char name[CAN_MAX_DEVICE_NAME];
    struct can_queue *q_in;
    struct can_queue *q_out;
    int (*link_state)(struct can_device *self);
    int (*send)(struct can_device *self, void *buf, int len);
    int (*poll)(struct can_device *self, int loop_score);
    void (*destroy)(struct can_device *self);
};

int can_device_init(struct can_device *dev, uint8_t id, const char *name);

void can_device_destroy(struct can_device *dev);

int can_devices_loop(int loop_score, int direction);

struct can_device* can_get_device(uint8_t id);

int can_device_link_state(struct can_device *dev);

#endif /* CAN_DEVICE_H */
