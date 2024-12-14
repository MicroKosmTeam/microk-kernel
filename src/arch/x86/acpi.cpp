#include <arch/x86/acpi.hpp>
#include <printk.hpp>
#include <arch/x86/cpu.hpp>
#include <kinfo.hpp>
#include <memory.hpp>
#include <capability.hpp>
#include <pmm.hpp>

namespace x86 {
int InitializeACPI(ACPI *acpi) {
	KInfo *info = GetInfo();

	acpi->RSDP = (RSDP_t*)info->RSDP;
	VMM::MMap(info->KernelVirtualSpace, VMM::VirtualToPhysical(info->RSDP), info->RSDP, PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);

	uptr rsdpPageAligned = VMM::VirtualToPhysical(info->RSDP);
	ROUND_DOWN_TO_PAGE(rsdpPageAligned);

	PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
	CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, rsdpPageAligned, PAGE_SIZE, ACCESS | READ);

	PRINTK::PrintK(PRINTK_DEBUG "RSDP at 0x%x\r\n", acpi->RSDP);

	if(Memcmp(acpi->RSDP->Signature, "RSD PTR ", 8) != 0) {
		PRINTK::PrintK(PRINTK_DEBUG "Invalid RSDP\r\n");
		return -1;
	}

	PRINTK::PrintK(PRINTK_DEBUG "Valid RSDP:\r\n"
			            "RSDT: 0x%x\r\n"
				    "XSDT: 0x%x\r\n",
				    acpi->RSDP->RsdtAddress,
				    acpi->RSDP->XsdtAddress);

	if(acpi->RSDP->XsdtAddress) {
		uptr addr = acpi->RSDP->XsdtAddress;
		ROUND_DOWN_TO_PAGE(addr);

		PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
		CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, addr, PAGE_SIZE, ACCESS | READ);

