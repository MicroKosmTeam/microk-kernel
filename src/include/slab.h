#pragma once
#include <capability.h>

#define PAGE_SIZE 4096
#define ELEMENTS_PER_SLAB 128

typedef struct slab_t {
    struct slab_t *next;
    struct slab_t *previous;
    uint16_t free_elements;
    cte_t elements[ELEMENTS_PER_SLAB];
} slab_t;

typedef struct SlabAllocator {
    slab_t *free_slabs;
    slab_t *used_slabs;
    slab_t *full_slabs;
} SlabAllocator;
