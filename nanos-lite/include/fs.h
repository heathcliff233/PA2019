#ifndef __FS_H__
#define __FS_H__

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

//size_t ramdisk_read(void*, int, size_t);

//size_t ramdisk_write(void*, int, size_t);
int fs_open(const char* pathname, int flags, int mode);
int fs_close(int fd);
size_t fs_read(int fd, void* buf, size_t len);
size_t fs_write(int fd, void* buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);

#endif

