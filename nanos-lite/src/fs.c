#include "fs.h"

size_t ramdisk_read(const void *buf, size_t offset, size_t len);
size_t ramdisk_write(void *buf, size_t offset, size_t len);

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

int fs_open(const char *pathname, int flags, int mode){
  printf("fsopen\n"); 
  int i;
  for (i = 0; i < NR_FILES; i++){
    if (strcmp(pathname, file_table[i].name) == 0){
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("The file is not found!");
}

int fs_close(int fd){
  printf("fsclose\n");
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len){
  printf("fsread\n");
  
  if (file_table[fd].read){
    return file_table[fd].read(buf, file_table[fd].disk_offset, len);
  }else{
    size_t count = file_table[fd].open_offset + len >= file_table[fd].size ? file_table[fd].size - file_table[fd].open_offset : len;
    ramdisk_read(buf, file_table[fd].open_offset + file_table[fd].disk_offset, count);
    file_table[fd].open_offset += count;
    return count;
  }
  
  /*
  size_t fs_size = file_table[fd].size;
  if(file_table[fd].open_offset+len > fs_size){
    len = fs_size - file_table[fd].open_offset;
  }
  if(file_table[fd].read == NULL){
    ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  }else{
    len = file_table[fd].read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
	return len;
  }
  */
}

size_t fs_write(int fd, void *buf, size_t len){
  if (file_table[fd].write == NULL){
    size_t count = file_table[fd].open_offset + len >= file_table[fd].size ? file_table[fd].size - file_table[fd].open_offset : len;
    if(fd != FD_STDOUT && fd != FD_STDERR){
	  ramdisk_write(buf, file_table[fd].open_offset + file_table[fd].disk_offset, count);
	}else{
	  for(int i=0; i<count; i++){
	    _putc(*(char*)(buf+i));
	  }
	}
    file_table[fd].open_offset += count;
    return count;
  }
  else{
    return file_table[fd].write(buf, file_table[fd].disk_offset, len);
  }
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch (whence){
    case SEEK_SET:
      assert(offset < file_table[fd].size);
      file_table[fd].open_offset = offset;
      break;

    case SEEK_CUR:
      assert(file_table[fd].open_offset + offset < file_table[fd].size);
      file_table[fd].open_offset += offset;
      break;

    case SEEK_END:
      assert(offset <= 0);
      file_table[fd].open_offset = file_table[fd].size + offset;
      break;

    default:
      panic("Should not reach here.");
  }

  return file_table[fd].open_offset;
}
