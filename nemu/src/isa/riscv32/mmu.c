#include "nemu.h"

paddr_t page_translate(vaddr_t vaddr){
  if(cpu.satp>>31){
    uint32_t satpppn = cpu.satp<<10>>10;
	uint32_t va3122 = vaddr>>22;
	uint32_t pte = paddr_read(satpppn*4096+va3122*4,4);
	uint32_t pteppn = pte>>10;
	uint32_t leaf = pteppn*4096+((vaddr<<10>>10)>>12)*4;
	uint32_t pte2 = paddr_read(leaf,4);
	return (pte2>>10)*4096+(vaddr<<20>>20);

  }else{
    return vaddr;
  }
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if((addr>>12)==((addr+len-1)>>12)){
    paddr_t paddr = page_translate(addr);
	return paddr_read(paddr,len);
  
  }else{
    assert(0);
  }
  //return paddr_read(addr, len);
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if((addr>>12)==((addr+len-1)>>12)){
	paddr_t paddr = page_translate(addr);
	paddr_write(paddr,data,len);
  
  }else{
    assert(0);
  }
  //paddr_write(addr, data, len);
}
