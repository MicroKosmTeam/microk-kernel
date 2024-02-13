#pragma once
#include <cdefs.h>

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
	usize UpperPowerOfTwoUSIZE(usize x) {
		return UpperPowerOfTwoU64(x);
	}

	inline __attribute__((always_inline))
	usize POW(usize num, usize pow) {
		usize result = 1;

		while (pow--) {
			result *= num;
		}

		return result;
	}

	/* Only applicable to numbers that ARE powers of two */
	inline __attribute__((always_inline))
	usize GetPowerOfTwo(usize num) {
		return __builtin_ctz(num); /* Count to get trailing zeros */
	}

	inline __attribute__((always_inline))
	usize ElevatePowerOfTwo(usize exp) {
		return (1 << exp);
	}

}
