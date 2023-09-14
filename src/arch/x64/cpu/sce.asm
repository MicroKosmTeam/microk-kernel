[bits 64]

global EnableSCE
EnableSCE:
	mov rcx, 0xc0000080 ; EFER MSR
	rdmsr               ; read current EFER
	or rax, 1           ; enable SCE bit
	wrmsr               ; write back new EFER

	; TODO: fix STAR MSR, pretty sure what we're doing is incorrect
	mov rcx, 0xc0000081 ; STAR MSR
	;rdmsr               ; read current STAR
	;mov edx, 0x00480028 ; load up GDT segment bases 0x28 (kernel) and 0x48 (user)
	mov rax, 0x28 ; kernel cs
	shl rax, 32 
	or rax, 0x30 ; kernel ss
	wrmsr               ; write back new STAR

	mov rcx, 0xc0000082 ; LSTAR
	mov rdx, rdi
	mov eax, edx
	shr rdx, 32
	wrmsr

	mov rcx, 0xc0000084 ; FMASK
	mov rax, 0x200 ;fmmask
	wrmsr

	ret                 ; return back
