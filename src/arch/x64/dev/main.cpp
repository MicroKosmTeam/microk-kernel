#include <arch/x64/dev/main.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/timer.hpp>

#include <arch/x64/cpu/cpu.hpp>
#include <arch/x64/dev/apic.hpp>
#include <arch/x64/dev/hpet.hpp>
#include <arch/x64/dev/pit.hpp>
#include <arch/x64/dev/ioapic.hpp>

static u8 ValidateACPITable(u8 *ptr, usize size) {
	u8 checksum = 0;

	for (usize i = 0; i < size; ++i) checksum += ptr[i];

	return checksum;
}

static inline void AddIOAPICToMachine(DEV::CPU::TopologyStructure *machine, x86_64::IOAPIC::IOAPIC *ioapic) {
	x86_64::PerMachineTopology *machineTopology = (x86_64::PerMachineTopology*)machine->ArchitectureSpecificInformation;

	x86_64::IOAPICNode *current = machineTopology->IOAPICList;
	x86_64::IOAPICNode *previous = NULL;

	while (current != NULL) {
		previous = current;
		current = current->Next;
	}

	current = new x86_64::IOAPICNode;
	current->IOAPIC = ioapic;
	current->Next = NULL;
	current->Previous = previous;
	
	PRINTK::PrintK(PRINTK_DEBUG "Added IOAPIC #%d to machine #%d\r\n", ioapic->ID, machine->ID);

	if (previous != NULL) previous->Next = current;
}

