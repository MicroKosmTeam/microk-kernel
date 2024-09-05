#pragma once
#include <cstdint.h>

#ifdef __cplusplus
extern "C" {
#endif

usize Strnlen(const char *str, usize maxLength);
char *Strncpy(char *dest, const char *src, usize maxLength);
int Strncmp(const char *first, const char *second, usize maxLength);

usize Strcspn(const char *s1, const char *s2);
usize Strspn(const char *s1, const char *s2);
char *Strtok(char *string, const char *delim, char **savePtr);

void Itoa (char *buf, int base, long long int d);
long long int Atoi(char *str);

void *Memset(void *start, u8 value, u64 num);
void *Memclr(void *start, usize num);
void *Memcpy(void *dest, const void *src, usize n);
int Memcmp(const void* buf1, const void* buf2, usize count);
void *Memmove(void *dest, const void *src, usize n);

int memcmp(const void* buf1, const void* buf2, usize count);
void *memcpy(void *dest, const void *src, usize n);
void *memmove(void *dest, const void *src, usize n);
void *memset(void *start, u8 value, usize num);

#ifdef __cplusplus
}
#endif
