#include <arch/x64/dev/main.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>

#include <arch/x64/dev/apic.hpp>
#include <arch/x64/dev/hpet.hpp>
#include <arch/x64/dev/pit.hpp>
#include <arch/x64/dev/ioapic.hpp>

struct RSDP2 {
	unsigned char Signature[8];
	uint8_t Checksum;
	uint8_t OEMID[6];
	uint8_t Revision;
	uint32_t RSDTAddress;
	uint32_t Length;
	uint64_t XSDTAddress;
	uint8_t ExtendedChecksum;
	uint8_t Reserved[3];
}__attribute__((packed));

struct SDTHeader {
	unsigned char Signature[4];
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	uint8_t OEMID[6];
	uint8_t OEMTableID[8];
	uint32_t OEMRevision;
	uint32_t CreatorID;
	uint32_t CreatorRevision;
}__attribute__((packed));

struct GenericAddressStructure {
	uint8_t AddressSpace;
	uint8_t BitWidth;
	uint8_t BitOffset;
	uint8_t AccessSize;
	uint64_t Address;
}__attribute__((packed));

struct MADTHeader : public SDTHeader {
	uint32_t LocalAPICAddress;
	uint32_t Flags;
}__attribute__((packed));

#define MADT_RECORD_START_OFFSET 0x2C
struct MADTRecord {
	uint8_t EntryType;
	uint8_t RecordLength;
}__attribute__((packed));

#define MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC 0x00
struct MADTRecordProcessorLocalAPIC : public MADTRecord {
	uint8_t ACPIProcessorID;
	uint8_t APICID;
	uint32_t Flags;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC 0x01
struct MADTRecordIOAPIC : public MADTRecord {
	uint8_t  IOAPICID;
	uint8_t Reserved0;
	uint32_t IOAPICAddress;
	uint32_t GlobalSystemInterruptBase;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE 0x02
struct MADTRecordIOAPICInterruptSourceOverride : public MADTRecord {
	uint8_t BusSource;
	uint8_t IRQSource;
	uint32_t GlobalSystemInterrupt;
	uint16_t Flags;
}__attribute__((packed));

#define MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE 0x03
struct MADTRecordIOAPICNonMaskableInterruptSource : public MADTRecord {
	uint8_t NMISource;
	uint8_t Reserved0;
	uint16_t Flags;
	uint32_t GlobalSystemInterrupt;
}__attribute__((packed));

#define MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS 0x04
struct MADTRecordLocalAPICNonMaskableInterrupts: public MADTRecord {
	uint8_t ACPIProcessorID;
	uint16_t Flags;
	uint8_t LINT;
}__attribute__((packed));

#define MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE 0x05
struct MADTRecordLocalAPICAddressOverride: public MADTRecord {
	uint16_t Reserved0;
	uint64_t LocalAPICAddress;
}__attribute__((packed));

#define MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC 0x09
struct MADTRecordProcessorLocalx2APIC: public MADTRecord {
	uint16_t Reserved0;
	uint32_t ProcessorLocalx2APICID;
	uint32_t Flags;
	uint32_t ACPIProcessorID;
}__attribute__((packed));

#define SRAT_RECORD_START_OFFSET 0x2C
struct SRATRecord {
	uint8_t EntryType;
	uint8_t RecordLength;
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
    uint8_t HardwareRevisionID;
    uint8_t ComparatorCount : 5;
    uint8_t CounterSize : 1;
    uint8_t Reserved0 : 1;
    uint8_t LegacyReplacement : 1;
    uint16_t PCIVendorID;
    GenericAddressStructure Address;
    uint8_t HPETNumber;
    uint16_t MinimumTick;
    uint8_t PageProtection;
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
	SDTHeader *sdtAddr = rsdp->XSDTAddress ? (SDTHeader*)rsdp->XSDTAddress : (SDTHeader*)(uintptr_t)rsdp->RSDTAddress;
	sdtAddr = (SDTHeader*)((uintptr_t)sdtAddr + info->HigherHalfMapping);
	uint8_t size = rsdp->XSDTAddress ? 8 : 4;

