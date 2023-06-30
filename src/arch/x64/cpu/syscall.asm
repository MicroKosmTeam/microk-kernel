[bits 64]

extern HandleSyscall 
extern StartSyscallStack

section .syscall.entrypoint

global SyscallEntry
SyscallEntry:
	push rdi
	push rsi

	push rax
	push rbx
	push rcx
	push rdx

	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

	mov rbx, rsp
	mov rsp, [StartSyscallStack]; TODO FIX

	push rbx

	mov rbp, rsp

	push r9

	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax

	call HandleSyscall

	mov rsp, rbp

	pop rbx

	mov rsp, rbx

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8

	pop rdx
	pop rcx
	pop rbx
	pop rax

	pop rsi
	pop rdi

	o64 sysret
