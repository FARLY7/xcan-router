#include <stdio.h>
#include <time.h>

#include "can_stack.h"
#include "can_dev_socketcan.h"


int main(int argc, char *argv[])
{
    struct can_device *dev0, *dev1;

    printf("***** XCAN Linux Example *****\n");
    printf("Debug: Enabled\n\n");

    /* Initialise the XCAN stack */
    can_stack_init();

    /* Create the SocketCAN interface 0 */
    dev0 = can_socketcan_create(0, "SocketCAN-0");
    if(!dev0)
        return -1;

    /* Create the SocketCAN interface 1 */
    dev1 = can_socketcan_create(1, "SocketCAN-1");
    if(!dev1)
        return -1;

    /* Enter a forever loop, processing CAN frames */
    while(1)
    {
        can_stack_tick();
        usleep(1000 * 500);
    }

    return 0;
}