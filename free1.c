#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "malloc.c"
#include "include/free.h"

/*
 * -----------------------------------------------------------------------------------------------------------------
 * 
 * This file contains the implementaion of the function free()
 * For the details on few data structs such as meta_block and meta_ptr refer to the file malloc.c
 * 
 * -----------------------------------------------------------------------------------------------------------------
 */

/* The function merge_block() checks if the next block is free and merges it with the block passed as an argument */
meta_ptr merge_blocks(meta_ptr block)
{
    if (block->next && block->next->free)
    {
        block->size += META_BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
    }
    if (block->next)
    {
        block->next->prev = block;
    }
    return block;
}

/*
 * --------------------------------------------------------------------------------------------------------------------
 *
 * The pointer is first checked if valid. If it's valid we set the free field value of the block to 1
 * Then if the previous block exists, it is checked if its free and then merged with the current block.
 * Same is done for the next block also.
 * And finally if the freed block is at the end of the linkedlist, it is removed from the linkedlist and the break line
 * of the heap is set to the corresponding last address in the linkedlist using the syscall brk()
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */
void free(void *ptr)
{
    if (is_addr_valid(ptr))
    {
        meta_ptr block = get_block_addr(ptr);
        block->free = 1;
        if (block->prev && block->prev->free)
        {
            block = merge_blocks(block->prev);
        }

        if (block->next)
        {
            block = merge_blocks(block);
        }
        else
        {
            if (block->prev)
            {
                block->prev->next = NULL;
            }
            else
            {
                base = NULL;
            }
            brk(block);
        }
    }
}
