#pragma once
#include <cstdint.hpp>

class PageMapIndexer {
public:
        PageMapIndexer(u64 virtual_address);
        u64 PDP_i;
        u64 PD_i;
        u64 PT_i;
        u64 P_i;
};
