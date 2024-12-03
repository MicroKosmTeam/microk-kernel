#pragma once
#include <cdefs.h>
#include <arch/x86/ioapic.hpp>

namespace x86 {
	struct RSDP_t {
		char Signature[8];
		uint8_t Checksum;
		char OEMID[6];
		uint8_t Revision;
		uint32_t RsdtAddress;

		uint32_t Length;
		uint64_t XsdtAddress;
		uint8_t ExtendedChecksum;
		uint8_t reserved[3];
	} __attribute__ ((packed));

	struct SDTHeader_t {
		char Signature[4];
		uint32_t Length;
		uint8_t Revision;
		uint8_t Checksum;
		char OEMID[6];
		char OEMTableID[8];
		uint32_t OEMRevision;
		uint32_t CreatorID;
		uint32_t CreatorRevision;
	} __attribute__ ((packed));


	struct MADTEntry_t {
		u8 EntryType;
		u8 RecordLength;
	}__attribute__((packed));

	#define MADT_ENTRY_LAPIC 0
	struct MADTLAPICEntry_t : public MADTEntry_t {

	}__attribute__((packed));

	#define MADT_ENTRY_IOAPIC 1
	struct MADTIOAPICEntry_t : public MADTEntry_t {
		u8 IOAPICID;
		u8 Reserved0;
		u32 IOAPICAddress;
		u32 GlobalSystemInterruptBase;
	}__attribute__((packed));

	#define MADT_ENTRY_APIC_SOURCE_OVERRIDE 2
	struct MADTAPICSourceOverrideEntry_t : public MADTEntry_t {

	}__attribute__((packed));

	#define MADT_ENTRY_IOAPIC_NONMASK_SOURCE 3
	struct MADTIOAPICNonmaskableSourceEntry_t : public MADTEntry_t {

	}__attribute__((packed));
	#define MADT_ENTRY_LAPIC_NONMASK_INTERRUPT 4
	struct MADTLAPICNonmaskableInterruptEntry_t : public MADTEntry_t {

	}__attribute__((packed));
	#define MADT_ENTRY_LAPIC_ADDRESS_OVERRIDE 5
	struct MADTLAPICAddressOverrideEntry_t : public MADTEntry_t {

	}__attribute__((packed));
	#define MADT_ENTRY_LX2APIC 9
	struct MADTX2APICEntry_t : public MADTEntry_t {

	}__attribute__((packed));

	struct MADT_t : public SDTHeader_t {
		u32 LAPICAddress;
		u32 Flags;
		MADTEntry_t FirstEntry;
	}__attribute__((packed));

	struct SRATEntry_t {
		u8 EntryType;
		u8 RecordLength;
	}__attribute__((packed));

	#define SRAT_ENTRY_LAPIC 0
	struct SRATLapicEntry_t : public SRATEntry_t {
		uint8_t DomainLow;
		uint8_t APICID;
		uint32_t Flags;
		uint8_t SAPICEID;
		uint8_t DomainHigh[3];
		uint32_t ClockDomain;
	} __attribute__((packed));

	#define SRAT_ENTRY_MEMORY 1
	struct SRATMemoryEntry_t : public SRATEntry_t {
		uint32_t Domain;
		uint8_t Reserved1[2];
		uint32_t BaseLow;
		uint32_t BaseHigh;
		uint32_t LengthLow;
		uint32_t LengthHigh;
		uint8_t Reserved2[4];
		uint32_t Flags;
		uint8_t Reserved3[8];
	} __attribute__ ((packed));

	#define SRAT_ENTRY_LAPIC2 2
	struct SRATLapic2Entry_t : public SRATEntry_t {
		uint8_t Reserved1[2];
		uint32_t Domain;
		uint32_t X2APICID;
		uint32_t Flags;
		uint32_t ClockDomain;
		uint8_t Reserved2[4];
	} __attribute__((packed));

	struct SRAT_t : public SDTHeader_t {
		u8 Reserved[12];

		SRATEntry_t FirstEntry;
	}__attribute__((packed));

	struct MCFGEntry_t {
		uptr BaseAddress;
		u16 PCISeg;
		u8 StartPCIBus;
		u8 EndPCIBus;
		u8 Reserved[4];
	}__attribute__((packed));

	struct MCFG_t : public SDTHeader_t {
		u8 Reserved[8];

		MCFGEntry_t FirstEntry;
	}__attribute__((packed));

	struct PCIDeviceHeader_t {
		uint16_t VendorID;
		uint16_t DeviceID;
		uint16_t Command;
		uint16_t Status;
		uint8_t RevisionID;
		uint8_t ProgIF;
		uint8_t Subclass;
		uint8_t Class;
		uint8_t CacheLineSize;
		uint8_t LatencyTimer;
		uint8_t HeaderType;
		uint8_t BIST;
	}__attribute__((packed));


	#define MAX_IOAPIC 4

	struct ACPI {
		RSDP_t *RSDP;
		SDTHeader_t *MainSDT;
		u8 MainSDTPointerSize;

		u32 APICCount;
		u32 IOAPICCount;
		IOAPIC IOApics[MAX_IOAPIC];
	};

	int InitializeACPI(ACPI *acpi);
	int InitializeMADT(ACPI *acpi, MADT_t *madt);
	int InitializeSRAT(ACPI *acpi, SRAT_t *srat);
	int InitializeMCFG(ACPI *acpi, MCFG_t *srat);
}
