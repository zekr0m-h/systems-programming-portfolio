/**
 * @brief   Creacion de las estructuras basicas 
 * @author  Henrry Burnside Matos
 * 
 * @date    2025-11-06
 */

#include "types.h"
#include "./../utilities/util.c"
#include <string.h>

#define METADT_SIZE sizeof(MemoryBlock)
#define MINIMUM 1

void *my_malloc(size_t size){
    if(size <= 0) return NULL;

    MemoryBlock* block = findFreeBlock(size);
    if(NULL == block) return NULL;
    
    // Verifying if we can divide the block
    // In the worst case it will create the new space with a byte of free space
    size_t requiredSpace = size + METADT_SIZE + MINIMUM;
    
    if (block->size >= requiredSpace){
        MemoryBlock* freeBlock = (MemoryBlock*)((char*)block+METADT_SIZE+size);

        // Updating the freeblock
        freeBlock->canary = MAGIC_CANARY;
        freeBlock->free = 1;
        freeBlock->size = block->size - size - METADT_SIZE;    
        freeBlock->prev = block;
        freeBlock->next = block->next;

        if(block->next){
            block->next->prev = freeBlock;
        }

        // Updating the block
        block->size = size;
        block->next = freeBlock;
    }
    
    block->free = 0;
    // Return the space to be taken
    return (char*)block + METADT_SIZE;

}

/**
 * @file    types.c
 * @brief   This free a block with coalascing 
 * @date    2025-11-12
 * @param   void* ptr   // ptr of the space of memory taken
 */
void my_free(void* ptr){
    MemoryBlock* block = (MemoryBlock*)((char*)ptr - METADT_SIZE);

    // Verifying the canary
    if (block->canary !=MAGIC_CANARY){
        inform_corruption(block);
        return;
    }

    // free the block
    block->free = 1;

    // In case next block is free too
    if (block->next && block->next->free){
        block->size += block->next->size + METADT_SIZE;
        block->next = block->next->next;
        if(block->next){
            block->next->prev = block;
        }
    }
    // In case prev block is free too
    if (block->prev->free && block->prev){
        block->prev->size += block->size + METADT_SIZE;
        block->prev->next = block->next;
        if (block->next){
            block->next->prev = block->prev;
        }
    }
}


void *my_realloc(void *ptr,size_t size){
    if(!ptr || size <= 0) return NULL;
    
    MemoryBlock* block = (MemoryBlock*)((char*)ptr - METADT_SIZE);
    if (!block || size <= block->size ) return ptr;
    
    // Compare the canary
    if (block->canary!=MAGIC_CANARY){
        inform_corruption(block);
        return;
    }

    // In case its posible reallocation in-situ
    if(block->next && block->next->free){
        size_t newSize = block->next->size + block->size + METADT_SIZE;
        // it can be expanded
        if (newSize == size){
            block->size = size;
            block->next = block->next->next;
            if(block->next){
                block->next->prev = block;
            }
        } else if(newSize>size){
            if(newSize>=(size+METADT_SIZE+MINIMUM)){
                MemoryBlock* freeBlock = (MemoryBlock*)((char*)block+ METADT_SIZE+size);
                
                freeBlock->free = 1;
                freeBlock->prev = block;
                freeBlock->size = newSize - block->size - METADT_SIZE;
                freeBlock->next = block->next->next;
                if (freeBlock->next){
                    freeBlock->next->prev = freeBlock;
                }

                block->free = 0;
                block->next = freeBlock;
                block->size = size;
            }
        }
    }

    // in case isn't possible reallocation in-situ
    MemoryBlock* newBlock = findFreeBlock(size);
    if (!newBlock) return NULL;     // In case there is not a block with that size

    // Verifying the new Block can split in two blocks
    char* newBlockData = (char*)newBlock + METADT_SIZE;
    char* blockData = (char*)block+METADT_SIZE;
    memcpy(newBlockData,blockData,block->size);
    newBlock->free = 0;
    my_free(block);

    if(newBlock->size == size){
        newBlock->size = size;
        
    } else if(newBlock->size >= size+METADT_SIZE+MINIMUM){
        MemoryBlock* freeBlock = (MemoryBlock*)((char*)newBlock+METADT_SIZE+newBlock->size);
        freeBlock->free = 1;
        freeBlock->next = newBlock->next;
        freeBlock->prev = newBlock;
        freeBlock->size = newBlock->size - size - METADT_SIZE;
        
        newBlock->size = size;
        newBlock->next = freeBlock;
    }
    
    return newBlockData;
}
