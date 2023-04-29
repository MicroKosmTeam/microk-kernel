#include <proc/smp.hpp>

#include <arch/x64/cpu/mp.hpp>

#ifdef CONFIG_MP_SMP

namespace PROC {
namespace SMP {
void Init() {
	x86_64::DetectCores();
}
}
}

#endif
