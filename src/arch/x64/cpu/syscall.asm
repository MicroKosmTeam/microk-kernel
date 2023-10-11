[bits 64]

%include "src/arch/x64/cpu/macros.asm"

section .syscall

extern HandleSyscall 

global SyscallEntry
SyscallEntry:
	swapgs

	; Switch to the new CR3
	switch_to_kernel_cr3

	; Switch to a new stack
	switch_to_kernel_stack

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
	switch_to_user_stack

	; Save and restore RAX, get back the old CR3
	switch_to_user_cr3
	
	swapgs

	o64 sysret
