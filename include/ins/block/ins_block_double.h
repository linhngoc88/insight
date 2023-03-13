#ifndef INS_BLOCK_DOUBLE_H_
#define INS_BLOCK_DOUBLE_H_

#include <stdlib.h>
#include <ins/ins_errno.h>

// The `ins_block_struct` structure contains two components, the `size` and
// the `base`. `size` is the number of doubles in the block and `base` is the
// pointer pointing to the allocated memory.
struct ins_block_struct {
  size_t size;
  double * base;
};

typedef struct ins_block_struct ins_block;

// Allocates memory for a block of `count` doubles and returns a pointer to the
// block struct. The block is not initialized and so the values of its elements
// are undefined.
// Zero-count requests are valid and return a non-null result.
// A `NULL` pointer is returned if there is not enough memory to create a block.
ins_block * ins_block_alloc(const size_t count);

// Similar to `ins_block_alloc` but this functions initializes all elements of
// the block to zero.
ins_block * ins_block_calloc(const size_t count);

// Frees the memory used by a block `block` previously allocated with either
// `ins_block_alloc` or `ins_block_calloc`.
void ins_block_free(ins_block * block);

#endif // INS_BLOCK_DOUBLE_H_
