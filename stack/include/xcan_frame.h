#ifndef XCAN_FRAME_H
#define XCAN_FRAME_H

#include "xcan_config.h"

struct xcan_frame {

    /* Connect for queues */
    struct xcan_frame *next;

    /* Pointer to XCAN device this frame belongs to */
    struct xcan_device *dev;

    uint32_t id;    /* 32 bit CAN_ID + EFF/RTR/ERR flags */
    uint8_t  flags;  /* Flags */
    uint16_t len;   /* Frame payload length in bytes */
    uint8_t *data;  /* Frame payload buffer */
    
    /* Pointer to frame usage count, which is stored in the last byte
       after the frame buffer */
    uint8_t *usage_count; 
};

void xcan_frame_discard(struct xcan_frame *f);

struct xcan_frame* xcan_frame_alloc(uint32_t size);

struct xcan_frame* xcan_frame_copy(struct xcan_frame *f);

struct xcan_frame* xcan_frame_deepcopy(struct xcan_frame *f);

#endif
