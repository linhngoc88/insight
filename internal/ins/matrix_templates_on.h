#if defined(INS_USE_NUMERIC_TYPE_INT)
#define INS_MATRIX_ELEM_TYPE int
#define INS_MATRIX_ELEM_ZERO 0
#define INS_MATRIX_ELEM_ONE 1
#define INS_MATRIX_INPUT_FORMAT "%d"
#define INS_MATRIX_OUTPUT_FORMAT "%d"
#else
#error Unkown INS_USE_NUMERIC_TYPE_ DIRECTIVE
#endif

#define CONCAT2x(a,b) a ## _ ## b
#define CONCAT2(a,b) CONCAT2x(a,b)
#define CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define CONCAT3(a,b,c) CONCAT3x(a,b,c)

#define INS_MATRIX_TYPE CONCAT2(ins_matrix, INS_MATRIX_ELEM_TYPE)
#define INS_MATRIX_FUNC(name) CONCAT3(ins_matrix, INS_MATRIX_ELEM_TYPE, name)
