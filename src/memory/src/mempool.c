#include "../include/mempool.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* next;
} Node;

struct MemoryPool {
    void* memory_chunk;
    Node* free_list;
    size_t block_size;
};

MemoryPool* pool_create(size_t block_size, size_t block_count) {
    MemoryPool* pool = malloc(sizeof(MemoryPool));
    if (!pool) return NULL;

    // Align block size to pointer size for safety
    pool->block_size = (block_size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    
    size_t total_size = pool->block_size * block_count;
    pool->memory_chunk = malloc(total_size);
    if (!pool->memory_chunk) {
        free(pool);
        return NULL;
    }

    pool->free_list = NULL;
    for (size_t i = 0; i < block_count; ++i) {
        Node* new_node = (Node*)((char*)pool->memory_chunk + i * pool->block_size);
        new_node->next = pool->free_list;
        pool->free_list = new_node;
    }
    return pool;
}

void* pool_alloc(MemoryPool* pool) {
    if (!pool || !pool->free_list) {
        // In a real-world scenario, you might want to grow the pool here.
        // For our example, we'll just fail.
        fprintf(stderr, "Memory pool is out of memory!\n");
        return NULL;
    }
    Node* block = pool->free_list;
    pool->free_list = block->next;
    return block;
}

// For this simple implementation, free doesn't do anything because we reset the whole pool.
// A more complex pool would add the block back to the free_list.
void pool_free(MemoryPool* pool, void* block) {
    // (void)pool; 
    // (void)block;
    // For a more advanced pool:
    // Node* node = (Node*)block;
    // node->next = pool->free_list;
    // pool->free_list = node;
}

void pool_destroy(MemoryPool* pool) {
    if (!pool) return;
    free(pool->memory_chunk);
    free(pool);
}