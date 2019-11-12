# CAN Gateway Framework
This repository contains an experimental implementation of a CAN Gateway device, with additional examples of ECU's for generating and receiving CAN frames.
The intention of this project is to provide a open-source framework for a CAN Gateway, which can be ported to multiple platforms.

Initial testing was done on [Freescale/NXP MPC5748G](https://www.nxp.com/products/processors-and-microcontrollers/power-architecture/mpc55xx-5xxx-mcus/ultra-reliable-mpc57xx-mcus/ultra-reliable-mcus-for-automotive-industrial-control-and-gateway:MPC574xB-C-G) MCUs.

```Note: This is quite an old implementation, it is currently being redesigned and reimplemented with correct software practises.```

**Features**
* Frame-based routing of CAN frames across multiple CAN interfaces, using statically defined routing tables.
* Routing of CAN frames to alternative protocols, such as LIN, ETH, or Serial Debug.
* Firmware updates over CAN.
