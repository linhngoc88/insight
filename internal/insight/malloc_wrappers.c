#include "insight/malloc_wrappers.h"

void* insight_malloc(size_t count, size_t size) {
  return malloc(count * size);
}

void* insight_calloc(size_t count, size_t size) {
  return calloc(count, size);
}

void insight_free(void* ptr) {
  free(ptr);
}
