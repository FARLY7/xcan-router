#ifndef XCAN_QUEUE_H
#define XCAN_QUEUE_H

#include "xcan_config.h"
#include "xcan_frame.h"

struct xcan_queue {
    uint32_t frames;
    uint32_t max_frames;
    struct xcan_frame *head;
    struct xcan_frame *tail;
};

static inline int xcan_enqueue(struct xcan_queue *q, struct xcan_frame *f)
{
    if((q->max_frames) &&  (q->frames >= q->max_frames)) {
        /* Queue full */
        return -1;
    }

    f->next = NULL;
    if(!q->head) {
        /* Queue is empty */
        q->head = f;
        q->tail = f;
        q->frames = 0;
    } else {
        /* Add frame to tail */
        q->tail->next = f;
        q->tail = f;
    }

    q->frames++;
    return 0;
}

static inline struct xcan_frame* xcan_dequeue(struct xcan_queue *q)
{
    struct xcan_frame *f = q->head;

    if(!f)
        return NULL;
    
    if(q->frames < 1)
        return NULL;

    /* New head is 1 before frame */
    q->head = f->next;
    q->frames--;

    if(q->head == NULL)
        q->tail = NULL;

    f->next = NULL;
    return f;
}

static inline struct xcan_frame* xcan_queue_peek(struct xcan_queue *q)
{
    struct xcan_frame *f = q->head;
    if(q->frames < 1)
        return NULL;
    
    return f;
}

static inline void xcan_queue_empty(struct xcan_queue *q)
{
    struct xcan_frame *f = xcan_dequeue(q);
    while(f) {
        xcan_frame_discard(f);
        f = xcan_dequeue(q);
    }
}

#endif /* XCAN_QUEUE_H */