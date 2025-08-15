#pragma once
#include <stddef.h>

typedef struct MemoryPool MemoryPool;

MemoryPool* pool_create(size_t block_size, size_t block_count);
void* pool_alloc(MemoryPool* pool);
void pool_free(MemoryPool* pool, void* block); 
void pool_destroy(MemoryPool* pool);