		VMM::MMap(info->KernelVirtualSpace, addr, VMM::PhysicalToVirtual(addr), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);
		acpi->MainSDT = (SDTHeader_t*)VMM::PhysicalToVirtual(acpi->RSDP->XsdtAddress);
		acpi->MainSDTPointerSize = 8;
	} else if (acpi->RSDP->RsdtAddress) {
		uptr addr = acpi->RSDP->RsdtAddress;
		ROUND_DOWN_TO_PAGE(addr);

		PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
		CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, addr, PAGE_SIZE, ACCESS | READ);

		VMM::MMap(info->KernelVirtualSpace, addr, VMM::PhysicalToVirtual(addr), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);
		acpi->MainSDT = (SDTHeader_t*)VMM::PhysicalToVirtual(acpi->RSDP->RsdtAddress);
		acpi->MainSDTPointerSize = 4;
	} else {
		return -1;
	}

	int entries = (acpi->MainSDT->Length - sizeof(SDTHeader_t)) / acpi->MainSDTPointerSize;
	PRINTK::PrintK(PRINTK_DEBUG "Entries: %d\r\n", entries);

	for (int i = 0; i < entries; i++) {
		char sig[4 + 1] = { '\0' };
		if (acpi->MainSDTPointerSize == 8) {
			uptr *ptr = (uptr*)((uptr)acpi->MainSDT + sizeof(SDTHeader_t) + i * 8);

			PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
			CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, *ptr, PAGE_SIZE, ACCESS | READ);

			VMM::MMap(info->KernelVirtualSpace, *ptr, VMM::PhysicalToVirtual(*ptr), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);

			SDTHeader_t *sdt = (SDTHeader_t*)VMM::PhysicalToVirtual(*ptr);

			Memcpy(sig, sdt->Signature, 4);
			PRINTK::PrintK(PRINTK_DEBUG "%d: 0x%x -> %s\r\n", i, *ptr, sig);

			if (Memcmp(sdt->Signature, "FACP", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing FADT\r\n");
				InitializeFADT(acpi, (FADT_t*)sdt);
			} else if (Memcmp(sdt->Signature, "APIC", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing MADT\r\n");
				InitializeMADT(acpi, (MADT_t*)sdt);
			} else if (Memcmp(sdt->Signature, "SRAT", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing SRAT\r\n");
				InitializeSRAT(acpi, (SRAT_t*)sdt);
			} else if (Memcmp(sdt->Signature, "MCFG", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing MCFG\r\n");
				InitializeMCFG(acpi, (MCFG_t*)sdt);
			} else if (Memcmp(sdt->Signature, "HPET", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing HPET\r\n");
				InitializeHPET(acpi, (HPET_t*)sdt);
			}
		} else {
			u32 *ptr = (u32*)((uptr)acpi->MainSDT + sizeof(SDTHeader_t) + i * 4);

			PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
			CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, *ptr, PAGE_SIZE, ACCESS | READ);
			
			VMM::MMap(info->KernelVirtualSpace, *ptr, VMM::PhysicalToVirtual(*ptr), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);

			SDTHeader_t *sdt = (SDTHeader_t*)VMM::PhysicalToVirtual(*ptr);

			Memcpy(sig, sdt->Signature, 4);
			
			PRINTK::PrintK(PRINTK_DEBUG "%d: 0x%x -> %s\r\n", i, *ptr, sig);

			if (Memcmp(sdt->Signature, "APIC", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing MADT\r\n");
				InitializeMADT(acpi, (MADT_t*)sdt);
			} else if (Memcmp(sdt->Signature, "SRAT", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing SRAT\r\n");
				InitializeSRAT(acpi, (SRAT_t*)sdt);
			} else if (Memcmp(sdt->Signature, "MCFG", 4) == 0) {
				PRINTK::PrintK(PRINTK_DEBUG "Processing MCFG\r\n");
				InitializeMCFG(acpi, (MCFG_t*)sdt);
			}
		}
	}

	return 0;
}
	
int InitializeFADT(ACPI *acpi, FADT_t *fadt) {
	KInfo *info = GetInfo();

	SDTHeader_t *dsdt;
	if (fadt->X_Dsdt) {
		VMM::MMap(info->KernelVirtualSpace, fadt->X_Dsdt, VMM::PhysicalToVirtual(fadt->X_Dsdt), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);
		dsdt = (SDTHeader_t*)VMM::PhysicalToVirtual(fadt->X_Dsdt);
	} else if (fadt->Dsdt) {
		VMM::MMap(info->KernelVirtualSpace, fadt->X_Dsdt, VMM::PhysicalToVirtual(fadt->X_Dsdt), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_NOEXEC);
		dsdt = (SDTHeader_t*)VMM::PhysicalToVirtual(fadt->Dsdt);
	}

	usize size = dsdt->Length;
	ROUND_UP_TO_PAGE(size);

	PRINTK::PrintK(PRINTK_DEBUG "DSDT at 0x%x of size %d\r\n", dsdt, size);

	PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
	CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, VMM::VirtualToPhysical((uptr)dsdt), size, ACCESS | READ);


	return 0;
}

int InitializeMADT(ACPI *acpi, MADT_t *madt) {
	uptr currentPtr = (uptr)&madt->FirstEntry;
	uptr entriesEnd = (uptr)madt + madt->Length;

	while(currentPtr < entriesEnd) {
		MADTEntry_t *current = (MADTEntry_t*)currentPtr;

		switch(current->EntryType) {
			case MADT_ENTRY_LAPIC: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT LAPIC entry\r\n");
			}
			break;
			case MADT_ENTRY_IOAPIC: {
				MADTIOAPICEntry_t *entry = (MADTIOAPICEntry_t*)current;
				PRINTK::PrintK(PRINTK_DEBUG "MADT IOAPIC entry:\r\n"
						" IOAPIC ID: 0x%x\r\n"
						" IOAPIC Address: 0x%x\r\n"
						" Global System Interrupt Base: 0x%x\r\n",
						entry->IOAPICID,
						entry->IOAPICAddress,
						entry->GlobalSystemInterruptBase);

				if (acpi->IOAPICCount < MAX_IOAPIC) {
					InitializeIOAPIC(&acpi->IOApics[acpi->IOAPICCount++], entry->IOAPICID, entry->IOAPICAddress);
				}
			}
			break;
			case MADT_ENTRY_APIC_SOURCE_OVERRIDE: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT APIC source override entry\r\n");
			}
			break;
			case MADT_ENTRY_IOAPIC_NONMASK_SOURCE: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT IOAPIC nonmaskable source entry\r\n");
			}
			break;
			case MADT_ENTRY_LAPIC_NONMASK_INTERRUPT: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT LAPIC nonmaskable interrupt entry\r\n");
			}
			break;
			case MADT_ENTRY_LAPIC_ADDRESS_OVERRIDE: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT LAPIC address override entry\r\n");
			}
			break;
			case MADT_ENTRY_LX2APIC: {
				PRINTK::PrintK(PRINTK_DEBUG "MADT X2APIC entry\r\n");
			}
			break;
		}

		currentPtr += current->RecordLength;
	}

	return 0;
}

