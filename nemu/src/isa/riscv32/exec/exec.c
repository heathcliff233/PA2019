#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EXW(lbu,4), EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EMPTY, EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static OpcodeEntry math_table [8] = {
  EXW(add,4), EMPTY, EMPTY, EMPTY, EXW(div,4), EXW(sr,4), EXW(rem,4), EMPTY
};

static make_EHelper(math) {
  //decinfo.width = math_table[decinfo.isa.instr.func3].width;
  //printf("%d",decinfo.isa.instr.funct3);
  idex(pc, &math_table[decinfo.isa.instr.funct3]);
}

/*
static OpcodeEntry jmp_table [8] = {
  EXW(j,4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EXW(jal,4)
};

static make_EHelper(jmp){
  decinfo.width = jmp_table[decinfo.isa.instr.funct3].width;
  idex(pc, &jmp_table[decinfo.isa.instr.funct3]);
}
*/
static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(Addi,addi), IDEX(U,auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(Math,math), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(BQ,bq), IDEX(JR, jr), EX(nemu_trap), IDEX(J,jal),IDEX(EC,ec), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
