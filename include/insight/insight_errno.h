#ifndef INSIGHT_ERRNO_H_
#define INSIGHT_ERRNO_H_

#include <stdio.h>
#include <errno.h>

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
const char * insight_strerror(const int insight_errno);

#endif /* INSIGHT_ERRNO_H_ */
