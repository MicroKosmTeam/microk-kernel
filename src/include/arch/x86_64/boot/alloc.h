#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t *memory;       // Pointer to the start of the memory pool
    size_t total_pages;   // Total number of pages in the pool
    size_t used_pages;     // Number of pages already allocated
} MemoryManager;

void initialize_memory_manager(MemoryManager *mm, void *memory, size_t size);
void* request_page(MemoryManager *mm);
void* request_pages(MemoryManager *mm, size_t page_count);
