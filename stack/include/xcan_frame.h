#ifndef XCAN_FRAME_H
#define XCAN_FRAME_H

#include "xcan_config.h"

struct xcan_frame {

    /* Connect for queues */
    struct xcan_frame *next;

    /* XCAN frame ID */
    uint16_t id;	

    /* Start of whole buffer, total frame length */
    uint8_t *buffer;
    uint32_t buffer_len;

    /* Pointer to XCAN device this frame belongs to */
    struct xcan_device *dev;

    /* Pointer to frame usage count, which is stored in the last byte
       after the frame buffer */
    uint8_t *usage_count; 
};

void xcan_frame_discard(struct xcan_frame *f);

struct xcan_frame* xcan_frame_alloc(uint32_t size);

struct xcan_frame* xcan_frame_copy(struct xcan_frame *f);

struct xcan_frame* xcan_frame_deepcopy(struct xcan_frame *f);

#endif
