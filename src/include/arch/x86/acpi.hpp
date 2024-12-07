#pragma once
#include <cdefs.h>
#include <arch/x86/ioapic.hpp>

namespace x86 {
	struct RSDP_t {
		char Signature[8];
		u8 Checksum;
		char OEMID[6];
		u8 Revision;
		u32 RsdtAddress;

		u32 Length;
		u64 XsdtAddress;
		u8 ExtendedChecksum;
		u8 reserved[3];
	} __attribute__ ((packed));

	struct SDTHeader_t {
		char Signature[4];
		u32 Length;
		u8 Revision;
		u8 Checksum;
		char OEMID[6];
		char OEMTableID[8];
		u32 OEMRevision;
		u32 CreatorID;
		u32 CreatorRevision;
	} __attribute__ ((packed));

	struct AddressStructure_t {
		u8 AddressSpaceID;    // 0 - system memory, 1 - system I/O
		u8 RegisterBitWidth;
		u8 RegisterBitOffset;
		u8 Reserved0;
		u64 Address;
	}__attribute__((packed));

	struct FADT_t : public SDTHeader_t {
		u32 FirmwareCtrl;
		u32 Dsdt;

		// field used in ACPI 1.0; no longer in use, for compatibility only
		u8  Reserved;

		u8  PreferredPowerManagementProfile;
		u16 SCI_Interrupt;
		u32 SMI_CommandPort;
		u8  AcpiEnable;
		u8  AcpiDisable;
		u8  S4BIOS_REQ;
		u8  PSTATE_Control;
		u32 PM1aEventBlock;
		u32 PM1bEventBlock;
		u32 PM1aControlBlock;
		u32 PM1bControlBlock;
		u32 PM2ControlBlock;
		u32 PMTimerBlock;
		u32 GPE0Block;
		u32 GPE1Block;
		u8  PM1EventLength;
		u8  PM1ControlLength;
		u8  PM2ControlLength;
		u8  PMTimerLength;
		u8  GPE0Length;
		u8  GPE1Length;
		u8  GPE1Base;
		u8  CStateControl;
		u16 WorstC2Latency;
		u16 WorstC3Latency;
		u16 FlushSize;
		u16 FlushStride;
		u8  DutyOffset;
		u8  DutyWidth;
		u8  DayAlarm;
		u8  MonthAlarm;
		u8  Century;

		// reserved in ACPI 1.0; used since ACPI 2.0+
		u16 BootArchitectureFlags;

		u8  Reserved2;
		u32 Flags;

		// 12 byte structure; see below for details
		AddressStructure_t ResetReg;

		u8  ResetValue;
		u8  Reserved3[3];

		// 64bit pointers - Available on ACPI 2.0+
		u64                X_FirmwareControl;
		u64                X_Dsdt;

		AddressStructure_t X_PM1aEventBlock;
		AddressStructure_t X_PM1bEventBlock;
		AddressStructure_t X_PM1aControlBlock;
		AddressStructure_t X_PM1bControlBlock;
		AddressStructure_t X_PM2ControlBlock;
		AddressStructure_t X_PMTimerBlock;
		AddressStructure_t X_GPE0Block;
		AddressStructure_t X_GPE1Block;
	}__attribute__((packed));

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
		u8 DomainLow;
		u8 APICID;
		u32 Flags;
		u8 SAPICEID;
		u8 DomainHigh[3];
		u32 ClockDomain;
	} __attribute__((packed));

	#define SRAT_ENTRY_MEMORY 1
	struct SRATMemoryEntry_t : public SRATEntry_t {
		u32 Domain;
		u8 Reserved1[2];
		u32 BaseLow;
		u32 BaseHigh;
		u32 LengthLow;
		u32 LengthHigh;
		u8 Reserved2[4];
		u32 Flags;
		u8 Reserved3[8];
	} __attribute__ ((packed));

	#define SRAT_ENTRY_LAPIC2 2
	struct SRATLapic2Entry_t : public SRATEntry_t {
		u8 Reserved1[2];
		u32 Domain;
		u32 X2APICID;
		u32 Flags;
		u32 ClockDomain;
		u8 Reserved2[4];
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
		u16 VendorID;
		u16 DeviceID;
		u16 Command;
		u16 Status;
		u8 RevisionID;
		u8 ProgIF;
		u8 Subclass;
		u8 Class;
		u8 CacheLineSize;
		u8 LatencyTimer;
		u8 HeaderType;
		u8 BIST;
	}__attribute__((packed));

	struct PCIHeader0_t : public PCIDeviceHeader_t {
		u32 BAR0;
		u32 BAR1;
		u32 BAR2;
		u32 BAR3;
		u32 BAR4;
		u32 BAR5;
		u32 CardbusCISPointer;
		u16 SunbystemVendorID;
		u16 SubsystemID;
		u32 ExpansionROMBaseAddress;
		u8 CapabilitiesPointer;
		u8 Reserved0[3];
		u32 Reserved1;
		u8 InterruptLine;
		u8 InterruptPIN;
		u8 MinGrant;
		u8 MaxLatency;
	}__attribute__((packed));

	struct PCIHeader1_t : public PCIDeviceHeader_t {
		u32 BAR0;
		u32 BAR1;
		u8 PrimaryBusNumber;
		u8 SecondaryBusNumber;
		u8 SubordinateBusNumber;
		u8 SecondaryLatencyTimer;
		u8 IOBase;
		u8 IOLimit;
		u16 SecondaryStatus;
		u16 MemoryBase;
		u16 MemoryLimit;
		u16 PrefetchableMemoryBase;
		u16 PrefetchableMemoryLimit;
		u32 PrefetchableBaseUpper;
		u32 PrefetchableLimitUpper;
		u16 IOBaseUpper;
		u16 IOLimitUpper;
		u8 CapabilityPointer;
		u8 Reserved0[3];
		u32 ExpansionROMBaseAddress;
		u8 InterruptLine;
		u8 InterruptPIN;
		u16 BridgeControl;
	}__attribute__((packed));

	struct PCIHeader2_t : public PCIDeviceHeader_t {
		u32 CardBusBaseAddress;
		u8 CapabilityListOffset;
		u8 Reserved0;
		u16 SecondaryStatus;
		u8 PCIBusNumber;
		u8 CardBusNumber;
		u8 SubordinateBusNumber;
		u8 CardBusLatencyTimer;
		u32 MemoryBaseAddress0;
		u32 MemoryLimit0;
		u32 MemoryBaseAddress1;
		u32 MemoryLimit1;
		u32 IOBaseAddress0;
		u32 IOLimit0;
		u32 IOBaseAddress1;
		u32 IOLimit1;
		u8 InterruptLine;
		u8 InterruptPIN;
		u16 BridgeControl;
		u16 SubsystemDeviceID;
		u16 SubsystemVendorID;
		u32 LegacyModeBaseAddress;
	}__attribute__((packed));

	struct HPET_t: public SDTHeader_t {
		u8 HardwareRevID;
		u8 Comparator_count:5;
		u8 CounterSize:1;
		u8 Reserved0 :1;
		u8 LegacyReplacement:1;
		u16 PCIVendorID;
		AddressStructure_t Address;
		u8 HPETNumber;
		u16 MinimumTick;
		u8 PageProtection;
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
	int InitializeFADT(ACPI *acpi, FADT_t *fadt);
	int InitializeMADT(ACPI *acpi, MADT_t *madt);
	int InitializeSRAT(ACPI *acpi, SRAT_t *srat);
	int InitializeMCFG(ACPI *acpi, MCFG_t *srat);
	int InitializeHPET(ACPI *acpi, HPET_t *hpet);
}
