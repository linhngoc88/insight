#include <stdlib.h>
#include "insight/insight_errno.h"

// Global variables
FILE* insight_stream = NULL;
insight_stream_handler_t* insight_stream_handler = NULL;
insight_error_handler_t* insight_error_handler = NULL;

// The error handler that does nothing.
static void
no_error_handler(const char* reason, const char* file, int line, int error_code);

const char* insight_strerror(const int gsl_errno) {
  switch (gsl_errno) {
  case INSIGHT_SUCCESS:
    return "success" ;
  case INSIGHT_FAILURE:
    return "failure" ;
  case INSIGHT_CONTINUE:
    return "the iteration has not converged yet";
  case INSIGHT_EDOM:
    return "input domain error";
  case INSIGHT_ERANGE:
    return "output range error";
  case INSIGHT_EFAULT:
    return "invalid pointer";
  case INSIGHT_EINVAL:
    return "invalid argument supplied by user";
  case INSIGHT_EFAILED:
    return "generic failure";
  case INSIGHT_EFACTOR:
    return "factorization failed";
  case INSIGHT_ESANITY:
    return "sanity check failed - shouldn't happen";
  case INSIGHT_ENOMEM:
    return "malloc failed";
  case INSIGHT_EBADFUNC:
    return "problem with user-supplied function";
  case INSIGHT_ERUNAWAY:
    return "iterative process is out of control";
  case INSIGHT_EMAXITER:
    return "exceeded max number of iterations";
  case INSIGHT_EZERODIV:
    return "tried to divide by zero";
  case INSIGHT_EBADTOL:
    return "specified tolerance is invalid or theoretically unattainable";
  case INSIGHT_ETOL:
    return "failed to reach the specified tolerance";
  case INSIGHT_EUNDRFLW:
    return "underflow";
  case INSIGHT_EOVRFLW:
    return "overflow";
  case INSIGHT_ELOSS:
    return "loss of accuracy";
  case INSIGHT_EROUND:
    return "roundoff error";
  case INSIGHT_EBADLEN:
    return "matrix/vector sizes are not conformant";
  case INSIGHT_ENOTSQR:
    return "matrix not square";
  case INSIGHT_ESING:
    return "singularity or extremely bad function behavior detected";
  case INSIGHT_EDIVERGE:
    return "integral or series is divergent";
  case INSIGHT_EUNSUP:
    return "the required feature is not supported by this hardware platform";
  case INSIGHT_EUNIMPL:
    return "the requested feature is not (yet) implemented";
  case INSIGHT_ECACHE:
    return "cache limit exceeded";
  case INSIGHT_ETABLE:
    return "table limit exceeded";
  case INSIGHT_ENOPROG:
    return "iteration is not making progress towards solution";
  case INSIGHT_ENOPROGJ:
    return "jacobian evaluations are not improving the solution";
  case INSIGHT_ETOLF:
    return "cannot reach the specified tolerance in F";
  case INSIGHT_ETOLX:
    return "cannot reach the specified tolerance in X";
  case INSIGHT_ETOLG:
    return "cannot reach the specified tolerance in gradient";
  case INSIGHT_EOF:
    return "end of file";
  default:
    return "unknown error code";
  }
}

insight_stream_handler_t*
insight_set_stream_handler(insight_stream_handler_t* new_handler) {
  insight_stream_handler_t* previous_handler = insight_stream_handler;
  insight_stream_handler = new_handler;
  return previous_handler;
}

FILE* insight_set_stream(FILE* new_stream) {
  FILE* previous_stream;
  if (insight_stream == NULL) {
    insight_stream = stderr;
  }
  previous_stream = insight_stream;
  insight_stream = new_stream;
  return previous_stream;
}

void insight_stream_printf(const char* label, const char* file, int line,
                           const char* reason) {
  if (insight_stream == NULL) {
    insight_stream = stderr;
  }

  if (insight_stream_handler) {
    // It's a good practice to dereference function pointer here.
    (*insight_stream_handler)(label, file, line, reason);
    return;
  }

  fprintf(insight_stream, "insight: %s:%d: %s: %s\n", file, line, label, reason);
}

insight_error_handler_t*
insight_set_error_handler(insight_error_handler_t* new_handler) {
  insight_error_handler_t* previous_handler = insight_error_handler;
  insight_error_handler = new_handler;
  return previous_handler;
}

insight_error_handler_t* insight_set_error_handler_off(void) {
  insight_error_handler_t* previous_handler = insight_error_handler;
  insight_error_handler = no_error_handler;
  return previous_handler;
}

void
insight_error(const char* reason, const char* file, int line, int error_code) {
  if (insight_error_handler) {
    (*insight_error_handler)(reason, file, line, error_code);
    return;
  }

  insight_stream_printf("ERROR", file, line, reason);

  fflush(stdout);
  fprintf(stderr, "Default Insight error handler invoked.\n");
  fflush(stderr);

  abort();
}

static void
no_error_handler(const char* reason, const char* file, int line, int error_code) {
  // Do nothing
  (void) reason;
  (void) file;
  (void) line;
  (void) error_code;
  return;
}