int InitializeSRAT(ACPI *acpi, SRAT_t *srat) {
	uptr currentPtr = (uptr)&srat->FirstEntry;
	uptr entriesEnd = (uptr)srat + srat->Length;

	while(currentPtr < entriesEnd) {
		SRATEntry_t *current = (SRATEntry_t*)currentPtr;
		
		switch(current->EntryType) {
			case SRAT_ENTRY_LAPIC: {
				SRATLapicEntry_t *entry = (SRATLapicEntry_t*)current;
				PRINTK::PrintK(PRINTK_DEBUG "SRAT Lapic Entry:\r\n"
			            " Domain: 0x%x\r\n"
				    " APIC ID: 0x%x\r\n",
				    ((usize)entry->DomainHigh[2] << 24 | entry->DomainHigh[1] << 16 | entry->DomainHigh[0] << 8 | entry->DomainLow),
				    entry->APICID);

			}
			break;
			case SRAT_ENTRY_MEMORY: {
				SRATMemoryEntry_t *entry = (SRATMemoryEntry_t*)current;
				PRINTK::PrintK(PRINTK_DEBUG "SRAT Memory Entry:\r\n"
			            " Domain: 0x%x\r\n"
				    " Base: 0x%x\r\n"
				    " Length: 0x%x\r\n",
				    entry->Domain,
				    ((uptr)entry->BaseHigh << 32 | entry->BaseLow),
				    ((uptr)entry->LengthHigh << 32 | entry->LengthLow));

			}
			break;
			case SRAT_ENTRY_LAPIC2: {
				SRATLapic2Entry_t *entry = (SRATLapic2Entry_t*)current;
				PRINTK::PrintK(PRINTK_DEBUG "SRAT Lapic2 Entry:\r\n"
			            " Domain: 0x%x\r\n"
				    " X2APIC ID: 0x%x\r\n",
				    entry->Domain,
				    entry->X2APICID);

			}
			break;
		}

		currentPtr += current->RecordLength;
	}

	return 0;
}

inline void CheckBar(volatile u32 *bar, volatile u32 *nextBar) {
	KInfo *info = GetInfo();

	if (*bar & 0b1) {
		// IO SPACE BAR
		// TODO: manage io ports
		uptr addr = *bar & 0xFFF0;
		if (addr != 0) {
			PRINTK::PrintK(PRINTK_DEBUG "    16 bit BAR at 0x%x\r\n", addr);
		}
	} else {
		u8 type = ((*bar & 0b110) >> 1);
		if (type == 0) {
			// 32 bit bar
			uptr addr = *bar & 0xfffffff0;
			usize size = PAGE_SIZE;

			volatile u32 originalBar = *bar;
			*bar = 0xffffffff;
			size = ~(*bar) + 1;
			ROUND_UP_TO_PAGE(size);
			*bar = originalBar;

			if (addr != 0) {
				PRINTK::PrintK(PRINTK_DEBUG "    32 bit BAR at 0x%x of size 0x%x\r\n", addr, size);
				PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
				CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, addr, size, ACCESS | READ | WRITE);
			}
		} else if (type == 2) {
			//64 bit bar
			uptr addr = ((*bar & 0xFFFFFFF0) + (((uptr)*nextBar & 0xFFFFFFFF) << 32));
			usize size = PAGE_SIZE;
			
			volatile u32 originalBar = *bar;
			*bar = 0xFFFFFFFF;
			size = ~(*bar) + 1;
			ROUND_UP_TO_PAGE(size);
			*bar = originalBar;
			

			if (addr != 0) {
				PRINTK::PrintK(PRINTK_DEBUG "    64 bit BAR at 0x%x of size 0x%x\r\n", addr, size);
				PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
				CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, addr, size, ACCESS | READ | WRITE);
			}
		}
	}
}

