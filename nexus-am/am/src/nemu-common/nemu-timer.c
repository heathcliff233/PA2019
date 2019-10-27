#include <am.h>
#include <amdev.h>
#include <nemu.h>

//static _DEV_TIMER_UPTIME_t boot_time;
static uint32_t boo_time;
size_t __am_timer_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
      _DEV_TIMER_UPTIME_t *uptime = (_DEV_TIMER_UPTIME_t *)buf;
/*      static uint32_t last_hi=0,last_lo=0;
      uptime->lo = inl(RTC_ADDR);
      if(uptime->lo < last_lo) ++last_hi;
	  last_lo = uptime->lo;
	  uptime->hi = last_hi;
	  if(uptime->lo < boot_time.lo) uptime->hi--;
	  uptime->lo -= boot_time.lo;
*/	  
	  uint32_t ms = inl(RTC_ADDR)-boo_time;
	  uptime->hi = 0;
	  uptime->lo = ms;
	  return sizeof(_DEV_TIMER_UPTIME_t);
    }
    case _DEVREG_TIMER_DATE: {
      _DEV_TIMER_DATE_t *rtc = (_DEV_TIMER_DATE_t *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2000;
      return sizeof(_DEV_TIMER_DATE_t);
    }
  }
  return 0;
}

void __am_timer_init() {
}
