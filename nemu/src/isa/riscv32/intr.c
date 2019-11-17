#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_li(&cpu.sepc, epc);
  rtl_li(&cpu.scause, NO);
  vaddr_t addr = cpu.stvec;
  cpu.pc = addr;
}

bool isa_query_intr(void) {
  return false;
}
