[bits 64]

%include "src/arch/x64/cpu/macros.asm"

extern HandleSyscall 
extern StartSyscallStack

section .syscall.entrypoint

global SyscallEntry
SyscallEntry:
	cli
	swapgs

	; Save mandatory registers
	push rbx
	push r12
	push r13
	push r14
	push r15

	; Save for later sysret 
	push rcx
	push r11

	; Switch stack and save the stack pointer
	mov r12, rsp
	mov r13, rbp

	; This is because some syscall are legacy and switch address spaces
jmp .abnormal
	; Using gs
	mov rsp, gs:0
.abnormal:
	mov rsp, [StartSyscallStack]; TODO FIX

	push r12
	push r13
	
	mov rbp, rsp

	; Get arguments in the correct order

	push r10

	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax

	; Handle system call
	call HandleSyscall

	; Return to base
	mov rsp, rbp	

	; Restore stack pointer
	pop r13
	pop r12
	
	mov rsp, r12
	mov rbp, r13
	
	; Restore for sysret
	pop r11
	pop rcx

	; Restore mandatory registers
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	
	swapgs

	o64 sysret
