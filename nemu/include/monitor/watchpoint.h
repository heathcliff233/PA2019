#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  vaddr_t addr;
  char *expr;
  uint32_t old_value;
  uint32_t new_value;
  bool active;
} WP;

#endif
