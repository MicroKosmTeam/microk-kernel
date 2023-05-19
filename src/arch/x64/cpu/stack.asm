[bits 64]

global SwitchStack 
SwitchStack:
;	cli

	push rbp
	mov rbp, rsp

	push rsi
	push rdi

	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	
	push rdx
	push rcx
	push rbx
	push rax

	push rbp

	mov [rdi], rsp
	mov rsp, [rsi]

	pop rbp
	
	pop rax
	pop rbx
	pop rcx
	pop rdx

	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15

	pop rdi
	pop rsi

	o64 leave
;	sti

	ret

