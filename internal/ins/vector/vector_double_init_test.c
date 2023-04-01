#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_vector.h>

static void alloc_success(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(3);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, v->size);

  ins_vector_free(v);
}

static void calloc_success(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_calloc(3);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, v->size);

  const double expected_mem[] = {0.0, 0.0, 0.0};
  assert_memory_equal(v->data, expected_mem, v->size * sizeof(double));

  ins_vector_free(v);
}

static void test_alloc_from_block_offset_zero_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(3);
  ins_vector *vector = ins_vector_alloc_from_block(block, 0, 3, 1);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 1);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data);
  assert_memory_equal(vector->data, block->data, 3 * sizeof(double));

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_zero_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(5);
  ins_vector *vector = ins_vector_alloc_from_block(block, 0, 3, 2);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 2);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data);

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_one_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(4);
  ins_vector *vector = ins_vector_alloc_from_block(block, 1, 3, 1);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 1);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data + 1);
  assert_memory_equal(vector->data, block->data + 1, 3 * sizeof(double));

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_block_offset_one_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *block = ins_block_alloc(6);
  ins_vector *vector = ins_vector_alloc_from_block(block, 1, 3, 2);

  assert_non_null(vector);
  assert_int_equal(vector->size, 3);
  assert_int_equal(vector->stride, 2);

  assert_non_null(vector->data);
  assert_ptr_equal(vector->data, block->data + 1);

  assert_non_null(vector->block);
  assert_ptr_equal(vector->block, block);
  assert_int_equal(vector->owner, 0);

  ins_vector_free(vector);
  ins_block_free(block);
}

