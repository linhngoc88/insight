#include "ins/ins_vector.h"
#include "ins/ins_blas.h"

#define INS_BASE_DOUBLE
#include "ins/templates_on.h"
#include "ins/vector/oper_source.c"
#include "ins/templates_off.h"
#undef INS_BASE_DOUBLE