namespace x86_64 {
int InitDevices() {
	KInfo *info = GetInfo();

	RSDP2 *rsdp = (RSDP2*)info->RSDP;

	/* We only accept ACPI 2.0+ */
	if (rsdp->Revision < 2) {
		PRINTK::PrintK(PRINTK_DEBUG "Invalid ACPI version.\r\n");
		return -1;
	}

	if (ValidateACPITable((u8*)rsdp, rsdp->Length)) {
		PRINTK::PrintK(PRINTK_DEBUG "Invalid RSDP 2 table.\r\n");
		return -1;

	}

	/* Select either the XSDT or the RSDT seeing whats available */
	SDTHeader *sdtAddr = rsdp->XSDTAddress != 0 ?
			     (SDTHeader*)(rsdp->XSDTAddress + info->HigherHalfMapping) :
			     (SDTHeader*)(rsdp->RSDTAddress + info->HigherHalfMapping);

	u8 size = rsdp->XSDTAddress != 0 ? sizeof(u64) : sizeof(u32);

	if (ValidateACPITable((u8*)sdtAddr, sdtAddr->Length)) {
		PRINTK::PrintK(PRINTK_DEBUG "Invalid SDT table.\r\n");
		return -1;
	}

	/* Scan through all the entries in the SDT */
	usize entries = (sdtAddr->Length - sizeof(SDTHeader)) / size;
	for (usize i = 0; i < entries; i++) {
		uptr addr = *(uptr*)((uptr)sdtAddr + sizeof(SDTHeader) + (i * size));
		SDTHeader *newSDTHeader = (SDTHeader*)(addr + info->HigherHalfMapping);
				
		if (ValidateACPITable((u8*)newSDTHeader, newSDTHeader->Length))
			continue;

		if (Memcmp(newSDTHeader->Signature, "APIC", 4) == 0) {
			HandleMADT((MADTHeader*)newSDTHeader);
		} else if (Memcmp(newSDTHeader->Signature, "SRAT", 4) == 0) {
			HandleSRAT((SRATHeader*)newSDTHeader);
		} else if (Memcmp(newSDTHeader->Signature, "HPET", 4) == 0) {
			HandleHPET((HPETHeader*)newSDTHeader);
		}
	}

	return 0;
}

int HandleMADT(MADTHeader *madt) {
	KInfo *info = GetInfo();

	PerCoreCPUTopology *bootCoreInfo = (PerCoreCPUTopology*)info->BootCore->ArchitectureSpecificInformation;

	MADTRecord *record;
	uptr offset = sizeof(MADTHeader);
	while(offset < madt->Length) {
		record = (MADTRecord*)((uptr)madt + offset);
		offset += record->RecordLength;

		switch(record->EntryType) {
			case MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC: {
				MADTRecordProcessorLocalAPIC *plapic = (MADTRecordProcessorLocalAPIC*)record;
				PRINTK::PrintK(PRINTK_DEBUG "Processor Local APIC:\r\n"
					       " - ACPI Processor ID: %d\r\n"
				               " - APIC ID: %d\r\n"
					       " - Flags: %d\r\n",
						plapic->ACPIProcessorID, plapic->APICID, plapic->Flags);

				if (plapic->APICID == bootCoreInfo->LocalAPIC->ID) {
					PRINTK::PrintK(PRINTK_DEBUG " - It's the current CPU.\r\n");
					info->BootCore->ID = plapic->ACPIProcessorID;
				} else {
					DEV::CPU::TopologyStructure *coreParent = info->BootCore->Parent;
					DEV::CPU::TopologyStructure *core = DEV::CPU::CreateTopologyStructure(coreParent, plapic->ACPIProcessorID);

					PerCoreCPUTopology *coreInfo = new PerCoreCPUTopology;
					core->ArchitectureSpecificInformation = (void*)coreInfo;
				}

				}
				break;
			case MADT_RECORD_TYPE_IOAPIC: {
				MADTRecordIOAPIC *ioapicRecord = (MADTRecordIOAPIC*)record;
				PRINTK::PrintK(PRINTK_DEBUG "IOAPIC:\r\n"
					       " - IOAPIC ID: %d\r\n"
					       " - IOAPIC Address: 0x%x\r\n"
					       " - Global System Interrupt Base: 0x%x\r\n",
					       ioapicRecord->IOAPICID, ioapicRecord->IOAPICAddress, ioapicRecord->GlobalSystemInterruptBase);

				IOAPIC::IOAPIC *ioapic = (IOAPIC::IOAPIC*)IOAPIC::CreateIOAPICDevice();
				DEV::InitializeDevice((DEV::Device*)ioapic, (u32)ioapicRecord->IOAPICID, ioapicRecord->IOAPICAddress, ioapicRecord->GlobalSystemInterruptBase);

				AddIOAPICToMachine(info->DefaultMachine, ioapic);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE: {
				MADTRecordIOAPICInterruptSourceOverride *ioapic = (MADTRecordIOAPICInterruptSourceOverride*)record;
				PRINTK::PrintK(PRINTK_DEBUG "IOAPIC Interrupt Source Override:\r\n"
					       " - Bus source: %d\r\n"
					       " - IRQ source: %d\r\n"
					       " - Global System Interrupt: 0x%x\r\n"
					       " - Flags: %d\r\n",
					       ioapic->BusSource, ioapic->IRQSource, ioapic->GlobalSystemInterrupt, ioapic->Flags);
				}
				break;
			case MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE: {
				MADTRecordIOAPICNonMaskableInterruptSource *ioapic = (MADTRecordIOAPICNonMaskableInterruptSource*)record;
				PRINTK::PrintK(PRINTK_DEBUG "IOAPIC Non Maskable Interrupt Source:\r\n"
					       " - NMI source: %d\r\n"
					       " - Flags : 0x%x\r\n"
					       " - Global System Interrupt: 0x%x\r\n",
					       ioapic->NMISource, ioapic->Flags, ioapic->GlobalSystemInterrupt);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS: {
				MADTRecordLocalAPICNonMaskableInterrupts *lapic = (MADTRecordLocalAPICNonMaskableInterrupts*)record;
				PRINTK::PrintK(PRINTK_DEBUG "Local APIC Non Maskable Interrupts:\r\n"
					       " - ACPI Processor ID: %d\r\n"
					       " - Flags : %d\r\n"
					       " - LINT#: %d\r\n",
					       lapic->ACPIProcessorID, lapic->Flags, lapic->LINT);
				}
				break;
			case MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE: {
				MADTRecordLocalAPICAddressOverride *lapic = (MADTRecordLocalAPICAddressOverride*)record;
				PRINTK::PrintK(PRINTK_DEBUG "Local APIC Address Override:\r\n"
					       " - Address: 0x%x\r\n",
					       lapic->LocalAPICAddress);
				}
				break;
			case MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC: {
				MADTRecordProcessorLocalx2APIC *x2apic = (MADTRecordProcessorLocalx2APIC*)record;
				PRINTK::PrintK(PRINTK_DEBUG "Processor Local x2APIC:\r\n"
					       " - Local x2APIC ID: %d\r\n"
					       " - Flags: %d\r\n"
					       " - ACPI Processor ID: %d\r\n",
					       x2apic->ProcessorLocalx2APICID, x2apic->Flags, x2apic->ACPIProcessorID);
				}
				break;
			default:
				PRINTK::PrintK(PRINTK_DEBUG "Unknown MADT Record: %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleSRAT(SRATHeader *srat) {
	SRATRecord *record;
	uptr offset = sizeof(SRATHeader);
	while(offset < srat->Length) {
		record = (SRATRecord*)((uptr)srat + offset);
		offset += record->RecordLength;
		
		switch(record->EntryType) {
			case SRAT_RECORD_TYPE_PROCESSOR_LOCAL_APIC_AFFINITY: {
				SRATRecordProcessorLocalAPICAffinity *lapicAffinity = (SRATRecordProcessorLocalAPICAffinity*)record;
				u32 domain = lapicAffinity->DomainLow | (lapicAffinity->DomainHigh << 8);
				PRINTK::PrintK(PRINTK_DEBUG "Local APIC Affinity:\r\n"
					       " - Domain: %d\r\n"
					       " - APIC ID: %d\r\n"
					       " - Flags: %d\r\n"
					       " - SAPIC EID: %d\r\n"
					       " - Clock Domain: %d\r\n",
					       domain, lapicAffinity->APIC_ID, lapicAffinity->Flags, lapicAffinity->SAPIC_EID, lapicAffinity->ClockDomain);
				}
				break;
			case SRAT_RECORD_TYPE_MEMORY_AFFINITY: {
				SRATRecordMemoryAffinity *memAffinity = (SRATRecordMemoryAffinity*)record;
				uptr base = memAffinity->BaseAddressLow | (memAffinity->BaseAddressHigh << 32);
				usize length = memAffinity->LengthLow | (memAffinity->LengthHigh << 32);
				PRINTK::PrintK(PRINTK_DEBUG "Memory Affinity:\r\n"
					       " - Domain: %d\r\n"
					       " - Range: [0x%x - 0x%x]\r\n"
					       " - Flags: %d\r\n",
					       memAffinity->Domain, base, base + length, memAffinity->Flags);
				}
				break;
			case SRAT_RECORD_TYPE_PROCESSOR_LOCAL_x2APIC_AFFINITY: {
				PRINTK::PrintK(PRINTK_DEBUG "Local x2APIC Affinity:\r\n"
					       " - A\r\n",
					       0);
				}
				break;
			default:
				PRINTK::PrintK(PRINTK_DEBUG "Unknown SRAT Record. %d\r\n", record->EntryType);
				break;
		}
	}

	return 0;
}

int HandleHPET(HPETHeader *hpet) {
	PRINTK::PrintK(PRINTK_DEBUG "HPET:\r\n"
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
	PRINTK::PrintK(PRINTK_DEBUG "CPU is running at %d.%dMHz\r\n", tscPerSecond / 1000000, tscPerSecond % 1000000 / 1000);
	
	
	KInfo *info = GetInfo();
	PerCoreCPUTopology *coreInfo = (PerCoreCPUTopology*)info->BootCore->ArchitectureSpecificInformation; 
	DEV::InitializeDevice((DEV::Device*)coreInfo->TimeStampCounter, tscPerSecond);

/*
	SetAPICTimer(tscPerSecond / 2000);
	EnableAPIC();
*/

	return 0;
}
}
