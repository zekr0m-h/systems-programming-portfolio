#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAGIC_CANARY 0xDEADBEEF


// metadata structure
typedef struct MemoryBlock MemoryBlock;
struct MemoryBlock {
    uint32_t canary;        // It shows if exist any memory corruption
    size_t size;            // size of the block without metada
    int free;               // 1 if is free
    MemoryBlock* prev;      //* This link this block with the previous
    MemoryBlock* next;      //* and the next block
};

// Main functions 
void *my_malloc(size_t size);
void my_free(void* ptr);
void *my_realloc(void* ptr, size_t size);
#endif