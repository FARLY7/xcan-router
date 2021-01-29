# XCAN Routing Stack

## Background

CAN is a robust message-based multi-master bus protocol which is particularly dominant in the automotive industry. In large control systems with different domains, there are often several independent CAN buses with each bus connecting together the nodes of a particular domain in the system.
It is the role of a central **gateway/router** device to pass certain messages between each these buses to enable the sharing of information across domains.

This is where the XCAN Routing Stack fits in.

The intention of this project is to provide an open-source CAN routing stack.

## Features

This initial version will support the basic functionality:
- Frame-based routing.
- Static routing table creation tool.
- Per interface frame filtering.
- SocketCAN interface for testing.

## Building
Project uses the `cmake` build system.

``` shell
$ cmake .
$ make
```

## Example
An example is provided which can be run on Linux using virtual SocketCAN interfaces.

### Setup
Enable the virtual CAN kernel module and create 2 virtual CAN interfaces:
``` shell
$ sudo modprobe vcan
$ sudo ip link add dev vcan0 type vcan
$ sudo ip link add dev vcan1 type vcan
```

Bring up the two virtual CAN interfaces:
``` shell
$ sudo ip link set up vcan0
$ sudo ip link set up vcan1
```
sudo ifconfig vcan0 up

### Running
Send data to the CAN bus, use the `cansend` utility:
```
$ cansend can0 123#1122334455667788
```
