#include <arch/x86/vm/vm.hpp>
#include <arch/x86/vm/svm.hpp>
#include <pmm.hpp>
#include <vmm.hpp>
#include <memory.hpp>

namespace x86 {

void StartVM(uptr rip, uptr rsp, uptr rflags, uptr cr3) {
	VMData *vmdata = (VMData*)PMM::RequestPages(sizeof(VMData) / PAGE_SIZE);
	Memclr(vmdata, sizeof(VMData));
	vmdata->Self = vmdata;

	SVM::InitializeVMCB(vmdata, rip, rsp, rflags, cr3);
	SVM::LoadVM(VMM::VirtualToPhysical((uptr)vmdata->GuestVMCB));
	SVM::LaunchVM(VMM::VirtualToPhysical((uptr)vmdata->GuestVMCB));
}
}
