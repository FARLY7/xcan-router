#include "can_frame.h"

static struct can_frame* can_frame_do_alloc(uint32_t size)
{
    struct can_frame *f = CAN_ZALLOC(sizeof(struct can_frame));

    if(!f)
        return NULL;

    /* Allocate space for frame payload and usage counter */
    f->buffer = CAN_ZALLOC(size + sizeof(uint8_t));
    if(!f->buffer)
        CAN_FREE(f);

    f->buffer_len = size;
    f->usage_count = (f->buffer + size);
    return f;
}

void can_frame_discard(struct can_frame *f)
{
    if(!f)
        return;

    *f->usage_count--;

    if(*f->usage_count == 0) {
        CAN_FREE(f->buffer);
        CAN_FREE(f);
    }
}

struct can_frame* can_frame_alloc(uint32_t size)
{
    return can_frame_do_alloc(size);
}

/* Only copies frame descriptor, not frame payload. */
struct can_frame* can_frame_copy(struct can_frame *f)
{
    //struct can_frame *f = malloc()
}

struct can_frame* can_frame_deepcopy(struct can_frame *f)
{
    
}
