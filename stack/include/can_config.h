#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

#ifndef __KERNEL__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#else
#include <linux/types.h>
#endif /* __KERNEL__ */

#define CAN_ZALLOC(x) calloc(x, 0)
#define CAN_FREE(x) free(x)

#endif /* CAN_CONFIG_H */