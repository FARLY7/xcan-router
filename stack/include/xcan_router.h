#ifndef XCAN_ROUTER_H
#define XCAN_ROUTER_H

#include "xcan_config.h"
#include "xcan_frame.h"


struct xcan_routing_table {
    struct xcan_routing_entry *entry;
    uint32_t no_entries;
};

struct xcan_routing_entry {
    uint32_t can_id;
    uint8_t *interface_id;
    uint8_t no_interfaces;
};

int xcan_router_init(struct xcan_routing_table *routing_table);

int xcan_router_receive(struct xcan_frame *f);


int xcan_router_loop(int loop_score);

#endif