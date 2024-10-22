#pragma once
#include <cdefs.h>

namespace x86 {
namespace SVM {
	/*
	 * 32-bit intercept words in the VMCB Control Area, starting
	 * at Byte offset 000h.
	 */

	#define INTERCEPT_MSR_PROT (1UL << 28) // MSR_PROT—intercept RDMSR or WRMSR accesses to selected MSRs.
#define INTERCEPT_CPUID    (1UL << 18) // Intercept CPUID Instruction.

#define INTERCEPT_VMRUN    (1UL << 0)  // Intercept VMRUN instruction.
#define INTERCEPT_VMMCALL  (1UL << 1)  // Intercept VMMCALL instruction.

	enum intercept_words {
		INTERCEPT_CR = 0,
		INTERCEPT_DR,
		INTERCEPT_EXCEPTION,
		INTERCEPT_WORD3,
		INTERCEPT_WORD4,
		INTERCEPT_WORD5,
		MAX_INTERCEPT,
	};

	struct VMCBControlArea {
		u32 Intercepts[MAX_INTERCEPT];
		u32 Reserved1[15 - MAX_INTERCEPT];
		u16 PauseFilterThresh;
		u16 PauseFilterCount;
		u64 IOPMBasePa;
		u64 MSRPMBasePa;
		u64 TSCOffset;
		u32 asid;
		u8 tlb_ctl;
		u8 reserved_2[3];
		u32 int_ctl;
		u32 int_vector;
		u32 int_state;
		u8 reserved_3[4];
		u32 exit_code;
		u32 exit_code_hi;
		u64 exit_info_1;
		u64 exit_info_2;
		u32 exit_int_info;
		u32 exit_int_info_err;
		u64 nested_ctl;
		u64 avic_vapic_bar;
		u64 ghcb_gpa;
		u32 event_inj;
		u32 event_inj_err;
		u64 nested_cr3;
		u64 virt_ext;
		u32 clean;
		u32 reserved_5;
		u64 next_rip;
		u8 insn_len;
		u8 insn_bytes[15];
		u64 avic_backing_page;	/* Offset 0xe0 */
		u8 reserved_6[8];	/* Offset 0xe8 */
		u64 avic_logical_id;	/* Offset 0xf0 */
		u64 avic_physical_id;	/* Offset 0xf8 */
		u8 reserved_7[8];
		u64 vmsa_pa;		/* Used for an SEV-ES guest */
		u8 reserved_8[720];
		/*
		 * Offset 0x3e0, 32 bytes reserved
		 * for use by hypervisor/software.
		 */
		u8 reserved_sw[32];
	} __attribute__((packed));

	struct VMCBSeg {
		u16 Selector;
		u16 Attrib;
		u32 Limit;
		u64 Base;
	} __attribute__((packed));

	/* Save area definition for legacy and SEV-MEM guests */
	struct VMCBSaveArea {
		VMCBSeg ES;
		VMCBSeg CS;
		VMCBSeg SS;
		VMCBSeg DS;
		VMCBSeg FS;
		VMCBSeg GS;
		VMCBSeg GDTR;
		VMCBSeg LDTR;
		VMCBSeg IDTR;
		VMCBSeg TR;
		/* Reserved fields are named following their struct offset */
		u8 Reserved0xa0[42];
		u8 VMPL;
		u8 CPL;
		u8 Reserved0xcc[4];
		u64 EFER;
		u8 Reserved0xd8[112];
		u64 CR4;
		u64 CR3;
		u64 CR0;
		u64 DR7;
		u64 DR6;
		u64 RFLAGS;
		u64 RIP;
		u8 Reserved0x180[88];
		u64 RSP;
		u64 SCET;
		u64 SSP;
		u64 ISSTAddr;
		u64 RAX;
		u64 STAR;
		u64 LSTAR;
		u64 CSTAR;
		u64 SFMASK;
		u64 KernelGSBase;
		u64 SysenterCS;
		u64 SysenterESP;
		u64 SysenterEIP;
		u64 CR2;
		u8 Reserved0x248[32];
		u64 GPAT;
		u64 DbgCtl;
		u64 BRFrom;
		u64 BRTrp;
		u64 LastExcpFrom;
		u64 LastExcpTo;
		u8 Reserved0x298[72];
		u64 SpecCtrl;		/* Guest version of SPEC_CTRL at 0x2E0 */
	} __attribute__((packed));

	struct VMCB {
		struct VMCBControlArea Control;
		struct VMCBSaveArea Save;
	}__attribute__((packed));

	int InitializeVMCB(VMCB *vmcb);
	void LoadVM(uptr statePhysAddr);
	void SaveVM(uptr statePhysAddr);
	void LaunchVM(uptr vmcbPhysAddr);
}
}
