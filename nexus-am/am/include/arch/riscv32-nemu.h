#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {

  //struct _AddressSpace *as;
  uintptr_t gpr[31];
  struct _AddressSpace *as;
  uintptr_t cause; 
  uintptr_t status;
  uintptr_t epc;
  //struct _AddressSpace *as;
  
  
};

#define GPR1 gpr[16]
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[2]

#endif
