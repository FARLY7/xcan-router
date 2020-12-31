#include <stdio.h>
#include <unistd.h>

#include "xcan_stack.h"
#include "xcan_dev_socketcan.h"


int main(int argc, char *argv[])
{
    struct xcan_device *dev0, *dev1;

    printf("***** XXCAN Linux Example *****\n");
#ifdef DEBUG
    printf("Debug: Enabled\n\n");
#else
    printf("Debug: Disabled\n\n");
#endif

    /* Initialise the XCAN stack */
    xcan_stack_init();

    /* Create the SocketCAN interface 0 */
    dev0 = xcan_socketcan_create(0, "SocketCAN-0");
    if(!dev0)
        return -1;

    /* Create the SocketCAN interface 1 */
    dev1 = xcan_socketcan_create(1, "SocketCAN-1");
    if(!dev1)
        return -1;

    /* Enter a forever loop, processing XCAN frames */
    while(1)
    {
        xcan_stack_tick();
        usleep(1000 * 500);
    }

    return 0;
}