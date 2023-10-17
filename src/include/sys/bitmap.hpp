#pragma once

#include <cstdint.hpp>

class Bitmap {
public:
	bool operator[](u64 index);
	bool Get(u64 index);
	bool Set(u64 index, bool value);
	usize size;
	u8 *buffer;
};
