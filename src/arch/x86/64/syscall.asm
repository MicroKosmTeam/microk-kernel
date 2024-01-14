[bits 64]

extern SyscallMain

global HandleSyscall64
HandleSyscall64:
	; Save cross-function registers
	push rbx
	push r12
	push r13
	push r14
	push r15

	; Save for later sysret 
	push rcx
	push r11

	; Get arguments in the correct order
	; rax rdi rsi rdx r8 r9 r10
	;  |   |   |   |   |  |  |
	; rdi rsi rdx rcx r8 r9 stack
	mov rbp, rsp

	push r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax

	; Handle system call
	call SyscallMain 

	; Return to base
	mov rsp, rbp

	; Restore for sysret
	pop r11
	pop rcx

	; Restore mandatory registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx

	o64 sysret

