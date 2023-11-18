#pragma once
#include <cstdint.hpp>

namespace MATH {
	inline __attribute__((always_inline))
	bool IsPowerOfTwo(usize x) {
		return (!(x & (x - 1)) && x);
	}
	
	inline __attribute__((always_inline))
	u64 UpperPowerOfTwoU64(u64 x) {
		--x;
		x |= x >> 1;
		x |= x >> 2;
		x |= x >> 4;
		x |= x >> 8;
		x |= x >> 16;
		x |= x >> 32;
		++x;

		return x;
	}

	inline __attribute__((always_inline))
	u64 UpperPowerOfTwoUSIZE(u64 x) {
		return UpperPowerOfTwoU64(x);
	}
}
