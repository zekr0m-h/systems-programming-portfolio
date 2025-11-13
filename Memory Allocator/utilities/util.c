#include "./../core/types.h"
#define VHEAP_SIZE 1024*1024

// Initialize pointer to the Virtual Heap
void* VMEM = NULL;
MemoryBlock* head = NULL;




/**
 * @brief Creates the virtual heap with 1Mb of memory
 * @attention IF I want to resize this amount of memory, just change VHEAP_SIZE
 */
void initialize_heap(){
    VMEM = malloc(VHEAP_SIZE);
    if (!VMEM){
        perror("Failed to allocate the virtual Heap\n");
        exit(1);
    }

    MemoryBlock* firstBlock = (MemoryBlock*)VMEM;
    head = firstBlock;
    firstBlock->canary = MAGIC_CANARY;
    firstBlock->free = 1;
    firstBlock->prev = NULL;
    firstBlock->next = NULL;
    firstBlock->size = VHEAP_SIZE - sizeof(MemoryBlock);
}


/**
 * @brief Find a free block using best-fit
 * @attention Implement several threads to find that block
 */

MemoryBlock *findFreeBlock(size_t size){
    MemoryBlock* bestBlock = NULL;
    size_t bestSize = SIZE_MAX;
    
    // Starting from the head
    MemoryBlock* current = (MemoryBlock*)head;
    
    while (current){
        // No write in any corrupted block
        if (current->canary!=MAGIC_CANARY) {
            inform_corruption(current);
            current = current->next;
            continue;
        }

        // Finding the best free block
        if ((current->free) && (current->size >= size)){
            if (current->size < bestSize) {
                bestBlock = current;
                bestSize = current->size;
            }
        }
        // it breaks when it's the tail
        current = current->next;
    }
    return bestBlock;
}

void inform_corruption(MemoryBlock* block){
    fprintf(stderr,"Corrupted block in %p\n",block);
    fprintf(stderr,"Canary value expected: %p\nCanary value recibed: %p", MAGIC_CANARY,block->canary);
    abort();

}