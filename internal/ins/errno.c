#include <stdlib.h>
#include "ins/ins_errno.h"

// Global variables
FILE* ins_stream = NULL;
ins_stream_handler_t* ins_stream_handler = NULL;
ins_error_handler_t* ins_error_handler = NULL;

// The error handler that does nothing.
static void
no_error_handler(const char* reason, const char* file, int line, int error_code);

const char* ins_strerror(const int gsl_errno) {
  switch (gsl_errno) {
  case INS_SUCCESS:
    return "success" ;
  case INS_FAILURE:
    return "failure" ;
  case INS_CONTINUE:
    return "the iteration has not converged yet";
  case INS_EDOM:
    return "input domain error";
  case INS_ERANGE:
    return "output range error";
  case INS_EFAULT:
    return "invalid pointer";
  case INS_EINVAL:
    return "invalid argument supplied by user";
  case INS_EFAILED:
    return "generic failure";
  case INS_EFACTOR:
    return "factorization failed";
  case INS_ESANITY:
    return "sanity check failed - shouldn't happen";
  case INS_ENOMEM:
    return "malloc failed";
  case INS_EBADFUNC:
    return "problem with user-supplied function";
  case INS_ERUNAWAY:
    return "iterative process is out of control";
  case INS_EMAXITER:
    return "exceeded max number of iterations";
  case INS_EZERODIV:
    return "tried to divide by zero";
  case INS_EBADTOL:
    return "specified tolerance is invalid or theoretically unattainable";
  case INS_ETOL:
    return "failed to reach the specified tolerance";
  case INS_EUNDRFLW:
    return "underflow";
  case INS_EOVRFLW:
    return "overflow";
  case INS_ELOSS:
    return "loss of accuracy";
  case INS_EROUND:
    return "roundoff error";
  case INS_EBADLEN:
    return "matrix/vector sizes are not conformant";
  case INS_ENOTSQR:
    return "matrix not square";
  case INS_ESING:
    return "singularity or extremely bad function behavior detected";
  case INS_EDIVERGE:
    return "integral or series is divergent";
  case INS_EUNSUP:
    return "the required feature is not supported by this hardware platform";
  case INS_EUNIMPL:
    return "the requested feature is not (yet) implemented";
  case INS_ECACHE:
    return "cache limit exceeded";
  case INS_ETABLE:
    return "table limit exceeded";
  case INS_ENOPROG:
    return "iteration is not making progress towards solution";
  case INS_ENOPROGJ:
    return "jacobian evaluations are not improving the solution";
  case INS_ETOLF:
    return "cannot reach the specified tolerance in F";
  case INS_ETOLX:
    return "cannot reach the specified tolerance in X";
  case INS_ETOLG:
    return "cannot reach the specified tolerance in gradient";
  case INS_EOF:
    return "end of file";
  default:
    return "unknown error code";
  }
}

ins_stream_handler_t*
ins_set_stream_handler(ins_stream_handler_t* new_handler) {
  ins_stream_handler_t* previous_handler = ins_stream_handler;
  ins_stream_handler = new_handler;
  return previous_handler;
}

FILE* ins_set_stream(FILE* new_stream) {
  FILE* previous_stream;
  if (ins_stream == NULL) {
    ins_stream = stderr;
  }
  previous_stream = ins_stream;
  ins_stream = new_stream;
  return previous_stream;
}

void ins_stream_printf(const char* label, const char* file, int line,
                       const char* reason) {
  if (ins_stream == NULL) {
    ins_stream = stderr;
  }

  if (ins_stream_handler) {
    // It's a good practice to dereference function pointer here.
    (*ins_stream_handler)(label, file, line, reason);
    return;
  }

  fprintf(ins_stream, "ins: %s:%d: %s: %s\n", file, line, label, reason);
}

ins_error_handler_t*
ins_set_error_handler(ins_error_handler_t* new_handler) {
  ins_error_handler_t* previous_handler = ins_error_handler;
  ins_error_handler = new_handler;
  return previous_handler;
}

ins_error_handler_t* ins_set_error_handler_off(void) {
  ins_error_handler_t* previous_handler = ins_error_handler;
  ins_error_handler = no_error_handler;
  return previous_handler;
}

void
ins_error(const char* reason, const char* file, int line, int error_code) {
  if (ins_error_handler) {
    (*ins_error_handler)(reason, file, line, error_code);
    return;
  }

  ins_stream_printf("ERROR", file, line, reason);

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
