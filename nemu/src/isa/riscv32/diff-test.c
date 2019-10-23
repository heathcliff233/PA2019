#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  ref_difftest_getregs(&ref_r);
  int i = 0;
  for(i=0;i<32;i++){
    if(cpu.gpr[i]._32 != ref_r.gpr[i]._32){
	  printf("nemu get gpr[%4d]=0x%x while it should be 0x%x",i,cpu.gpr[i]._32,ref_r.gpr[i]._32);
	  return false;
	}
  }
  if(cpu.pc != ref_r.pc){
	printf("nemu pc points to 0x%x while it should be at 0x%x",cpu.pc,ref_r.pc);
	return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
