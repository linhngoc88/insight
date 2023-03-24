#ifndef INS_INTERNAL_INS_BLAS_H_
#define INS_INTERNAL_INS_BLAS_H_

#include "ins/internal/config.h"

#if defined(INSIGHT_USE_OPEN_BLAS)
#include <cblas.h>
#elif defined(INSIGHT_USE_ACCELERATE_BLAS)
#include <Accelerate.h>
#else
#error At least one BLAS option must be selected!
#endif

#endif // INS_INTERNAL_INS_BLAS_H_
