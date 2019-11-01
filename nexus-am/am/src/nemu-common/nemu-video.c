#include <am.h>
#include <amdev.h>
#include <nemu.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info->width = inl(SCREEN_ADDR);
      info->height = info->width & 0xffff;
	  info->width = info->width >> 16;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
	  int W = 300*400;
	  uint32_t *fb=(uint32_t*)(uintptr_t)FB_ADDR;
	  uint32_t *base = fb +ctl->y*W +ctl->x;
	  for(int dy=0; dy < ctl->h; ++dy){
		for(int dx=0; dx<ctl->w; ++dx){
			base[dy*W+dx]=ctl->pixels[dy*ctl->w+dx];
		}
	  }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
}
