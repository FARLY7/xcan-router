#ifndef CAN_QUEUE_H
#define CAN_QUEUE_H

#include "can_config.h"
#include "can_frame.h"

struct can_queue {
    uint32_t frames;
    uint32_t max_frames;
    struct can_frame *head;
    struct can_frame *tail;
};

static inline int can_enqueue(struct can_queue *q, struct can_frame *f)
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

static inline struct can_frame* can_dequeue(struct can_queue *q)
{
    struct can_frame *f = q->head;

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

static inline struct can_frame* can_queue_peek(struct can_queue *q)
{
    struct can_frame *f = q->head;
    if(q->frames < 1)
        return NULL;
    
    return f;
}

static inline void can_queue_empty(struct can_queue *q)
{
    struct can_frame *f = can_dequeue(q);
    while(f) {
        can_frame_discard(f);
        f = can_dequeue(q);
    }
}

#endif /* CAN_QUEUE_H */