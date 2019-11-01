#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  bool in_use;
  union{
	  vaddr_t addr;
	  struct{
		  char *expr;
		  uint32_t old_val;
		  uint32_t new_val;
	  };
  };
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

#endif
