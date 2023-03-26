[bits 64]

global EnableSCE
EnableSCE:
	mov rcx, 0xc0000080 ; EFER MSR
	rdmsr               ; read current EFER
	or eax, 1           ; enable SCE bit
	wrmsr               ; write back new EFER
	mov rcx, 0xc0000081 ; STAR MSR
	rdmsr               ; read current STAR
	mov edx, 0x00180008 ; load up GDT segment bases 0x0 (kernel) and 0x18 (user)
	wrmsr               ; write back new STAR
	ret                 ; return back to
