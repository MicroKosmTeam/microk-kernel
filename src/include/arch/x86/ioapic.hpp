#pragma once
#include <cdefs.h>

#define IOAPIC_ID	0x00
#define IOAPIC_VER	0x01
#define IOAPIC_ARB	0x02
#define IOAPIC_REDTBL(n)	(0x10 + 2 * n)

namespace x86 {
	struct IOAPIC {
		u32 ID;

		uptr Base;
		uptr MappedAddress;
	};

	int InitializeIOAPIC(IOAPIC *ioapic, u32 id, uptr address);
}
