[bits 64]

; Function to load the GDT
global FlushGDT
FlushGDT:
	lgdt [rdi]

reloadSegments:
	; Reload CS register:
	push 0x28                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
	lea rax, [rel .reloadCS]  ; Load address of .reload_CS into RAX
	push rax                  ; Push this value to the stack
	o64 retf                  ; Perform a far return
.reloadCS:
	; Reload data segment registers
	mov   ax, 0x30
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   gs, ax
	mov   ss, ax
	ret

; Function to load the TSS
global FlushTSS
FlushTSS:
	mov ax, 0x48
	ltr ax
	ret
