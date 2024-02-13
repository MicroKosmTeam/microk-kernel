#pragma once
#include <cdefs.h>

namespace x86 {
	void OutB(u16 port, u8 val);
	void OutW(u16 port, u16 val);
	void OutD(u16 port, u32 val);
#if defined(__x86_64__)
	void OutQ(u16 port, u64 val);
#endif

	u8 InB(u16 port);
	u16 InW(u16 port);
	u32 InD(u16 port);
#if defined(__x86_64__)
	u64 InQ(u16 port);
#endif


	void IoWait();
}
