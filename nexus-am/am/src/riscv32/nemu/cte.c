#include <am.h>
#include <riscv32.h>
#include <klib.h>

static _Context* (*user_handler)(_Event, _Context*) = NULL;

_Context* __am_irq_handle(_Context *c) {
  _Context *next = c;
  printf("cause %x\nstatus %x\nepc %x\n",c->cause,c->status,c->epc);
  printf("a21 %x\n",c->gpr[2]);
  printf("%x\n",c->as);
  if (user_handler) {
    _Event ev = {0};
    switch (c->cause) {	
		case 0: 
			ev.event = _EVENT_YIELD;break;
		/*
		case 0x1:
			ev.event = _EVENT_SYSCALL;break;
	    */
		default: 
			ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, c);
    if (next == NULL) {
      next = c;
    }
  }

  return next;
}

extern void __am_asm_trap(void);

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("li a7, -1; ecall");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
