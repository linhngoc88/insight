#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "insight/hello.h"


static void integer_success(void **state) {
  (void) state; /* unused */
  int i = insight_add_two_integers(1, 2);
  assert_int_equal(i, 3);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(integer_success)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
