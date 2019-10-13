#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  reg_l(id_dest->reg)=cpu.pc+((int32_t)id_src->val);
  print_asm_template2(aupic);
}

make_EHelper(ret){
  //rtl_jr(&cpu.gpr[1]._32);
  cpu.pc = reg_l(1);
  decinfo_set_jmp(true);
}
/*
make_EHelper(j){
  rtl_j(id_src->val);
}
*/

make_EHelper(jal){
  //rtl_addi(&id_dest->reg, &cpu.pc, 4);
  //rtl_j(id_src->val);
  //reg_l(/*id_dest->reg*/1)=cpu.pc+4;
  //reg_l(id_dest->reg)=cpu.pc+4;
  //int offset = (int32_t)id_src->val;
  //offset = (offset>>25);
  reg_l(id_dest->reg)=cpu.pc+4;
  if(!(id_src->val&0x100000)){
    cpu.pc=id_src->val+cpu.pc;
  }
  else{
    id_src->val=id_src->val | 0xfff00000;
	int32_t offset = (int32_t)id_src->val;
	cpu.pc = cpu.pc + offset -1;
	//cpu.pc = 0x80100010;
  }
  
  decinfo_set_jmp(true);
  printf("jump\n");
  print_asm_template3(jal);
}

make_EHelper(add){
  //rtl_addi(&id_dest->reg,&id_src->val,id_src2->val);
  rtl_addi(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val);
  //rtl_addi(&cpu.gpr[id_dest->reg]._32,&cpu.gpr[id_src->reg]._32,id_src->val);

}

