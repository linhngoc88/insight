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

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success),
    cmocka_unit_test(calloc_success)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
