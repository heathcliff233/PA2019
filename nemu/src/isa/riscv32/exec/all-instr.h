#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);
make_EHelper(ldd);//signed

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(add);
make_EHelper(sub);
make_EHelper(sr);
make_EHelper(sl);
make_EHelper(div);
make_EHelper(rem);
make_EHelper(slt);
make_EHelper(sltu);
make_EHelper(and);

make_EHelper(auipc);
make_EHelper(ret);
make_EHelper(jal);

make_EHelper(addi);
make_EHelper(slli);
make_EHelper(srli);
make_EHelper(slti);
make_EHelper(sltiu);
make_EHelper(andi);
make_EHelper(srai);
make_EHelper(xori);

make_EHelper(beq);
make_EHelper(bne);

make_EHelper(jr);
make_EHelper(ec);
//make_EHelper(math);

