#ifndef INS_ERRNO_H_
#define INS_ERRNO_H_

#include <stdio.h>

/* Copied from gsl_errno.h */

enum {
  INS_SUCCESS  = 0,
  INS_FAILURE  = -1,
  INS_CONTINUE = -2,  /* iteration has not converged */
  INS_EDOM     = 1,   /* input domain error, e.g sqrt(-1) */
  INS_ERANGE   = 2,   /* output range error, e.g. exp(1e100) */
  INS_EFAULT   = 3,   /* invalid pointer */
  INS_EINVAL   = 4,   /* invalid argument supplied by user */
  INS_EFAILED  = 5,   /* generic failure */
  INS_EFACTOR  = 6,   /* factorization failed */
  INS_ESANITY  = 7,   /* sanity check failed - shouldn't happen */
  INS_ENOMEM   = 8,   /* malloc failed */
  INS_EBADFUNC = 9,   /* problem with user-supplied function */
  INS_ERUNAWAY = 10,  /* iterative process is out of control */
  INS_EMAXITER = 11,  /* exceeded max number of iterations */
  INS_EZERODIV = 12,  /* tried to divide by zero */
  INS_EBADTOL  = 13,  /* user specified an invalid tolerance */
  INS_ETOL     = 14,  /* failed to reach the specified tolerance */
  INS_EUNDRFLW = 15,  /* underflow */
  INS_EOVRFLW  = 16,  /* overflow  */
  INS_ELOSS    = 17,  /* loss of accuracy */
  INS_EROUND   = 18,  /* failed because of roundoff error */
  INS_EBADLEN  = 19,  /* matrix, vector lengths are not conformant */
  INS_ENOTSQR  = 20,  /* matrix not square */
  INS_ESING    = 21,  /* apparent singularity detected */
  INS_EDIVERGE = 22,  /* integral or series is divergent */
  INS_EUNSUP   = 23,  /* requested feature is not supported by the hardware */
  INS_EUNIMPL  = 24,  /* requested feature not (yet) implemented */
  INS_ECACHE   = 25,  /* cache limit exceeded */
  INS_ETABLE   = 26,  /* table limit exceeded */
  INS_ENOPROG  = 27,  /* iteration is not making progress towards solution */
  INS_ENOPROGJ = 28,  /* jacobian evaluations are not improving the solution */
  INS_ETOLF    = 29,  /* cannot reach the specified tolerance in F */
  INS_ETOLX    = 30,  /* cannot reach the specified tolerance in X */
  INS_ETOLG    = 31,  /* cannot reach the specified tolerance in gradient */
  INS_EOF      = 32   /* end of file */
} ;


// Similar to std:strerror()
const char* ins_strerror(const int error_code);

/* STREAM HANDLER */

// Insight stream handler type
typedef void (ins_stream_handler_t)(const char*, const char*, int, const char*);

// Sets the global variable `ins_stream_handler` to the `new_handler` and
// returns the previous stream handler.
ins_stream_handler_t*
ins_set_stream_handler(ins_stream_handler_t* new_handler);

// Sets the global variable `ins_stream` to the given `new_stream` and
// returns the previous stream.
FILE* ins_set_stream(FILE* new_stream);

// If the global variable `ins_stream_handler` is defined, then this
// function equivalent to invoking
//
//   `ins_stream_handler(label, file, line, reason)`.
//
// Otherwise, it's equivalent to
//
//   `fprintf(ins_stream, "gsl: %s:%d: %s: %s\n", file, line, label, reason)`
//
// in which `ins_stream` is a global variable pointing to a stream.
// (default to stderr).
void ins_stream_printf(const char* label, const char* file, int line,
                       const char* reason);

/* ERROR HANDLER */

// Insight error handler type
typedef void (ins_error_handler_t)(const char* reason, const char* file,
                                   int line, int error_code);

// Sets the global variable `ins_error_handler` to the `new_handler` and
// returns the previous handler.
ins_error_handler_t*
ins_set_error_handler(ins_error_handler_t* new_handler);

// Sets the global variable `ins_error_handler` to the "do nothing" handler
// and returns the previous handler.
ins_error_handler_t* ins_set_error_handler_off(void);

// If the global variable `ins_error_handler` is not NULL, then this
// function is equivalent to invoking
//
//   `ins_error_handler(reason, file, line, error_code)`
//
// Otherwise, it flushes the output to stderr.
void ins_error(const char* reason, const char* file, int line, int error_code);

/* CONVINIENT MACROS */

// INS_ERROR: call the error handler, and return the error code
#define INS_ERROR(reason, error_code)                   \
  do {                                                  \
    ins_error(reason, __FILE__, __LINE__, error_code);  \
    return error_code;                                  \
  } while (0)

// INS_ERROR_VAL: call the error handler, and return the given value
#define INS_ERROR_VAL(reason, error_code, value)        \
  do {                                                  \
    ins_error(reason, __FILE__, __LINE__, error_code);  \
    return value;                                       \
  } while (0)

// INS_ERROR_VOID: call the error handler, and return.
#define INS_ERROR_VOID(reason, error_code)              \
  do {                                                  \
    ins_error(reason, __FILE__, __LINE__, error_code);  \
    return;                                             \
  } while (0)

#endif /* INS_ERRNO_H_ */
