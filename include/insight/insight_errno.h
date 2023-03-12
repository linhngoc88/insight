#ifndef INSIGHT_ERRNO_H_
#define INSIGHT_ERRNO_H_

#include <stdio.h>

/* Copied from gsl_errno.h */
enum {
  INSIGHT_SUCCESS  = 0,
  INSIGHT_FAILURE  = -1,
  INSIGHT_CONTINUE = -2,  /* iteration has not converged */
  INSIGHT_EDOM     = 1,   /* input domain error, e.g sqrt(-1) */
  INSIGHT_ERANGE   = 2,   /* output range error, e.g. exp(1e100) */
  INSIGHT_EFAULT   = 3,   /* invalid pointer */
  INSIGHT_EINVAL   = 4,   /* invalid argument supplied by user */
  INSIGHT_EFAILED  = 5,   /* generic failure */
  INSIGHT_EFACTOR  = 6,   /* factorization failed */
  INSIGHT_ESANITY  = 7,   /* sanity check failed - shouldn't happen */
  INSIGHT_ENOMEM   = 8,   /* malloc failed */
  INSIGHT_EBADFUNC = 9,   /* problem with user-supplied function */
  INSIGHT_ERUNAWAY = 10,  /* iterative process is out of control */
  INSIGHT_EMAXITER = 11,  /* exceeded max number of iterations */
  INSIGHT_EZERODIV = 12,  /* tried to divide by zero */
  INSIGHT_EBADTOL  = 13,  /* user specified an invalid tolerance */
  INSIGHT_ETOL     = 14,  /* failed to reach the specified tolerance */
  INSIGHT_EUNDRFLW = 15,  /* underflow */
  INSIGHT_EOVRFLW  = 16,  /* overflow  */
  INSIGHT_ELOSS    = 17,  /* loss of accuracy */
  INSIGHT_EROUND   = 18,  /* failed because of roundoff error */
  INSIGHT_EBADLEN  = 19,  /* matrix, vector lengths are not conformant */
  INSIGHT_ENOTSQR  = 20,  /* matrix not square */
  INSIGHT_ESING    = 21,  /* apparent singularity detected */
  INSIGHT_EDIVERGE = 22,  /* integral or series is divergent */
  INSIGHT_EUNSUP   = 23,  /* requested feature is not supported by the hardware */
  INSIGHT_EUNIMPL  = 24,  /* requested feature not (yet) implemented */
  INSIGHT_ECACHE   = 25,  /* cache limit exceeded */
  INSIGHT_ETABLE   = 26,  /* table limit exceeded */
  INSIGHT_ENOPROG  = 27,  /* iteration is not making progress towards solution */
  INSIGHT_ENOPROGJ = 28,  /* jacobian evaluations are not improving the solution */
  INSIGHT_ETOLF    = 29,  /* cannot reach the specified tolerance in F */
  INSIGHT_ETOLX    = 30,  /* cannot reach the specified tolerance in X */
  INSIGHT_ETOLG    = 31,  /* cannot reach the specified tolerance in gradient */
  INSIGHT_EOF      = 32   /* end of file */
} ;


// Similar to std:strerror()
const char* insight_strerror(const int insight_errno);

/* STREAM HANDLER */

// Insight stream handler type
typedef void (insight_stream_handler_t)(const char*, const char*, int, const char*);

// Sets the global variable `insight_stream_handler` to the `new_handler` and
// returns the previous stream handler.
insight_stream_handler_t*
insight_set_stream_handler(insight_stream_handler_t* new_handler);

// Sets the global variable `insight_stream` to the given `new_stream` and
// returns the previous stream.
FILE* insight_set_stream(FILE* new_stream);

// If the global variable `insight_stream_handler` is defined, then this
// function equivalent to invoking
//
//   `insight_stream_handler(label, file, line, reason)`.
//
// Otherwise, it's equivalent to
//
//   `fprintf(insight_stream, "gsl: %s:%d: %s: %s\n", file, line, label, reason)`
//
// in which `insight_stream` is a global variable pointing to a stream.
// (default to stderr).
void insight_stream_printf(const char* label, const char* file, int line,
                           const char* reason);

/* ERROR HANDLER */

// Insight error handler type
typedef void (insight_error_handler_t)(const char* reason, const char* file,
                                       int line, int insight_errno);

// Sets the global variable `insight_error_handler` to the `new_handler` and
// returns the previous handler.
insight_error_handler_t*
insight_set_error_handler(insight_error_handler_t* new_handler);

// Sets the global variable `insight_error_handler` to the "do nothing" handler
// and returns the previous handler.
insight_error_handler_t* insight_set_error_handler_off(void);

// If the global variable `insight_error_handler` is not NULL, then this
// function is equivalent to invoking
//
//   `insight_error_handler(reason, file, line, insight_errno)`
//
// Otherwise, it flushes the output to stderr.
void insight_error(const char* reason, const char* file, int line,
                   int insight_errno);

/* CONVINIENT MACROS */

// INSIGHT_ERROR: call the error handler, and return the error code
#define INSIGHT_ERROR(reason, insight_errno)                    \
  do {                                                          \
    insight_error(reason, __FILE__, __LINE__, insight_errno);   \
    return insight_errno;                                       \
  } while (0)

// INSIGHT_ERROR_VAL: call the error handler, and return the given value
#define INSIGHT_ERROR_VAL(reason, insight_errno, value)         \
  do {                                                          \
    insight_error(reason, __FILE__, __LINE__, insight_errno);   \
    return value;                                               \
  } while (0)

// INSIGHT_ERROR_VOID: call the error handler, and return.
#define INSIGHT_ERROR_VOID(reason, insight_errno)               \
  do {                                                          \
    insight_error(reason, __FILE__, __LINE__, insight_errno);   \
    return;                                                     \
  } while (0)

#endif /* INSIGHT_ERRNO_H_ */
