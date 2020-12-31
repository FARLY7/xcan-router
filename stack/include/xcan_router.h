#ifndef XCAN_ROUTER_H
#define XCAN_ROUTER_H

#include "xcan_config.h"
#include "xcan_frame.h"

int xcan_router_receive(struct xcan_frame *f);

#endif