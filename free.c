#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "malloc.c"
#include "include/free.h"

/***************************************************************************************************
 
 * This file contains the implementation of the function free()
 * For the details on few data structs such as meta_block and meta_ptr refer to the file malloc.c
 
 ***************************************************************************************************/


/***************************************************************************************************
 * The function get_block_addr() returns the address of the meta_block on taking the addr of 
 * data as an argument.
 *
 * Type casting the passed void pointer to char pointer and subtracting the size of the meta_block 
 * will give us the required address.
 
 ***************************************************************************************************/
meta_ptr get_block_addr(void *p)
{
    	char *tmp = p;
    	tmp = tmp - META_BLOCK_SIZE;
    	p = tmp;
    	return (p);
}

/****************************************************************************************************
 
 * This function checks if the given pointer address is indeed created by invoking malloc() or not.
 * We use the field ptr in the meta_block() to check if the passed address is same as the one 
 * present in the meta_block()
 
 ****************************************************************************************************/
int is_addr_valid(void *p)
{
    	if (base)
    	{
        	if (p > base && p < sbrk(0))
        	{
            		return (p == get_block_addr(p)->ptr);
        	}
    	}
    	return 0;
}

