[bits 64]

extern SyscallEntry

global EnableSCE
EnableSCE:
	mov rcx, 0xc0000080 ; EFER MSR
	rdmsr               ; read current EFER
	or eax, 1           ; enable SCE bit
	wrmsr               ; write back new EFER

	mov rcx, 0xc0000081 ; STAR MSR
	rdmsr               ; read current STAR
	mov edx, 0x00480028 ; load up GDT segment bases 0x28 (kernel) and 0x48 (user)
	wrmsr               ; write back new STAR

	mov rcx, 0xc0000082 ; LSTAR MSR
	mov edx, dword [rdi]
	mov eax, dword [rdi+4]
	wrmsr               ; write back new LSTAR

	mov rcx, 0xc0000084

	ret                 ; return back
