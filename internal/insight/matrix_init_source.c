#include <string.h>
#include "insight/malloc_wrappers.h"

INSIGHT_MATRIX_TYPE *
INSIGHT_MATRIX_FUNC(alloc) (const size_t num_rows, const size_t num_cols) {
  size_t elem_count = num_rows * num_cols;
  size_t elem_size = sizeof(INSIGHT_MATRIX_ELEM_TYPE);

  INSIGHT_MATRIX_TYPE * matrix;
  INSIGHT_MATRIX_ELEM_TYPE * elem;

  // TODO(linh): check to make sure that matrix and elem are not NULL before
  // proceeding. And also check for potential oveflow when num_rows * num_cols
  // is too big.
  matrix = insight_malloc(1, sizeof(INSIGHT_MATRIX_TYPE));
  elem = insight_malloc(elem_count, elem_size);

  // Initialize the matrix.
  matrix->num_rows = num_rows;
  matrix->num_cols = num_cols;
  matrix->owner = 1;
  matrix->elem = elem;

  return matrix;
}

INSIGHT_MATRIX_TYPE *
INSIGHT_MATRIX_FUNC(calloc) (const size_t num_rows, const size_t num_cols) {
  size_t elem_count = num_rows * num_cols;
  size_t elem_size = sizeof(INSIGHT_MATRIX_ELEM_TYPE);

  INSIGHT_MATRIX_TYPE * matrix;
  INSIGHT_MATRIX_ELEM_TYPE * elem;

  // TODO(linh): check to make sure that matrix and elem are not NULL before
  // proceeding. And also check for potential oveflow when num_rows * num_cols
  // is too big.
  // Should we rely on calloc or malloc and then memset?
  matrix = insight_malloc(1, sizeof(INSIGHT_MATRIX_TYPE));
  elem = insight_calloc(elem_count, elem_size);

  // Initialize the matrix.
  matrix->num_rows = num_rows;
  matrix->num_cols = num_cols;
  matrix->owner = 1;
  matrix->elem = elem;

  return matrix;
}

void
INSIGHT_MATRIX_FUNC(free) (INSIGHT_MATRIX_TYPE * matrix) {
  if (!matrix) { return; }

  if (matrix->owner) {
    insight_free(matrix->elem);
  }

  insight_free(matrix);
}

void
INSIGHT_MATRIX_FUNC(set_zero) (INSIGHT_MATRIX_TYPE * matrix) {
  if (!matrix) { return; }

  size_t elem_count = matrix->num_rows * matrix->num_cols;
  size_t elem_size = sizeof(INSIGHT_MATRIX_ELEM_TYPE);

  // Initialize matrix to zero.
  memset(matrix->elem, 0, elem_count * elem_size);
}

void
INSIGHT_MATRIX_FUNC(set_identity) (INSIGHT_MATRIX_TYPE * matrix) {
  if (!matrix) { return; }

  size_t i, j;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  INSIGHT_MATRIX_ELEM_TYPE * const elem = matrix->elem;

  const INSIGHT_MATRIX_ELEM_TYPE zero = INSIGHT_MATRIX_ELEM_ZERO;
  const INSIGHT_MATRIX_ELEM_TYPE one = INSIGHT_MATRIX_ELEM_ONE;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      *(elem + (i * num_cols + j)) = ((i == j) ? one : zero);
    }
  }
}

void
INSIGHT_MATRIX_FUNC(set_value) (INSIGHT_MATRIX_TYPE * matrix,
                                INSIGHT_MATRIX_ELEM_TYPE value) {
  if (!matrix) { return; }

  size_t i, j;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  INSIGHT_MATRIX_ELEM_TYPE * const elem = matrix->elem;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      *(elem + (i * num_cols + j)) = value;
    }
  }
}
