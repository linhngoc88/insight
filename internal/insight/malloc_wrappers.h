#ifndef INSIGHT_INTERNAL_MALLOC_WRAPPERS_H_
#define INSIGHT_INTERNAL_MALLOC_WRAPPERS_H_

#include <stdlib.h>

// Allocates enough space for `count` objects that are `size` bytes of memory
// each and returns a pointer to the allocated memory. The allocated memory
// is uninitialized.
void * insight_malloc(size_t count, size_t size);

// Allocates enough space for `count` objects that are `size` bytes of memory
// each and returns a pointer to the allocated memory. The allocated memory
// is filled with bytes of value zero.
void * insight_calloc(size_t count, size_t size);

// Deallocates the the memory allocation pointed to by `ptr`. If `ptr` is a
// NULL pointer, no operation is performed.
void insight_free(void *ptr);

#endif /* INSIGHT_INTERNAL_MALLOC_WRAPPERS_H_ */
