#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
//  struct _AddressSpace *as;
  uintptr_t  gpr[31];
  struct _AddressSpace *as;
  uintptr_t cause;
  uintptr_t status;
  uintptr_t epc;
//  uintptr_t  gpr[31];

 // struct _AddressSpace *as;
};

#define GPR1 gpr[17]
#define GPR2 gpr[10]//cause
#define GPR3 gpr[11]//status
#define GPR4 gpr[12]//epc
#define GPRx gpr[10]

#endif
