#include <arch/x86/vm/svm.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/gdt.hpp>
#include <vmm.hpp>
#include <pmm.hpp>
#include <printk.hpp>

namespace x86 {
namespace SVM {

int InitializeVMCB(VMData *vcpu, uptr rip, uptr rsp, uptr rflags, uptr cr3) {
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

	guestVmcb->Control.NestedCtl |= 0;// |= NESTED_CTL_NP_ENABLE;
	guestVmcb->Control.NestedCR3 = cr3;

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

void LaunchVM(uptr vmcbPhysAddr) {
	while (true) {
	        asm volatile("sub %%rsp, 8\n\t" ::: "memory"); // Align stack to 16-byte boundary


		__asm__ __volatile__("vmload %[vmcbPhysAddr]\n"
				     "vmrun\n"
				     "vmsave\n"
				     : : [vmcbPhysAddr] "a"(vmcbPhysAddr) : "memory");
		{
			uptr addr;
			asm volatile ("mov %0, %%rax" : "=r"(addr) : : "memory");
			vmcbPhysAddr = addr;
			VMCB *vmcb = (VMCB*)VMM::PhysicalToVirtual(addr);

			PRINTK::PrintK(PRINTK_DEBUG "Hello, VMEXIT: 0x%x\r\n", vmcb->Control.ExitCode);

			switch(vmcb->Control.ExitCode) {
				case _VMMCALL:
					PRINTK::PrintK(PRINTK_DEBUG "VMMCALL\r\n");
					vmcb->Save.RIP += 2;
					break;
			}

			vmcb->Control.ExitCode = 0;
		}
	}
}

}
}
