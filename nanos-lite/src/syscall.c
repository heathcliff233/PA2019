#include "common.h"
#include "syscall.h"
void sys_exit(_Context *c);
void sys_yield(_Context *c);
void sys_brk(_Context *c);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
	  case SYS_exit :
		  sys_exit(c);
		  break;
	  case SYS_yield :
		  sys_yield(c);
		  break;
	  case SYS_brk :
		  sys_brk(c);
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
  c->GPRx = 0;
  c->GPR1 = 0;
  do_syscall(c);
}

void sys_brk(_Context *c){
  c->GPRx = 0;
  return;
}
