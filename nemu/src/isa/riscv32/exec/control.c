#include "cpu/exec.h"
#include "monitor/monitor.h"
#include "rtl/rtl.h"
make_EHelper(beq){
	if(reg_l(id_src->reg)==reg_l(id_dest->reg)){
		id_src2->val=id_src2->val<<19;
		int32_t off=(int32_t)(id_src2->val)>>19;
		cpu.pc=cpu.pc+off;
		decinfo_set_jmp(true);
	}
}
make_EHelper(bne){
     if(reg_l(id_src->reg)!=reg_l(id_dest->reg)){
         id_src2->val=id_src2->val<<19;
		 int32_t off=(int32_t)(id_src2->val)>>19;
         cpu.pc=cpu.pc+off;
         decinfo_set_jmp(true);
    }
 }
make_EHelper(bge){
	if((int32_t)reg_l(id_src->reg)>=(int32_t)reg_l(id_dest->reg)){
		id_src2->val=id_src2->val<<19;
		int32_t off=(int32_t)(id_src2->val)>>19;
		cpu.pc=cpu.pc+off;
		decinfo_set_jmp(true);
	}
}
make_EHelper(bgeu){
	if((uint32_t)reg_l(id_src->reg)>=(uint32_t)reg_l(id_dest->reg)){
		id_src2->val=id_src2->val<<19;
		int32_t off=(int32_t)(id_src2->val)>>19;
		cpu.pc=cpu.pc+off;
		decinfo_set_jmp(true);
	}
}
make_EHelper(blt){
	if((int32_t)reg_l(id_src->reg)<(int32_t)reg_l(id_dest->reg)){
		id_src2->val=id_src2->val<<19;
		int32_t off=(int32_t)(id_src2->val)>>19;
		cpu.pc=cpu.pc+off;
		decinfo_set_jmp(true);
	}
}
make_EHelper(bltu){
	if((uint32_t)reg_l(id_src->reg)<(uint32_t)reg_l(id_dest->reg)){
		id_src2->val=id_src2->val<<19;
		int32_t off=(int32_t)(id_src2->val)>>19;
		cpu.pc=cpu.pc+off;
		decinfo_set_jmp(true);
	}
}

