#include "xcan_frame.h"

static struct xcan_frame* xcan_frame_do_alloc(uint32_t size)
{
    struct xcan_frame *f = XCAN_ZALLOC(sizeof(struct xcan_frame));

    if(!f)
        return NULL;

    /* Allocate space for frame payload and usage counter */
    f->data = XCAN_ZALLOC(size + sizeof(uint8_t));
    if(!f->data)
        XCAN_FREE(f);

    f->len = size;
    f->flags = 0;
    f->usage_count = (f->data + size);
    return f;
}

void xcan_frame_discard(struct xcan_frame *f)
{
    if(!f)
        return;

    *(f->usage_count) += -1;

    if(*f->usage_count == 0) {
        XCAN_FREE(f->data);
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

    new->data = XCAN_ZALLOC(new->len + sizeof(uint8_t));
    memcpy(new->data, f->data, new->len);

    new->usage_count = new->data + new->len;
    *(new->usage_count) = 1;
    return new;
}
