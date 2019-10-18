#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}

make_EHelper(auipc){
  reg_l(id_dest->reg)=cpu.pc+((int32_t)id_src->val);
  print_asm_template2(aupic);
}

make_EHelper(jal){
  
  if(id_dest->reg != 0) reg_l(id_dest->reg)=cpu.pc+4;
  cpu.pc = (uint32_t)((int32_t)cpu.pc+(((int32_t)id_src->val)<<12>>12));

  decinfo_set_jmp(true);
  print_asm_template3(jal);
}

make_EHelper(addi){
  rtl_addi(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val);
}

make_EHelper(slli){
  rtl_shli(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val);
}

make_EHelper(srli){
  rtl_shri(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val);
}

make_EHelper(slti){
  if(reg_l(id_src->reg) < id_src2->val) reg_l(id_dest->reg)=1;
  else reg_l(id_dest->reg)=0;
}

make_EHelper(sltiu){
  if(reg_l(id_src->reg) < (uint32_t)id_src2->val) reg_l(id_dest->reg)=1;
  else reg_l(id_dest->reg)=0;
}

make_EHelper(andi){
  rtl_andi(&reg_l(id_dest->reg), &reg_l(id_src->reg),id_src2->val);
}

make_EHelper(srai){
  reg_l(id_dest->reg) = reg_l(id_src->reg)>>(id_src2->val&0x11);
}

make_EHelper(bq) {
  int sign =(int) (reg_l(id_src->reg) == reg_l(id_src2->reg));
  cpu.pc = cpu.pc+sign*id_dest->val;  
}

make_EHelper(jr) {
  if(id_dest->reg != 0) reg_l(id_dest->reg)=cpu.pc+4;
  cpu.pc = (reg_l(id_src->reg) + id_src2->val)&(~0x1);
  decinfo_set_jmp(true);

}

make_EHelper(ec){
}

make_EHelper(add){
  switch(decinfo.isa.instr.funct7 ){
	  case 0 : rtl_add(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
	  case 1 : rtl_mul_lo(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
	  case 32: rtl_sub(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
      //default: assert(0);
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
