#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include "can_config.h"
#include "can_device.h"

struct can_frame {

	/* Connect for queues */
	struct can_frame *next;

	/* CAN frame ID */
	uint16_t id;	

	/* Start of whole buffer, total frame length */
	uint8_t *buffer;
	uint32_t buffer_len;

	/* Pointer to CAN device this frame belongs to */
	struct can_device *dev;

	/* Pointer to frame usage count, which is stored in the last byte
       after the frame buffer */
	uint8_t *usage_count; 
};

void can_frame_discard(struct can_frame *f);

struct can_frame* can_frame_alloc(uint32_t size);

struct can_frame* can_frame_copy(struct can_frame *f);

struct can_frame* can_frame_deepcopy(struct can_frame *f);

#endif
