#pragma once
#include <stdint.h>
#include <stddef.h>

char* itoa(int num, char* str);
char* ltoa(long num, char* str);
char* itoh(int num, char* str);
char* ltoh(long num, char* str);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memclr(void *s, size_t n);
void *memmove(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
