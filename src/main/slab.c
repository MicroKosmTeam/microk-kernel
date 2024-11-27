#include <string.h>
#include <printf.h>
#include <slab.h>

// Initialize the slab allocator
void initialize_slab_allocator(SlabAllocator *sa) {
    sa->free_slabs = NULL;
    sa->used_slabs = NULL;
    sa->full_slabs = NULL;
}
