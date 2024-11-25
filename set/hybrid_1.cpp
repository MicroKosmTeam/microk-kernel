#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t uptr;

// Assuming PMM::RequestPage() is defined elsewhere and returns a pointer to a new page
void* PMM_RequestPage()  { return malloc(4096); }

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

    return data;
}

// Deallocate a slab of memory and add it back to the free slabs list
void deallocate_slab(SlabAllocator* allocator, void* ptr) {
    Slab* new_free_slab = (Slab*)((char*)ptr - sizeof(Slab));
    new_free_slab->next = allocator->free_slabs;
    allocator->free_slabs = new_free_slab;
}

// Hybrid Set Implementation
typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    SlabAllocator* slab_allocator;
    Node** array;
    size_t size; // Current size of the set
    size_t capacity; // Current capacity of the array
} HybridSet;

void hybrid_set_init(HybridSet* set, size_t initial_capacity) {
    set->slab_allocator = (SlabAllocator*)PMM_RequestPage();
    slab_allocator_init(set->slab_allocator, sizeof(Node));
    set->array = (Node**)PMM_RequestPage();
    for (size_t i = 0; i < initial_capacity; ++i) {
        set->array[i] = NULL;
    }
    set->size = 0;
    set->capacity = initial_capacity;
}

void hybrid_set_add(HybridSet* set, int value) {
    if (set->size >= set->capacity) {
        // Resize the array
        size_t new_capacity = set->capacity * 2;
        Node** new_array = (Node**)PMM_RequestPage();
        for (size_t i = 0; i < set->capacity; ++i) {
            new_array[i] = set->array[i];
        }
        // Initialize new elements
        for (size_t i = set->capacity; i < new_capacity; ++i) {
            new_array[i] = NULL;
        }
        deallocate_slab(set->slab_allocator, set->array);
        set->array = new_array;
        set->capacity = new_capacity;
    }

    Node* node = (Node*)allocate_slab(set->slab_allocator, sizeof(Node));
    if (!node) {
        return; // Allocation failed
    }
    node->data = value;
    node->next = NULL;

    size_t index = abs(value) % set->capacity;
    if (set->array[index] == NULL) {
        set->array[index] = node;
    } else {
        Node* current = set->array[index];
        while (current->next != NULL) {
            if (current->data == value) {
                deallocate_slab(set->slab_allocator, node);
                return; // Value already exists
            }
            current = current->next;
        }
        if (current->data == value) {
            deallocate_slab(set->slab_allocator, node);
            return; // Value already exists
        }
        current->next = node;
    }
    set->size++;
}

int hybrid_set_contains(HybridSet* set, int value) {
    size_t index = abs(value) % set->capacity;
    Node* current = set->array[index];
    while (current != NULL) {
        if (current->data == value) {
            return 1; // Value found
        }
        current = current->next;
    }
    return 0; // Value not found
}

void hybrid_set_remove(HybridSet* set, int value) {
    size_t index = abs(value) % set->capacity;
    Node* current = set->array[index];
    Node* prev = NULL;

    while (current != NULL) {
        if (current->data == value) {
            if (prev == NULL) {
                set->array[index] = current->next;
            } else {
                prev->next = current->next;
            }
            deallocate_slab(set->slab_allocator, current);
            set->size--;
            return; // Value removed
        }
        prev = current;
        current = current->next;
    }
}

void hybrid_set_free(HybridSet* set) {
    for (size_t i = 0; i < set->capacity; ++i) {
        Node* current = set->array[i];
        while (current != NULL) {
            Node* next = current->next;
            deallocate_slab(set->slab_allocator, current);
            current = next;
        }
    }
    deallocate_slab(set->slab_allocator, set->array);
    deallocate_slab(set->slab_allocator, set->slab_allocator);
}

int main() {
    HybridSet set;
    hybrid_set_init(&set, 1000);

    for (int i = 0; i < 100; ++i) {
	    hybrid_set_add(&set, i);
    }

    printf("Contains 5: %d\n", hybrid_set_contains(&set, 5)); // Should print 1
    printf("Contains 10: %d\n", hybrid_set_contains(&set, 10)); // Should print 1
    printf("Contains 20: %d\n", hybrid_set_contains(&set, 20)); // Should print 0

    hybrid_set_remove(&set, 10);
    printf("Contains 10 after removal: %d\n", hybrid_set_contains(&set, 10)); // Should print 0

    hybrid_set_free(&set);

    return 0;
}
