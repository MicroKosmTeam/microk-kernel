#pragma once

#include <cstdint.hpp>

struct Bitmap {
	bool operator[](usize index);
	bool Get(usize index);
	bool Set(usize index, bool value);

	usize Size;
	u8 *Buffer;
};
