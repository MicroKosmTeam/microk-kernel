#pragma once
#include <cstdint.hpp>

typedef struct {
    const u64 addr;
    const char *name;
} Symbol;

extern const Symbol symbols[];
extern const u64 symbolCount;

const Symbol *LookupSymbol(const char *name);
const Symbol *LookupSymbol(const u64 addr);
