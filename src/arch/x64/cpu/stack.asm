[bits 64]

global SwitchStack 
SwitchStack:
	push rbp
	mov rbp, rsp

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

	push rbp

	mov [rdi], rsp
	mov rsp, [rsi]

	pop rbp
	
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

	o64 leave
	ret

