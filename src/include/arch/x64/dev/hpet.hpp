#pragma once
#include <stdint.h>

int CalibrateTSCWithHPET(uintptr_t hpetAddress, uint64_t *tscCyclesPerMicrosecond);
