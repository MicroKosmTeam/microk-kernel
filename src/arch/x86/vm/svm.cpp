#include <arch/x86/vm/svm.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/gdt.hpp>
#include <vmm.hpp>
#include <pmm.hpp>
#include <printk.hpp>

namespace x86 {
namespace SVM {

int InitializeVMCB(VCpu *vcpu, uptr rip, uptr rsp, uptr rflags) {
	VMCB *guestVmcb = (VMCB*)vcpu->GuestVMCB;
	VMCB *hostSave = (VMCB*)vcpu->HostSave;
	VMCB *hostVmcb = (VMCB*)vcpu->HostVMCB;
	VMCB *sharedVmcb = (VMCB*)vcpu->SharedPage;

	// CONTROL
	guestVmcb->Control.Intercepts[INTERCEPT_WORD3] |= INTERCEPT_MSR_PROT |
					             INTERCEPT_CPUID;

	guestVmcb->Control.Intercepts[INTERCEPT_WORD4] |= INTERCEPT_VMRUN |
						     INTERCEPT_VMMCALL;
	guestVmcb->Control.asid = 1;

	guestVmcb->Control.MSRPMBasePa = VMM::VirtualToPhysical((uptr)sharedVmcb);

	// SAVE
	guestVmcb->Save.CR0 = GetCR0();
	guestVmcb->Save.CR2 = GetCR2();
	guestVmcb->Save.CR3 = GetCR3();
	guestVmcb->Save.CR4 = GetCR4();

	u32 msrLo = 0, msrHi = 0;
	GetMSR(MSR_EFER, &msrLo, &msrHi);
	guestVmcb->Save.EFER = ((u64)msrHi << 32) | msrLo;

	guestVmcb->Save.RSP = rsp;
	guestVmcb->Save.RIP = rip;
	guestVmcb->Save.RFLAGS = rflags; 
 	GetMSR(MSR_PAT, &msrLo, &msrHi);
	guestVmcb->Save.GPAT = ((u64)msrHi << 32) | msrLo;

	_DTR gdtr;
	GetGDTR(&gdtr);
	guestVmcb->Save.GDTR.Base = gdtr.Base;
	guestVmcb->Save.GDTR.Limit = gdtr.Limit;

	_DTR idtr;
	GetIDTR(&idtr);
	guestVmcb->Save.IDTR.Base = idtr.Base;
	guestVmcb->Save.IDTR.Limit = idtr.Limit;

	guestVmcb->Save.ES.Selector = GetES();
	guestVmcb->Save.CS.Selector = GetCS();
	guestVmcb->Save.SS.Selector = GetSS();
	guestVmcb->Save.DS.Selector = GetDS();
	guestVmcb->Save.FS.Selector = GetFS();
	guestVmcb->Save.GS.Selector = GetGS();

	guestVmcb->Save.ES.Limit = GetLimit((GDT*)gdtr.Base, GetES());
	guestVmcb->Save.CS.Limit = GetLimit((GDT*)gdtr.Base, GetCS());
	guestVmcb->Save.SS.Limit = GetLimit((GDT*)gdtr.Base, GetSS());
	guestVmcb->Save.DS.Limit = GetLimit((GDT*)gdtr.Base, GetDS());
	guestVmcb->Save.FS.Limit = GetLimit((GDT*)gdtr.Base, GetFS());
	guestVmcb->Save.GS.Limit = GetLimit((GDT*)gdtr.Base, GetGS());

	guestVmcb->Save.ES.Base = GetBase((GDT*)gdtr.Base, GetES());
	guestVmcb->Save.CS.Base = GetBase((GDT*)gdtr.Base, GetCS());
	guestVmcb->Save.SS.Base = GetBase((GDT*)gdtr.Base, GetSS());
	guestVmcb->Save.DS.Base = GetBase((GDT*)gdtr.Base, GetDS());
	guestVmcb->Save.FS.Base = GetBase((GDT*)gdtr.Base, GetFS());
	guestVmcb->Save.GS.Base = GetBase((GDT*)gdtr.Base, GetGS());

	guestVmcb->Save.ES.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetES()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetES());
	guestVmcb->Save.CS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetCS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetCS());
	guestVmcb->Save.SS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetSS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetSS());
	guestVmcb->Save.DS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetDS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetDS());
	guestVmcb->Save.FS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetFS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetFS());
	guestVmcb->Save.GS.Attrib = ((u16)GetAccess((GDT*)gdtr.Base, GetGS()) << 8 ) | GetGranularity((GDT*)gdtr.Base, GetGS());

	SaveVM(VMM::VirtualToPhysical((uptr)guestVmcb));

	msrLo = VMM::VirtualToPhysical((uptr)hostSave) & 0xFFFFFFFF;
	msrHi = (VMM::VirtualToPhysical((uptr)hostSave) >> 32) & 0xFFFFFFFF;
	SetMSR(MSR_VM_HSAVE_PA, msrLo, msrHi);

	SaveVM(VMM::VirtualToPhysical((uptr)hostVmcb));
	
	return 0;
}

void LoadVM(uptr statePhysAddr) {
	__asm__ __volatile__("vmload %[statePhysAddr]" : : [statePhysAddr] "a"(statePhysAddr) : "memory");
}

void SaveVM(uptr statePhysAddr) {
	__asm__ __volatile__("vmsave %[statePhysAddr]" : : [statePhysAddr] "a"(statePhysAddr) : "memory");
}

char stack[256];
void *stackTop = (void*)((uptr)stack + 256);
void LaunchVM(uptr vmcbPhysAddr) {
	while (true) {
		__asm__ __volatile__("vmrun %[vmcbPhysAddr]" : : [vmcbPhysAddr] "a"(vmcbPhysAddr) : "memory");
		SaveVM(vmcbPhysAddr);

		PRINTK::PrintK(PRINTK_DEBUG "Hello, world\r\n");

		while(true);

		VMCB *guestVMCB = (VMCB*)VMM::PhysicalToVirtual(vmcbPhysAddr);
		switch(guestVMCB->Control.ExitCode) {
			case _VMRUN:
		PRINTK::PrintK(PRINTK_DEBUG "Run\r\n");
				break;
			case _VMMCALL:
		PRINTK::PrintK(PRINTK_DEBUG "Call\r\n");
				break;
		}

		LoadVM(vmcbPhysAddr);
	}
}

}
}