#include "xcan_router.h"
#include "xcan_device.h"
#include "xcan_queue.h"

static struct xcan_routing_table *m_tbl;


static int route_frame(struct xcan_frame, int loop_score)
{
    for(int i = 0 ; i < m_tbl->no_entries ; i++)
    {
        if(m_tbl->entry[i].can_id == f->id)
        {
            for(int i = 0 ; i < m_tbl->entry[i].no_interfaces ; i++)
            {
                if(xcan_enqueue(m_tbl->entry[i].interface_id[i]) == 0) {
                    loop_score--;
                }
                xcan_device_send
            }
        }
    }
    loop_score--;

    return loop_score;
}


int xcan_router_init(struct xcan_routing_table *routing_table)
{
    m_tbl = routing_table;
}


int xcan_router_receive(struct xcan_frame *f)
{
    dbg("XCAN Router: Received a frame!");
    return 0;
}


int xcan_router_loop(int loop_score)
{
    struct xcan_frame *f;

    while(loop_score > 0)
    {
        if(dev->q_in->frames == 0)
            break;

        f = xcan_dequeue(dev->q_in);
        if(f) {
            loop_score = route_frame(f, loop_score);
        }
    }

    return loop_score;
}