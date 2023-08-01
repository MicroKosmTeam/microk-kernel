#pragma once
#include <stdint.h>

int CalibrateTSCWith(void *hpetAddress, uint64_t *tscCyclesPerMicrosecond);
