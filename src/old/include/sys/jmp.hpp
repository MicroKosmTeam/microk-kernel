#pragma once
#include <cstdint.hpp>

typedef iptr *jmpbuf_t[5];
#define SetJMP __builtin_setjmp
#define LongJMP __builtin_longjmp
