#include "cpu/exec.h"
#include "rtl/rtl.h"

make_EHelper(ld) {
  rtl_lm(&reg_l(id_dest->reg), &id_src->addr, decinfo.width);
  rtl_sr(id_dest->reg, &reg_l(id_dest->reg), 4);

  switch (decinfo.width) {
    case 4: print_asm_template2(lw); break;
    case 2: print_asm_template2(lhu); break;
    case 1: print_asm_template2(lbu); break;
    default: assert(0);
  }
  printf("%x\n",cpu.gpr[1]._32);
}
make_EHelper(lh) {
     rtl_lm(&reg_l(id_dest->reg), &id_src->addr, decinfo.width);
     rtl_sext(&reg_l(id_dest->reg), &reg_l(id_dest->reg), decinfo.width); 
	 printf("%x\n",cpu.gpr[1]._32);

}
make_EHelper(st) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);

  switch (decinfo.width) {
    case 4: print_asm_template2(sw); break;
    case 2: print_asm_template2(sh); break;
    case 1: print_asm_template2(sb); break;
    default: assert(0);
  }
}
