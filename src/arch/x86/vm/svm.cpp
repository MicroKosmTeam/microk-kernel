#include <arch/x86/vm/svm.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/gdt.hpp>
#include <vmm.hpp>
#include <printk.hpp>

namespace x86 {
namespace SVM {

u64 Stack[256];
extern "C" void TestFunc() {
	PRINTK::PrintK(PRINTK_DEBUG "Hello from VM!\r\n");
	while(true) { }
}

int InitializeVMCB(VMCB *vmcb) {
	// CONTROL
	vmcb->Control.Intercepts[INTERCEPT_WORD3] |= INTERCEPT_MSR_PROT |
					             INTERCEPT_CPUID;

	vmcb->Control.Intercepts[INTERCEPT_WORD4] |= INTERCEPT_VMRUN;
	vmcb->Control.asid = 1;

	vmcb->Control.MSRPMBasePa = VMM::VirtualToPhysical((uptr)vmcb);

	// SAVE
	vmcb->Save.CR0 = GetCR0();
	vmcb->Save.CR2 = GetCR2();
	vmcb->Save.CR3 = GetCR3();
	vmcb->Save.CR4 = GetCR4();

	u32 msrLo = 0, msrHi = 0;
	GetMSR(MSR_EFER, &msrLo, &msrHi);
	vmcb->Save.EFER = ((u64)msrHi << 32) | msrLo;

	vmcb->Save.RSP = (uptr)&Stack;
	vmcb->Save.RIP = (uptr)&TestFunc;
	vmcb->Save.RFLAGS = GetRFLAGS();
 	GetMSR(MSR_PAT, &msrLo, &msrHi);
	vmcb->Save.GPAT = ((u64)msrHi << 32) | msrLo;


	_DTR gdtr;
	GetGDTR(&gdtr);
	vmcb->Save.GDTR.Base = gdtr.Base;
	vmcb->Save.GDTR.Limit = gdtr.Limit;

	_DTR idtr;
	GetIDTR(&idtr);
	vmcb->Save.IDTR.Base = idtr.Base;
	vmcb->Save.IDTR.Limit = idtr.Limit;

	vmcb->Save.ES.Selector = GetES();
	vmcb->Save.CS.Selector = GetCS();
	vmcb->Save.SS.Selector = GetSS();
	vmcb->Save.DS.Selector = GetDS();
	vmcb->Save.FS.Selector = GetFS();
	vmcb->Save.GS.Selector = GetGS();

	vmcb->Save.ES.Limit = GetLimit((GDT*)gdtr.Base, GetES());
	vmcb->Save.CS.Limit = GetLimit((GDT*)gdtr.Base, GetCS());
	vmcb->Save.SS.Limit = GetLimit((GDT*)gdtr.Base, GetSS());
	vmcb->Save.DS.Limit = GetLimit((GDT*)gdtr.Base, GetDS());
	vmcb->Save.FS.Limit = GetLimit((GDT*)gdtr.Base, GetFS());
	vmcb->Save.GS.Limit = GetLimit((GDT*)gdtr.Base, GetGS());

	vmcb->Save.ES.Base = GetBase((GDT*)gdtr.Base, GetES());
	vmcb->Save.CS.Base = GetBase((GDT*)gdtr.Base, GetCS());
	vmcb->Save.SS.Base = GetBase((GDT*)gdtr.Base, GetSS());
	vmcb->Save.DS.Base = GetBase((GDT*)gdtr.Base, GetDS());
	vmcb->Save.FS.Base = GetBase((GDT*)gdtr.Base, GetFS());
	vmcb->Save.GS.Base = GetBase((GDT*)gdtr.Base, GetGS());

	vmcb->Save.ES.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetES()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetES());
	vmcb->Save.CS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetCS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetCS());
	vmcb->Save.SS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetSS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetSS());
	vmcb->Save.DS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetDS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetDS());
	vmcb->Save.FS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetFS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetFS());
	vmcb->Save.GS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetGS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetGS());

	return 0;
}

void LoadVM(uptr statePhysAddr) {
	__asm__ __volatile__("vmload %[statePhysAddr]" : : [statePhysAddr] "a"(statePhysAddr) : "memory");
}

void SaveVM(uptr statePhysAddr) {
	__asm__ __volatile__("vmsave %[statePhysAddr]" : : [statePhysAddr] "a"(statePhysAddr) : "memory");
}

void LaunchVM(uptr vmcbPhysAddr) {
	__asm__ __volatile__("vmrun %[vmcbPhysAddr]" : : [vmcbPhysAddr] "a"(vmcbPhysAddr) : "memory");
}

}
}
