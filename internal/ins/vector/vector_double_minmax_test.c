#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <ins/ins_vector.h>

static void test_setup(void **state) {
  (void) state;

  assert_int_equal(1 + 1, 3);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_setup)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
