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
    cmocka_unit_test(test_vector_add_constant_stride_two)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
