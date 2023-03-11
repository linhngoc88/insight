#ifndef INSIGHT_MATRIX_INT_H_
#define INSIGHT_MATRIX_INT_H_

#include <stdlib.h>

typedef struct {
  size_t num_rows;
  size_t num_cols;
  int owner; /* 1 if this matrix owns the elem* or 0 otherwise */
  int * elem;
} insight_matrix_int;

/* Allocation */

// TODO(linh): add docs
insight_matrix_int *
insight_matrix_int_alloc(const size_t num_rows, const size_t num_cols);

// TODO(linh): add docs
insight_matrix_int *
insight_matrix_int_calloc(const size_t num_rows, const size_t num_cols);

// TODO(linh): add docs
void insight_matrix_int_free(insight_matrix_int * matrix);

/* Operations */

// TODO(linh): add docs
void insight_matrix_int_set_zero(insight_matrix_int * matrix);
void insight_matrix_int_set_identity(insight_matrix_int * matrix);
void insight_matrix_int_set_value(insight_matrix_int * matrix, int value);

#endif /* INSIGHT_MATRIX_INT_H_ */