	/* Scan through all the entries in the SDT */
	size_t entries = (sdtAddr->Length - sizeof(SDTHeader)) / size;
	for (size_t i = 0; i < entries; i++) {
		uintptr_t addr = *(uintptr_t*)((uintptr_t)sdtAddr + sizeof(SDTHeader) + (i * size));
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
	PRINTK::PrintK("Local APIC detected at 0x%x.\r\n", madt->LocalAPICAddress);

	MADTRecord *record;
	uintptr_t offset = MADT_RECORD_START_OFFSET;
	while(offset < madt->Length) {
		record = (MADTRecord*)((uintptr_t)madt + offset);
		offset += record->RecordLength;

		switch(record->EntryType) {
			case MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC: {
				MADTRecordProcessorLocalAPIC *plapic = (MADTRecordProcessorLocalAPIC*)record;
				PRINTK::PrintK("Processor Local APIC:\r\n"
					       " - ACPI Processor ID: %d\r\n"
				               " - APIC ID: %d\r\n"
					       " - Flags: %d\r\n",
						plapic->ACPIProcessorID, plapic->APICID, plapic->Flags);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC: {
				MADTRecordIOAPIC *ioapic = (MADTRecordIOAPIC*)record;
				PRINTK::PrintK("IOAPIC:\r\n"
					       " - IOAPIC ID: %d\r\n"
					       " - IOAPIC Address: 0x%x\r\n"
					       " - Global System Interrupt Base: 0x%x\r\n",
					       ioapic->IOAPICID, ioapic->IOAPICAddress, ioapic->GlobalSystemInterruptBase);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE: {
				MADTRecordIOAPICInterruptSourceOverride *ioapic = (MADTRecordIOAPICInterruptSourceOverride*)record;
				PRINTK::PrintK("IOAPIC Interrupt Source Override:\r\n"
					       " - Bus source: %d\r\n"
					       " - IRQ source: %d\r\n"
					       " - Global System Interrupt: 0x%x\r\n"
					       " - Flags: %d\r\n",
					       ioapic->BusSource, ioapic->IRQSource, ioapic->GlobalSystemInterrupt, ioapic->Flags);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE: {
				MADTRecordIOAPICNonMaskableInterruptSource *ioapic = (MADTRecordIOAPICNonMaskableInterruptSource*)record;
				PRINTK::PrintK("IOAPIC Non Maskable Interrupt Source:\r\n"
					       " - NMI source: %d\r\n"
					       " - Flags : 0x%x\r\n"
					       " - Global System Interrupt: 0x%x\r\n",
					       ioapic->NMISource, ioapic->Flags, ioapic->GlobalSystemInterrupt);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS: {
				MADTRecordLocalAPICNonMaskableInterrupts *lapic = (MADTRecordLocalAPICNonMaskableInterrupts*)record;
				PRINTK::PrintK("Local APIC Non Maskable Interrupts:\r\n"
					       " - ACPI Processor ID: %d\r\n"
					       " - Flags : %d\r\n"
					       " - LINT#: %d\r\n",
					       lapic->ACPIProcessorID, lapic->Flags, lapic->LINT);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE: {
				MADTRecordLocalAPICAddressOverride *lapic = (MADTRecordLocalAPICAddressOverride*)record;
				PRINTK::PrintK("Local APIC Address Override:\r\n"
					       " - Address: 0x%x\r\n",
					       lapic->LocalAPICAddress);
				}
				break;
			case MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC: {
				MADTRecordProcessorLocalx2APIC *x2apic = (MADTRecordProcessorLocalx2APIC*)record;
				PRINTK::PrintK("Processor Local x2APIC:\r\n"
					       " - Local x2APIC ID: %d\r\n"
					       " - Flags: %d\r\n"
					       " - ACPI Processor ID: %d\r\n",
					       x2apic->ProcessorLocalx2APICID, x2apic->Flags, x2apic->ACPIProcessorID);
				}
				break;
			default:
				PRINTK::PrintK("Unknown MADT Record: %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleSRAT(SDTHeader *srat) {
	SRATRecord *record;
	uintptr_t offset = SRAT_RECORD_START_OFFSET;
	while(offset < srat->Length) {
		record = (SRATRecord*)((uintptr_t)srat + offset);
		offset += record->RecordLength;
		
		switch(record->EntryType) {
			default:
				PRINTK::PrintK("Unknown SRAT Record. %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleHPET(HPETHeader *hpet) {
	PRINTK::PrintK("HPET:\r\n"
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

	uint64_t tscPerSecond = 0;
	CalibrateTSCWithHPET(hpet->Address.Address, &tscPerSecond);
	PRINTK::PrintK("CPU is running at %d.%dMHz\r\n", tscPerSecond / 1000000, tscPerSecond % 1000000 / 1000);

	SetAPICTimer(tscPerSecond / 2000);
	EnableAPIC();

	return 0;
}
}
