#include "xcan_stack.h"

int xcan_stack_recv(struct xcan_device *dev, uint8_t buffer, uint32_t len)
{

    return 0;
}

/* ------- Initialisation ------- */
int xcan_stack_init(void)
{

    return 0;
}

/* ------- Loop Function -------- */
void xcan_stack_tick(void)
{
    xcan_devices_loop(1, 1);
}