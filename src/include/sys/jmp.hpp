#pragma once
#include <stdint.h>

typedef intptr_t *jmpbuf_t[5];
#define SetJMP __builtin_setjmp
#define LongJMP __builtin_longjmp
