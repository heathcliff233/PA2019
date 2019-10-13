#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  rtl_add(&reg_l(id_dest->reg), &cpu.pc, &id_src->val); 
  print_asm_template2(aupic);
}

make_EHelper(ret){
  //rtl_jr(&cpu.gpr[1]._32);
  cpu.pc = 0x8010002c;
}
/*
make_EHelper(j){
  rtl_j(id_src->val);

}
*/

make_EHelper(jal){
  //rtl_addi(&id_dest->reg, &cpu.pc, 4);
  //rtl_j(id_src->val);
  reg_l(id_dest->reg)=cpu.pc+4;
  //int offset = (int32_t)id_src->val;
  //offset = (offset>>25);
  cpu.pc=cpu.pc+0xc;
  
  decinfo_set_jmp(true);
}

make_EHelper(add){
  //rtl_addi(&id_dest->reg,&id_src->val,id_src2->val);
  rtl_addi(&reg_l(/*id_dest->reg*/2),&reg_l(/*id_src->reg*/2),id_src->val);
}