static void test_alloc_from_vector_offset_zero_stride_one(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(3);
  ins_vector *w = ins_vector_alloc_from_vector(v, 0, 3, 1);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data);
  assert_memory_equal(w->data, v->data, 3 * sizeof(double));

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_zero_stride_two(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(5);
  ins_vector *w = ins_vector_alloc_from_vector(v, 0, 3, 2);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_one_stride_one(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(4);
  ins_vector *w = ins_vector_alloc_from_vector(v, 1, 3, 1);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 1);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_vector_offset_two_stride_two(void **state) {
  (void) state; /* unused */

  ins_vector *v = ins_vector_alloc(7);
  ins_vector *w = ins_vector_alloc_from_vector(v, 2, 3, 2);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);

  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 2);

  assert_non_null(w->block);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_alloc_from_block_and_vector(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(15);
  ins_vector *v = ins_vector_alloc_from_block(b, 0, 8, 2);
  ins_vector *w = ins_vector_alloc_from_vector(v, 1, 3, 3);

  assert_non_null(v);
  assert_int_equal(v->size, 8);
  assert_int_equal(v->stride, 2);
  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);
  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_non_null(w);
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 6);
  assert_non_null(w->data);
  assert_ptr_equal(w->data, v->data + 2);
  assert_ptr_equal(w->block, v->block);
  assert_int_equal(w->owner, 0);

  ins_vector_free(w);
  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_zero(void **state) {
  (void) state; /* unused */

  const double expected_mem[] = {0.0, 0.0, 0.0};
  ins_vector *v = ins_vector_alloc(3);
  ins_vector_set_zero(v);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_memory_equal(v->data, expected_mem, 3 * sizeof(double));

  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_init_from_block_stride_one_set_zero(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(4);
  b->data[0] = 1.0;
  b->data[1] = 2.0;
  b->data[2] = 3.0;
  b->data[3] = 4.0;

  // Check b before set zero
  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 3.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);
  ins_vector_set_zero(v);

  // Check v

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(v->data[0], 0.0, 0.0);
  assert_double_equal(v->data[1], 0.0, 0.0);
  assert_double_equal(v->data[2], 0.0, 0.0);

  // Check b
  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 0.0, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_init_from_block_stride_two_set_zero(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(4);
  b->data[0] = 1.0;
  b->data[1] = 2.0;
  b->data[2] = 3.0;
  b->data[3] = 4.0;

  // Check b before set zero
  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 3.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 2, 2);
  ins_vector_set_zero(v);

  // Check v

  assert_non_null(v);
  assert_int_equal(v->size, 2);
  assert_int_equal(v->stride, 2);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  // Check b
  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_all(void **state) {
  (void) state; /* unused */

  const double expected_mem[] = {4.0, 4.0, 4.0};

  ins_vector *v = ins_vector_alloc(3);
  ins_vector_set_all(v, 4.0);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_memory_equal(v->data, expected_mem, 3 * sizeof(double));

  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_init_from_block_stride_one_set_all(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(4);
  b->data[0] = 1.0;
  b->data[1] = 2.0;
  b->data[2] = 3.0;
  b->data[3] = 4.0;

  // Check b before set all
  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 3.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);
  ins_vector_set_all(v, 0.5);

  // Check v

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], 0.5, 0.0);
  assert_double_equal(v->data[2], 0.5, 0.0);

  // Check b
  assert_double_equal(b->data[0], 0.5, 0.0);
  assert_double_equal(b->data[1], 0.5, 0.0);
  assert_double_equal(b->data[2], 0.5, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_init_from_block_stride_two_set_all(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(4);
  b->data[0] = 1.0;
  b->data[1] = 2.0;
  b->data[2] = 3.0;
  b->data[3] = 4.0;

  // Check b before set all
  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 3.0, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 2, 2);
  ins_vector_set_all(v, 0.5);

  // Check v

  assert_non_null(v);
  assert_int_equal(v->size, 2);
  assert_int_equal(v->stride, 2);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  // Check b
  assert_double_equal(b->data[0], 0.5, 0.0);
  assert_double_equal(b->data[1], 2.0, 0.0);
  assert_double_equal(b->data[2], 0.5, 0.0);
  assert_double_equal(b->data[3], 4.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_one(void **state) {
  (void) state; /* unused */

  const double expected_mem[] = {1.0, 0.0, 0.0};

  ins_vector *v = ins_vector_alloc(3);
  ins_vector_set_basis(v, 0);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_memory_equal(v->data, expected_mem, 3 * sizeof(double));

  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_set_basis_two(void **state) {
  (void) state; /* unused */

  const double expected_mem[] = {0.0, 1.0, 0.0};

  ins_vector *v = ins_vector_alloc(3);
  ins_vector_set_basis(v, 1);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_memory_equal(v->data, expected_mem, 3 * sizeof(double));

  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_set_basis_three(void **state) {
  (void) state; /* unused */

  const double expected_mem[] = {0.0, 0.0, 1.0};

  ins_vector *v = ins_vector_alloc(3);
  ins_vector_set_basis(v, 2);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_memory_equal(v->data, expected_mem, 3 * sizeof(double));

  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_set_basis_one_from_block_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);
  ins_vector_set_basis(v, 0);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 0.0, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 4.5, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_two_from_block_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);
  ins_vector_set_basis(v, 1);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 1.0, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 4.5, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_three_from_block_stride_one(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);
  ins_vector_set_basis(v, 2);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 0.0, 0.0);
  assert_double_equal(b->data[2], 1.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 4.5, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_one_from_block_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 2);
  ins_vector_set_basis(v, 0);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 1.5, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 0.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_two_from_block_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 2);
  ins_vector_set_basis(v, 1);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 1.5, 0.0);
  assert_double_equal(b->data[2], 1.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 0.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_set_basis_three_from_block_stride_two(void **state) {
  (void) state; /* unused */

  ins_block *b = ins_block_alloc(5);
  b->data[0] = 0.5;
  b->data[1] = 1.5;
  b->data[2] = 2.5;
  b->data[3] = 3.5;
  b->data[4] = 4.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 2);
  ins_vector_set_basis(v, 2);

  assert_non_null(v);
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);

  assert_non_null(v->data);
  assert_ptr_equal(v->data, b->data);

  assert_non_null(v->block);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 1.5, 0.0);
  assert_double_equal(b->data[2], 0.0, 0.0);
  assert_double_equal(b->data[3], 3.5, 0.0);
  assert_double_equal(b->data[4], 1.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_vector_get_when_stride_is_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  v->data[0] = 0.5;
  v->data[1] = -2.5;
  v->data[2] = 4.0;

  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), -2.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 4.0, 0.0);

  assert_double_equal(ins_vector_get(v, 1 - 1), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2 - 1), -2.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1 + 1), 4.0, 0.0);

  assert_double_equal(ins_vector_get(v + 1 - 1, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get((v + 1) - 1, 1), -2.5, 0.0);
  assert_double_equal(ins_vector_get(v + 2 - 2, 2), 4.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_get_when_stride_is_greater_than_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(7);

  v->data[0] = 1.0;
  v->data[1] = 2.0;
  v->data[2] = 3.0;
  v->data[3] = 4.0;
  v->data[4] = 5.0;
  v->data[5] = 6.0;
  v->data[6] = 7.0;

  v->stride = 2;

  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 3.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 5.0, 0.0);
  assert_double_equal(ins_vector_get(v, 3), 7.0, 0.0);

  v->stride = 3;

  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 7.0, 0.0);

  ins_vector_free(v);
}