int InitializeMCFG(ACPI *acpi, MCFG_t *mcfg) {
	KInfo *info = GetInfo();

	uptr currentPtr = (uptr)&mcfg->FirstEntry;
	uptr entriesEnd = (uptr)mcfg + mcfg->Length;

	while(currentPtr < entriesEnd) {
		MCFGEntry_t *current = (MCFGEntry_t*)currentPtr;
		PRINTK::PrintK(PRINTK_DEBUG "Device addr: 0x%x\r\n"
				            "Seg:         0x%x\r\n"
					    "Start bus:   0x%x\r\n"
					    "End bus:     0x%x\r\n", 
					    current->BaseAddress, current->PCISeg,
					    current->StartPCIBus, current->EndPCIBus);



		for (usize bus = current->StartPCIBus; bus < current->EndPCIBus; ++bus) {
			u64 offset = bus << 20;
			uptr busAddress = current->BaseAddress + offset;

			PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
			CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, busAddress, ACCESS | READ | WRITE);
			VMM::MMap(info->KernelVirtualSpace, busAddress, VMM::PhysicalToVirtual(busAddress), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);

			PCIDeviceHeader_t *header = (PCIDeviceHeader_t*)VMM::PhysicalToVirtual(busAddress);
			if(header->DeviceID == 0) continue;
			if(header->DeviceID == 0xFFFF) continue;
			
			PRINTK::PrintK(PRINTK_DEBUG"Bus addr: 0x%x\r\n", busAddress);

			for (usize device = 0; device < 32; ++device) {
				u64 offset = device << 15;
				uptr deviceAddress = busAddress + offset;

				if (deviceAddress != busAddress) {
					PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
					CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, deviceAddress, ACCESS | READ | WRITE);
					VMM::MMap(info->KernelVirtualSpace, deviceAddress, VMM::PhysicalToVirtual(deviceAddress), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
				}

				PCIDeviceHeader_t *header = (PCIDeviceHeader_t*)VMM::PhysicalToVirtual(deviceAddress);
				if(header->DeviceID == 0) continue;
				if(header->DeviceID == 0xFFFF) continue;
				
				PRINTK::PrintK(PRINTK_DEBUG" Device addr: 0x%x\r\n", deviceAddress);

				for (usize function = 0; function < 8; ++function) {
					u64 offset = function << 12;
					uptr functionAddress = deviceAddress + offset;

					if (functionAddress != deviceAddress) {
						VMM::MMap(info->KernelVirtualSpace, functionAddress, VMM::PhysicalToVirtual(functionAddress), PAGE_SIZE, VMM_FLAGS_READ | VMM_FLAGS_WRITE | VMM_FLAGS_NOEXEC);
						PMM::CheckSpace(info->RootCSpace, DEFAULT_CHECK_SPACE);
						CAPABILITY::GenerateCapability(info->RootCSpace, MMIO_MEMORY, functionAddress, ACCESS | READ | WRITE);
					}

					PCIDeviceHeader_t *header = (PCIDeviceHeader_t*)VMM::PhysicalToVirtual(functionAddress);
					if(header->DeviceID == 0) continue;
					if(header->DeviceID == 0xFFFF) continue;
					
					PRINTK::PrintK(PRINTK_DEBUG"  Function addr: 0x%x\r\n", functionAddress);
					PRINTK::PrintK(PRINTK_DEBUG"   ID: %x:%x\r\n", header->DeviceID, header->VendorID);

					if (header->HeaderType == 0) {
						PCIHeader0_t *header0 = (PCIHeader0_t*)header;
						PRINTK::PrintK(PRINTK_DEBUG "   Type 0\r\n"
								            "    BAR0: 0x%x\r\n"
								            "    BAR1: 0x%x\r\n"
								            "    BAR2: 0x%x\r\n"
								            "    BAR3: 0x%x\r\n"
								            "    BAR4: 0x%x\r\n"
								            "    BAR5: 0x%x\r\n",
									    header0->BAR0,
									    header0->BAR1,
									    header0->BAR2,
									    header0->BAR3,
									    header0->BAR4,
									    header0->BAR5);
						CheckBar(&header0->BAR0, &header0->BAR1);
						CheckBar(&header0->BAR1, &header0->BAR2);
						CheckBar(&header0->BAR2, &header0->BAR3);
						CheckBar(&header0->BAR3, &header0->BAR4);
						CheckBar(&header0->BAR4, &header0->BAR5);
						CheckBar(&header0->BAR5, 0);

					} else if (header->HeaderType == 1) {
						PCIHeader1_t *header1 = (PCIHeader1_t*)header;
						PRINTK::PrintK(PRINTK_DEBUG "   Type 1\r\n"
								            "    BAR0: 0x%x\r\n"
								            "    BAR1: 0x%x\r\n",
									    header1->BAR0,
									    header1->BAR1);
						CheckBar(&header1->BAR0, &header1->BAR1);
						CheckBar(&header1->BAR1, 0);

					} else if (header->HeaderType == 2) {
						PCIHeader2_t *header2 = (PCIHeader2_t*)header;
						PRINTK::PrintK(PRINTK_DEBUG "   Type 2\r\n"
								            "    Base: 0x%x\r\n",
									    header2->CardBusBaseAddress);
					} else {
						PRINTK::PrintK(PRINTK_DEBUG "   Unknown header type: %d\r\n",
								header->HeaderType);
					}
				}
			}
		}

		currentPtr += sizeof(MCFGEntry_t);
	}



	return 0;
}

int InitializeHPET(ACPI *acpi, HPET_t *hpet) {
	return 0;
}

}
