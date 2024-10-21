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
    	s = allign4(size);
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
        	block = init_memory_system(NULL, s);
        	if (!block) {
            		return NULL;
        	}
        	base = block;
    	}
    	return block->data;
}
