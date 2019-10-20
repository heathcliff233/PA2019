#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EXW(ldd,4), EXW(ldd,4), EXW(ldd, 4), EMPTY, EXW(ld,4), EXW(ld,4), EXW(ld,4), EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st,4), EXW(st,4), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry math_table [8] = {
  EXW(add,4), EXW(sl,4), EXW(slt,4), EXW(sltu,4), EXW(div,4), EXW(sr,4), EXW(rem,4), EXW(and,4)
};

static make_EHelper(math) {
  //decinfo.width = math_table[decinfo.isa.instr.func3].width;
  //printf("%d",decinfo.isa.instr.funct3);
  idex(pc, &math_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry mathi_table [8] = {
  EXW(addi,4), EXW(slli,4), EXW(slti,4), EXW(sltiu,4), EXW(xori,4), EXW(srai,4), EXW(srli,4), EXW(andi,4)
};

static make_EHelper(mathi) {
  idex(pc, &mathi_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry bq_table [8] = {
  EXW(beq,4), EXW(bne,4), EMPTY, EMPTY, EXW(blt,4), EXW(bge,4), EXW(bltu,4), EXW(bgeu,4)
};

static make_EHelper(bq) {
  idex(pc, &bq_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(Mathi,mathi), IDEX(U,auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(Math,math), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(BQ,bq), IDEX(JR, jr), EX(nemu_trap), IDEX(J,jal),IDEX(EC,ec), EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
