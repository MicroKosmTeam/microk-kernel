#include <random.hpp>

static u64 next = 1;

#define RANDOM_MAX 255

namespace RANDOM {
u8 Rand()  {
	next = next * 1103515245 + 12345;
	return (u8)(next / ((RANDOM_MAX + 1) * 2)) % (RANDOM_MAX + 1);
}

void SRand(u64 seed) {
	next = seed;
}

int Rng(u8 *dest, usize size) {
	for (usize i = 0; i < size; ++i) {
		dest[i] = Rand();
	}

	return 1;
}
}
