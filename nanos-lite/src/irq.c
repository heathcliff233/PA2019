#include "common.h"
#include "proc.h"
_Context *do_syscall(_Context*);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
	case _EVENT_YIELD :
	  //printf("event yield\n");break;
	  c=schedule(c);
	  break;
    case _EVENT_SYSCALL :
      //printf("event syscall\n");
	  do_syscall(c); break;	   
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
