#include "cpu/exec.h"
#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t epc);

make_EHelper(ecb){
//  cpu.scause = instr_fetch(pc-4, 4);
//  decinfo_set_jmp(true); 
  if(id_src2->val == 0){
	TODO();
  	raise_intr(0,cpu.pc); 
  }
  else if(id_src2->val == 0x102){
    /*	
	uint32_t sipe = 0;
	sipe = cpu.sstatus;
	sipe = sipe<<26>>31<<1;
	cpu.sstatus |= sipe;
	cpu.sstatus |= 0x20;
	cpu.sstatus &= 0xfffffeff;
	*/
	TODO();
	cpu.pc = cpu.sepc+4;
  }
  else{
    TODO();
  }
}

make_EHelper(csrrc){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause &= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus &= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc &= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec; 
				   cpu.stvec &= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}



make_EHelper(csrrs){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause |= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus |= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc |= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec; 
				   cpu.stvec |= reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}

make_EHelper(csrrw){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause = reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus = reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc = reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec;
				   cpu.stvec = reg_l(id_src->reg);
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}

make_EHelper(csrrci){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause &= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus &= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc &= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec; 
				   cpu.stvec &= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}


make_EHelper(csrrsi){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause |= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus |= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc |= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec; 
				   cpu.stvec |= id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}

make_EHelper(csrrwi){
  uint32_t t;
  switch(id_src2->val){
       case 0x142: t = cpu.scause; 
				   cpu.scause = id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x100: t = cpu.sstatus;
				   cpu.sstatus = id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x141: t = cpu.sepc; 
				   cpu.sepc = id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x105: t = cpu.stvec; 
				   cpu.stvec = id_src->reg;
				   reg_l(id_dest->reg) = t;
				   break;
       case 0x0  : break;
  }
  if(id_dest->reg == 0) reg_l(0) = 0;
  raise_intr(cpu.scause, cpu.pc);
}

