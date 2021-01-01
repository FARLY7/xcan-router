#include "xcan_router.h"

struct xcan_routing_table routing_table = {
    .no_entries = 3,
    .entry = (struct xcan_routing_entry[]) { 
        {
            .can_id = 0,
            .no_interfaces = 2,
            .interface_id = (uint8_t[]) { 1, 2 }
        },
        {
            .can_id = 1,
            .no_interfaces = 2,
            .interface_id = (uint8_t[]) { 0, 1 }

        },
        {
            .can_id = 2,
            .no_interfaces = 2,
            .interface_id = (uint8_t[]) { 0, 2 }
        }
    }
};