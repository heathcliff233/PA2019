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

void init_wp_pool();
WP* new_wp();
void free_wp(WP* wp);
void list_watchpoints();
bool del_watchpoint(int NO);
int set_watchpoint(char* e);
WP* scan_wp();


#endif
