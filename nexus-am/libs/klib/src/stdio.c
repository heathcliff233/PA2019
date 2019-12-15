/*#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
typedef struct OutputEngine {
    char ch;      // mutable
    int index;    // mutable
    void (*fn)(struct OutputEngine*);
    char* buf;
    int maxlen;
} OutputEngine;
static void buf_w(OutputEngine* eng) {
    char ch = eng->ch;
    int index = eng->index;
    int maxlen = eng->maxlen;
    if(index < maxlen - 1) {
        eng->buf[index] = ch;
    } else if(index == maxlen - 1) {
        eng->buf[index] = '\0';
    }
}

static void io_w(OutputEngine* eng) {
    char ch = eng->ch;
    _putc(ch);
}

static void exec(OutputEngine* eng, char ch, int index) {
    eng->ch = ch;
    eng->index = index;
    eng->fn(eng);
}

static int atoi_internal(char* buf, size_t x_, int base, int is_unsigned) {
    const char* alphabet = "0123456789abcdef";
    int prefix = 0;
    size_t x = x_;
    if(x < 0 && !is_unsigned) {
        buf[0] = '-';
        x = -x_;
        prefix = 1;
    } else if(x == 0) {
        buf[0] = '0';
        return 1;
    }
    int idx = prefix;
    while(x > 0) {
        buf[idx] = alphabet[x % base];
        x /= base;
        ++idx;
    }
    int iter1 = prefix;
    int iter2 = idx - 1;
    while(iter1 < iter2) {
        char tmp = buf[iter1];
        buf[iter1] = buf[iter2];
        buf[iter2] = tmp;
        ++iter1;
        --iter2;
    }
    return idx;
}

int handler(OutputEngine* eng, const char* fmt, va_list va) {
    int dest_idx = 0;

    while(*fmt) {
        if(*fmt != '%') {
            exec(eng, *fmt, dest_idx++);
            ++fmt;
            continue;
        }
        assert(*fmt == '%');
        ++fmt;
        int width = 0;
        char padding = ' ';
        if(*fmt == '0') {
            padding = '0';
            ++fmt;
        }

        while('0' <= *fmt && *fmt <= '9') {
            width *= 10;
            width += *fmt - '0';
            ++fmt;
        }
        switch(*fmt) {
            case 'd':
            case 'x': {
                int is_base16 = *fmt == 'x';
                int x = va_arg(va, int);
                char buf[30];
                const int delta = atoi_internal(buf, x, is_base16 ? 16 : 10, is_base16);
                while(delta < width) {
                    exec(eng, padding, dest_idx++);
                    --width;
                }
                for(int i = 0; i < delta; ++i) {
                    exec(eng, buf[i], dest_idx++);
                }
                ++fmt;
                continue;
            }
            case 'p': {
                width = sizeof(size_t) * 2;
                padding = '0';
                size_t x = va_arg(va, size_t);
                char buf[30];
                const int delta = atoi_internal(buf, x, 16, 1);
                while(delta < width) {
                    exec(eng, padding, dest_idx++);
                    --width;
                }
                for(int i = 0; i < delta; ++i) {
                    exec(eng, buf[i], dest_idx++);
                }
                ++fmt;
                continue;
            }
            case 's': {
                const char* str = va_arg(va, char*);
                const int delta = strlen(str);

                while(delta < width) {
                    exec(eng, padding, dest_idx++);
                    --width;
                }

                while(*str) {
                    exec(eng, *str++, dest_idx++);
                }
                ++fmt;
                continue;
            }
            default: goto FUCK;
        }
    }
    exec(eng, 0, dest_idx++);
FUCK:
    return dest_idx - 1;
}
int printf(const char *fmt, ...) {
  OutputEngine eng;
  eng.fn=io_w;
  va_list va;
  va_start(va,fmt);
  int ref=handler(&eng,fmt,va);
  va_end(va);
  return ref;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
    OutputEngine eng;
    eng.fn = buf_w;
    eng.buf = out;
    eng.maxlen = 1 << 30;
    va_list va;
    va_start(va, fmt);
    int ref = handler(&eng, fmt, va);
    va_end(va);
    return ref;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif*/
#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
#define MAX 200
    char buf[200];
    int i,n=vsnprintf(buf,-1,fmt,ap);
    assert(n<MAX);
#undef MAX
    for(i=0;i<n;++i){
        _putc(buf[i]);
    }
    return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    return vsnprintf(out,-1,fmt,ap);
}

int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    return vsnprintf(out,-1,fmt,ap);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    va_list ap;
    va_start(ap,fmt);
    return vsnprintf(out,n,fmt,ap);
}
//Added my Michael
//Should work as a assistant.
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap){
#define output(A) if(cnt<n-1){out[cnt++]=A;}else{va_end(ap);out[cnt]='\0';return cnt;}
    size_t cnt=0;
    int i=0;
//    double d;
    const char *p, *sval;
    char fill,num[10];
    int ival,fill_width;
//    double dval;
    uintptr_t uval;
    char cval;
    for(p=fmt;*p!='\0';++p){
        if(*p!='%'){
            output(*p);
            continue;
        }
        fill_width=0;
        fill=' ';
        //do {
            if (*p == '0') {
            fill = '0';
            ++p;
            }
            fill_width = 0;
            ++p;
            while (*p >= '0' && *p <= '9') {
                fill_width *= 10;
                fill_width += *p - '0';
                ++p;
            }
            switch (*p) {
                case 'c':
                    cval=va_arg(ap,int);
                    output(cval);
                    break;
                case 'u':
                    uval = va_arg(ap, uint32_t);
                    i = 0;
                    while (uval > 0) {
                        num[i++] = uval % 10 + '0';
                        uval /= 10;
                    }
                    while (fill_width > i) {
                        num[i++] = fill;
                    }
                    if (i == 0) {
                        output('0');
                    } else {
                        while (i > 0) {
                            output(num[--i]);
                        }
                    }
                    break;
                case 'x':
                case 'p':
                    uval = (uintptr_t) va_arg(ap, void * );
                    i = 8;
                    while (i > 0) {
                        output(
                                (uval >> ((sizeof(void *) << 3) - 4)) < 10 ?
                                (uval >> ((sizeof(void *) << 3) - 4)) + '0' :
                                (uval >> ((sizeof(void *) << 3) - 4)) - 10 + 'a');
                        uval <<= 4;
                        --i;
                    }
                    break;
                case 'd':
                    ival = va_arg(ap, int);
                    if (ival < 0) {
                        output('-');
                        ival = -ival;
                    }
                    while (ival > 0) {
                        num[i++] = ival % 10 + '0';
                        ival /= 10;
                    }
                    while (fill_width > i) {
                        num[i++] = fill;
                    }
                    if (i == 0) {
                        output('0');
                    } else {
                        while (i > 0) {
                            output(num[--i]);
                        }
                    }
                    break;
/*
				case 'f':
                    dval = va_arg(ap, double);
                    d=10000;
                    while (d > 0.001) {
                        int j = (int) dval / d;
                        output(j + '0');
                        dval -= ((int) dval / d) * d;
                        d /= 10;
                    }
                    break;
*/
				case 's':
                    for (sval = va_arg(ap, char * ); *sval != '\0'; ++sval) {
                        output(*sval);
                    }
                    break;
                default:
                    if (*p < '0' || *p > '9') {
                        output(*p);
                        printf("%%%c not realized.\n");
                        assert(0);
                        cnt+=vsprintf(out+cnt,"not realized\n",ap);
                        break;
                    }
            }
        //}while(0);
    }
    va_end(ap);
    output('\0');
    return cnt-1;
#undef output
}
#endif

