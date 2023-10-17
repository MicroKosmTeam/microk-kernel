#include <arch/x64/mm/pageindexer.hpp>

PageMapIndexer::PageMapIndexer(u64 virtual_address) {
        virtual_address >>= 12;
        P_i = virtual_address & 0x1ff;
        virtual_address >>=9;
        PT_i = virtual_address & 0x1ff;
        virtual_address >>=9;
        PD_i = virtual_address & 0x1ff;
        virtual_address >>=9;
        PDP_i = virtual_address & 0x1ff;
}
