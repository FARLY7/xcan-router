# CAN Gateway Framework (Development branch)

This repository contains an implementation of a CAN frame-based router.

The intention of this projet is to provide a simple open-source CAN routing stack which can be easily ported to multiple platforms.

This initial version will support rudimentary functions:
- CAN frame based routing.
- Static CAN routing table.
- Per interface CAN filtering.
- Frames routed on best effort basis, no timing or buffering.
- Mock SW CAN interface for testing.

Initial testing was done on [Freescale/NXP MPC5748G](https://www.nxp.com/products/processors-and-microcontrollers/power-architecture/mpc55xx-5xxx-mcus/ultra-reliable-mpc57xx-mcus/ultra-reliable-mcus-for-automotive-industrial-control-and-gateway:MPC574xB-C-G) MCUs.
