#ifndef INS_BLOCK_DOUBLE_H_
#define INS_BLOCK_DOUBLE_H_

#include <stdlib.h>
#include <ins/ins_errno.h>

// The `ins_block_struct` structure contains two components, the `size` and
// the `data`. `size` is the number of doubles in the block and `data` is the
// pointer pointing to the allocated memory.
struct ins_block_struct {
  size_t size;
  double * data;
};

typedef struct ins_block_struct ins_block;

/* Allocation */

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

/* Operation */

// Reads into the block `block` from the given open stream `stream` in binary
// format. The block `block` must be preallocated with the correct length
// since the function uses the size of `block` to determine how many bytes to
// read. The return value is `0` for success and `INS_EFAILED` if there was a
// problem reading from the file. (`man fread` for more details).
int ins_block_fread(ins_block * block, FILE * stream);

// Writes the elements of the given `block` to the specified stream `stream`
// in binary format. The return value is `0` for success and `INS_EFAILED`
// if there was a problem writing to the file. (`man fwrite` for more details).
int ins_block_fwrite(const ins_block * block, FILE * stream);

// Writes elements of the given block `block` line-by-line to the specified
// stream `stream` using the format specifier `format`, which should be one
// of `%g`, `%e` or `%f` formats for floating point numbers and `%d` for
// integers. The return value is `0` for success and `INS_EFAILED` if there
// was a problem writing to the file.
int ins_block_fprintf(const ins_block *block, FILE *stream, const char *format);

// Reads formatted data from the given stream `stream` into the specified
// block `block`. The block `block` must be preallocated with the correct
// length since the function uses the size of `block` to determine how many
// numbers to read. The function returns `0` for success and `INS_EFAILED`
// if there was a problem reading from the file.
int ins_block_fscanf(ins_block *block, FILE *stream);

#endif // INS_BLOCK_DOUBLE_H_
