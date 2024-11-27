#include <stdint.h>
#include <arch/x86_64/boot/alloc.h>
#include <printf.h>

// Define the size of one page
#define PAGE_SIZE 4096

// Initialize the memory manager with a given area of memory
void initialize_memory_manager(MemoryManager *mm, void *memory, size_t size) {
    mm->memory = (uint8_t *)memory;
    mm->total_pages = size / PAGE_SIZE;
    mm->used_pages = 0;

    printk("Initialized memory manager 0x%x\r\n", mm);
}

// Allocate one page and return it
void* request_page(MemoryManager *mm) {
    if (mm->used_pages >= mm->total_pages) {
        printk("Memory pool is exhausted!\n");
        return NULL;
    }
    
    void *page = &mm->memory[mm->used_pages * PAGE_SIZE];
    mm->used_pages++;
    return page;
}

// Allocate more pages and return them
void * request_pages(MemoryManager *mm, size_t page_count) {
    if (mm->used_pages + page_count >= mm->total_pages) {
        printk("Memory pool is exhausted!\n");
        return NULL;
    }
    
    void *page = &mm->memory[mm->used_pages * PAGE_SIZE];
    mm->used_pages+=page_count;
    return page;
}
