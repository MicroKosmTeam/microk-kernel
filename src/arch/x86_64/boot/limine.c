#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

void limine_main(void);

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memory_map = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_paging_mode_request paging_request = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.mode = LIMINE_PAGING_MODE_DEFAULT,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request higher_half = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_kernel_address_request kernel_address = {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0,
};

// Halt and catch fire function.
static void hcf(void) {
	for (;;) {
		asm ("hlt");
	}
}

void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ( "outb %w1, %b0" : : "a"(val), "Nd"(port) : "memory");
}

void putc(char c) {
	outb(0xe9, c);
}

void puts(char *s) { 
	while(*s) {
		putc(*s++);
	}
}

typedef struct {
	uintptr_t addr;
	size_t size;
	uint16_t type;
} memmap_entry_t;

#include <printf.h>
#include <arch/x86_64/boot/alloc.h>
#include <arch/x86_64/boot/gdt.h>
void limine_main(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false ||
	   memory_map.response == NULL) {
		hcf();
	}

	puts("Hello, world\r\n");
	

	printk("Loading memmap\r\n");

	size_t entry_count = memory_map.response->entry_count;
	printk("Loading %d memmap entries\r\n", entry_count);

	memmap_entry_t entries[entry_count];

	size_t longest_entry_size = 0;
	size_t longest_entry_index = 0;

	for (size_t i = 0; i < entry_count; ++i) {
		entries[i].addr = memory_map.response->entries[i]->base;
		entries[i].size = memory_map.response->entries[i]->length;
		entries[i].type = memory_map.response->entries[i]->type;

		printk("Entry %d: [%lx-%lx] of type %d\r\n", i, entries[i].addr, entries[i].addr + entries[i].size, entries[i].type);

		if (entries[i].type == LIMINE_MEMMAP_USABLE &&
			entries[i].size >= longest_entry_size) {
			longest_entry_size = entries[i].size;
			longest_entry_index = i;
		}
	}

	printk("Longest entry: [%lx-%lx] of type %d\r\n", entries[longest_entry_index].addr, entries[longest_entry_index].addr + entries[longest_entry_index].size, entries[longest_entry_index].type);

	MemoryManager mm;
	initialize_memory_manager(&mm, (void*)entries[longest_entry_index].addr, entries[longest_entry_index].size);

	GDT gdt;
	GDTPointer gdt_ptr;
	TSS tss;
	load_gdt(&gdt, &gdt_ptr);
	tss_init(&gdt, &tss, 0);

	printk("GDT loaded.\r\n");

	// We're done, just hang...
	hcf();
}

__attribute__((used, section(".limine_requests")))
static volatile struct limine_entry_point_request entry_point = {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.entry = &limine_main,
};

void kmain(void) {

}
