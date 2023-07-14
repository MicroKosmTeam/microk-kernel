[bits 64]

; Function to activate SSE
global EnableSSE 
EnableSSE:
	push rax

	mov rax, cr0
	and ax, 0xFFFB		; Clear coprocessor emulation CR0.EM
	or ax, 0x2		; Set coprocessor monitoring  CR0.MP
	mov cr0, rax
	mov rax, cr4
	or ax, 3 << 9		; Set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
	mov cr4, rax

	pop rax

	ret

global EnableAVX
EnableAVX:
	push rax
	push rcx
	push rdx
 
	xor rcx, rcx
	xgetbv         ; Load XCR0 register
	or eax, 7      ; Set AVX, SSE, X87 bits
	xsetbv         ; Save back to XCR0
 
	pop rdx
	pop rcx
	pop rax
	ret
