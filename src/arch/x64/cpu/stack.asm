[bits 64]

global SwitchStack
SwitchStack:
	; Save current thread's context
	push rbp
	mov rbp, rsp
	
	push r15
	push r14
	push r13
	push r12
	push rbx
	push rbp

	mov [rdi], rsp

	; Load new thread's context
	mov rsp, rsi

	pop rbp
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15

	; Leave the stack frame
	o64 leave
	ret
