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

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_scale_when_stride_is_one),
    cmocka_unit_test(test_scale_when_alloc_from_block_stride_one),
    cmocka_unit_test(test_scale_when_alloc_from_block_stride_two)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