static void test_vector_set_when_stride_is_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  ins_vector_set(v, 0, 1.5);
  ins_vector_set(v, 1, 2.4);
  ins_vector_set(v, 2, 0.2);

  assert_double_equal(v->data[0], 1.5, 0.0);
  assert_double_equal(v->data[1], 2.4, 0.0);
  assert_double_equal(v->data[2], 0.2, 0.0);

  assert_double_equal(ins_vector_get(v, 0), 1.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 2.4, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 0.2, 0.0);

  ins_vector_set(v, 1 - 1, 0.25 * 2);
  ins_vector_set(v, 2 - 1, 1.0 - 2.5);
  ins_vector_set(v, 1 + 1, 0.5 + 1.0);

  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], -1.5, 0.0);
  assert_double_equal(v->data[2], 1.5, 0.0);

  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), -1.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 1.5, 0.0);

  const size_t i = 1;
  ins_vector *w = v;
  ins_vector_set(w, i, 12.5);

  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], 12.5, 0.0);
  assert_double_equal(v->data[2], 1.5, 0.0);

  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 12.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 1.5, 0.0);

  assert_double_equal(w->data[0], 0.5, 0.0);
  assert_double_equal(w->data[1], 12.5, 0.0);
  assert_double_equal(w->data[2], 1.5, 0.0);

  assert_double_equal(ins_vector_get(w, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(w, 1), 12.5, 0.0);
  assert_double_equal(ins_vector_get(w, 2), 1.5, 0.0);

  ins_vector_free(v);
}

static void test_vector_set_when_stride_is_greater_than_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(7);

  // stride = 1
  ins_vector_set(v, 0, 1.0);
  ins_vector_set(v, 1, 2.0);
  ins_vector_set(v, 2, 3.0);
  ins_vector_set(v, 3, 4.0);
  ins_vector_set(v, 4, 5.0);
  ins_vector_set(v, 5, 6.0);
  ins_vector_set(v, 6, 7.0);

  // stride = 2, size = 4
  v->stride = 2;

  ins_vector_set(v, 0, 0.5);
  ins_vector_set(v, 1, 1.5);
  ins_vector_set(v, 2, 2.5);
  ins_vector_set(v, 3, 3.5);

  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], 2.0, 0.0);
  assert_double_equal(v->data[2], 1.5, 0.0);
  assert_double_equal(v->data[3], 4.0, 0.0);
  assert_double_equal(v->data[4], 2.5, 0.0);
  assert_double_equal(v->data[5], 6.0, 0.0);
  assert_double_equal(v->data[6], 3.5, 0.0);

  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 1.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 2.5, 0.0);
  assert_double_equal(ins_vector_get(v, 3), 3.5, 0.0);

  // stride = 3, size = 3
  v->stride = 3;

  ins_vector_set(v, 0, -2.4);
  ins_vector_set(v, 1, 0.0);
  ins_vector_set(v, 2, 1.6);

  assert_double_equal(v->data[0], -2.4, 0.0);
  assert_double_equal(v->data[1], 2.0, 0.0);
  assert_double_equal(v->data[2], 1.5, 0.0);
  assert_double_equal(v->data[3], 0.0, 0.0);
  assert_double_equal(v->data[4], 2.5, 0.0);
  assert_double_equal(v->data[5], 6.0, 0.0);
  assert_double_equal(v->data[6], 1.6, 0.0);

  assert_double_equal(ins_vector_get(v, 0), -2.4, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 0.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 1.6, 0.0);

  ins_vector_free(v);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success),
    cmocka_unit_test(calloc_success),
    cmocka_unit_test(test_alloc_from_block_offset_zero_stride_one),
    cmocka_unit_test(test_alloc_from_block_offset_zero_stride_two),
    cmocka_unit_test(test_alloc_from_block_offset_one_stride_one),
    cmocka_unit_test(test_alloc_from_block_offset_one_stride_two),
    cmocka_unit_test(test_alloc_from_vector_offset_zero_stride_one),
    cmocka_unit_test(test_alloc_from_vector_offset_zero_stride_two),
    cmocka_unit_test(test_alloc_from_vector_offset_one_stride_one),
    cmocka_unit_test(test_alloc_from_vector_offset_two_stride_two),
    cmocka_unit_test(test_alloc_from_block_and_vector),
    cmocka_unit_test(test_set_zero),
    cmocka_unit_test(test_init_from_block_stride_one_set_zero),
    cmocka_unit_test(test_init_from_block_stride_two_set_zero),
    cmocka_unit_test(test_set_all),
    cmocka_unit_test(test_init_from_block_stride_one_set_all),
    cmocka_unit_test(test_init_from_block_stride_two_set_all),
    cmocka_unit_test(test_set_basis_one),
    cmocka_unit_test(test_set_basis_two),
    cmocka_unit_test(test_set_basis_three),
    cmocka_unit_test(test_set_basis_one_from_block_stride_one),
    cmocka_unit_test(test_set_basis_two_from_block_stride_one),
    cmocka_unit_test(test_set_basis_three_from_block_stride_one),
    cmocka_unit_test(test_set_basis_one_from_block_stride_two),
    cmocka_unit_test(test_set_basis_two_from_block_stride_two),
    cmocka_unit_test(test_set_basis_three_from_block_stride_two),
    cmocka_unit_test(test_vector_get_when_stride_is_one),
    cmocka_unit_test(test_vector_get_when_stride_is_greater_than_one),
    cmocka_unit_test(test_vector_set_when_stride_is_one),
    cmocka_unit_test(test_vector_set_when_stride_is_greater_than_one)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
