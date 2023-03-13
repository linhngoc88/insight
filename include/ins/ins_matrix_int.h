#ifndef INS_MATRIX_INT_H_
#define INS_MATRIX_INT_H_

#include <stdlib.h>

// Row-major (see [1]) matrix structure.
//
//                              +-------------...------------+
//   +------------------+       |           storage          |
//   |     elem         |-----> +-------------...------------+
//   +------------------+
//   |     owners       |
//   +------------------+
//   |     num_cols     |
//   +------------------+
//   |     num_rows     |
//   +------------------+
//
// [1] - https://en.wikipedia.org/wiki/Row-_and_column-major_order
typedef struct {
  size_t num_rows;
  size_t num_cols;
  int owner; /* 1 if this matrix owns the elem* or 0 otherwise */
  int* elem;
} ins_matrix_int;

/* Allocation */

// Allocates memory for a num_rows x num_cols matrix. All elements of the
// matrix are uninitialized and the matrix owns these elements.
ins_matrix_int*
ins_matrix_int_alloc(const size_t num_rows, const size_t num_cols);

// Allocates memory for a num_rows x num_cols matrix. All elements of the
// matrix are initialized to zero and the matrix owns these elements.
ins_matrix_int*
ins_matrix_int_calloc(const size_t num_rows, const size_t num_cols);

// Deallocates memory occupied by the given matrix.
void ins_matrix_int_free(ins_matrix_int* matrix);

/* Operations */

// Sets all elements of the given matrix to zero.
void ins_matrix_int_set_zero(ins_matrix_int* matrix);

// Set emements of the given matrix to one if they lie on the main diagonal or
// to zero if otherwise.
void ins_matrix_int_set_identity(ins_matrix_int* matrix);

// Set all elements of the given matrix to the specified value.
void ins_matrix_int_set_value(ins_matrix_int* matrix, int value);

#endif /* INS_MATRIX_INT_H_ */
