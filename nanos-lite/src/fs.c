#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char* pathname, int flag, int type){
  for(int i=0; i<NR_FILES; ++i){
    if(strcmp(file_table[i].name, pathname)) {
		file_table[i].open_offset = 0;
		return i;
	}
  }
  printf("no such file !\n");
  return 1;
}

size_t fs_read(int no, void *buf, size_t len){
  ReadFn read = file_table[no].read==NULL ? (ReadFn)ramdisk_read : file_table[no].read;
  if(file_table[no].open_offset+len > file_table[no].size) len=file_table[no].size-file_table[no].open_offset;
  int ret = read(buf, file_table[no].open_offset+file_table[no].disk_offset, len);
  file_table[no].open_offset += len;
  return ret;

}

size_t fs_size(int no){
  return file_table[no].size;
}
