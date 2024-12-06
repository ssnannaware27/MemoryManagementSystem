 #include <stddef.h>
 /* meta_block is a struct to store the meta information about every chuck of memory being allocated.
 * A doubly-linked list is maintained with all the meta_blocks
 * to maintain the space being allocated and deallocated
 * Size of meta_block is considered to be 20 (4 bytes for each of the  variables).
 */

#define META_BLOCK_SIZE 20

/* The macro align4 is used to set the requested size to multiple of four greater than requested size */
#define align4(x) ((x + 3)/ 4* 4)

/* meta_ptr is a pointer of type meta_block, it is type defined for simplicity and to avoid confusion */
typedef struct meta_block *meta_ptr;



/*
 * The meta_block contains the variables free, size, next, data.
 * free is set to 1 when the respective block is free and vice versa.
 * size is used to store the size of the respective block.
 * The character array data gives the next address after the
 * meta_block as the data variable is defined at last.
 * The data variable is made as a character pointer assuming that the character takes 1 byte,
 * and so it makes the pointer arithmetic simpler
 * The next and prev pointers points to the block in the
 * doubly linked list present next and previous to the curr block.
 */
struct meta_block
{
    int free;
    size_t size;
    meta_ptr next;
    meta_ptr prev;
    void *ptr;
    char data[1];
};

extern void* base;

void* malloc(size_t size);
void *calloc(size_t number, size_t size);
void free(void *ptr);
void *realloc(void *p, size_t size);
