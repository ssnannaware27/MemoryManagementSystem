#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "malloc.c"
#include "include/calloc.h"

/*
 * ------------------------------------------------------------------------------------
 *
 * This file contains the implementaion of the function calloc()
 * For the details on malloc() and allign4() refer the file malloc.c
 * calloc() first creates the required chunk of memory using malloc()
 * then it sets the values of each byte to 0 by iterating through it.
 *
 * The process follows as:
 * 	- maintaining a pointer iterating through each byte
 *	- setting each byte's value to 0
 * 
 * Time complexity : O(n)
 *
 * ------------------------------------------------------------------------------------
 */

void *calloc(size_t number, size_t size) {
    	size_t *new;
    	size_t s, i;

    	new = malloc(number * size);

    	if (new) {

        	s = align4(number * size) << 2;

        	for (i = 0; i < s; i++) {
            		new[i] = 0;
        	}
    	}
    	return new;
}
