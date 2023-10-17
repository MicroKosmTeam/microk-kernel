#pragma once
#include <cstdint.hpp>

int CalibrateTSCWithHPET(uptr hpetAddress, u64 *tscTicksPerSecond);
