#include "nemu.h"

const char *regsl[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
    for(int i=0 ; i<32 ; i++){
        uint32_t a = reg_l(i);
        printf("%s:0x%08x\n",regsl[i],a);
	}
}


uint32_t isa_reg_str2val(const char *s, bool *success) {
	int i=0;
	int sg=0;
	while(!strcmp(*regsl[i], *s)){
		if(i==31){
			*success = false;
			return 0;
		}
		i++;
	}
	return reg_l(i);
}

