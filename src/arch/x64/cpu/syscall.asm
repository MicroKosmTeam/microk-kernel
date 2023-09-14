[bits 64]

%include "src/arch/x64/cpu/macros.asm"

extern HandleSyscall 

section .syscall

global SyscallEntry
SyscallEntry:
	swapgs

	; Save the stack pointer
	mov gs:8, rsp
	mov gs:16, rbp

	; Switch to a new stack
	mov rsp, gs:0	

	; Save mandatory registers
	push rbx
	push r12
	push r13
	push r14
	push r15

	; Save for later sysret 
	push rcx
	push r11

	; Get arguments in the correct order
	mov rbp, rsp

	push r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax

	; Handle system call
	call HandleSyscall

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

	; Restore stack pointer
	mov rsp, gs:8
	mov rbp, gs:16
	
	swapgs

	o64 sysret
