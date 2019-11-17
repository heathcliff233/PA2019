#include "cpu/exec.h"

make_EHelper(ec){
//  cpu.scause = instr_fetch(pc-4, 4);
  decinfo_set_jmp(true);  
}
