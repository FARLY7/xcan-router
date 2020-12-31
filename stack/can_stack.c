#include "can_stack.h"

int can_stack_recv(struct can_device *dev, uint8_t buffer, uint32_t len)
{

    return 0;
}

/* ------- Initialisation ------- */
int can_stack_init(void)
{

    return 0;
}

/* ------- Loop Function -------- */
void can_stack_tick(void)
{

    can_devices_loop(1, 1);

    
}