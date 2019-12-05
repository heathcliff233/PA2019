#include "fs.h"

size_t ramdisk_read(const void *buf, size_t offset, size_t len);
size_t ramdisk_write(void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);


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
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, events_read, invalid_write},
  {"/dev/fb", 0, 0, invalid_read, fb_write},
  {"/dev/fbsync", 0, 0, invalid_read, fbsync_write},
  {"/dev/tty", 0, 0, invalid_read, serial_write},
  {"/proc/dispinfo", 128, 0, dispinfo_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  int screen_height();
  int screen_width();
  file_table[4].size = screen_width() * screen_height() * 4;
  return;
}

int fs_open(const char *pathname, int flags, int mode){
  //printf("fsopen\n"); 
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
  //printf("fsclose\n");
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len){
  //printf("fsread\n");
  
  if (file_table[fd].read){
    size_t ret = file_table[fd].read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
    file_table[fd].open_offset += ret;
	return ret;
  }else{
  
    size_t count = file_table[fd].open_offset + len >= file_table[fd].size ? file_table[fd].size - file_table[fd].open_offset : len;
    ramdisk_read(buf, file_table[fd].open_offset + file_table[fd].disk_offset, count);
    file_table[fd].open_offset += count;
    return count;
  }
  
}

size_t fs_write(int fd, void *buf, size_t len){
  if (file_table[fd].write == NULL){
    size_t count = file_table[fd].open_offset + len >= file_table[fd].size ? file_table[fd].size - file_table[fd].open_offset : len;
	ramdisk_write(buf, file_table[fd].open_offset + file_table[fd].disk_offset, count);
	/*
	if(fd == 1 || fd == 2){
	  for(int i=0; i<count; i++){
	    _putc(*(char*)(buf+i));
	  }
	}
	*/
    file_table[fd].open_offset += count;
    return count;
  }
  
  else{
    size_t ret =  file_table[fd].write(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
	file_table[fd].open_offset += ret;
	return ret;
  }
  
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch (whence){
    case SEEK_SET:
      assert(offset <= file_table[fd].size);
      file_table[fd].open_offset = offset;
      break;

    case SEEK_CUR:
      assert(file_table[fd].open_offset + offset <= file_table[fd].size);
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
