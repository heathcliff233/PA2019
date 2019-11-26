#include "cpu/exec.h"
#include "rtl/rtl.h"
#include "monitor/monitor.h"
#include "cpu/decode.h"
#include "memory/memory.h"
make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);
  print_asm_template2(lui);
}
make_EHelper(auipc){
	int32_t se=(int32_t)id_src->val;
	reg_l(id_dest->reg)= cpu.pc+se;
	print_asm_template2(auipc);
}
make_EHelper(jal){
	if(id_dest->reg!=0){
	reg_l(id_dest->reg)=cpu.pc+4;
	}
	id_src->val=id_src->val<<11;
	int32_t of=(int32_t)id_src->val>>11;
	/*if(!(id_src->val&0x100000)){
	  cpu.pc=id_src->val+cpu.pc;
	}
	else{
		id_src->val=id_src->val | 0xfff00000;
		int32_t offset=(int32_t)id_src->val;
		cpu.pc=cpu.pc+offset-1;
	}*/
	cpu.pc=cpu.pc+of;
	decinfo_set_jmp(true);
	print_asm_template3(jal);
}
make_EHelper(jalr){
	if(id_dest->reg!=0){
	reg_l(id_dest->reg)=cpu.pc+4;
	}
	id_src2->val=id_src2->val<<11;
	int32_t of=(int32_t)id_src2->val>>11; printf("of %d\n",of);
	cpu.pc=reg_l(id_src->reg)+of;
	cpu.pc=cpu.pc&(~1);
	decinfo_set_jmp(true);
	print_asm_template3(jal);
}

make_EHelper(sltiu){
	id_src2->val=id_src2->val<<20;
	int32_t im=(int32_t)id_src2->val>>20;
    uint32_t imm=(uint32_t)im;
	if((uint32_t)reg_l(id_src->reg)<imm){
		reg_l(id_dest->reg)=1;
	}
	else{
		reg_l(id_dest->reg)=0;
	}
}
make_EHelper(slti){
	id_src2->val=id_src2->val<<20;
	int32_t im=(int32_t)id_src2->val>>20;
	if(reg_l(id_src->reg)<im){
		reg_l(id_dest->reg)=1;
	}
	else{
		reg_l(id_dest->reg)=0;
	}
}

make_EHelper(srai){
	switch(id_src2->val>>6){
		case 0b010000: {  rtl_sari(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val&0b000000111111);
					   break;
					   }
		case 0b000000: {  rtl_shri(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val&0b000000111111);
						  break;
                       }
		default: assert(0);
	}
}
make_EHelper(slli){
	switch(id_src2->val>>6){
		case 0b000000: {  rtl_shli(&reg_l(id_dest->reg),&reg_l(id_src->reg),id_src2->val&0b000000111111);
					   break;
					   }
		default: assert(0);
	}
}

make_EHelper(andi){
	id_src2->val=id_src2->val<<20;
	int32_t off=(int32_t)id_src2->val>>20;
	rtl_andi(&reg_l(id_dest->reg),&reg_l(id_src->reg),off);
}
make_EHelper(xori){
	id_src2->val=id_src2->val<<20;
	int32_t off=(int32_t)id_src2->val>>20;
	rtl_xori(&reg_l(id_dest->reg),&reg_l(id_src->reg),off);
}
make_EHelper(ori){
	id_src2->val=id_src2->val<<20;
	int32_t off=(int32_t)id_src2->val>>20;
	rtl_ori(&reg_l(id_dest->reg),&reg_l(id_src->reg),off);
}
/*make_EHelper(ret){
	cpu.pc=reg_l(1);
	decinfo_set_jmp(true);
   	//rtl_jr(&cpu.gpr[0]._32);
	//printf("ret");
}*/
make_EHelper(addi){
	id_src2->val=id_src2->val<<20;
	int32_t off=(int32_t)id_src2->val>>20;
	rtl_addi(&reg_l(id_dest->reg), &reg_l(id_src->reg),off);
	print_asm_template3(addi);
}
make_EHelper(add){
	switch(decinfo.isa.instr.funct7){
		case 32: reg_l(id_dest->reg)=reg_l(id_src->reg)-reg_l(id_src2->reg);
				 break;
		case 0: reg_l(id_dest->reg)=reg_l(id_src->reg)+reg_l(id_src2->reg);
				break;
		case 1: { if(decinfo.isa.instr.funct3==4){    
				  rtl_mul_lo(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
				   }
				  else if(decinfo.isa.instr.funct3==0){
                 rtl_mul_lo(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
				   }
				  break;
				}
		default: assert(0);
	}
}
make_EHelper(srl){
	switch(decinfo.isa.instr.funct7){
		case 0:rtl_shr(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 32:rtl_sar(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 1:rtl_div_q(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;

		default: assert(0);
	}
}
make_EHelper(div){
	switch(decinfo.isa.instr.funct7){
		case 1:rtl_idiv_q(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 0:rtl_xor(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		default: assert(0);
	}
}

make_EHelper(sll){
	switch(decinfo.isa.instr.funct7){
		case 0:rtl_shl(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 1:rtl_imul_hi(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		default: assert(0);
	}
}
make_EHelper(or){
	switch(decinfo.isa.instr.funct7){
		case 0:rtl_or(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 1:rtl_idiv_r(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		default: assert(0);
	}
}
make_EHelper(and){
	switch(decinfo.isa.instr.funct7){
		case 0:rtl_and(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		case 1:rtl_div_r(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;

		default: assert(0);
	}
}
make_EHelper(slt){
	switch(decinfo.isa.instr.funct7){
		case 0:if((int32_t)reg_l(id_src->reg)<(int32_t)reg_l(id_src2->reg)){
				   reg_l(id_dest->reg)=1;
			   }
			   else{
				   reg_l(id_dest->reg)=0;
			   }
			   break;
		default: assert(0);
	}
}
make_EHelper(sltu){
	switch(decinfo.isa.instr.funct7){
		case 0:if((uint32_t)reg_l(id_src->reg)<(uint32_t)reg_l(id_src2->reg)){
				   reg_l(id_dest->reg)=1;
			   }
			   else{
				   reg_l(id_dest->reg)=0;
			   }
			   break;
		case 1:rtl_mul_hi(&reg_l(id_dest->reg),&reg_l(id_src->reg),&reg_l(id_src2->reg));
			   break;
		default: assert(0);
	}
}
