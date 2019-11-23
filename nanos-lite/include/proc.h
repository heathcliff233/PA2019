#ifndef __PROC_H__
#define __PROC_H__

#include "common.h"
#include "memory.h"

#define STACK_SIZE (8 * PGSIZE)

typedef union {
  uint8_t stack[STACK_SIZE] PG_ALIGN;
  struct {
    _Context *cp;
    _AddressSpace as;
    // we do not free memory, so use `max_brk' to determine when to call _map()
    uintptr_t max_brk;
  };
} PCB;

extern PCB *current;

#endif

int fs_open(const char*, int, size_t);
size_t fs_size(int);
size_t fs_read(int, void*, size_t);
size_t ramdisk_read(void*, size_t, size_t);

void naive_uload(PCB*, const char*);
