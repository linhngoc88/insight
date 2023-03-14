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

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(alloc_success)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
