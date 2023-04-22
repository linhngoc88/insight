#if defined(INS_BASE_DOUBLE)
#define INS_BASE double
#define INS_FLOATING_POINT 1
#define INS_NUMERIC_INPUT_FORMAT "%lg"
#define INS_NUMERIC_OUT_FORMAT "%g"
#define INS_NUMERIC_ZERO 0.0
#define INS_NUMERIC_ONE 1.0
#elif defined(INS_BASE_FLOAT)
#define INS_BASE float
#define INS_FLOATING_POINT 1
#define INS_NUMERIC_INPUT_FORMAT "%g"
#define INS_NUMERIC_OUT_FORMAT "%g"
#define INS_NUMERIC_ZERO 0.0F
#define INS_NUMERIC_ONE 1.0F
#elif defined(INS_USE_NUMERIC_TYPE_INT)
#define INS_BASE int
#define INS_NUMERIC_INPUT_FORMAT "%d"
#define INS_NUMERIC_OUT_FORMAT "%d"
#define INS_NUMERIC_ZERO 0
#define INS_NUMERIC_ONE 1
#else
#error Unkown INS_USE_NUMERIC_TYPE_ DIRECTIVE
#endif

#define CONCAT2x(a,b) a ## _ ## b
#define CONCAT2(a,b) CONCAT2x(a,b)
#define CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define CONCAT3(a,b,c) CONCAT3x(a,b,c)

#if defined(INS_BASE_DOUBLE)
#define INS_FUNC(dir,name) CONCAT2(dir,name)
#define INS_TYPE(dir) dir
#else
#define INS_FUNC(dir,name) CONCAT3(dir,INS_BASE,name)
#define INS_TYPE(dir) CONCAT2(dir,INS_BASE)
#endif

//
// Convinient macros (should live at the bottom!)
//

#define INS_BLOCK_TYPE INS_TYPE(ins_block)
#define INS_BLOCK_FUNC(name) INS_FUNC(ins_block, name)

#define INS_VECTOR_TYPE INS_TYPE(ins_vector)
#define INS_VECTOR_FUNC(name) INS_FUNC(ins_vector, name)
