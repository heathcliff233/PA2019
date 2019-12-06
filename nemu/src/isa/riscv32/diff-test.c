#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool flag = true;
  for(int i=0; i<32;i++){
    if(cpu.gpr[i]._32 != (*ref_r).gpr[i]._32) {
		printf("GPR %d is %x but should get %x\n",i, cpu.gpr[i]._32, (*ref_r).gpr[i]._32);
		flag = false;
	}
  }
  if(cpu.stvec != (*ref_r).stvec){
    printf("stvec is %x but should get %x\n", cpu.stvec, (*ref_r).stvec);
    flag = true;
  }

  if(cpu.pc != (*ref_r).pc){
    printf("pc is %x but should be %x\n", cpu.pc, (*ref_r).pc);
	flag = false;
  }
  return flag;
}

void isa_difftest_attach(void) {
}
