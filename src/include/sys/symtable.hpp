#pragma once
#include <stdint.h>

/*
    The kernel symble table that contains the addresses of functions available to
    modules
*/
extern uint64_t *KRNLSYMTABLE;

void SetupSymtable();