#pragma once
#include <dev/acpi/acpi.hpp>

namespace x86_64 {
	struct MADTHeader {
		ACPI::SDTHeader Header;
		uint32_t LAPICAddress;
		uint32_t Flags;
		char EntriesBegin[];
        }__attribute__((packed));

	void DetectCores();
}
