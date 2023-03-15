#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_block.h>


static void alloc_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_alloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);
  ins_block_free(block);
}

static void calloc_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_calloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);

  const double expected_mem[] = {0.0, 0.0, 0.0};
  assert_memory_equal(block->data, expected_mem, block->size * sizeof(double));
  ins_block_free(block);
}

static void fwrite_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_calloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);

  FILE * file = fopen("block_double_fwrite_test.dat", "wb");
  assert_non_null(file);

  const int ret = ins_block_fwrite(block, file);
  assert_int_equal(ret, 0);

  fclose(file);
  ins_block_free(block);
}

static void fread_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_alloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);

  FILE * file = fopen("block_double_fwrite_test.dat", "rb");
  assert_non_null(file);

  const int ret = ins_block_fread(block, file);
  assert_int_equal(ret, 0);

  fclose(file);
  ins_block_free(block);
}

static void fprintf_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_alloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);

  // Manually populate block data
  block->data[0] = 1.0;
  block->data[1] = 2.0;
  block->data[2] = 3.0;

  FILE * file = fopen("block_double_fprintf_test.dat", "wb");
  assert_non_null(file);

  const int ret = ins_block_fprintf(block, file, "%g");
  assert_int_equal(ret, INS_SUCCESS);

  fclose(file);
  ins_block_free(block);
}

static void fscanf_success(void **state) {
  (void) state; /* unused */

  ins_block * block = ins_block_alloc(3);
  assert_non_null(block);
  assert_non_null(block->data);
  assert_int_equal(block->size, 3);

  FILE * file = fopen("block_double_fprintf_test.dat", "rb");
  assert_non_null(file);

  const int ret = ins_block_fscanf(block, file);
  assert_int_equal(ret, INS_SUCCESS);
  assert_double_equal(block->data[0], 1.0, 0.0);
  assert_double_equal(block->data[1], 2.0, 0.0);
  assert_double_equal(block->data[2], 3.0, 0.0);

  fclose(file);
  ins_block_free(block);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success),
    cmocka_unit_test(calloc_success),
    cmocka_unit_test(fwrite_success),
    cmocka_unit_test(fread_success),
    cmocka_unit_test(fprintf_success),
    cmocka_unit_test(fscanf_success)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
