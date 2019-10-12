#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  rtl_add(&id_dest->reg, &cpu.pc, &id_src->val); 
  print_asm_template2(aupic);
}

/*
make_EHelper(addi){
  rtl_add(&id_dest->reg, &id_src->reg, id_src->val); 
  print_asm_template2(aupic);
}
*/

make_EHelper(sw){
  rtl_sm(&id_src->addr,&id_src2->val,4);
}

make_EHelper(ret){
  rtl_jr(&cpu.gpr[1]._32);
}

make_EHelper(jal){
  rtl_addi(&id_dest->reg, &cpu.pc, 4);
  rtl_j(id_src->val);
}

