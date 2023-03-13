#include <string.h>
#include "ins/malloc_wrappers.h"

INS_MATRIX_TYPE*
INS_MATRIX_FUNC(alloc)(const size_t num_rows, const size_t num_cols) {
  size_t elem_count = num_rows * num_cols;
  size_t elem_size = sizeof(INS_MATRIX_ELEM_TYPE);

  INS_MATRIX_TYPE* matrix;
  INS_MATRIX_ELEM_TYPE* elem;

  // TODO(linh): check to make sure that matrix and elem are not NULL before
  // proceeding. And also check for potential oveflow when num_rows * num_cols
  // is too big.
  matrix = ins_malloc(1, sizeof(INS_MATRIX_TYPE));
  elem = ins_malloc(elem_count, elem_size);

  // Initialize the matrix.
  matrix->num_rows = num_rows;
  matrix->num_cols = num_cols;
  matrix->owner = 1;
  matrix->elem = elem;

  return matrix;
}

INS_MATRIX_TYPE*
INS_MATRIX_FUNC(calloc)(const size_t num_rows, const size_t num_cols) {
  size_t elem_count = num_rows * num_cols;
  size_t elem_size = sizeof(INS_MATRIX_ELEM_TYPE);

  INS_MATRIX_TYPE* matrix;
  INS_MATRIX_ELEM_TYPE* elem;

  // TODO(linh): check to make sure that matrix and elem are not NULL before
  // proceeding. And also check for potential oveflow when num_rows * num_cols
  // is too big.
  // Should we rely on calloc or malloc and then memset?
  matrix = ins_malloc(1, sizeof(INS_MATRIX_TYPE));
  elem = ins_calloc(elem_count, elem_size);

  // Initialize the matrix.
  matrix->num_rows = num_rows;
  matrix->num_cols = num_cols;
  matrix->owner = 1;
  matrix->elem = elem;

  return matrix;
}

void INS_MATRIX_FUNC(free)(INS_MATRIX_TYPE* matrix) {
  if (!matrix) { return; }

  if (matrix->owner) {
    ins_free(matrix->elem);
  }

  ins_free(matrix);
}

void INS_MATRIX_FUNC(set_zero)(INS_MATRIX_TYPE* matrix) {
  if (!matrix) { return; }

  size_t elem_count = matrix->num_rows * matrix->num_cols;
  size_t elem_size = sizeof(INS_MATRIX_ELEM_TYPE);

  // Initialize matrix to zero.
  memset(matrix->elem, 0, elem_count * elem_size);
}

void INS_MATRIX_FUNC(set_identity)(INS_MATRIX_TYPE* matrix) {
  if (!matrix) { return; }

  size_t i, j;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  INS_MATRIX_ELEM_TYPE* const elem = matrix->elem;

  const INS_MATRIX_ELEM_TYPE zero = INS_MATRIX_ELEM_ZERO;
  const INS_MATRIX_ELEM_TYPE one = INS_MATRIX_ELEM_ONE;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      *(elem + (i * num_cols + j)) = ((i == j) ? one : zero);
    }
  }
}

void
INS_MATRIX_FUNC(set_value)(INS_MATRIX_TYPE* matrix,
                               INS_MATRIX_ELEM_TYPE value) {
  if (!matrix) { return; }

  size_t i, j;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  INS_MATRIX_ELEM_TYPE* const elem = matrix->elem;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      *(elem + (i * num_cols + j)) = value;
    }
  }
}
