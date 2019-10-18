#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  reg_l(id_dest->reg)=cpu.pc+((int32_t)id_src->val);
  print_asm_template2(aupic);
}
/*
make_EHelper(ret){
  //rtl_jr(&cpu.gpr[1]._32);
  cpu.pc = reg_l(1);
  decinfo_set_jmp(true);
}

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
	cpu.pc = cpu.pc + offset;
  }
  
  decinfo_set_jmp(true);
  //printf("jump\n");
  print_asm_template3(jal);
}

make_EHelper(addi){
  //rtl_addi(&id_dest->reg,&id_src->val,id_src2->val);
  rtl_addi(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val);
  //rtl_addi(&cpu.gpr[id_dest->reg]._32,&cpu.gpr[id_src->reg]._32,id_src->val);

}

make_EHelper(bq) {
  int sign =(int) (id_src->val ==id_src2->val);
  cpu.pc = cpu.pc+sign*id_dest->val;  
}

make_EHelper(jr) {
  reg_l(id_dest->reg)=cpu.pc+4;
  cpu.pc = id_src->val + (id_src2->val&(~0x1));
  decinfo_set_jmp(true);

}

make_EHelper(ec){
}
/*
make_EHelper(math){
  id_dest->val = id_src->val + id_src2->val;
}
*/

make_EHelper(add){
  //printf("shit");
  if(decinfo.isa.instr.funct7 == 0){
	  rtl_add(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }else{
	  rtl_sub(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }
}

make_EHelper(sr) {
  if(decinfo.isa.instr.funct7 == 0) {
    rtl_shr(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }else if(decinfo.isa.instr.funct7==0x1){
    rtl_mul_lo(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }else{
    rtl_sar(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg)); 
  }
}

make_EHelper(sl) {
  if(decinfo.isa.instr.funct7==0){
	rtl_shl(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }else if(decinfo.isa.instr.funct7==0x1){
    rtl_imul_hi(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }
}

make_EHelper(div) {
  if(decinfo.isa.instr.funct7 == 0) {
	rtl_xor(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }else{
	rtl_idiv_q(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
  }
}

make_EHelper(rem) {
  rtl_div_r(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
}

make_EHelper(sltu) {
  if((uint32_t)reg_l(id_src->reg) < (uint32_t)reg_l(id_src2->reg)) {
	  reg_l(id_dest->reg)=1;
  }else {
	  reg_l(id_dest->reg)=0;
  }
}

make_EHelper(slt) {
  if(reg_l(id_src->reg) < reg_l(id_src2->reg)) {
	  reg_l(id_dest->reg)=1;
  }else {
	  reg_l(id_dest->reg)=0;
  }
}
