/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* We want the symbol _start to be our entry point */
ENTRY(EfiMain)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions */
PHDRS
{
	text		PT_LOAD	FLAGS((1 << 0) | (1 << 2));		/* Execute + Read */
	rodata		PT_LOAD	FLAGS((1 << 2));			/* Read only */
	data		PT_LOAD	FLAGS((1 << 1) | (1 << 2));		/* Write + Read */
	essentials 	PT_LOAD	FLAGS((1 << 0) | (1 << 1) | (1 << 2));	/* Execute + Read + Write */
}

SECTIONS
{
	/* We wanna be placed in the topmost 2GiB of the address space, for optimisations */
	/* Any address in this region will do, but often 0xffffffff80000000 is chosen as */
	/* that is the beginning of the region. */

	. = 0xffffffff80000000;

	KernelBinaryEssentialStart = .;

	.essentials : {
		KEEP(*(.interrupt))
		KEEP(*(.syscall))
	} :essentials

	KernelBinaryEssentialEnd = .;

	/* Move to the next memory page for .text */
	. = ALIGN(0x1000);

	KernelBinaryTextStart = .;

	.text : {
		*(.text .text.*)
	} :text

	KernelBinaryTextEnd = .;

	/* Move to the next memory page for .rodata */
	. = ALIGN(0x1000);

	KernelBinaryRODataStart = .;

	.rodata : {
		*(.rodata .rodata.*)
	} :rodata
	
	KernelBinaryRODataEnd = .;

	. = ALIGN(0x1000);

	KernelBinaryDataStart = .;

	.data : {
		*(.data .data.*)
	} :data

	.bss : {
		*(COMMON)
		*(.bss .bss.*)
	} :data

	KernelBinaryDataEnd = .;

	/* Discard .note.* and .eh_frame since they may cause issues on some hosts. */
	/DISCARD/ : {
		*(.eh_frame .eh_frame.*)
		*(.note .note.*)
	}
}
