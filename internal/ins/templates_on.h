#if defined(INS_BASE_DOUBLE)
#define INS_BASE double
#define INS_SHORT double
#define INS_FLOATING_POINT 1
#define INS_INPUT_FORMAT "%lg"
#define INS_OUTPUT_FORMAT "%g"
#define INS_ZERO 0.0
#define INS_ONE 1.0
#elif defined(INS_BASE_FLOAT)
#define INS_BASE float
#define INS_SHORT float
#define INS_FLOATING_POINT 1
#define INS_INPUT_FORMAT "%g"
#define INS_OUTPUT_FORMAT "%g"
#define INS_ZERO 0.0F
#define INS_ONE 1.0F
#elif defined(INS_BASE_INT)
#define INS_BASE int
#define INS_SHORT int
#define INS_INPUT_FORMAT "%d"
#define INS_OUTPUT_FORMAT "%d"
#define INS_ZERO 0
#define INS_ONE 1
#else
#error Unkown INS_BASE_ DIRECTIVE
#endif

#define CONCAT2x(a,b) a ## _ ## b
#define CONCAT2(a,b) CONCAT2x(a,b)
#define CONCAT3x(a,b,c) a ## _ ## b ## _ ## c
#define CONCAT3(a,b,c) CONCAT3x(a,b,c)
#define CONCAT4x(a,b,c,d) a ## _ ## b ## _ ## c ## _ ## d
#define CONCAT4(a,b,c,d) CONCAT4x(a,b,c,d)

#ifndef INS_USE_QUALIFIER
#define INS_QUALIFIER
#endif

#ifdef INS_USE_QUALIFIER
#if defined(INS_BASE_DOUBLE)
#define INS_FUNC(dir,name) CONCAT3(dir,INS_QUALIFIER,name)
#define INS_TYPE(dir) dir
#define INS_VIEW(dir,name) CONCAT2(dir,name)
#define INS_QUALIFIED_TYPE(dir) INS_QUALIFIER dir
#define INS_QUALIFIED_VIEW(dir,name) CONCAT3(dir,INS_QUALIFIER,name)
#else // INS_BASE_DOUBLE
#define INS_FUNC(dir,name) CONCAT4(dir,INS_SHORT,INS_QUALIFIER,name)
#define INS_TYPE(dir) CONCAT2(dir,INS_SHORT)
#define INS_VIEW(dir,name) CONCAT3(dir,INS_SHORT,name)
#define INS_QUALIFIED_TYPE(dir) INS_QUALIFIER CONCAT2(dir,INS_SHORT)
#define INS_QUALIFIED_VIEW(dir,name) CONCAT4(dir,INS_SHORT,INS_QUALIFIER,name)
#endif
#else // INS_USE_QUALIFIER
#if defined(INS_BASE_DOUBLE)
#define INS_FUNC(dir,name) CONCAT2(dir,name)
#define INS_TYPE(dir) dir
#define INS_VIEW(dir,name) CONCAT2(dir,name)
#define INS_QUALIFIED_TYPE(dir) INS_TYPE(dir)
#define INS_QUALIFIED_VIEW(dir,name) CONCAT2(dir,name)
#else // INS_BASE_DOUBLE
#define INS_FUNC(dir,name) CONCAT3(dir,INS_SHORT,name)
#define INS_TYPE(dir) CONCAT2(dir,INS_SHORT)
#define INS_VIEW(dir,name) CONCAT3(dir,INS_SHORT,name)
#define INS_QUALIFIED_TYPE(dir) INS_TYPE(dir)
#define INS_QUALIFIED_VIEW(dir,name) CONCAT3(dir,INS_SHORT,name)
#endif
#endif // INS_USE_QUALIFIER

//
// Convinient macros (should live at the bottom!)
//

#define INS_BLOCK_TYPE INS_TYPE(ins_block)
#define INS_BLOCK_FUNC(name) INS_FUNC(ins_block, name)

#define INS_VECTOR_TYPE INS_TYPE(ins_vector)
#define INS_VECTOR_FUNC(name) INS_FUNC(ins_vector, name)
