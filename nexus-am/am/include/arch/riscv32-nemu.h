#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  
  uintptr_t gpr[31];
  struct _AddressSpace *as;
  uintptr_t cause, status, epc;
  
  /*
  uintptr_t epc, status, cause;
  uintptr_t gpr[31];
  struct _AddressSpace *as;
  */
  
};

#define GPR1 gpr[17]
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
