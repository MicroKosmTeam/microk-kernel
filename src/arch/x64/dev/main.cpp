#include <arch/x64/dev/main.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

#include <arch/x64/dev/apic.hpp>
#include <arch/x64/dev/hpet.hpp>
#include <arch/x64/dev/pit.hpp>
#include <arch/x64/dev/ioapic.hpp>

struct RSDP2 {
	unsigned char Signature[8];
	u8 Checksum;
	u8 OEMID[6];
	u8 Revision;
	u32 RSDTAddress;
	u32 Length;
	u64 XSDTAddress;
	u8 ExtendedChecksum;
	u8 Reserved[3];
}__attribute__((packed));

struct SDTHeader {
	unsigned char Signature[4];
	u32 Length;
	u8 Revision;
	u8 Checksum;
	u8 OEMID[6];
	u8 OEMTableID[8];
	u32 OEMRevision;
	u32 CreatorID;
	u32 CreatorRevision;
}__attribute__((packed));

struct GenericAddressStructure {
	u8 AddressSpace;
	u8 BitWidth;
	u8 BitOffset;
	u8 AccessSize;
	u64 Address;
}__attribute__((packed));

struct MADTHeader : public SDTHeader {
	u32 LocalAPICAddress;
	u32 Flags;
}__attribute__((packed));

#define MADT_RECORD_START_OFFSET 0x2C
struct MADTRecord {
	u8 EntryType;
	u8 RecordLength;
}__attribute__((packed));

