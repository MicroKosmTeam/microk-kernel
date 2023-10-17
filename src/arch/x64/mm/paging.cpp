#include <arch/x64/mm/paging.hpp>

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled) {
        u64 bit_selector = (u64)1 << flag;
        value &= ~bit_selector;
        if (enabled) {
                value |= bit_selector;
        }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag) {
        u64 bit_selector = (u64)1 << flag;
        return value & (bit_selector > 0 ? true : false);
}

void PageDirectoryEntry::SetAddress(u64 address) {
        address &= 0x000000ffffffffff;
        value &= 0xfff0000000000fff;
        value |= (address << 12);
}

u64 PageDirectoryEntry::GetAddress() {
        return (value & 0x000ffffffffff000) >> 12;
}
