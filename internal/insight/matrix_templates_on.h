#if defined(INSIGHT_USE_NUMERIC_TYPE_INT)
#define INSIGHT_MATRIX_ELEM_TYPE int
#define INSIGHT_MATRIX_ELEM_ZERO 0
#define INSIGHT_MATRIX_ELEM_ONE 1
#define INSIGHT_MATRIX_INPUT_FORMAT "%d"
#define INSIGHT_MATRIX_OUTPUT_FORMAT "%d"
#else
#error Unkown INSIGHT_USE_NUMERIC_TYPE_ DIRECTIVE
#endif

#define CONCAT2x(a,b) a ## _ ## b
#define CONCAT2(a,b) CONCAT2x(a,b)
#define CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define CONCAT3(a,b,c) CONCAT3x(a,b,c)

#define INSIGHT_MATRIX_TYPE                         \
  CONCAT2(insight_matrix, INSIGHT_MATRIX_ELEM_TYPE)

#define INSIGHT_MATRIX_FUNC(name)                           \
  CONCAT3(insight_matrix, INSIGHT_MATRIX_ELEM_TYPE, name)
