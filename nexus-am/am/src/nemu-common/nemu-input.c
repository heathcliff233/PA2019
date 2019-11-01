#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      uint32_t data=inl(KBD_ADDR);
	  kbd->keydown = data & KEYDOWN_MASK ? 1 : 0;
      kbd->keycode = data & ~KEYDOWN_MASK;
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
