#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_vector.h>

static void test_vector_fwrite_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  v->data[0] = 0.5;
  v->data[1] = 2.0;
  v->data[2] = 4.0;

  FILE *file = fopen("vector_double_fwrite_stride_one.dat", "wb");
  assert_non_null(file);

  const int status = ins_vector_fwrite(v, file);
  assert_int_equal(status, INS_SUCCESS);

  fclose(file);
  ins_vector_free(v);
}

static void test_vector_fwrite_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(5);

  v->data[0] = 0.5;
  v->data[1] = -1.5;
  v->data[2] = 2.0;
  v->data[3] = 0.0;
  v->data[4] = 4.0;

  v->size = 3;
  v->stride = 2;

  FILE *file = fopen("vector_double_fwrite_stride_two.dat", "wb");
  assert_non_null(file);

  const int status = ins_vector_fwrite(v, file);
  assert_int_equal(status, INS_SUCCESS);

  fclose(file);
  ins_vector_free(v);
}

static void test_vector_fread_stride_one(void **state) {
  (void) state;

  ins_vector *v = ins_vector_alloc(3);

  FILE *file = fopen("vector_double_fwrite_stride_one.dat", "rb");
  assert_non_null(file);

  const int status = ins_vector_fread(v, file);
  assert_int_equal(status, INS_SUCCESS);

  // check v's data
  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], 2.0, 0.0);
  assert_double_equal(v->data[2], 4.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 1);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  fclose(file);
  ins_vector_free(v);
}

static void test_vector_fread_stride_two(void **state) {
  (void) state;

  ins_vector *v = ins_vector_calloc(5);

  v->size = 3;
  v->stride = 2;

  FILE *file = fopen("vector_double_fwrite_stride_two.dat", "rb");
  assert_non_null(file);

  const int status = ins_vector_fread(v, file);
  assert_int_equal(status, INS_SUCCESS);

  // Check v's elements
  assert_double_equal(ins_vector_get(v, 0), 0.5, 0.0);
  assert_double_equal(ins_vector_get(v, 1), 2.0, 0.0);
  assert_double_equal(ins_vector_get(v, 2), 4.0, 0.0);

  // Check v's data
  assert_double_equal(v->data[0], 0.5, 0.0);
  assert_double_equal(v->data[1], 0.0, 0.0);
  assert_double_equal(v->data[2], 2.0, 0.0);
  assert_double_equal(v->data[3], 0.0, 0.0);
  assert_double_equal(v->data[4], 4.0, 0.0);

  // Check v's state
  assert_int_equal(v->size, 3);
  assert_int_equal(v->stride, 2);
  assert_non_null(v->block);
  assert_ptr_equal(v->block->data, v->data);
  assert_int_equal(v->owner, 1);

  fclose(file);
  ins_vector_free(v);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_vector_fwrite_stride_one),
    cmocka_unit_test(test_vector_fwrite_stride_two),
    cmocka_unit_test(test_vector_fread_stride_one),
    cmocka_unit_test(test_vector_fread_stride_two)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
