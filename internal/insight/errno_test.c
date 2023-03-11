#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "insight/insight_errno.h"

static void errno_success(void **state) {
  (void) state; /* unused */
  const char * expected_str = "success";
  const char * actual_str = insight_strerror(INSIGHT_SUCCESS);
  assert_string_equal(actual_str, expected_str);
}

static void errno_failure(void **state) {
  (void) state; /* unused */
  const char * expected_str = "failure";
  const char * actual_str = insight_strerror(INSIGHT_FAILURE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_continue(void **state) {
  (void) state; /* unused */
  const char * expected_str = "the iteration has not converged yet";
  const char * actual_str = insight_strerror(INSIGHT_CONTINUE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_edom(void **state) {
  (void) state; /* unused */
  const char * expected_str = "input domain error";
  const char * actual_str = insight_strerror(INSIGHT_EDOM);
  assert_string_equal(actual_str, expected_str);
}

static void errno_erange(void **state) {
  (void) state; /* unused */
  const char * expected_str = "output range error";
  const char * actual_str = insight_strerror(INSIGHT_ERANGE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_efault(void **state) {
  (void) state; /* unused */
  const char * expected_str = "invalid pointer";
  const char * actual_str = insight_strerror(INSIGHT_EFAULT);
  assert_string_equal(actual_str, expected_str);
}

static void errno_einval(void **state) {
  (void) state; /* unused */
  const char * expected_str = "invalid argument supplied by user";
  const char * actual_str = insight_strerror(INSIGHT_EINVAL);
  assert_string_equal(actual_str, expected_str);
}

static void errno_efailed(void **state) {
  (void) state; /* unused */
  const char * expected_str = "generic failure";
  const char * actual_str = insight_strerror(INSIGHT_EFAILED);
  assert_string_equal(actual_str, expected_str);
}

static void errno_efactor(void **state) {
  (void) state; /* unused */
  const char * expected_str = "factorization failed";
  const char * actual_str = insight_strerror(INSIGHT_EFACTOR);
  assert_string_equal(actual_str, expected_str);
}

static void errno_esanity(void **state) {
  (void) state; /* unused */
  const char * expected_str = "sanity check failed - shouldn't happen";
  const char * actual_str = insight_strerror(INSIGHT_ESANITY);
  assert_string_equal(actual_str, expected_str);
}

static void errno_enomem(void **state) {
  (void) state; /* unused */
  const char * expected_str = "malloc failed";
  const char * actual_str = insight_strerror(INSIGHT_ENOMEM);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ebadfunc(void **state) {
  (void) state; /* unused */
  const char * expected_str = "problem with user-supplied function";
  const char * actual_str = insight_strerror(INSIGHT_EBADFUNC);
  assert_string_equal(actual_str, expected_str);
}

static void errno_erunaway(void **state) {
  (void) state; /* unused */
  const char * expected_str = "iterative process is out of control";
  const char * actual_str = insight_strerror(INSIGHT_ERUNAWAY);
  assert_string_equal(actual_str, expected_str);
}

static void errno_emaxiter(void **state) {
  (void) state; /* unused */
  const char * expected_str = "exceeded max number of iterations";
  const char * actual_str = insight_strerror(INSIGHT_EMAXITER);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ezerodiv(void **state) {
  (void) state; /* unused */
  const char * expected_str = "tried to divide by zero";
  const char * actual_str = insight_strerror(INSIGHT_EZERODIV);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ebadtol(void **state) {
  (void) state; /* unused */
  const char * expected_str =
    "specified tolerance is invalid or theoretically unattainable";
  const char * actual_str = insight_strerror(INSIGHT_EBADTOL);
  assert_string_equal(actual_str, expected_str);
}

static void errno_etol(void **state) {
  (void) state; /* unused */
  const char * expected_str = "failed to reach the specified tolerance";
  const char * actual_str = insight_strerror(INSIGHT_ETOL);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eundrflw(void **state) {
  (void) state; /* unused */
  const char * expected_str = "underflow";
  const char * actual_str = insight_strerror(INSIGHT_EUNDRFLW);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eovrflw(void **state) {
  (void) state; /* unused */
  const char * expected_str = "overflow";
  const char * actual_str = insight_strerror(INSIGHT_EOVRFLW);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eloss(void **state) {
  (void) state; /* unused */
  const char * expected_str = "loss of accuracy";
  const char * actual_str = insight_strerror(INSIGHT_ELOSS);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eround(void **state) {
  (void) state; /* unused */
  const char * expected_str = "roundoff error";
  const char * actual_str = insight_strerror(INSIGHT_EROUND);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ebadlen(void **state) {
  (void) state; /* unused */
  const char * expected_str = "matrix/vector sizes are not conformant";
  const char * actual_str = insight_strerror(INSIGHT_EBADLEN);
  assert_string_equal(actual_str, expected_str);
}

static void errno_enotsqr(void **state) {
  (void) state; /* unused */
  const char * expected_str = "matrix not square";
  const char * actual_str = insight_strerror(INSIGHT_ENOTSQR);
  assert_string_equal(actual_str, expected_str);
}

static void errno_esing(void **state) {
  (void) state; /* unused */
  const char * expected_str =
    "singularity or extremely bad function behavior detected";
  const char * actual_str = insight_strerror(INSIGHT_ESING);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ediverge(void **state) {
  (void) state; /* unused */
  const char * expected_str = "integral or series is divergent";
  const char * actual_str = insight_strerror(INSIGHT_EDIVERGE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eunsup(void **state) {
  (void) state; /* unused */
  const char * expected_str =
    "the required feature is not supported by this hardware platform";
  const char * actual_str = insight_strerror(INSIGHT_EUNSUP);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eunimpl(void **state) {
  (void) state; /* unused */
  const char * expected_str = "the requested feature is not (yet) implemented";
  const char * actual_str = insight_strerror(INSIGHT_EUNIMPL);
  assert_string_equal(actual_str, expected_str);
}

static void errno_ecache(void **state) {
  (void) state; /* unused */
  const char * expected_str = "cache limit exceeded";
  const char * actual_str = insight_strerror(INSIGHT_ECACHE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_etable(void **state) {
  (void) state; /* unused */
  const char * expected_str = "table limit exceeded";
  const char * actual_str = insight_strerror(INSIGHT_ETABLE);
  assert_string_equal(actual_str, expected_str);
}

static void errno_enoprog(void **state) {
  (void) state; /* unused */
  const char * expected_str = "iteration is not making progress towards solution";
  const char * actual_str = insight_strerror(INSIGHT_ENOPROG);
  assert_string_equal(actual_str, expected_str);
}

static void errno_enoprogj(void **state) {
  (void) state; /* unused */
  const char * expected_str = "jacobian evaluations are not improving the solution";
  const char * actual_str = insight_strerror(INSIGHT_ENOPROGJ);
  assert_string_equal(actual_str, expected_str);
}

static void errno_etolf(void **state) {
  (void) state; /* unused */
  const char * expected_str = "cannot reach the specified tolerance in F";
  const char * actual_str = insight_strerror(INSIGHT_ETOLF);
  assert_string_equal(actual_str, expected_str);
}

static void errno_etolx(void **state) {
  (void) state; /* unused */
  const char * expected_str = "cannot reach the specified tolerance in X";
  const char * actual_str = insight_strerror(INSIGHT_ETOLX);
  assert_string_equal(actual_str, expected_str);
}

static void errno_etolg(void **state) {
  (void) state; /* unused */
  const char * expected_str = "cannot reach the specified tolerance in gradient";
  const char * actual_str = insight_strerror(INSIGHT_ETOLG);
  assert_string_equal(actual_str, expected_str);
}

static void errno_eof(void **state) {
  (void) state; /* unused */
  const char * expected_str = "end of file";
  const char * actual_str = insight_strerror(INSIGHT_EOF);
  assert_string_equal(actual_str, expected_str);
}

static void errno_unknown(void **state) {
  (void) state; /* unused */
  const char * expected_str = "unknown error code";
  const char * actual_str = insight_strerror(33);
  assert_string_equal(actual_str, expected_str);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(errno_success),
    cmocka_unit_test(errno_failure),
    cmocka_unit_test(errno_continue),
    cmocka_unit_test(errno_edom),
    cmocka_unit_test(errno_erange),
    cmocka_unit_test(errno_efault),
    cmocka_unit_test(errno_einval),
    cmocka_unit_test(errno_efailed),
    cmocka_unit_test(errno_efactor),
    cmocka_unit_test(errno_esanity),
    cmocka_unit_test(errno_enomem),
    cmocka_unit_test(errno_ebadfunc),
    cmocka_unit_test(errno_erunaway),
    cmocka_unit_test(errno_emaxiter),
    cmocka_unit_test(errno_ezerodiv),
    cmocka_unit_test(errno_ebadtol),
    cmocka_unit_test(errno_etol),
    cmocka_unit_test(errno_eundrflw),
    cmocka_unit_test(errno_eovrflw),
    cmocka_unit_test(errno_eloss),
    cmocka_unit_test(errno_eround),
    cmocka_unit_test(errno_ebadlen),
    cmocka_unit_test(errno_enotsqr),
    cmocka_unit_test(errno_esing),
    cmocka_unit_test(errno_ediverge),
    cmocka_unit_test(errno_eunsup),
    cmocka_unit_test(errno_eunimpl),
    cmocka_unit_test(errno_ecache),
    cmocka_unit_test(errno_etable),
    cmocka_unit_test(errno_enoprog),
    cmocka_unit_test(errno_enoprogj),
    cmocka_unit_test(errno_etolf),
    cmocka_unit_test(errno_etolx),
    cmocka_unit_test(errno_etolg),
    cmocka_unit_test(errno_eof),
    cmocka_unit_test(errno_unknown)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
