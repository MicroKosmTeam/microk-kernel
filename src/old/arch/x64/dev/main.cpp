#include <arch/x64/dev/main.hpp>
#include <init/kinfo.hpp>
#include <sys/printk.hpp>
#include <sys/timer.hpp>

#include <arch/x64/cpu/cpu.hpp>
#include <arch/x64/dev/apic.hpp>
#include <arch/x64/dev/hpet.hpp>
#include <arch/x64/dev/pit.hpp>
#include <arch/x64/dev/ioapic.hpp>

static void AddIOAPICToMachine(DEV::CPU::TopologyStructure *machine, x86_64::IOAPIC::IOAPIC *ioapic) {
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
static u8 ValidateACPITable(u8 *ptr, usize size) {
	u8 checksum = 0;

	for (usize i = 0; i < size; ++i) checksum += ptr[i];

	return checksum;
}

static void PrintDebugACPITableInfo(uptr addr) {
	SDTHeader *sdtHeader = (SDTHeader*)addr;

	char sig[5];
	sig[4] = '\0';

	Memcpy(sig, sdtHeader->Signature, 4);

	PRINTK::PrintK(PRINTK_DEBUG "ACPI: %s 0x%x\r\n", sig, addr);
}

int InitDevices() {
	KInfo *info = GetInfo();

	RSDP2 *rsdp = (RSDP2*)info->RSDP;

	DEV::CPU::TopologyStructure *machine = info->DefaultMachine;
	PerMachineTopology *machineInfo = (PerMachineTopology*)machine->ArchitectureSpecificInformation;

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
		uptr addr = VMM::PhysicalToVirtual(*(uptr*)((uptr)sdtAddr + sizeof(SDTHeader) + (i * size)));

		SDTHeader *sdtHeader = (SDTHeader*)addr;
		if (ValidateACPITable((u8*)sdtHeader, sdtHeader->Length)) {
			continue;
		}

		PrintDebugACPITableInfo(addr);

		if (Memcmp(sdtHeader->Signature, "APIC", 4) == 0) {
			machineInfo->ACPI_TableMADT = addr;
		} else if (Memcmp(sdtHeader->Signature, "SRAT", 4) == 0) {
			machineInfo->ACPI_TableSRAT = addr;
		} else if (Memcmp(sdtHeader->Signature, "HPET", 4) == 0) {
			machineInfo->ACPI_TableHPET = addr;
		}
	}

	return 0;
}

int HandleMADT(MADTHeader *madt) {
	KInfo *info = GetInfo();
/*
	PerCoreCPUTopology *bootCoreInfo = (PerCoreCPUTopology*)info->BootCore->ArchitectureSpecificInformation;
	PerCoreCPUTopology *coreInfo = bootCoreInfo;
*/
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
/*
				if (plapic->ACPIProcessorID != info->BootCore->ID) {
					coreInfo = new PerCoreCPUTopology;
					core->ArchitectureSpecificInformation = (void*)coreInfo;
				}

				APIC::APIC *localAPIC = (APIC::APIC*)APIC::CreateAPICDevice();
				coreInfo->LocalAPIC = localAPIC;
*/

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
	KInfo *info = GetInfo();

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


				DEV::CPU::TopologyStructure *node = DEV::CPU::FindTopologyStructure(info->DefaultMachine, domain);
				
				if (node == NULL) {
					PRINTK::PrintK(PRINTK_DEBUG "Create node #%d.\r\n", domain);
					node = DEV::CPU::CreateTopologyStructure(info->DefaultMachine, domain);
				}

				if (lapicAffinity->APIC_ID != info->BootCore->ID) {
					DEV::CPU::TopologyStructure *core = DEV::CPU::CreateTopologyStructure(node, lapicAffinity->APIC_ID);
					(void)core;
				} else {
					PRINTK::PrintK(PRINTK_DEBUG " - It's the current CPU.\r\n");
					info->BootCore->Parent = node;
				}
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

				DEV::CPU::TopologyStructure *node = DEV::CPU::FindTopologyStructure(info->DefaultMachine, memAffinity->Domain);
				
				if (node == NULL) {
					PRINTK::PrintK(PRINTK_DEBUG "Create node #%d.\r\n", memAffinity->Domain);
					node = DEV::CPU::CreateTopologyStructure(info->DefaultMachine, memAffinity->Domain);
				}
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