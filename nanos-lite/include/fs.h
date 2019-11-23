#ifndef __FS_H__
#define __FS_H__

#include "common.h"

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

#endif

size_t ramdisk_read(void*, int, size_t);

size_t ramdisk_write(void*, int, size_t);
