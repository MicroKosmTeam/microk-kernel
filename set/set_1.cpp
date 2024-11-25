#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef struct {
    void* (*add_slab)(size_t size);
    void (*remove_slab)(void* slab);
} SlabAllocator;

typedef uint64_t uptr;

struct Element {
    struct Element* next;
    struct Element* prev;
    uptr Object;
};

#define INITIAL_TABLE_SIZE 128
#define HASH_MULTIPLIER 33

// Hash function (simple djb2 variant)
static unsigned int hash(uptr key, unsigned int table_size) {
    unsigned int hash = 5381;
    for (unsigned char* str = (unsigned char*)&key; *str != '\0'; str++) {
        hash = ((hash << 5) + hash) + *str; /* hash * 33 + c */
    }
    return hash % table_size;
}

struct Set {
    struct Element** buckets;
    struct Element* head;
    struct Element* tail;
    unsigned int size;
    SlabAllocator* allocator;
};

// Initialize the set
void set_init(struct Set* set, SlabAllocator* allocator) {
    set->buckets = (Element**)allocator->add_slab(INITIAL_TABLE_SIZE * sizeof(struct Element*));
    memset(set->buckets, 0, INITIAL_TABLE_SIZE * sizeof(struct Element*));
    set->head = NULL;
    set->tail = NULL;
    set->size = INITIAL_TABLE_SIZE;
    set->allocator = allocator;
}

// Add an element to the set
int set_add(struct Set* set, uptr obj) {
    unsigned int index = hash(obj, set->size);
    struct Element* new_elem = (Element*)set->allocator->add_slab(sizeof(struct Element));
    if (!new_elem) return -1; // Memory allocation failed

    new_elem->Object = obj;
    new_elem->next = NULL;

    if (set->buckets[index]) {
        struct Element* current = set->buckets[index];
        while (current && current->Object != obj) {
            current = current->next;
        }
        if (current) {
            set->allocator->remove_slab(new_elem); // Object already exists
            return 0;
        }

        new_elem->prev = set->tail;
        if (set->tail) {
            set->tail->next = new_elem;
        } else {
            set->head = new_elem;
        }
        set->tail = new_elem;
    } else {
        new_elem->prev = set->tail;
        if (set->tail) {
            set->tail->next = new_elem;
        } else {
            set->head = new_elem;
        }
        set->tail = new_elem;
    }

    set->buckets[index] = new_elem;
    return 1; // Element added successfully
}

// Remove an element from the set
int set_remove(struct Set* set, uptr obj) {
    unsigned int index = hash(obj, set->size);
    struct Element* current = set->buckets[index];
    while (current && current->Object != obj) {
        current = current->next;
    }
    if (!current) return 0; // Object not found

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        set->head = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    } else {
        set->tail = current->prev;
    }

    set->buckets[index] = current->next; // Update bucket
    set->allocator->remove_slab(current); // Free memory
    return 1; // Element removed successfully
}

// Check if an element exists in the set
int set_contains(struct Set* set, uptr obj) {
    unsigned int index = hash(obj, set->size);
    struct Element* current = set->buckets[index];
    while (current && current->Object != obj) {
        current = current->next;
    }
    return current != NULL; // Object found
}

// Iterate over the set from head to tail
void set_iterate(struct Set* set, void (*callback)(struct Element*)) {
    struct Element* current = set->head;
    while (current) {
        callback(current);
        current = current->next;
    }
}

#include <stdio.h>
#include <cstdlib>

// Dummy slab allocator functions for demonstration purposes
void* add_slab(size_t size) {
    return malloc(size);
}

void remove_slab(void* slab) {
    free(slab);
}

int main() {
    SlabAllocator allocator = {add_slab, remove_slab};
    struct Set set;
    set_init(&set, &allocator);

    // Add elements
    set_add(&set, 10);
    set_add(&set, 20);
    set_add(&set, 30);

    // Iterate over the set
    set_iterate(&set, [](struct Element* elem) {
        printf("Object: %lu\n", elem->Object);
    });

    // Check if an element exists
    if (set_contains(&set, 20)) {
        printf("Element 20 is in the set.\n");
    } else {
        printf("Element 20 is not in the set.\n");
    }

    // Remove an element
    set_remove(&set, 20);
    if (!set_contains(&set, 20)) {
        printf("Element 20 has been removed from the set.\n");
    } else {
        printf("Failed to remove element 20.\n");
    }

    return 0;
}
