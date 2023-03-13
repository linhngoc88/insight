#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "ins/ins_matrix.h"


static void alloc_success(void** state) {
  (void) state; /* unused */

  ins_matrix_int* matrix = ins_matrix_int_alloc(2, 3);

  assert_non_null(matrix);
  assert_non_null(matrix->elem);

  assert_int_equal(matrix->num_rows, 2);
  assert_int_equal(matrix->num_cols, 3);
  assert_true(matrix->owner);

  ins_matrix_int_free(matrix);
}

static void calloc_success(void** state) {
  (void) state; /* unused */

  ins_matrix_int* matrix = ins_matrix_int_calloc(2, 3);

  assert_non_null(matrix);
  assert_non_null(matrix->elem);

  assert_int_equal(matrix->num_rows, 2);
  assert_int_equal(matrix->num_cols, 3);
  assert_true(matrix->owner);

  size_t i, j;
  const int* elem = matrix->elem;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  const int zero = 0;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      assert_int_equal(*(elem + (i * num_cols + j)), zero);
    }
  }

  ins_matrix_int_free(matrix);
}

static void set_zero_success(void** state) {
  (void) state; /* unused */

  ins_matrix_int* matrix = ins_matrix_int_alloc(2, 3);
  ins_matrix_int_set_zero(matrix);

  size_t i, j;
  const int* elem = matrix->elem;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;
  const int zero = 0;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      assert_int_equal(*(elem + (i * num_cols + j)), zero);
    }
  }

  const int expected_elem[] = {
    0, 0, 0,
    0, 0, 0
  };

  assert_memory_equal(elem, expected_elem, num_rows * num_cols * sizeof(int));
  ins_matrix_int_free(matrix);
}

static void set_identity_success(void** state) {
  (void) state; /* unused */

  ins_matrix_int* matrix = ins_matrix_int_alloc(2, 2);
  ins_matrix_int_set_identity(matrix);

  const int* elem = matrix->elem;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;

  const int zero = 0;
  const int one = 1;

  size_t i, j;
  int elem_ij;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      elem_ij = *(elem + (i * num_cols + j));
      if (i == j) {
        assert_int_equal(elem_ij, one);
      } else {
        assert_int_equal(elem_ij, zero);
      }
    }
  }

  const int expected_elem[] = {
    1, 0,
    0, 1
  };

  assert_memory_equal(elem, expected_elem, num_rows * num_cols * sizeof(int));
  ins_matrix_int_free(matrix);
}

static void set_value_success(void** state) {
  (void) state; /* unused */

  ins_matrix_int* matrix = ins_matrix_int_alloc(2, 3);
  ins_matrix_int_set_value(matrix, 4);

  const int * elem = matrix->elem;
  const size_t num_rows = matrix->num_rows;
  const size_t num_cols = matrix->num_cols;

  size_t i, j;
  int elem_ij;

  for (i = 0; i < num_rows; ++i) {
    for (j = 0; j < num_cols; ++j) {
      elem_ij = *(elem + (i * num_cols + j));
      assert_int_equal(elem_ij, 4);
    }
  }

  const int expected_elem[] = {
    4, 4, 4,
    4, 4, 4
  };

  assert_memory_equal(elem, expected_elem, num_rows * num_cols * sizeof(int));
  ins_matrix_int_free(matrix);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success),
    cmocka_unit_test(calloc_success),
    cmocka_unit_test(set_zero_success),
    cmocka_unit_test(set_identity_success),
    cmocka_unit_test(set_value_success)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
