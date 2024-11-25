#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint64_t uptr;

// Assuming PMM::RequestPage() is defined elsewhere and returns a pointer to a new page
void* PMM_RequestPage() { return malloc(4096); };

#define PAGE_SIZE 4096

struct Slab {
    struct Slab* next;
    char* data; // Pointer to the start of the slab's memory
};

typedef struct {
    Slab* free_slabs;
} SlabAllocator;

// Initialize a slab allocator with page size and slab size
void slab_allocator_init(SlabAllocator* allocator, size_t slab_size) {
    allocator->free_slabs = NULL;
}

// Request a new page from the PMM and add it to the free slabs list
static void add_new_page_to_free_slabs(SlabAllocator* allocator, size_t slab_size) {
    Slab* new_slab = (Slab*)PMM_RequestPage();
    if (!new_slab) {
        return; // No page available from PMM
    }

    new_slab->next = allocator->free_slabs;
    new_slab->data = (char*)new_slab + sizeof(Slab);
    allocator->free_slabs = new_slab;
}

// Allocate a slab of memory from the free slabs list
void* allocate_slab(SlabAllocator* allocator, size_t slab_size) {
    if (!allocator->free_slabs) {
        add_new_page_to_free_slabs(allocator, slab_size);
    }

    Slab* current = allocator->free_slabs;
    void* data = current->data;

    allocator->free_slabs = current->next;
    //free(current); // Free the slab structure itself

    return data;
}

// Deallocate a slab of memory and add it back to the free slabs list
void deallocate_slab(SlabAllocator* allocator, void* ptr) {
    Slab* new_free_slab = (Slab*)((char*)ptr - sizeof(Slab));
    new_free_slab->next = allocator->free_slabs;
    allocator->free_slabs = new_free_slab;
}

#include <stdio.h>

// Example usage
int main() {
    SlabAllocator allocator;
    size_t slab_size = 128; // Size of each slab

    slab_allocator_init(&allocator, slab_size);

    void* slab1 = allocate_slab(&allocator, slab_size);
    if (slab1) {
        printf("Slab allocated at %p\n", slab1);
        deallocate_slab(&allocator, slab1);
        printf("Slab deallocated\n");
    }

    return 0;
}
