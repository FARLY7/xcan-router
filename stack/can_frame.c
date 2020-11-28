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
    struct can_frame *new = CAN_ZALLOC(sizeof(struct can_frame));

    if(!new)
        return NULL;

    memcpy(new, f, sizeof(struct can_frame));
    *(new->usage_count) += 1;
    new->next = NULL;
    return new;
}

struct can_frame* can_frame_deepcopy(struct can_frame *f)
{
    struct can_frame *new = can_frame_copy(f);

    if(!f)
        return NULL;

    new->buffer = CAN_ZALLOC(new->buffer_len + sizeof(uint8_t));
    memcpy(new->buffer, f->buffer, new->buffer_len);

    new->usage_count = new->buffer + new->buffer_len;
    *new->usage_count = 1;
    return new;
}
