[bits 64]

%include "src/arch/x64/cpu/macros.asm"

section .syscall

extern HandleSyscall 

global SyscallEntry
SyscallEntry:
	swapgs

	; Save and restore RAX, switch to the new CR3
	mov gs:40, rax
	mov rax, gs:32
	mov cr3, rax
	mov rax, gs:40

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

	; Save and restore RAX, get back the old CR3
	mov gs:40, rax
	mov rax, gs:24
	mov cr3, rax
	mov rax, gs:40
	
	swapgs

	o64 sysret
