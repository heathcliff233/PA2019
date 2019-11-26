#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
  Elf_Ehdr elf;
  ramdisk_read(&elf, 0, sizeof(elf));

  Elf_Phdr segment[elf.e_phnum];
  ramdisk_read(&segment, elf.e_phoff, elf.e_phentsize * elf.e_phnum);
  
  for(int i=0; i<elf.e_phnum; i++){
    if(segment[i].p_type == PT_LOAD){
	  size_t content[segment[i].p_filesz];
	  ramdisk_read(content, segment[i].p_offset, segment[i].p_filesz);
      uint32_t *anch1 = (uint32_t*)segment[i].p_vaddr;
	  memcpy(anch1, content, segment[i].p_filesz);

	  if(segment[i].p_memsz > segment[i].p_filesz){
	    char* anch2 = (char*)(segment[i].p_vaddr + segment[i].p_filesz);
		memset(anch2, 0, segment[i].p_memsz - segment[i].p_filesz);
	  }
	}
  }
  return elf.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
