#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EXW(lh,1), EXW(lh, 2), EXW(lh, 4), EMPTY,  EXW(ld,1), EXW(ld,2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st,1), EXW(st,2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}
static OpcodeEntry bq_table [8] = {
  EXW(beq,4), EXW(bne,4), EMPTY, EMPTY, EXW(blt,4), EXW(bge,4), EXW(bltu,4), EXW(bgeu,4)
};
static make_EHelper(bq) {
  decinfo.width = bq_table[decinfo.isa.instr.funct3].width;
  idex(pc, &bq_table[decinfo.isa.instr.funct3]);
 }

static OpcodeEntry adim_table [8] = {
  EXW(addi,4), EXW(slli,4), EXW(slti,4), EXW(sltiu,4), EXW(xori,4), EXW(srai,4), EXW(ori,4),    EXW(andi,4)
};

static make_EHelper(adim) {
  decinfo.width = adim_table[decinfo.isa.instr.funct3].width;
  idex(pc, &adim_table[decinfo.isa.instr.funct3]);
 }

static OpcodeEntry ad_table [8] = {
  EXW(add,4), EXW(sll,4), EXW(slt,4), EXW(sltu,4), EXW(div,4), EXW(srl,4), EXW(or,4), EXW(and, 4)
};

static make_EHelper(ad) {
  decinfo.width = ad_table[decinfo.isa.instr.funct3].width;
  idex(pc, &ad_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry jmp_table [8] = {
  EX(jalr), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(jmp) {
  decinfo.width = jmp_table[decinfo.isa.instr.funct3].width;
  idex(pc, &jmp_table[decinfo.isa.instr.funct3]);
}


/*
static OpcodeEntry ec_table [8] = {
  EX(ecb), EX(csrrw), EX(csrrs), EX(csrrc), EMPTY, EX(csrrwi), EX(csrrsi), EX(csrrci)
};
*/
static OpcodeEntry ec_table [8] = {
  EX(ecb), EX(csrrw), EX(csrrs), EX(csrrc), EMPTY, EMPTY, EX(csrrsi), EX(csrrci)
};

static make_EHelper(ec) {
  decinfo.width = jmp_table[decinfo.isa.instr.funct3].width;
  idex(pc, &ec_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I,adim), IDEX(U,auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(R,ad), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(B,bq), IDEX(I,jmp), EX(nemu_trap), IDEX(J,jal), IDEX(EC,ec), EMPTY,  EMPTY, EMPTY
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
