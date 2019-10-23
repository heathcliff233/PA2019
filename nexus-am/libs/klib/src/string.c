#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
    const char *p = s;
    while(*p++);
 
    return (size_t)(p - s - 1);
}


char *strcpy(char* dst,const char* src) {
  assert(dst != NULL && src != NULL);
  char* ret = dst;
  while((*dst++ = *src++)!='\0');
  return ret;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst != NULL && src != NULL);
  char* ret = dst;
  while(n){
	  if((*ret++=*src++)) n--;
  }
  return dst;
}

char* strcat(char* dst, const char* src) {
  assert(dst != NULL && src != NULL);
  char* ret = dst;
  while(*dst)dst++;
  while((*dst++ = *src++)!='\0');
  return ret;
}

int strcmp(const char* s1, const char* s2) {
  while((*s1 == *s2) && (*s1)){
    s1++;
	s2++;
  }
  if(*(unsigned char*)s1 > *(unsigned char*)s2)return 1;
  else if(*(unsigned char*)s1 < *(unsigned char*)s2)return -1;
  return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
   if (!n)  return 0;
 
   while (--n && *s1 && *s1 == *s2) s1++,s2++;
 
   return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
}


void* memset(void* v,int c,size_t n) {
  if(v==NULL || n<0) return NULL;
  char* tmp = v;
  while(n-- > 0)*tmp++ = c;
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  if(in == NULL || out == NULL || n<0) return NULL;
  char* tmpOut = (char*)out;
  char* tmpIn  = (char*)in ;
  while(n-- > 0) *tmpOut++ = *tmpIn++;
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
    assert(s1);
	assert(s2);
	char* a = (char*)s1;
	char* b = (char*)s2;
	while(n && *a==*b){
	  a++;
	  b++;
	  n--;
	}
	if(n==0)return 0;
	else return (*a-*b-'\0');
}

#endif
