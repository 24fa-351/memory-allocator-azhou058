#include "memory.h"
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define _DEFAULT_SOURCE


#define MEMORY_SIZE sizeof(BlockMemoryData)
#define HEAP_CAPACITY 128
#define ALIGN_8_BYTES(size) (((((size)-1) >> 3) << 3) + 8)

typedef struct BlockMemoryData {
    size_t size;
    bool is_free;
    struct BlockMemoryData* next;
} BlockMemoryData;

BlockMemoryData* free_heap[HEAP_CAPACITY];
size_t heap_size = 0;

static void* heap_start = NULL;

void memory_init(size_t memory_size) {
    heap_start = sbrk(memory_size);
    if (heap_start == (void*)-1) {
        fprintf(stderr, "Error: Unable to allocate heap memory.\n");
        return;
    }

    BlockMemoryData* initial_block = (BlockMemoryData*)heap_start;
    initial_block->size = memory_size - MEMORY_SIZE;
    initial_block->is_free = true;
    initial_block->next = NULL;

    free_heap[heap_size++] = initial_block;
}

void memory_deinit() {
    if (heap_start != NULL) {
        sbrk(-heap_size * MEMORY_SIZE);
        heap_start = NULL;
        heap_size = 0;
    }
}

void heap_top(size_t index) {
    if (index == 0) return;
    size_t parent = (index - 1) / 2;
    if (free_heap[index]->size < free_heap[parent]->size) {
        BlockMemoryData* temp = free_heap[index];
        free_heap[index] = free_heap[parent];
        free_heap[parent] = temp;
        heap_top(parent);
    }
}

void heap_down(size_t index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t smallest = index;

    if (left < heap_size && free_heap[left]->size < free_heap[smallest]->size) {
        smallest = left;
    }
    if (right < heap_size && free_heap[right]->size < free_heap[smallest]->size) {
        smallest = right;
    }
    if (smallest != index) {
        BlockMemoryData* temp = free_heap[index];
        free_heap[index] = free_heap[smallest];
        free_heap[smallest] = temp;
        heap_down(smallest);
    }
}

BlockMemoryData* find_free_block(size_t size) {
    for (size_t iteration = 0; iteration < heap_size; ++iteration) {
        if (free_heap[iteration]->is_free && free_heap[iteration]->size >= size) {
            BlockMemoryData* block = free_heap[iteration];
            free_heap[iteration] = free_heap[--heap_size];
            heap_down(iteration);
            return block;
        }
    }
    return NULL;
}

void insert_free_block(BlockMemoryData* block) {
    free_heap[heap_size] = block;
    heap_top(heap_size);
    heap_size++;
}

void split_block(BlockMemoryData* block, size_t size) {
    if (block->size > size + MEMORY_SIZE) {
        BlockMemoryData* new_block = (BlockMemoryData*)((char*)block + MEMORY_SIZE + size);
        new_block->size = block->size - size - MEMORY_SIZE;
        new_block->is_free = true;
        new_block->next = NULL;
        block->size = size;
        insert_free_block(new_block);
    }
}

void* memory_malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    size = ALIGN_8_BYTES(size);
    BlockMemoryData* block = find_free_block(size);
    if (block != NULL) {
        block->is_free = false;
        split_block(block, size);
        memset((char*)block + MEMORY_SIZE, 0, size);
        return (void*)((char*)block + MEMORY_SIZE);
    }

    fprintf(stderr, "Error: No block found for allocation.\n");
    return NULL;
}

void memory_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    BlockMemoryData* block = (BlockMemoryData*)((char*)ptr - MEMORY_SIZE);
    block->is_free = true;
    insert_free_block(block);
}

void* memory_realloc(void* ptr, size_t size) {
    if (ptr == NULL) {
        return memory_malloc(size);
    }

    BlockMemoryData* block = (BlockMemoryData*)((char*)ptr - MEMORY_SIZE);
    if (block->size >= size) {
        return ptr;
    }

    void* new_ptr = memory_malloc(size);
    if (new_ptr != NULL) {
        memcpy(new_ptr, ptr, block->size);
        memory_free(ptr);
    }
    return new_ptr;
}
