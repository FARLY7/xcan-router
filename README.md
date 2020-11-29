# CAN Routing Stack

## Background

CAN is a robust message-based multi-master bus protocol which is particularly dominant in the automotive industry. In large control systems with different domains, there is often several independent CAN buses with each bus connecting together the nodes of a particular domain in the system.
It is the role of a central **gateway** device to pass certain messages between each these buses and enable the sharing of information across domains.

This is where the CAN Routing Stack fits in.

## Features

The intention of this project is to provide an open-source CAN routing stack.

This initial version will support the basic functionality:
- Frame-based routing.
- Static routing table creation tool.
- Per interface frame filtering.
- Frames routed when they are received, no special buffering or timing.
- Mock SW interface for testing.

Initial testing was done on [Freescale/NXP MPC5748G](https://www.nxp.com/products/processors-and-microcontrollers/power-architecture/mpc55xx-5xxx-mcus/ultra-reliable-mpc57xx-mcus/ultra-reliable-mcus-for-automotive-industrial-control-and-gateway:MPC574xB-C-G) MCUs.
