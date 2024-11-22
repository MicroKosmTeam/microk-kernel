#include <arch/x86/vm/svm.hpp>
#include <arch/x86/cpu.hpp>
#include <arch/x86/object.hpp>
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

	guestVmcb->Control.NestedCtl |= 0 ; // NESTED_CTL_NP_ENABLE;
	guestVmcb->Control.NestedCR3 = cr3;
	

	// SAVE
	guestVmcb->Save.CR0 = GetCR0();
	guestVmcb->Save.CR2 = GetCR2();
	guestVmcb->Save.CR3 = GetCR3();
	guestVmcb->Save.CR4 = GetCR4();
	
	//guestVmcb->Save.CPL = 3;

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
	GeneralRegisters *context = NULL;
	while (true) {
		__asm__ __volatile__("vmload %[vmcbPhysAddr]\n"
				     "vmrun\n"
				     "vmsave\n"
				     "push %%r15\n\t"
			             "push %%r14\n\t"
			             "push %%r13\n\t"
  			             "push %%r12\n\t"
			             "push %%r11\n\t"
			             "push %%r10\n\t"
 			             "push %%r9\n\t"
				     "push %%r8\n\t"
			             "push %%rdx\n\t"
				     "push %%rcx\n\t"
				     "push %%rbx\n\t"
				     "push %%rax\n\t"
				     "push %%rsi\n\t"
				     "push %%rdi\n\t"
				     "mov %[context], %%rsp\n\t"
				     : : [vmcbPhysAddr] "a"(vmcbPhysAddr), [context] "m" (context): "memory");
		{
			uptr addr;
			asm volatile ("mov %0, %%rax" : "=r"(addr) : : "memory");
			vmcbPhysAddr = addr;

			VMCB *vmcb = (VMCB*)VMM::PhysicalToVirtual(addr);

			/* Unknown math, but its correct */
			context--;
			context = (GeneralRegisters*)((uptr*)context - sizeof(u64) * 2);
			PRINTK::PrintK(PRINTK_DEBUG "Context: %x\r\n", context->RAX);

			PRINTK::PrintK(PRINTK_DEBUG "Hello, VMEXIT: 0x%x\r\n", vmcb->Control.ExitCode);

			switch(vmcb->Control.ExitCode) {
				case _CPUID:
					PRINTK::PrintK(PRINTK_DEBUG "CPUID\r\n");
					// Example: Emulate CPUID instruction
					uint32_t eax, ebx, ecx, edx;
					__asm__ __volatile__("cpuid"
			                         : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                        			 : "a"(vmcb->Save.RAX), "c"(context->RCX));

					    vmcb->Save.RAX = eax;
					    context->RBX = ebx;
					    context->RCX = ecx;
					    context->RDX = edx;

					    vmcb->Save.RIP += 2;
					break;
				case _VMMCALL:
					PRINTK::PrintK(PRINTK_DEBUG "VMMCALL: 0x%x\r\n", vmcb->Save.RAX);
					vmcb->Save.RIP += 3;
					break;
				case _IOIO:
					vmcb->Save.RIP += 2;
					break;
				case _NPF:
					vmcb->Save.RIP += 2;
					break;
				default:
					break;
			}

			vmcb->Control.ExitCode = 0;
			
		}

		        __asm__ __volatile__(
            "pop %%rdi\n\t"
            "pop %%rsi\n\t"
            "pop %%rax\n\t"
            "pop %%rbx\n\t"
            "pop %%rcx\n\t"
            "pop %%rdx\n\t"
            "pop %%r8\n\t"
            "pop %%r9\n\t"
            "pop %%r10\n\t"
            "pop %%r11\n\t"
            "pop %%r12\n\t"
            "pop %%r13\n\t"
            "pop %%r14\n\t"
            "pop %%r15\n\t"
            : : : "memory");
	}
}

}
}
