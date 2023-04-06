[bits 64]

global EnableSCE
EnableSCE:
	mov rcx, 0xc0000080 ; EFER MSR
	rdmsr               ; read current EFER
	or eax, 1           ; enable SCE bit
	wrmsr               ; write back new EFER

	mov rcx, 0xc0000081 ; STAR MSR
	rdmsr               ; read current STAR
	mov edx, 0x00400028 ; load up GDT segment bases 0x28 (kernel) and 0x40 (user)
	wrmsr               ; write back new STAR

;
;	mov rcx, 0xc0000082 ; LSTAR MSR
;	rdmsr               ; read current LSTAR
;	mov rdx, rdi        ; reading from first argument kernel syscall entrypoint 
;	wrmsr               ; write back new LSTAR
;

	ret                 ; return back
