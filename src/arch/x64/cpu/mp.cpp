#include <arch/x64/cpu/mp.hpp>
#include <sys/panic.hpp>
#include <sys/printk.hpp>
#include <stddef.h>

namespace x86_64 {
void DetectCores() {
	MADTHeader *madt = FindTable(ACPI::GetXSDT(), (char*)"APIC");
	if (madt == 0) PANIC("MADT not fount");

	PRINTK::PrintK("MADT\r\n"
			" -> LAPIC: 0x%x\r\n"
			" -> Flags: 0x%x\r\n",
			madt->LAPICAddress,
			madt->Flags);


	for (uint8_t *ptr = madt->EntriesBegin;
			(uintptr_t)ptr < (uintptr_t)madt + madt->Header.Length;
			ptr += *(ptr + 1)) {
		PRINTK::PrintK("Type: %d\r\n", *ptr);
		switch (*ptr) {
			case 0:
				PRINTK::PrintK("-> PL APIC\r\n");
				break;
			case 1:
				PRINTK::PrintK("-> IO APIC\r\n");
				break;
			case 4:
				PRINTK::PrintK("-> LAPIC Interrupts\r\n");
				break;
			case 5:
				PRINTK::PrintK("-> LAPIC Address Override\r\n");
				break;
			default:
				PRINTK::PrintK("-> Unknown");
				break;
		}
	}
}
}