#define MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC 0x00
struct MADTRecordProcessorLocalAPIC : public MADTRecord {
	u8 ACPIProcessorID;
	u8 APICID;
	u32 Flags;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC 0x01
struct MADTRecordIOAPIC : public MADTRecord {
	u8  IOAPICID;
	u8 Reserved0;
	u32 IOAPICAddress;
	u32 GlobalSystemInterruptBase;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE 0x02
struct MADTRecordIOAPICInterruptSourceOverride : public MADTRecord {
	u8 BusSource;
	u8 IRQSource;
	u32 GlobalSystemInterrupt;
	u16 Flags;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE 0x03
struct MADTRecordIOAPICNonMaskableInterruptSource : public MADTRecord {
	u8 NMISource;
	u8 Reserved0;
	u16 Flags;
	u32 GlobalSystemInterrupt;
}__attribute__((packed));

#define MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS 0x04
struct MADTRecordLocalAPICNonMaskableInterrupts: public MADTRecord {
	u8 ACPIProcessorID;
	u16 Flags;
	u8 LINT;
}__attribute__((packed));

#define MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE 0x05
struct MADTRecordLocalAPICAddressOverride: public MADTRecord {
	u16 Reserved0;
	u64 LocalAPICAddress;
}__attribute__((packed));

#define MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC 0x09
struct MADTRecordProcessorLocalx2APIC: public MADTRecord {
	u16 Reserved0;
	u32 ProcessorLocalx2APICID;
	u32 Flags;
	u32 ACPIProcessorID;
}__attribute__((packed));

#define SRAT_RECORD_START_OFFSET 0x2C
struct SRATRecord {
	u8 EntryType;
	u8 RecordLength;
}__attribute__((packed));

#define SRAT_RECORD_TYPE_PROCESSOR_LOCAL_APIC_AFFINITY 0x00
struct SRATRecordProcessorLocalAPICAffinity : public SRATRecord {
}__attribute__((packed));

#define SRAT_RECORD_TYPE_MEMORY_AFFINITY 0x01
struct SRATRecordMemoryAffinity : public SRATRecord {
}__attribute__((packed));

#define SRAT_RECORD_TYPE_PROCESSOR_LOCAL_x2APIC_AFFINITY 0x02
struct SRATRecordProcessorLocalx2APICAffinity: public SRATRecord {
}__attribute__((packed));

struct HPETHeader : public SDTHeader {
    u8 HardwareRevisionID;
    u8 ComparatorCount : 5;
    u8 CounterSize : 1;
    u8 Reserved0 : 1;
    u8 LegacyReplacement : 1;
    u16 PCIVendorID;
    GenericAddressStructure Address;
    u8 HPETNumber;
    u16 MinimumTick;
    u8 PageProtection;
}__attribute__((packed));

namespace x86_64 {
int HandleMADT(MADTHeader *madt);
int HandleSRAT(SDTHeader *srat);
int HandleHPET(HPETHeader *hpet);

int InitDevices() {
	KInfo *info = GetInfo();

	RSDP2 *rsdp = (RSDP2*)info->RSDP;

	/* We only accept ACPI 2.0+ */
	if(rsdp->Revision < 2) return -1;

	/* Select either the XSDT or the RSDT seeing whats available */
	SDTHeader *sdtAddr = rsdp->XSDTAddress ? (SDTHeader*)rsdp->XSDTAddress : (SDTHeader*)(uptr)rsdp->RSDTAddress;
	sdtAddr = (SDTHeader*)((uptr)sdtAddr + info->HigherHalfMapping);
	u8 size = rsdp->XSDTAddress ? 8 : 4;

	/* Scan through all the entries in the SDT */
	usize entries = (sdtAddr->Length - sizeof(SDTHeader)) / size;
	for (usize i = 0; i < entries; i++) {
		uptr addr = *(uptr*)((uptr)sdtAddr + sizeof(SDTHeader) + (i * size));
		SDTHeader *newSDTHeader = (SDTHeader*)(addr + info->HigherHalfMapping);

		if (Memcmp(newSDTHeader->Signature, "APIC", 4) == 0) {
			HandleMADT((MADTHeader*)newSDTHeader);
		} else if (Memcmp(newSDTHeader->Signature, "SRAT", 4) == 0) {
			HandleSRAT(newSDTHeader);
		} else if (Memcmp(newSDTHeader->Signature, "HPET", 4) == 0) {
			HandleHPET((HPETHeader*)newSDTHeader);
		}
	}

	return 0;
}

int HandleMADT(MADTHeader *madt) {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Local APIC detected at 0x%x.\r\n", madt->LocalAPICAddress);

	MADTRecord *record;
	uptr offset = MADT_RECORD_START_OFFSET;
	while(offset < madt->Length) {
		record = (MADTRecord*)((uptr)madt + offset);
		offset += record->RecordLength;

		switch(record->EntryType) {
			case MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC: {
				MADTRecordProcessorLocalAPIC *plapic = (MADTRecordProcessorLocalAPIC*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Processor Local APIC:\r\n"
					       " - ACPI Processor ID: %d\r\n"
				               " - APIC ID: %d\r\n"
					       " - Flags: %d\r\n",
						plapic->ACPIProcessorID, plapic->APICID, plapic->Flags);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC: {
				MADTRecordIOAPIC *ioapic = (MADTRecordIOAPIC*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "IOAPIC:\r\n"
					       " - IOAPIC ID: %d\r\n"
					       " - IOAPIC Address: 0x%x\r\n"
					       " - Global System Interrupt Base: 0x%x\r\n",
					       ioapic->IOAPICID, ioapic->IOAPICAddress, ioapic->GlobalSystemInterruptBase);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE: {
				MADTRecordIOAPICInterruptSourceOverride *ioapic = (MADTRecordIOAPICInterruptSourceOverride*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "IOAPIC Interrupt Source Override:\r\n"
					       " - Bus source: %d\r\n"
					       " - IRQ source: %d\r\n"
					       " - Global System Interrupt: 0x%x\r\n"
					       " - Flags: %d\r\n",
					       ioapic->BusSource, ioapic->IRQSource, ioapic->GlobalSystemInterrupt, ioapic->Flags);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE: {
				MADTRecordIOAPICNonMaskableInterruptSource *ioapic = (MADTRecordIOAPICNonMaskableInterruptSource*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "IOAPIC Non Maskable Interrupt Source:\r\n"
					       " - NMI source: %d\r\n"
					       " - Flags : 0x%x\r\n"
					       " - Global System Interrupt: 0x%x\r\n",
					       ioapic->NMISource, ioapic->Flags, ioapic->GlobalSystemInterrupt);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS: {
				MADTRecordLocalAPICNonMaskableInterrupts *lapic = (MADTRecordLocalAPICNonMaskableInterrupts*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Local APIC Non Maskable Interrupts:\r\n"
					       " - ACPI Processor ID: %d\r\n"
					       " - Flags : %d\r\n"
					       " - LINT#: %d\r\n",
					       lapic->ACPIProcessorID, lapic->Flags, lapic->LINT);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE: {
				MADTRecordLocalAPICAddressOverride *lapic = (MADTRecordLocalAPICAddressOverride*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Local APIC Address Override:\r\n"
					       " - Address: 0x%x\r\n",
					       lapic->LocalAPICAddress);
				}
				break;
			case MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC: {
				MADTRecordProcessorLocalx2APIC *x2apic = (MADTRecordProcessorLocalx2APIC*)record;
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Processor Local x2APIC:\r\n"
					       " - Local x2APIC ID: %d\r\n"
					       " - Flags: %d\r\n"
					       " - ACPI Processor ID: %d\r\n",
					       x2apic->ProcessorLocalx2APICID, x2apic->Flags, x2apic->ACPIProcessorID);
				}
				break;
			default:
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Unknown MADT Record: %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleSRAT(SDTHeader *srat) {
	SRATRecord *record;
	uptr offset = SRAT_RECORD_START_OFFSET;
	while(offset < srat->Length) {
		record = (SRATRecord*)((uptr)srat + offset);
		offset += record->RecordLength;
		
		switch(record->EntryType) {
			default:
				PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "Unknown SRAT Record. %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleHPET(HPETHeader *hpet) {
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "HPET:\r\n"
			" - Hardware Revision ID: %d\r\n"
			" - Comparator Count: %d\r\n"
			" - Counter Size: %d\r\n"
			" - Legacy Replacement : %d\r\n"
			" - PCI Vendor ID: 0x%x\r\n"
			" - Address: 0x%x\r\n"
			" - HPET Number: %d\r\n"
			" - Minimum Tick: %d\r\n"
			" - Page Protection: %d\r\n",
			hpet->HardwareRevisionID, hpet->ComparatorCount, hpet->CounterSize, hpet->LegacyReplacement,
			hpet->PCIVendorID, hpet->Address.Address, hpet->HPETNumber, hpet->MinimumTick, hpet->PageProtection);

	u64 tscPerSecond = 0;
	CalibrateTSCWithHPET(hpet->Address.Address, &tscPerSecond);
	PRINTK::PrintK(PRINTK::DEBUG, MODULE_NAME, "CPU is running at %d.%dMHz\r\n", tscPerSecond / 1000000, tscPerSecond % 1000000 / 1000);

	SetAPICTimer(tscPerSecond / 2000);
	EnableAPIC();

	return 0;
}
}
