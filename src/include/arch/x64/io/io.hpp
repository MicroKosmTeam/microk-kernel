#pragma once
#include <stdint.h>

namespace x86_64 {
	void OutB(uint16_t port, uint8_t val);
	void OutW(uint16_t port, uint16_t val);
	void OutD(uint16_t port, uint32_t val);
	void OutQ(uint16_t port, uint64_t val);

	uint8_t InB(uint16_t port);
	uint16_t InW(uint16_t port);
	uint32_t InD(uint16_t port);
	uint64_t InQ(uint16_t port);

	void IoWait();
}
