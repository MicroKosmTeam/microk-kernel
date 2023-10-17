#pragma once
#include <cstdint.hpp>

namespace x86_64 {
	void OutB(u16 port, u8 val);
	void OutW(u16 port, u16 val);
	void OutD(u16 port, u32 val);
	void OutQ(u16 port, u64 val);

	u8 InB(u16 port);
	u16 InW(u16 port);
	u32 InD(u16 port);
	u64 InQ(u16 port);

	void IoWait();
}
