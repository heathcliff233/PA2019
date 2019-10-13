#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  rtl_add(&id_dest->reg, &cpu.pc, &id_src->val); 
  print_asm_template2(aupic);
}

make_EHelper(ret){
  //rtl_jr(&cpu.gpr[1]._32);
  cpu.pc = 0x8010002c;
}

make_EHelper(j){
  rtl_j(id_src->val);

}

make_EHelper(jal){
  //rtl_addi(&id_dest->reg, &cpu.pc, 4);
  //rtl_j(id_src->val);
  cpu.gpr[id_dest->reg]._32=cpu.pc+4;
  //printf("%d\n",id_dest->reg);
  int offset = (int32_t)id_src->val;
  offset = (offset>>25);
  cpu.pc=cpu.pc+0xc;
  decinfo_set_jmp(true);
}

make_EHelper(add){
  rtl_addi(&id_dest->reg,&id_src->val,id_src2->val);
}
