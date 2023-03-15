#include <ins/ins_block.h>

#define INS_USE_NUMERIC_TYPE_DOUBLE
#include "ins/templates_on.h"
#include "ins/block/init_source.c"
#include "ins/templates_off.h"
#undef INS_USE_NUMERIC_TYPE_DOUBLE

#define INS_USE_NUMERIC_TYPE_FLOAT
#include "ins/templates_on.h"
#include "ins/block/init_source.c"
#include "ins/templates_off.h"
#undef INS_USE_NUMERIC_TYPE_FLOAT

#define INS_USE_NUMERIC_TYPE_INT
#include "ins/templates_on.h"
#include "ins/block/init_source.c"
#include "ins/templates_off.h"
#undef INS_USE_NUMERIC_TYPE_INT
