#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(add);
make_EHelper(sub);
make_EHelper(sr);
make_EHelper(div);
make_EHelper(rem);

make_EHelper(auipc);
make_EHelper(ret);
make_EHelper(jal);
make_EHelper(addi);
make_EHelper(jr);
make_EHelper(bq);
make_EHelper(ec);
//make_EHelper(math);

