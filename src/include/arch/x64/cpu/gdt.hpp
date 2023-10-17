#pragma once

#include <cstdint.hpp>

struct GDTPointer{
    u16 size;
    u64 offset;
} __attribute__((packed));

struct GDTEntry{
    u16 limit;
    u16 base_low16;
    u8 base_mid8;
    u8 access;
    u8 granularity;
    u8 base_high8;
} __attribute__((packed));

struct TSS {
	u32 reserved0;
	u64 rsp0;
	u64 rsp1;
	u64 rsp2;
	u64 reserved1;
	u64 ist1;
	u64 ist2;
	u64 ist3;
	u64 ist4;
	u64 ist5;
	u64 ist6;
	u64 ist7;
	u64 reserved2;
	u16 reserved3;
	u16 iopb_offset;
} __attribute__((packed));

struct GDT {
	GDTEntry null;
	GDTEntry _16bit_code;
	GDTEntry _16bit_data;
	GDTEntry _32bit_code;
	GDTEntry _32bit_data;
	GDTEntry _64bit_code;
	GDTEntry _64bit_data;
	GDTEntry user_code;
	GDTEntry user_data;
	GDTEntry tss_low;
	GDTEntry tss_high;
} __attribute__((packed));

extern "C" void FlushGDT(GDTPointer *pointer);
extern "C" void FlushTSS();

namespace x86_64 {
	void LoadGDT();
	void TSSInit(uptr stackPointer);
}
