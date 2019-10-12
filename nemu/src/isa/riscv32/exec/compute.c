#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_Ehelper(auipc){
  rtl_add(&id_dest->reg, &cpu.pc, id_src->val); 
  print_asm_template2(aupic);
}

/*
make_Ehelper(addi){
  rtl_add(&id_dest->reg, &id_src->reg, id_src->val); 
  print_asm_template2(aupic);
}
*/

make_Ehelper(sw){
  rtl_sm(&id_src->addr,&id_src2->val,4);
}

make_Ehelper(ret){
  rtl_jr(&cpu.ra);
}

make_Ehelper(jal){
  rtl_add(&id_dest->reg, &cpu.pc, 4);
  rtl_j(id_src->val);
}

