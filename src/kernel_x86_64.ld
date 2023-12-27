/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* We want the symbol _start to be our entry point */
ENTRY(_start)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions */
PHDRS
{
	text		PT_LOAD	   FLAGS((1 << 0) | (1 << 2));		 /* Execute + Read */
	rodata		PT_LOAD	   FLAGS((1 << 2));			 /* Read only */
	data		PT_LOAD	   FLAGS((1 << 1) | (1 << 2));		 /* Write + Read */
	essentials 	PT_LOAD	   FLAGS((1 << 0) | (1 << 1) | (1 << 2));/* Execute + Read + Write */
	dynamic         PT_DYNAMIC FLAGS((1 << 1) | (1 << 2));           /* Dynamic PHDR for relocation*/
}

SECTIONS
{
	/* We wanna be placed in the topmost 2GiB of the address space, for optimisations */
	/* Any address in this region will do, we choose 0xffffffffff800000 (-8MB) */
	/* as it will be more than enough memory for our microkernel. */
	. = 0xffffffffff800000;

	PROVIDE(__KernelBinaryEssentialStart = .);

	.essentials : {
		KEEP(*(.interrupt))
		KEEP(*(.syscall))
	} :essentials

	PROVIDE(__KernelBinaryEssentialEnd = .);

	/* Move to the next memory page for .text */
	. = ALIGN(0x1000);

	PROVIDE(__KernelBinaryTextStart = .);

	.text : {
		*(.text .text.*)
	} :text

	PROVIDE(__KernelBinaryTextEnd = .);

	/* Move to the next memory page for .rodata */
	. = ALIGN(0x1000);

	PROVIDE(__KernelBinaryRODataStart = .);
	.rodata : {
		*(.rodata .rodata.*)
	} :rodata
	
	PROVIDE(__KernelBinaryRODataEnd = .);

	/* Move to the next memory page for .data */
	. = ALIGN(0x1000);

	PROVIDE(__KernelBinaryDataStart = .);

	.data : {
		*(.data .data.*)
	} :data
	
	PROVIDE(__KernelBinaryDataEnd = .);

	/* Move to the next memory page for .dynamic */
	. = ALIGN(0x1000);

	PROVIDE(__KernelBinaryDynamicStart = .);

	.dynamic : {
		*(.dynamic .dynamic.*)
	} :data :dynamic

	PROVIDE(__KernelBinaryDynamicEnd = .);

	/* Move to the next memory page for .bss */
	. = ALIGN(0x1000);

	/* BSS is placed at the end of the file, to avoid unnecessary zeros */
	PROVIDE(__KernelBinaryBSSStart = .);

	.bss : {
		*(.bss .bss.*)
		*(COMMON)
	} :data
	
	PROVIDE(__KernelBinaryBSSEnd = .);



	/* Discard .note.* and .eh_frame since they may cause issues on some hosts. */
	/DISCARD/ : {
		*(.eh_frame .eh_frame.*)
		*(.note .note.*)
	}
}
