#include "proc.h"
#include <elf.h>
#include "fs.h"

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf;
  
  //ramdisk_read(&elf, 0, sizeof(elf));
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &elf, sizeof(elf));

  Elf_Phdr segment[elf.e_phnum];
  
  //ramdisk_read(&segment, elf.e_phoff, elf.e_phentsize * elf.e_phnum);
  fs_lseek(fd, elf.e_phoff, SEEK_SET);
  fs_read(fd, &segment, elf.e_phnum * elf.e_phentsize);

  char buf[10000];
  for(int i=0; i<elf.e_phnum; i++){
    if(segment[i].p_type != PT_LOAD) continue;
	  //size_t content[segment[i].p_filesz];
	  //size_t buf[10000];

	  //ramdisk_read(content, segment[i].p_offset, segment[i].p_filesz);
    fs_lseek(fd, segment[i].p_offset, SEEK_SET);
	  
	int rem = segment[i].p_filesz;
	while(rem>0){
	  if(rem>=10000){
		fs_read(fd, buf, 10000);
		memcpy((void*)(segment[i].p_vaddr+segment[i].p_filesz-rem),buf,10000);
		rem -= 10000;
	  }else{

		fs_read(fd, buf, rem);
		memcpy((void*)(segment[i].p_vaddr+segment[i].p_filesz-rem),buf,rem);
		rem -= rem;
	  }
	}
	memset((void*)(segment[i].p_vaddr+segment[i].p_filesz), 0, segment[i].p_memsz - segment[i].p_filesz);
	
  }
  fs_close(fd);
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
