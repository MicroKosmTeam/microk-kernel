#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

void limine_main(void);

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

struct limine_paging_mode_request paging_request = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.mode = LIMINE_PAGING_MODE_DEFAULT,
};

struct limine_hhdm_request higher_half = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
};

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;

// Halt and catch fire function.
static void hcf(void) {
	for (;;) {
		asm ("hlt");
	}
}

static inline void outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ( "outb %w1, %b0" : : "a"(val), "Nd"(port) : "memory");
}

void limine_main(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false
	   ) {
		hcf();
	}

	outb(0xe9, 'H');
	outb(0xe9, 'e');
	outb(0xe9, 'l');
	outb(0xe9, 'l');
	outb(0xe9, 'o');

	// We're done, just hang...
	hcf();
}

__attribute__((used, section(".limine_requests")))
struct limine_entry_point_request entry_point = {
	.id = LIMINE_ENTRY_POINT_REQUEST,
	.revision = 0,
	.entry = &limine_main,
};

void kmain(void) {

}
