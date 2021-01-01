#include <stdio.h>
#include <unistd.h>

#include "xcan_stack.h"
#include "xcan_dev_socketcan.h"

extern struct xcan_routing_table routing_table;

int main(int argc, char *argv[])
{
    struct xcan_device *dev0, *dev1;

    printf("***** XXCAN Linux Example *****\n");
#ifdef DEBUG
    printf("Debug: Enabled\n\n");
#else
    printf("Debug: Disabled\n\n");
#endif

    /**
     * Initialise XCAN stack.
     */
    xcan_stack_init(&routing_table);

    /**
     * Register CAN-bus interfaces.
     */
    dev0 = xcan_socketcan_create(0, "vcan0");
    if(!dev0)
        return -1;

    dev1 = xcan_socketcan_create(1, "vcan1");
    if(!dev1)
        return -1;

    /**
     * Process CAN frames.
     */
    while(1)
    {
        xcan_stack_tick();
        sleep(2);
    }

    return 0;
}