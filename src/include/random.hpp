#pragma once
#include <object.hpp>

namespace RANDOM {
	u8 Rand();
	void SRand(u64 seed);

	/* Return 1 on success, 0 on failure */
	int Rng(u8 *dest, usize size);
}
