#include "common.h"
#include "syscall.h"
#include "fs.h"

void sys_exit(_Context *c);
void sys_yield(_Context *c);
void sys_write(_Context *c);
void sys_brk(_Context *c);
void sys_open(_Context *c);
void sys_close(_Context *c);
void sys_read(_Context *c);
void sys_lseek(_Context *c);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
	  case SYS_exit :
		  printf("sys_exit\n");
		  sys_exit(c);
		  break;
	  case SYS_yield :
		  printf("sys_yield\n");
		  sys_yield(c);
		  break;
	  case SYS_open :
		  printf("sys_open\n");
		  sys_open(c);
		  break;
	  case SYS_close :
		  printf("sys_close\n");
		  sys_close(c);
		  break;
	  case SYS_write :
		  printf("sys_write\n");
		  sys_write(c);
		  break;
	  case SYS_read :
		  printf("sys_read\n");
		  sys_read(c);
		  break;
	  case SYS_lseek :
		  printf("sys_lseek\n");
		  sys_lseek(c);
		  break;
	  case SYS_brk :
		  printf("sys_break\n");
		  sys_brk(c);
		  break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
void sys_exit(_Context *c){
  _halt(c->GPR2);
}

void sys_yield(_Context *c){
  _yield();
  c->GPRx = 0;
  //c->GPR1 = 0;
  //do_syscall(c);
}

/*
void sys_brk(_Context *c){
  c->GPRx = 0;
  return;
}

void sys_write(_Context *c){
  int fd = c->GPR2;
  char *ptr = (char*)(c->GPR3);
  uint32_t count = c->GPR4;
  if(fd == 1 || fd == 2){
    int i=0;
	for(i=0; i<count; i++){
	  _putc(ptr[i]);
	}
  	c->GPRx = i;
  }else{
    c->GPRx = -1;
  }
  return;
  
}
*/

void sys_write(_Context* c){
  c->GPRx = fs_write(c->GPR2, (char*)c->GPR3, c->GPR4);
  return;
}

void sys_brk(_Context* c){
  c->GPRx = 0;
  return;
}

void sys_open(_Context* c){
  c->GPRx = fs_open((char*)c->GPR2, 0, 0);
}

void sys_close(_Context* c){
  c->GPRx = fs_close(c->GPR2);
}

void sys_read(_Context* c){
  c->GPRx = fs_read(c->GPR2, (char*)c->GPR3, c->GPR4);
}

void sys_lseek(_Context* c){
  c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
}
