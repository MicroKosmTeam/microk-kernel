#pragma once
#include <cstdint.hpp>

struct RSDP2 {
	u8 Signature[8];
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
	u8 Signature[4];
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


#define MADT_RECORD_TYPE_PROCESSOR_LOCAL_APIC 0x00
#define MADT_RECORD_TYPE_IOAPIC 0x01
#define MADT_RECORD_TYPE_IOAPIC_INTERRUPT_SOURCE_OVERRIDE 0x02
#define MADT_RECORD_TYPE_IOAPIC_NON_MASKABLE_INTERRUPT_SOURCE 0x03
#define MADT_RECORD_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPTS 0x04
#define MADT_RECORD_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE 0x05
#define MADT_RECORD_TYPE_PROCESSOR_LOOCAL_x2APIC 0x09

struct MADTHeader : public SDTHeader {
	u32 LocalAPICAddress;
	u32 Flags;
}__attribute__((packed));

struct MADTRecord {
	u8 EntryType;
	u8 RecordLength;
}__attribute__((packed));

struct MADTRecordProcessorLocalAPIC : public MADTRecord {
	u8 ACPIProcessorID;
	u8 APICID;
	u32 Flags;
}__attribute__((packed));

struct MADTRecordIOAPIC : public MADTRecord {
	u8  IOAPICID;
	u8 Reserved0;
	u32 IOAPICAddress;
	u32 GlobalSystemInterruptBase;
}__attribute__((packed));

struct MADTRecordIOAPICInterruptSourceOverride : public MADTRecord {
	u8 BusSource;
	u8 IRQSource;
	u32 GlobalSystemInterrupt;
	u16 Flags;
}__attribute__((packed));

struct MADTRecordIOAPICNonMaskableInterruptSource : public MADTRecord {
	u8 NMISource;
	u8 Reserved0;
	u16 Flags;
	u32 GlobalSystemInterrupt;
}__attribute__((packed));

struct MADTRecordLocalAPICNonMaskableInterrupts: public MADTRecord {
	u8 ACPIProcessorID;
	u16 Flags;
	u8 LINT;
}__attribute__((packed));

struct MADTRecordLocalAPICAddressOverride: public MADTRecord {
	u16 Reserved0;
	u64 LocalAPICAddress;
}__attribute__((packed));

struct MADTRecordProcessorLocalx2APIC: public MADTRecord {
	u16 Reserved0;
	u32 ProcessorLocalx2APICID;
	u32 Flags;
	u32 ACPIProcessorID;
}__attribute__((packed));

#define SRAT_RECORD_TYPE_PROCESSOR_LOCAL_APIC_AFFINITY 0x00
#define SRAT_RECORD_TYPE_MEMORY_AFFINITY 0x01
#define SRAT_RECORD_TYPE_PROCESSOR_LOCAL_x2APIC_AFFINITY 0x02

struct SRATHeader : public SDTHeader {
	u8 Reserved0[12];
}__attribute__((packed));

struct SRATRecord {
	u8 EntryType;
	u8 RecordLength;
}__attribute__((packed));

struct SRATRecordProcessorLocalAPICAffinity : public SRATRecord {
	u8 DomainLow;
	u8 APIC_ID;
	u8 Flags;
	u8 SAPIC_EID;
	u32 DomainHigh : 24;
	u32 ClockDomain;
}__attribute__((packed));

struct SRATRecordMemoryAffinity : public SRATRecord {
	u32 Domain;
	u8 Reseved0[2];
	u32 BaseAddressLow;
	u32 BaseAddressHigh;
	u32 LengthLow;
	u32 LengthHigh;
	u8 Reserved1[4];
	u32 Flags;
	u8 Reserved2[8];
}__attribute__((packed));

struct SRATRecordProcessorLocalx2APICAffinity: public SRATRecord {
	u8 Reserved0[2];
	u32 Domain;
	u32 x2APIC_ID;
	u32 Flags;
	u32 ClockDomain;
	u8 Reserved1[4];
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
	int InitDevices();
	int HandleMADT(MADTHeader *madt);
	int HandleSRAT(SRATHeader *srat);
	int HandleHPET(HPETHeader *hpet);
}
