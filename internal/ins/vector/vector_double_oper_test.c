#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_vector.h>

static void test_scale_when_stride_is_one(void **state) {
  (void) state;

  ins_vector * v = ins_vector_alloc(3);

  v->data[0] = 0.5;
  v->data[1] = 1.5;
  v->data[2] = 2.5;

  ins_vector_scale(v, 2.0);

  // Check scaling result
  assert_double_equal(v->data[0], 1.0, 0.0);
  assert_double_equal(v->data[1], 3.0, 0.0);
  assert_double_equal(v->data[2], 5.0, 0.0);

  // Check vector state after scaling
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, v->size);

  ins_vector_free(v);
}

static void test_scale_when_alloc_from_block_stride_one(void **state) {
  (void) state;

  // Initialize block
  ins_block *b = ins_block_alloc(4);
  b->data[0] = 1.0;
  b->data[1] = -1.5;
  b->data[2] = 2.25;
  b->data[3] = 3.0;

  // Initialize vector from block
  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 1);

  // Check vector data before scaling.
  assert_double_equal(v->data[0], 1.0, 0.0);
  assert_double_equal(v->data[1], -1.5, 0.0);
  assert_double_equal(v->data[2], 2.25, 0.0);

  ins_vector_scale(v, 2.0);

  // Check vector data after scaling.
  assert_double_equal(v->data[0], 2.0, 0.0);
  assert_double_equal(v->data[1], -3.0, 0.0);
  assert_double_equal(v->data[2], 4.5, 0.0);

  // Check block data after scaling.
  assert_double_equal(b->data[0], 2.0, 0.0);
  assert_double_equal(b->data[1], -3.0, 0.0);
  assert_double_equal(b->data[2], 4.5, 0.0);
  assert_double_equal(b->data[3], 3.0, 0.0);

  // Check vector & block states after scaling.
  assert_int_equal(v->size, 3);
  assert_int_equal(b->size, 4);
  assert_int_equal(v->stride, 1);
  assert_ptr_equal(v->data, b->data);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_scale_when_alloc_from_block_stride_two(void **state) {
  (void) state;

  // Initialize the block
  ins_block *b = ins_block_alloc(7);

  b->data[0] = 1.0;
  b->data[1] = 2.0;
  b->data[2] = 3.0;
  b->data[3] = 4.0;
  b->data[4] = 5.0;
  b->data[5] = 6.0;
  b->data[6] = 7.0;

  ins_vector *v = ins_vector_alloc_from_block(b, 1, 3, 2);

  // Check vector data before scaling
  assert_double_equal(ins_vector_get(v, 0), 2.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 6.0, 0.0);

  ins_vector_scale(v, 0.5);

  // Check vector data after scaling.
  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 3.0, 0.0);

  // Check block data after scaling.
  assert_double_equal(b->data[0], 1.0, 0.0);
  assert_double_equal(b->data[1], 1.0, 0.0);
  assert_double_equal(b->data[2], 3.0, 0.0);
  assert_double_equal(b->data[3], 2.0, 0.0);
  assert_double_equal(b->data[4], 5.0, 0.0);
  assert_double_equal(b->data[5], 3.0, 0.0);
  assert_double_equal(b->data[6], 7.0, 0.0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_vector_add_two_vectors_same_stride_one(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  ins_vector *y = ins_vector_alloc(3);

  ins_vector_set(x, 0, 0.5);
  ins_vector_set(x, 1, 2.0);
  ins_vector_set(x, 2, -1.5);

  ins_vector_set(y, 0, 1.0);
  ins_vector_set(y, 1, 0.4);
  ins_vector_set(y, 2, 0.5);

  const int ret  = ins_vector_add(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x data
  assert_double_equal(ins_vector_get(x, 0), 1.5, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 2.4, 0.0);
  assert_double_equal(ins_vector_get(x, 2), -1.0, 0.0);

  // Check y data
  assert_double_equal(ins_vector_get(y, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 0.4, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.5, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_add_two_vectors_same_stride_two(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(5);

  x->data[0] = 0.05;
  x->data[1] = -1.0;
  x->data[2] = 1.0;
  x->data[3] = 4.5;
  x->data[4] = 6.0;

  x->size = 3;
  x->stride = 2;

  ins_vector *y = ins_vector_alloc(5);

  y->data[0] = 1.2;
  y->data[1] = 0.0;
  y->data[2] = 2.0;
  y->data[3] = 0.5;
  y->data[4] = 0.75;


  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_add(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x elements
  assert_double_equal(ins_vector_get(x, 0), 1.25, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 3.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 6.75, 0.0);

  // Check y elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.75, 0.0);

  // Check x data
  assert_double_equal(x->data[0], 1.25, 0.0);
  assert_double_equal(x->data[1], -1.0, 0.0);
  assert_double_equal(x->data[2], 3.0, 0.0);
  assert_double_equal(x->data[3], 4.5, 0.0);
  assert_double_equal(x->data[4], 6.75, 0.0);

  // Check y data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 0.0, 0.0);
  assert_double_equal(y->data[2], 2.0, 0.0);
  assert_double_equal(y->data[3], 0.5, 0.0);
  assert_double_equal(y->data[4], 0.75, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_add_two_vectors_different_strides(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 0.5;
  x->data[1] = 2.0;
  x->data[2] = -1.5;

  ins_vector *y = ins_vector_alloc(5);
  y->data[0] = 1.2;
  y->data[1] = 0.0;
  y->data[2] = 2.0;
  y->data[3] = 0.5;
  y->data[4] = 0.75;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_add(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x elements
  assert_double_equal(ins_vector_get(x, 0), 1.7, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), -0.75, 0.0);

  // Check y elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.75, 0.0);

  // Check x data
  assert_double_equal(x->data[0], 1.7, 0.0);
  assert_double_equal(x->data[1], 4.0, 0.0);
  assert_double_equal(x->data[2], -0.75, 0.0);

  // Check y data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 0.0, 0.0);
  assert_double_equal(y->data[2], 2.0, 0.0);
  assert_double_equal(y->data[3], 0.5, 0.0);
  assert_double_equal(y->data[4], 0.75, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_add_two_vectors_different_lengths(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  ins_vector *y = ins_vector_alloc(2);

  ins_set_error_handler_off();

  const int ret = ins_vector_add(x, y);
  assert_int_equal(ret, INS_EBADLEN);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_sub_same_stride_one(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 1.0;
  x->data[1] = 4.5;
  x->data[2] = -0.25;

  ins_vector *y = ins_vector_alloc(3);
  y->data[0] = 0.5;
  y->data[1] = 2.0;
  y->data[2] = 0.25;

  const int ret = ins_vector_sub(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements.
  assert_double_equal(ins_vector_get(x, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 2.5, 0.0);
  assert_double_equal(ins_vector_get(x, 2), -0.5, 0.0);

  // Check y's elements.
  assert_double_equal(ins_vector_get(y, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.25, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_sub_same_stride_two(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(7);
  x->data[0] = 0.25;
  x->data[1] = 1.7;
  x->data[2] = 1.4;
  x->data[3] = 12.35;
  x->data[4] = -0.25;
  x->data[5] = 2.15;
  x->data[6] = 3.14;

  x->size = 3;
  x->stride = 2;

  ins_vector *y = ins_vector_alloc(7);
  y->data[0] = 2.0;
  y->data[1] = -0.12;
  y->data[2] = 0.2;
  y->data[3] = 0.35;
  y->data[4] = -4.0;
  y->data[5] = 1.25;
  y->data[6] = 0.14;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_sub(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), -1.75, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 1.2, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 3.75, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 0.2, 0.0);
  assert_double_equal(ins_vector_get(y, 2), -4.0, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], -1.75, 0.0);
  assert_double_equal(x->data[1], 1.7, 0.0);
  assert_double_equal(x->data[2], 1.2, 0.0);
  assert_double_equal(x->data[3], 12.35, 0.0);
  assert_double_equal(x->data[4], 3.75, 0.0);
  assert_double_equal(x->data[5], 2.15, 0.0);
  assert_double_equal(x->data[6], 3.14, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 2.0, 0.0);
  assert_double_equal(y->data[1], -0.12, 0.0);
  assert_double_equal(y->data[2], 0.2, 0.0);
  assert_double_equal(y->data[3], 0.35, 0.0);
  assert_double_equal(y->data[4], -4.0, 0.0);
  assert_double_equal(y->data[5], 1.25, 0.0);
  assert_double_equal(y->data[6], 0.14, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_sub_different_strides(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 0.5;
  x->data[1] = 2.0;
  x->data[2] = -1.5;

  ins_vector *y = ins_vector_alloc(5);
  y->data[0] = 1.2;
  y->data[1] = 0.0;
  y->data[2] = 2.0;
  y->data[3] = 0.5;
  y->data[4] = 0.75;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_sub(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), -0.7, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 0.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), -2.25, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.75, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], -0.7, 0.0);
  assert_double_equal(x->data[1], 0.0, 0.0);
  assert_double_equal(x->data[2], -2.25, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 0.0, 0.0);
  assert_double_equal(y->data[2], 2.0, 0.0);
  assert_double_equal(y->data[3], 0.5, 0.0);
  assert_double_equal(y->data[4], 0.75, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_sub_different_lengths(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  ins_vector *y = ins_vector_alloc(2);

  ins_set_error_handler_off();

  const int ret = ins_vector_sub(x, y);
  assert_int_equal(ret, INS_EBADLEN);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_mul_same_stride_one(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 2.0;
  x->data[1] = 2.5;
  x->data[2] = 5.0;

  ins_vector *y = ins_vector_alloc(3);
  y->data[0] = 0.5;
  y->data[1] = 2.0;
  y->data[2] = 0.4;

  const int ret = ins_vector_mul(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 5.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 2.0, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.4, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_mul_same_stride_two(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(7);
  x->data[0] = 2.0;
  x->data[1] = 0.5;
  x->data[2] = 2.5;
  x->data[3] = 4.0;
  x->data[4] = 5.0;
  x->data[5] = -3.15;
  x->data[6] = -0.75;

  x->size = 3;
  x->stride = 2;

  ins_vector *y = ins_vector_alloc(7);
  y->data[0] = 0.5;
  y->data[1] = -0.12;
  y->data[2] = 2.0;
  y->data[3] = 1.25;
  y->data[4] = 0.4;
  y->data[5] = 3.14;
  y->data[6] = 0.0;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_mul(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 5.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 2.0, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.4, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 1.0, 0.0);
  assert_double_equal(x->data[1], 0.5, 0.0);
  assert_double_equal(x->data[2], 5.0, 0.0);
  assert_double_equal(x->data[3], 4.0, 0.0);
  assert_double_equal(x->data[4], 2.0, 0.0);
  assert_double_equal(x->data[5], -3.15, 0.0);
  assert_double_equal(x->data[6], -0.75, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 0.5, 0.0);
  assert_double_equal(y->data[1], -0.12, 0.0);
  assert_double_equal(y->data[2], 2.0, 0.0);
  assert_double_equal(y->data[3], 1.25, 0.0);
  assert_double_equal(y->data[4], 0.4, 0.0);
  assert_double_equal(y->data[5], 3.14, 0.0);
  assert_double_equal(y->data[6], 0.0, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_mul_different_strides(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 2.0;
  x->data[1] = 2.5;
  x->data[2] = 5.0;

  ins_vector *y = ins_vector_alloc(7);
  y->data[0] = 0.5;
  y->data[1] = -0.12;
  y->data[2] = 2.0;
  y->data[3] = 1.25;
  y->data[4] = 0.4;
  y->data[5] = 3.14;
  y->data[6] = 0.0;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_mul(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 5.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 2.0, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.4, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 1.0, 0.0);
  assert_double_equal(x->data[1], 5.0, 0.0);
  assert_double_equal(x->data[2], 2.0, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 0.5, 0.0);
  assert_double_equal(y->data[1], -0.12, 0.0);
  assert_double_equal(y->data[2], 2.0, 0.0);
  assert_double_equal(y->data[3], 1.25, 0.0);
  assert_double_equal(y->data[4], 0.4, 0.0);
  assert_double_equal(y->data[5], 3.14, 0.0);
  assert_double_equal(y->data[6], 0.0, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_mul_different_lengths(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(2);
  ins_vector *y = ins_vector_alloc(3);

  ins_set_error_handler_off();

  assert_int_equal(ins_vector_mul(x, y), INS_EBADLEN);
  assert_int_equal(ins_vector_mul(y, x), INS_EBADLEN);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_div_same_stride_one(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 0.0;
  x->data[1] = 1.6;
  x->data[2] = 0.3;

  ins_vector *y = ins_vector_alloc(3);
  y->data[0] = 1.0;
  y->data[1] = 4.0;
  y->data[2] = 0.2;

  const int ret = ins_vector_div(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 0.4, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 1.5, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.2, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_div_same_stride_two(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(5);
  x->data[0] = 0.0;
  x->data[1] = 0.5;
  x->data[2] = 1.6;
  x->data[3] = 0.2;
  x->data[4] = 0.3;

  x->size = 3;
  x->stride = 2;

  ins_vector *y = ins_vector_alloc(5);
  y->data[0] = 1.0;
  y->data[1] = 0.0;
  y->data[2] = 4.0;
  y->data[3] = 1.2;
  y->data[4] = 0.2;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_div(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 0.4, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 1.5, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.2, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 0.0, 0.0);
  assert_double_equal(x->data[1], 0.5, 0.0);
  assert_double_equal(x->data[2], 0.4, 0.0);
  assert_double_equal(x->data[3], 0.2, 0.0);
  assert_double_equal(x->data[4], 1.5, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.0, 0.0);
  assert_double_equal(y->data[1], 0.0, 0.0);
  assert_double_equal(y->data[2], 4.0, 0.0);
  assert_double_equal(y->data[3], 1.2, 0.0);
  assert_double_equal(y->data[4], 0.2, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_div_different_strides(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 0.0;
  x->data[1] = 1.6;
  x->data[2] = 0.3;

  ins_vector *y = ins_vector_alloc(5);
  y->data[0] = 1.0;
  y->data[1] = 0.0;
  y->data[2] = 4.0;
  y->data[3] = 1.2;
  y->data[4] = 0.2;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_div(x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.0, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 0.4, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 1.5, 0.0);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 0.2, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 0.0, 0.0);
  assert_double_equal(x->data[1], 0.4, 0.0);
  assert_double_equal(x->data[2], 1.5, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.0, 0.0);
  assert_double_equal(y->data[1], 0.0, 0.0);
  assert_double_equal(y->data[2], 4.0, 0.0);
  assert_double_equal(y->data[3], 1.2, 0.0);
  assert_double_equal(y->data[4], 0.2, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_div_different_lengths(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(2);
  ins_vector *y = ins_vector_alloc(3);

  ins_set_error_handler_off();

  assert_int_equal(ins_vector_div(x, y), INS_EBADLEN);
  assert_int_equal(ins_vector_div(y, x), INS_EBADLEN);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_add_constant_stride_one(void **state) {
  (void) state;

  ins_block *b = ins_block_calloc(5);
  b->data[1] = 0.5;
  b->data[2] = 4.0;
  b->data[3] = 2.5;

  ins_vector *v = ins_vector_alloc_from_block(b, 1, 3, 1);

  // Check v's elements before adding a constant.
  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 2.5, 0.0);

  const int ret = ins_vector_add_constant(v, 0.5);
  assert_int_equal(ret, INS_SUCCESS);

  // Check v's elements after adding a constant.
  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 3.0, 0.0);

  // Check b's data
  assert_double_equal(b->data[0], 0.0, 0.0);
  assert_double_equal(b->data[1], 1.0, 0.0);
  assert_double_equal(b->data[2], 4.5, 0.0);
  assert_double_equal(b->data[3], 3.0, 0.0);
  assert_double_equal(b->data[4], 0.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_ptr_equal(v->data, b->data + 1);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_vector_add_constant_stride_two(void **state) {
  (void) state;

  ins_block *b = ins_block_calloc(5);
  b->data[0] = 1.0;
  b->data[1] = 0.5;
  b->data[2] = 4.0;
  b->data[3] = 2.5;
  b->data[4] = 9.0;

  ins_vector *v = ins_vector_alloc_from_block(b, 0, 3, 2);

  // Check v's elements before adding a constant.
  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 9.0, 0.0);

  const int ret = ins_vector_add_constant(v, 0.5);
  assert_int_equal(ret, INS_SUCCESS);

  // Check v's elements after adding a constant.
  assert_double_equal(ins_vector_get(v, 0), 1.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 9.5, 0.0);

  // Check b's data
  assert_double_equal(b->data[0], 1.5, 0.0);
  assert_double_equal(b->data[1], 0.5, 0.0);
  assert_double_equal(b->data[2], 4.5, 0.0);
  assert_double_equal(b->data[3], 2.5, 0.0);
  assert_double_equal(b->data[4], 9.5, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);
  assert_ptr_equal(v->data, b->data);
  assert_ptr_equal(v->block, b);
  assert_int_equal(v->owner, 0);

  ins_vector_free(v);
  ins_block_free(b);
}

static void test_vector_sum_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  v->data[0] = 1.0;
  v->data[1] = 3.5;
  v->data[2] = 4.0;

  const double sum = ins_vector_sum(v);
  assert_double_equal(sum, 8.5, 0.0);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 3.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 4.0, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 1.0, 0.0);
  assert_double_equal(v->data[1], 3.5, 0.0);
  assert_double_equal(v->data[2], 4.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_vector_sum_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(5);

  v->data[0] = 1.0;
  v->data[1] = 3.5;
  v->data[2] = 4.0;
  v->data[3] = 1.25;
  v->data[4] = 0.75;

  v->size = 3;
  v->stride = 2;

  const double sum = ins_vector_sum(v);
  assert_double_equal(sum, 5.75, 0.0);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), 1.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 4.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 0.75, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 1.0, 0.0);
  assert_double_equal(v->data[1], 3.5, 0.0);
  assert_double_equal(v->data[2], 4.0, 0.0);
  assert_double_equal(v->data[3], 1.25, 0.0);
  assert_double_equal(v->data[4], 0.75, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);
  assert_non_null(v->data);
  assert_non_null(v->block);
  assert_int_equal(v->owner, 1);

  ins_vector_free(v);
}

static void test_vector_axpy_same_length_same_stride_one(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  x->data[0] = 0.5;
  x->data[1] = 1.0;
  x->data[2] = 4.0;

  ins_vector *y = ins_vector_alloc(3);
  y->data[0] = 0.2;
  y->data[1] = 1.5;
  y->data[2] = 2.0;

  const int ret = ins_vector_axpy(2.0, x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 3.5, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 10.0, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 3.5, 0.0);
  assert_double_equal(y->data[2], 10.0, 0.0);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 4.0, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 0.5, 0.0);
  assert_double_equal(x->data[1], 1.0, 0.0);
  assert_double_equal(x->data[2], 4.0, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_axpy_same_length_same_stride_two(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(5);

  x->data[0] = 0.5;
  x->data[1] = -1.0;
  x->data[2] = 1.0;
  x->data[3] = 0.0;
  x->data[4] = 4.0;

  x->size = 3;
  x->stride = 2;

  ins_vector *y = ins_vector_alloc(5);

  y->data[0] = 0.2;
  y->data[1] = 0.8;
  y->data[2] = 1.5;
  y->data[3] = 1.2;
  y->data[4] = 2.0;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_axpy(2.0, x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 3.5, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 10.0, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 0.8, 0.0);
  assert_double_equal(y->data[2], 3.5, 0.0);
  assert_double_equal(y->data[3], 1.2, 0.0);
  assert_double_equal(y->data[4], 10.0, 0.0);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 4.0, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 0.5, 0.0);
  assert_double_equal(x->data[1], -1.0, 0.0);
  assert_double_equal(x->data[2], 1.0, 0.0);
  assert_double_equal(x->data[3], 0.0, 0.0);
  assert_double_equal(x->data[4], 4.0, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_axpy_same_length_diff_strides(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);

  x->data[0] = 0.5;
  x->data[1] = 1.0;
  x->data[2] = 4.0;

  ins_vector *y = ins_vector_alloc(5);

  y->data[0] = 0.2;
  y->data[1] = 0.8;
  y->data[2] = 1.5;
  y->data[3] = 3.0;
  y->data[4] = 2.0;

  y->size = 3;
  y->stride = 2;

  const int ret = ins_vector_axpy(2.0, x, y);
  assert_int_equal(ret, INS_SUCCESS);

  // Check y's elements
  assert_double_equal(ins_vector_get(y, 0), 1.2, 0.0);
  assert_double_equal(ins_vector_get(y, 1), 3.5, 0.0);
  assert_double_equal(ins_vector_get(y, 2), 10.0, 0.0);

  // Check y's data
  assert_double_equal(y->data[0], 1.2, 0.0);
  assert_double_equal(y->data[1], 0.8, 0.0);
  assert_double_equal(y->data[2], 3.5, 0.0);
  assert_double_equal(y->data[3], 3.0, 0.0);
  assert_double_equal(y->data[4], 10.0, 0.0);

  // Check x's elements
  assert_double_equal(ins_vector_get(x, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(x, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(x, 2), 4.0, 0.0);

  // Check x's data
  assert_double_equal(x->data[0], 0.5, 0.0);
  assert_double_equal(x->data[1], 1.0, 0.0);
  assert_double_equal(x->data[2], 4.0, 0.0);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_axpy_diff_lengths(void **state) {
  (void) state;

  ins_vector *x = ins_vector_alloc(3);
  ins_vector *y = ins_vector_alloc(2);

  ins_set_error_handler_off();

  assert_int_equal(ins_vector_axpy(0.5, x, y), INS_EBADLEN);
  assert_int_equal(ins_vector_axpy(0.5, y, x), INS_EBADLEN);

  ins_vector_free(y);
  ins_vector_free(x);
}

static void test_vector_swap_same_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  double * const v_data = v->data;

  v_data[0] = 0.5;
  v_data[1] = 2.0;
  v_data[2] = 4.5;

  ins_vector *w = ins_vector_alloc(3);
  double * const w_data = w->data;

  w_data[0] = -1.2;
  w_data[1] = 0.25;
  w_data[2] = 8.0;

  const int status = ins_vector_swap(v, w);
  assert_int_equal(status, INS_SUCCESS);

  // Check v's data
  assert_double_equal(v->data[0], -1.2, 0.0);
  assert_double_equal(v->data[1], 0.25, 0.0);
  assert_double_equal(v->data[2], 8.0, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 0.5, 0.0);
  assert_double_equal(w->data[1], 2.0, 0.0);
  assert_double_equal(w->data[2], 4.5, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_ptr_equal(v->data, v_data);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v_data);
  assert_int_equal(v->owner, 1);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);
  assert_ptr_equal(w->data, w_data);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w_data);
  assert_int_equal(w->owner, 1);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_swap_same_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(5);
  double * const v_data = v->data;

  v->size = 3;
  v->stride = 2;

  v_data[0] = 0.5;
  v_data[1] = 1.0;
  v_data[2] = 2.0;
  v_data[3] = -2.5;
  v_data[4] = 4.5;

  ins_vector *w = ins_vector_alloc(5);
  double * const w_data = w->data;

  w->size = 3;
  w->stride = 2;

  w_data[0] = -1.2;
  w_data[1] = 0.0;
  w_data[2] = 0.25;
  w_data[3] = 4.0;
  w_data[4] = 8.0;

  const int status = ins_vector_swap(v, w);
  assert_int_equal(status, INS_SUCCESS);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), -1.2, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 0.25, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 8.0, 0.0);

  // Check w's elements
  assert_double_equal(ins_vector_get(w, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(w, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(w, 2), 4.5, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], -1.2, 0.0);
  assert_double_equal(v->data[1], 1.0, 0.0);
  assert_double_equal(v->data[2], 0.25, 0.0);
  assert_double_equal(v->data[3], -2.5, 0.0);
  assert_double_equal(v->data[4], 8.0, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 0.5, 0.0);
  assert_double_equal(w->data[1], 0.0, 0.0);
  assert_double_equal(w->data[2], 2.0, 0.0);
  assert_double_equal(w->data[3], 4.0, 0.0);
  assert_double_equal(w->data[4], 4.5, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);
  assert_ptr_equal(v->data, v_data);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v_data);
  assert_int_equal(v->owner, 1);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);
  assert_ptr_equal(w->data, w_data);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w_data);
  assert_int_equal(w->owner, 1);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_swap_diff_strides(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  double * const v_data = v->data;

  v_data[0] = 0.5;
  v_data[1] = 2.0;
  v_data[2] = 4.5;

  ins_vector *w = ins_vector_alloc(5);
  double * const w_data = w->data;

  w->size = 3;
  w->stride = 2;

  w_data[0] = -1.2;
  w_data[1] = 1.4;
  w_data[2] = 0.25;
  w_data[3] = 2.5;
  w_data[4] = 8.0;

  const int status = ins_vector_swap(v, w);
  assert_int_equal(status, INS_SUCCESS);

  // Check v's data & elements
  assert_double_equal(v->data[0], -1.2, 0.0);
  assert_double_equal(v->data[1], 0.25, 0.0);
  assert_double_equal(v->data[2], 8.0, 0.0);

  // Check w's elements
  assert_double_equal(ins_vector_get(w, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(w, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(w, 2), 4.5, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 0.5, 0.0);
  assert_double_equal(w->data[1], 1.4, 0.0);
  assert_double_equal(w->data[2], 2.0, 0.0);
  assert_double_equal(w->data[3], 2.5, 0.0);
  assert_double_equal(w->data[4], 4.5, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_ptr_equal(v->data, v_data);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v_data);
  assert_int_equal(v->owner, 1);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);
  assert_ptr_equal(w->data, w_data);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w_data);
  assert_int_equal(w->owner, 1);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_swap_diff_lengths(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  ins_vector *w = ins_vector_alloc(5);

  ins_set_error_handler_off();

  assert_int_equal(ins_vector_swap(v, w), INS_EINVAL);
  assert_int_equal(ins_vector_swap(w, v), INS_EINVAL);

  ins_vector_free(v);
  ins_vector_free(w);
}

static void test_vector_copy_same_stride_one(void **state) {
  (void) state;

  ins_vector *src = ins_vector_alloc(3);

  src->data[0] = 2.5;
  src->data[1] = 1.0;
  src->data[2] = 4.0;

  const double *src_data = src->data;
  const ins_block *src_block = src->block;

  ins_vector *dst = ins_vector_alloc(3);

  dst->data[0] = -1.5;
  dst->data[1] = 0.75;
  dst->data[2] = 2.25;

  const double *dst_data = dst->data;
  const ins_block *dst_block = dst->block;

  const int status = ins_vector_copy(dst, src);
  assert_int_equal(status, INS_SUCCESS);

  // Check src's data
  assert_double_equal(src->data[0], 2.5, 0.0);
  assert_double_equal(src->data[1], 1.0, 0.0);
  assert_double_equal(src->data[2], 4.0, 0.0);

  // Check dst's data
  assert_double_equal(dst->data[0], 2.5, 0.0);
  assert_double_equal(dst->data[1], 1.0, 0.0);
  assert_double_equal(dst->data[2], 4.0, 0.0);

  // Check src's state
  assert_int_equal(src->size, 3);
  assert_int_equal(src->stride, 1);
  assert_non_null(src->block);
  assert_ptr_equal(src->block->data, src->data);
  assert_int_equal(src->block->size, src->size);
  assert_int_equal(src->owner, 1);
  assert_ptr_equal(src->data, src_data);
  assert_ptr_equal(src->block, src_block);

  // Check dst's state
  assert_int_equal(dst->size, 3);
  assert_int_equal(dst->stride, 1);
  assert_non_null(dst->block);
  assert_ptr_equal(dst->block->data, dst->data);
  assert_int_equal(dst->block->size, dst->size);
  assert_int_equal(dst->owner, 1);
  assert_ptr_equal(dst->data, dst_data);
  assert_ptr_equal(dst->block, dst_block);

  ins_vector_free(dst);
  ins_vector_free(src);
}

static void test_vector_copy_same_stride_two(void **state) {
  (void) state;

  // Init src

  ins_vector *src = ins_vector_alloc(5);

  src->size = 3;
  src->stride = 2;

  src->data[0] = 2.5;
  src->data[1] = -0.75;
  src->data[2] = 1.0;
  src->data[3] = 3.15;
  src->data[4] = 4.0;

  const double *src_data = src->data;
  const ins_block *src_block = src->block;

  // Init dst

  ins_vector *dst = ins_vector_alloc(5);

  dst->size = 3;
  dst->stride = 2;

  dst->data[0] = -1.5;
  dst->data[1] = 0.25;
  dst->data[2] = 0.75;
  dst->data[3] = 1.75;
  dst->data[4] = 2.25;

  const double *dst_data = dst->data;
  const ins_block *dst_block = dst->block;

  // Copy src to dst.
  const int status = ins_vector_copy(dst, src);
  assert_int_equal(status, INS_SUCCESS);

  // Check src's data
  assert_double_equal(src->data[0], 2.5, 0.0);
  assert_double_equal(src->data[1], -0.75, 0.0);
  assert_double_equal(src->data[2], 1.0, 0.0);
  assert_double_equal(src->data[3], 3.15, 0.0);
  assert_double_equal(src->data[4], 4.0, 0.0);

  // Check dst's data
  assert_double_equal(dst->data[0], 2.5, 0.0);
  assert_double_equal(dst->data[1], 0.25, 0.0);
  assert_double_equal(dst->data[2], 1.0, 0.0);
  assert_double_equal(dst->data[3], 1.75, 0.0);
  assert_double_equal(dst->data[4], 4.0, 0.0);

  // Check src's elements
  assert_double_equal(ins_vector_get(src, 0), 2.5, 0.0);
  assert_double_equal(ins_vector_get(src, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(src, 2), 4.0, 0.0);

  // Check dst's elements
  assert_double_equal(ins_vector_get(dst, 0), 2.5, 0.0);
  assert_double_equal(ins_vector_get(dst, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(dst, 2), 4.0, 0.0);

  // Check src's state
  assert_int_equal(src->size, 3);
  assert_int_equal(src->stride, 2);
  assert_non_null(src->block);
  assert_ptr_equal(src->block->data, src->data);
  assert_int_equal(src->block->size, 5);
  assert_int_equal(src->owner, 1);
  assert_ptr_equal(src->data, src_data);
  assert_ptr_equal(src->block, src_block);

  // Check dst's state
  assert_int_equal(dst->size, 3);
  assert_int_equal(dst->stride, 2);
  assert_non_null(dst->block);
  assert_ptr_equal(dst->block->data, dst->data);
  assert_int_equal(dst->block->size, 5);
  assert_int_equal(dst->owner, 1);
  assert_ptr_equal(dst->data, dst_data);
  assert_ptr_equal(dst->block, dst_block);

  ins_vector_free(dst);
  ins_vector_free(src);
}

static void test_vector_copy_diff_strides(void **state) {
  (void) state;

  // Init src

  ins_vector *src = ins_vector_alloc(3);

  src->data[0] = 2.5;
  src->data[1] = 1.0;
  src->data[2] = 4.0;

  const double *src_data = src->data;
  const ins_block *src_block = src->block;

  // Init dst

  ins_vector *dst = ins_vector_alloc(5);

  dst->size = 3;
  dst->stride = 2;

  dst->data[0] = -1.5;
  dst->data[1] = 0.25;
  dst->data[2] = 0.75;
  dst->data[3] = 1.75;
  dst->data[4] = 2.25;

  const double *dst_data = dst->data;
  const ins_block *dst_block = dst->block;

  // Copy src to dst.
  const int status = ins_vector_copy(dst, src);
  assert_int_equal(status, INS_SUCCESS);

  // Check src's data
  assert_double_equal(src->data[0], 2.5, 0.0);
  assert_double_equal(src->data[1], 1.0, 0.0);
  assert_double_equal(src->data[2], 4.0, 0.0);

  // Check dst's data
  assert_double_equal(dst->data[0], 2.5, 0.0);
  assert_double_equal(dst->data[1], 0.25, 0.0);
  assert_double_equal(dst->data[2], 1.0, 0.0);
  assert_double_equal(dst->data[3], 1.75, 0.0);
  assert_double_equal(dst->data[4], 4.0, 0.0);

  // Check src's elements
  assert_double_equal(ins_vector_get(src, 0), 2.5, 0.0);
  assert_double_equal(ins_vector_get(src, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(src, 2), 4.0, 0.0);

  // Check dst's elements
  assert_double_equal(ins_vector_get(dst, 0), 2.5, 0.0);
  assert_double_equal(ins_vector_get(dst, 1), 1.0, 0.0);
  assert_double_equal(ins_vector_get(dst, 2), 4.0, 0.0);

  // Check src's state
  assert_int_equal(src->size, 3);
  assert_int_equal(src->stride, 1);
  assert_non_null(src->block);
  assert_ptr_equal(src->block->data, src->data);
  assert_int_equal(src->block->size, 3);
  assert_int_equal(src->owner, 1);
  assert_ptr_equal(src->data, src_data);
  assert_ptr_equal(src->block, src_block);

  // Check dst's state
  assert_int_equal(dst->size, 3);
  assert_int_equal(dst->stride, 2);
  assert_non_null(dst->block);
  assert_ptr_equal(dst->block->data, dst->data);
  assert_int_equal(dst->block->size, 5);
  assert_int_equal(dst->owner, 1);
  assert_ptr_equal(dst->data, dst_data);
  assert_ptr_equal(dst->block, dst_block);

  ins_vector_free(dst);
  ins_vector_free(src);
}

static void test_vector_copy_diff_lengths(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  ins_vector *w = ins_vector_alloc(2);

  ins_set_error_handler_off();

  assert_int_equal(ins_vector_copy(v, w), INS_EINVAL);
  assert_int_equal(ins_vector_copy(w, v), INS_EINVAL);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_dot_same_stride_one(void **state) {
  (void) state;

  // Init v

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 2.0;
  v->data[1] = 0.5;
  v->data[2] = -1.0;

  const double *v_data = v->data;
  const ins_block *v_block = v->block;

  // Init w

  ins_vector *w = ins_vector_alloc(3);

  w->data[0] = 4.0;
  w->data[1] = 2.5;
  w->data[2] = 3.0;

  const double *w_data = w->data;
  const ins_block *w_block = w->block;

  assert_double_equal(ins_vector_dot(v, w), 6.25, 0.0);
  assert_double_equal(ins_vector_dot(w, v), 6.25, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 2.0, 0.0);
  assert_double_equal(v->data[1], 0.5, 0.0);
  assert_double_equal(v->data[2], -1.0, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 4.0, 0.0);
  assert_double_equal(w->data[1], 2.5, 0.0);
  assert_double_equal(w->data[2], 3.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, 3);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->data, v_data);
  assert_ptr_equal(v->block, v_block);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 1);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w->data);
  assert_int_equal(w->block->size, 3);
  assert_int_equal(w->owner, 1);
  assert_ptr_equal(w->data, w_data);
  assert_ptr_equal(w->block, w_block);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_dot_same_stride_two(void **state) {
  (void) state;

  // Init v

  ins_vector *v = ins_vector_alloc(5);

  v->data[0] = 2.0;
  v->data[1] = -0.45;
  v->data[2] = 0.5;
  v->data[3] = 7.5;
  v->data[4] = -1.0;

  v->size = 3;
  v->stride = 2;

  const double *v_data = v->data;
  const ins_block *v_block = v->block;

  // Init w

  ins_vector *w = ins_vector_alloc(5);

  w->data[0] = 4.0;
  w->data[1] = 0.15;
  w->data[2] = 2.5;
  w->data[3] = -0.5;
  w->data[4] = 3.0;

  w->size = 3;
  w->stride = 2;

  const double *w_data = w->data;
  const ins_block *w_block = w->block;

  assert_double_equal(ins_vector_dot(v, w), 6.25, 0.0);
  assert_double_equal(ins_vector_dot(w, v), 6.25, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 2.0, 0.0);
  assert_double_equal(v->data[1], -0.45, 0.0);
  assert_double_equal(v->data[2], 0.5, 0.0);
  assert_double_equal(v->data[3], 7.5, 0.0);
  assert_double_equal(v->data[4], -1.0, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 4.0, 0.0);
  assert_double_equal(w->data[1], 0.15, 0.0);
  assert_double_equal(w->data[2], 2.5, 0.0);
  assert_double_equal(w->data[3], -0.5, 0.0);
  assert_double_equal(w->data[4], 3.0, 0.0);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), 2.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), -1.0, 0.0);

  // Check w's elements
  assert_double_equal(ins_vector_get(w, 0), 4.0, 0.0);
  assert_double_equal(ins_vector_get(w, 1), 2.5, 0.0);
  assert_double_equal(ins_vector_get(w, 2), 3.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, 5);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->data, v_data);
  assert_ptr_equal(v->block, v_block);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w->data);
  assert_int_equal(w->block->size, 5);
  assert_int_equal(w->owner, 1);
  assert_ptr_equal(w->data, w_data);
  assert_ptr_equal(w->block, w_block);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_dot_diff_strides(void **state) {
  (void) state;

  // Init v

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 2.0;
  v->data[1] = 0.5;
  v->data[2] = -1.0;

  const double *v_data = v->data;
  const ins_block *v_block = v->block;

  // Init w

  ins_vector *w = ins_vector_alloc(5);

  w->data[0] = 4.0;
  w->data[1] = 0.15;
  w->data[2] = 2.5;
  w->data[3] = -0.5;
  w->data[4] = 3.0;

  w->size = 3;
  w->stride = 2;

  const double *w_data = w->data;
  const ins_block *w_block = w->block;

  assert_double_equal(ins_vector_dot(v, w), 6.25, 0.0);
  assert_double_equal(ins_vector_dot(w, v), 6.25, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 2.0, 0.0);
  assert_double_equal(v->data[1], 0.5, 0.0);
  assert_double_equal(v->data[2], -1.0, 0.0);

  // Check w's data
  assert_double_equal(w->data[0], 4.0, 0.0);
  assert_double_equal(w->data[1], 0.15, 0.0);
  assert_double_equal(w->data[2], 2.5, 0.0);
  assert_double_equal(w->data[3], -0.5, 0.0);
  assert_double_equal(w->data[4], 3.0, 0.0);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), 2.0, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 2), -1.0, 0.0);

  // Check w's elements
  assert_double_equal(ins_vector_get(w, 0), 4.0, 0.0);
  assert_double_equal(ins_vector_get(w, 1), 2.5, 0.0);
  assert_double_equal(ins_vector_get(w, 2), 3.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->block->size, 3);
  assert_int_equal(v->owner, 1);
  assert_ptr_equal(v->data, v_data);
  assert_ptr_equal(v->block, v_block);

  // Check w's state
  assert_int_equal(w->size, 3);
  assert_int_equal(w->stride, 2);
  assert_non_null(w->block);
  assert_ptr_equal(w->block->data, w->data);
  assert_int_equal(w->block->size, 5);
  assert_int_equal(w->owner, 1);
  assert_ptr_equal(w->data, w_data);
  assert_ptr_equal(w->block, w_block);

  ins_vector_free(w);
  ins_vector_free(v);
}

static void test_vector_dot_diff_lengths(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);
  ins_vector *w = ins_vector_alloc(2);

  ins_set_error_handler_off();
  assert_int_equal(ins_vector_dot(v, w), INS_EINVAL);
  assert_int_equal(ins_vector_dot(w, v), INS_EINVAL);

  ins_vector_free(w);
  ins_vector_free(v);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_scale_when_stride_is_one),
    cmocka_unit_test(test_scale_when_alloc_from_block_stride_one),
    cmocka_unit_test(test_scale_when_alloc_from_block_stride_two),
    cmocka_unit_test(test_vector_add_two_vectors_same_stride_one),
    cmocka_unit_test(test_vector_add_two_vectors_same_stride_two),
    cmocka_unit_test(test_vector_add_two_vectors_different_strides),
    cmocka_unit_test(test_vector_add_two_vectors_different_lengths),
    cmocka_unit_test(test_vector_sub_same_stride_one),
    cmocka_unit_test(test_vector_sub_same_stride_two),
    cmocka_unit_test(test_vector_sub_different_strides),
    cmocka_unit_test(test_vector_sub_different_lengths),
    cmocka_unit_test(test_vector_mul_same_stride_one),
    cmocka_unit_test(test_vector_mul_same_stride_two),
    cmocka_unit_test(test_vector_mul_different_strides),
    cmocka_unit_test(test_vector_mul_different_lengths),
    cmocka_unit_test(test_vector_div_same_stride_one),
    cmocka_unit_test(test_vector_div_same_stride_two),
    cmocka_unit_test(test_vector_div_different_strides),
    cmocka_unit_test(test_vector_div_different_lengths),
    cmocka_unit_test(test_vector_add_constant_stride_one),
    cmocka_unit_test(test_vector_add_constant_stride_two),
    cmocka_unit_test(test_vector_sum_stride_one),
    cmocka_unit_test(test_vector_sum_stride_two),
    cmocka_unit_test(test_vector_axpy_same_length_same_stride_one),
    cmocka_unit_test(test_vector_axpy_same_length_same_stride_two),
    cmocka_unit_test(test_vector_axpy_same_length_diff_strides),
    cmocka_unit_test(test_vector_axpy_diff_lengths),
    cmocka_unit_test(test_vector_swap_same_stride_one),
    cmocka_unit_test(test_vector_swap_same_stride_two),
    cmocka_unit_test(test_vector_swap_diff_strides),
    cmocka_unit_test(test_vector_swap_diff_lengths),
    cmocka_unit_test(test_vector_copy_same_stride_one),
    cmocka_unit_test(test_vector_copy_same_stride_two),
    cmocka_unit_test(test_vector_copy_diff_strides),
    cmocka_unit_test(test_vector_copy_diff_lengths),
    cmocka_unit_test(test_vector_dot_same_stride_one),
    cmocka_unit_test(test_vector_dot_same_stride_two),
    cmocka_unit_test(test_vector_dot_diff_strides),
    cmocka_unit_test(test_vector_dot_diff_lengths)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
