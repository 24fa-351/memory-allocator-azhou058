#ifndef HEAP_MEMORY_MANAGER_H
#define HEAP_MEMORY_MANAGER_H

#include <stddef.h>

void* memory_malloc(size_t size);
void memory_free(void* ptr);
void* memory_realloc(void* ptr, size_t size);

void memory_init(size_t memory_size);
void memory_deinit();

#endif // HEAP_MEMORY_MANAGER_H