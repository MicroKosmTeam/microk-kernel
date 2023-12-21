[bits 64]

global EnterUserspace
EnterUserspace:
	mov rcx, rdi        ; first argument, new instruction pointer

	mov rsp, rsi        ; second argument, new stack pointer
	mov rbp, rsp

	mov r11, 0x0202     ; eflags

	xor rax, rax
	xor rbx, rbx
	xor rdx, rdx

	xor r8, r8
	xor r9, r9
	xor r10, r10
	xor r12, r12
	xor r13, r13
	xor r14, r14
	xor r15, r15

	xor rdi, rdi
	xor rsi, rsi

	o64 sysret
	
	jmp $
