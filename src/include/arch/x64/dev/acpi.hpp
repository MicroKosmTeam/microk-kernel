#pragma once
#include <dev/acpi/acpi.hpp>

namespace x86_64 {
	struct MADTHeader {
		ACPI::SDTHeader Header;
		uint32_t LAPICAddress;
		uint32_t Flags;
		char EntriesBegin[];
        }__attribute__((packed));

	struct MADTEntryHeader {
		uint8_t Type;
		uint8_t Length;
	}__attribute__((packed));

	struct PLAPICEntry {
		uint8_t ACPIProcessorID;
		uint8_t APICID;
		uint32_t Flags;
	}__attribute__((packed));

	struct IOAPICEntry {
		uint8_t IOAPICID;
		uint8_t Reserved;
		uint32_t IOAPICAddress;
		uint32_t GlobalSystemInterruptBase;
	}__attribute__((packed));

	void LoadMADT();
}
