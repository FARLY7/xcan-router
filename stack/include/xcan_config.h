#ifndef XCAN_CONFIG_H
#define XCAN_CONFIG_H

#ifndef __KERNEL__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#else
#include <linux/types.h>
#endif /* __KERNEL__ */

#ifdef DEBUG
#include <stdio.h>
#define dbg(f, ...) printf(f, ...)
#else
#define dbg(...) 
#endif

#define XCAN_ZALLOC(x) calloc(x, 0)
#define XCAN_FREE(x) free(x)

#endif /* XCAN_CONFIG_H */