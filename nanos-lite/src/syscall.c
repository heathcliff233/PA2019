#include "common.h"
#include "syscall.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
	  case SYS_exit :
		  _halt(0);
		  break;
	  case SYS_yield :
		  _yield();
		  printf("goooood\n");
		  c->GPRx = 0;
		  break;
	  case SYS_brk :
		  c->GPRx = 0;
		  break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
void sys_exit(_Context *c){
  _halt(0);
}

void sys_yield(_Context *c){
  _yield();
}
