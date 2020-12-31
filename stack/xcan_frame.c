#include "xcan_frame.h"

static struct xcan_frame* xcan_frame_do_alloc(uint32_t size)
{
    struct xcan_frame *f = XCAN_ZALLOC(sizeof(struct xcan_frame));

    if(!f)
        return NULL;

    /* Allocate space for frame payload and usage counter */
    f->buffer = XCAN_ZALLOC(size + sizeof(uint8_t));
    if(!f->buffer)
        XCAN_FREE(f);

    f->buffer_len = size;
    f->usage_count = (f->buffer + size);
    return f;
}

void xcan_frame_discard(struct xcan_frame *f)
{
    if(!f)
        return;

    *f->usage_count--;

    if(*f->usage_count == 0) {
        XCAN_FREE(f->buffer);
        XCAN_FREE(f);
    }
}

struct xcan_frame* xcan_frame_alloc(uint32_t size)
{
    return xcan_frame_do_alloc(size);
}

/* Only copies frame descriptor, not frame payload. */
struct xcan_frame* xcan_frame_copy(struct xcan_frame *f)
{
    struct xcan_frame *new = XCAN_ZALLOC(sizeof(struct xcan_frame));

    if(!new)
        return NULL;

    memcpy(new, f, sizeof(struct xcan_frame));
    *(new->usage_count) += 1;
    new->next = NULL;
    return new;
}

struct xcan_frame* xcan_frame_deepcopy(struct xcan_frame *f)
{
    struct xcan_frame *new = xcan_frame_copy(f);

    if(!f)
        return NULL;

    new->buffer = XCAN_ZALLOC(new->buffer_len + sizeof(uint8_t));
    memcpy(new->buffer, f->buffer, new->buffer_len);

    new->usage_count = new->buffer + new->buffer_len;
    *new->usage_count = 1;
    return new;
}
