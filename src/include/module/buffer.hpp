#pragma once
#include <mkmi.h>

namespace MODULE {
namespace Buffer {
	MKMI_Buffer *Create(uint64_t code, MKMI_BufferType type, size_t size);
	uint64_t IO(MKMI_Buffer *buffer, MKMI_BufferOperation operation, ...);
	uint64_t Delete(MKMI_Buffer *buffer);
}
}
