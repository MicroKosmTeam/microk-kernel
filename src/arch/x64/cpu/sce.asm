[bits 64]

global EnableSCE
EnableSCE:
	mov ecx, 0xc0000080 ; EFER MSR
	rdmsr               ; read current EFER
	or eax, 1           ; enable SCE bit
	wrmsr               ; write back new EFER

	mov rcx, 0xc0000081 ; STAR MSR
	mov eax, edi         ; Legacy syscall handler
	mov edx, 0x00380028 ; Set the seSYSENTERgment bases
	wrmsr               ; write back new STAR

	mov rcx, 0xc0000082 ; LSTAR
	mov rdx, rsi
	mov eax, edx
	shr rdx, 32
	wrmsr

	mov rcx, 0xc0000084 ; FMASK
	mov rax, 0x200 ;fmmask
	wrmsr

	ret                 ; return back

