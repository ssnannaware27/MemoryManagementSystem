#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "main.h"

/* base stores the head of the linked list */
/* The base pointer, which will point to the first block in the heap, is set to NULL initially.
 * This means no memory blocks are allocated yet.
 */
void *base = NULL;



/*
 * This implementation of malloc is based on first-fit style.

/* Function to search the memory blocks starting from base to find a free block that is large enough to hold the requested size
*/

/* Initializes b to point to the base block, which is the first block in the memory list.
 * This is the starting point for searching the memory blocks.
 */

/* The loop iterates over all blocks in the memory. The condition checks if the block b is free (b->free)
 * and if the block’s size (b->size) is greater than or equal to the requested size (size).
 * The loop will continue until it finds such a block or reaches the end of the memory list (b == NULL).
 */

/* The pointer last is updated to keep track of the last block examined during the search.
 * This is useful if the system needs to append a new block to the end of the list.
 */

/* b moves to the next block in the linked list, following the next pointer.
 */

/* The function returns the block that fits the requested size and is free.
 * If no such block is found, it returns the last block, allowing the memory system to allocate new memory if needed.
 */

/* time complexity for this function will be O(n) */
meta_ptr find_suitable_block(meta_ptr *last, size_t size) {
        meta_ptr b = base;
        while (b && !(b->free && b->size >= size)) {
                *last = b;
                b = b->next;
        }
        return *last;
}

/* Function to split a large free block into two smaller blocks. 
 * One block is of the requested size, and the other contains the remaining space. 
 * This helps efficiently manage memory by only allocating the necessary space and leaving the rest available for future allocations.
 */

/* meta_ptr new_block declares a new block that will hold the remaining memory after splitting.
 * The new block is positioned right after the data portion of the current block. 
 * Since block->data points to the start of the data region, adding size moves the pointer to the end of the allocated portion, 
 * effectively setting the address for the new block.
 */

/* The total size of the current block is reduced by the requested allocation size (size) 
 * and the size of the metadata structure (META_BLOCK_SIZE).
 */

/* The new block’s next pointer is set to point to the next block in the list, maintaining the linked list structure.
 * The new block is marked as free, indicating that it is available for future allocations.
 * The ptr field is set to point to the start of the data area in the new block.
 * The prev pointer of the new block is set to point back to the current block, maintaining the doubly linked list structure.
 * The next pointer of the current block is updated to point to the new block.
 * The size of the current block is updated to the requested size, ensuring that only the necessary space is allocated.
 */

/* If there is a block after the new block, its prev pointer is updated to point to the new block, 
 * ensuring the integrity of the doubly linked list.
 */
void split_space(meta_ptr block, size_t size) {
        meta_ptr new_block;
        new_block = (meta_ptr)(block->data + size);
        new_block->size = block->size - size - META_BLOCK_SIZE;
        new_block->next = block->next;
        new_block->free = 1;
        new_block->ptr = new_block->data;
        new_block->prev = block;
        block->next = new_block;
        block->size = size;
        if (new_block->next) {
                new_block->next->prev = new_block;
        }
        return;
}

/*
 * ----------------------------------------------------------------------------------------------------------------------
 *
 * extend_heap() is invoked when the already available blocks of memory is not sufficient
 * or if no blocks are currently available.
 * This creates a block of memory at the end of the heap.
 * 
 * It follows the below process:
 * 	- Using sbrk(0) ----> get the current end of the heap.
 * 	- Allocating the memory, considering both metadata block and the requested size
 * 	- If (memory allocation fails) ----> return NULL.
 * 	- else if (successful) ----> initialize the metadata block for the new memory block.
 * 	- Link the newly created block to the existing blocks (if any).
 * 
 * finally returns the address of the newly allocated heap memory.
 * 
 * Time complexity : O(1)
 * 
 * ----------------------------------------------------------------------------------------------------------------------
 */
meta_ptr extend_heap(meta_ptr last, size_t size) {
    	meta_ptr old_break, new_break;
    	old_break = sbrk(0);
    	//sbrk(0) --> returns the current end of the heap
    	new_break = sbrk(META_BLOCK_SIZE + size);
    	if (new_break == (void *)-1) {
        	return NULL;
    	}
    	old_break->size = size;
    	old_break->free = 0;
    	old_break->next = NULL;
    	old_break->prev = NULL;
    	old_break->ptr = old_break->data;
    	if (last) {
        	last->next = old_break;
    	}
    	return (old_break);
}

/*
 * ----------------------------------------------------------------------------------------------------------------------
 * 
 * malloc() allocates memory as follows:
 * 
 * # The requested size is first aligned to a multiple of 4 bytes for proper memory alignment.
 *    - Example: size = 5 → allocated = 8.
 * 
 * # Call find_suitable_block():
 *    - If (base != NULL) ----> find a free block that fits aligned size.
 * 
 * # Call split_space():
 *    - If block found, check leftover space for new metadata.
 *    - If enough, split block & mark as not free (block->free = 0).
 * 
 * # Call extend_heap():
 *    - If no block found, extend heap.
 *    - If extend_heap fails (NULL) -----> malloc returns NULL.
 * 
 * # Call init_memory_system():
 *    - If first malloc (base = NULL) -----> initialize memory system & set base.
 * 
 * Return pointer to usable data (block->data).
 *
 * ----------------------------------------------------------------------------------------------------------------------
 */
void *malloc(size_t size) {
    	meta_ptr block, last;
    	size_t s;
    	s = align4(size);
    	if (base) {
        	last = base;
        	block = find_suitable_block(&last, s);
        	if (block) {
            		if (block->size - s >= (META_BLOCK_SIZE + 4)) {
                		split_space(block, s);
            		}
            		block->free = 0;
        	}
        	else {
            		block = extend_heap(last, s);
            		if (!block) {
                		return NULL;
            		}
        	}
    	}
    	else {
        	block = extend_heap(NULL, s);
        	if (!block) {
            		return NULL;
        	}
        	base = block;
    	}
    	return block->data;
}
