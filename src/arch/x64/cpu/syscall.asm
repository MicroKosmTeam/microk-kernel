[bits 64]

extern HandleSyscall 
extern StartSyscallStack

section .syscall.entrypoint

global SyscallEntry
SyscallEntry:
	; Save for later sysret 
	push rcx
	push r11

	; Get the correct rcx and r11 parameters
	mov rcx, [rsp - 32]
	mov r11, [rsp - 24]

	; Switch stack and save the stack pointer
	mov r12, rsp
	mov r13, rbp
	mov rsp, [StartSyscallStack]; TODO FIX
	push r12
	push r13

	mov rbp, rsp

	; Get arguments in the correct order
	push r9

	mov r9, r8
	mov r8, rcx
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

	o64 sysret
