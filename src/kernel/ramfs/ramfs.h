#ifndef RAMFS_H
#define RAMFS_H

#include "stdint.h"

uint8_t* ramfs_get(const char *name, uint32_t *size_out);

#endif