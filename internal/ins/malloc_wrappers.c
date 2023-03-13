#include "ins/malloc_wrappers.h"

void* ins_malloc(size_t count, size_t size) {
  return malloc(count * size);
}

void* ins_calloc(size_t count, size_t size) {
  return calloc(count, size);
}

void ins_free(void* ptr) {
  free(ptr);
}
