#include <arch/x86/vm/vm.hpp>

/*
void StartVM() {
	VMData *vmdata = (VMData*)PMM::RequestPages(sizeof(VMData) / PAGE_SIZE);
	Memclr(vmdata, sizeof(VMData));
	vmdata->Self = vmdata;

	SVM::InitializeVMCB(vmdata, rip, rsp, rflags);
	SVM::LoadVM(VMM::VirtualToPhysical((uptr)vmdata->GuestVMCB));
	SVM::LaunchVM(VMM::VirtualToPhysical((uptr)vmdata->GuestVMCB));
}*/
