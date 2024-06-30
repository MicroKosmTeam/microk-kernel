#pragma once
#include <cdefs.h>

#define PAGE_SIZE 0x1000
#define HUGE_PAGE_SIZE (512 * PAGE_SIZE)
#define HUGER_PAGE_SIZE (512 * HUGE_PAGE_SIZE)

namespace x86 {
	struct InterruptStack {
		u64 VectorNumber;
		u64 ErrorCode;

		u64 IretRIP;
		u64 IretCS;
		u64 IretRFLAGS;
		u64 IretRSP;
		u64 IretSS;
	}__attribute__((packed));

	struct GeneralRegisters {
		u64 R15;
		u64 R14;
		u64 R13;
		u64 R12;
		u64 R11;
		u64 R10;
		u64 R9;
		u64 R8;

		u64 RDX;
		u64 RCX;
		u64 RBX;
		u64 RAX;

		u64 RSI;
		u64 RDI;
	}__attribute__((packed));
}

