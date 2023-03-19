// Template for ins_vector_[atomic] types.

INS_VECTOR_TYPE *
INS_VECTOR_FUNC(alloc)(const size_t n) {
  INS_BLOCK_TYPE *block;
  INS_VECTOR_TYPE *vector;

  // Allocates space for the vector struct.
  vector = (INS_VECTOR_TYPE *) malloc(sizeof(INS_VECTOR_TYPE));

  // Calls the error handler and returns 0 if malloc failed.
  if (vector == 0) {
    INS_ERROR_VAL("failed to allocate space for vector", INS_ENOMEM, 0);
  }

  // Allocates space for the block struct.
  block = INS_BLOCK_FUNC(alloc)(n);

  // Frees the newly allocated vector, calls the error handler and returns
  // 0 if the block allocation failed.
  if (block == 0) {
    free(vector);
    INS_ERROR_VAL("failed to allocate space for block", INS_ENOMEM, 0);
  }

  vector->size = n;
  vector->stride = 1;
  vector->data = block->data;
  vector->block = block;
  vector->owner = 1;

  return vector;
}

INS_VECTOR_TYPE *
INS_VECTOR_FUNC(calloc)(const size_t n) {
  INS_BLOCK_TYPE *block;
  INS_VECTOR_TYPE *vector;

  // Allocates space for the vector struct
  vector = (INS_VECTOR_TYPE *) malloc(sizeof(INS_VECTOR_TYPE));

  if (vector == 0) {
    INS_ERROR_VAL("failed to allocate space for vector", INS_ENOMEM, 0);
  }

  // Allocates space for the underlying block and initializes all of the
  // block elements to zero.
  block = INS_BLOCK_FUNC(calloc)(n);

  // Frees the newly allocated vector, calls the error handler and returns
  // 0 if the block allocation failed.
  if (block == 0) {
    free(vector);
    INS_ERROR_VAL("failed to allocate space for block", INS_ENOMEM, 0);
  }

  vector->size = n;
  vector->stride = 1;
  vector->data = block->data;
  vector->block = block;
  vector->owner = 1;

  return vector;
}

INS_VECTOR_TYPE *
INS_VECTOR_FUNC(alloc_from_block)(ins_block * block,
                                 const size_t offset,
                                 const size_t n,
                                 const size_t stride) {
  INS_VECTOR_TYPE *vector;

  // Check to make sure that the given `stride` is a positive integer.
  if (stride == 0) {
    INS_ERROR_VAL("stride must be positive integer", INS_EINVAL, 0);
  }

  // Check to make sure that `block` has enough elements for the vector.
  // We have `v[i] = block[offset + i * stride]` for `i = 0, 1, ...n-1`,
  // therefore the index of the last vector element `offset + (n-1) * stride`
  // must less than `block->size`.
  if (block->size <= offset + (n > 0 ? n - 1 : 0) * stride) {
    INS_ERROR_VAL("vector would extend past the end of the input block",
                  INS_EINVAL, 0);
  }

  vector = (INS_VECTOR_TYPE *) malloc(sizeof(INS_VECTOR_TYPE));

  if (vector == 0) {
    INS_ERROR_VAL("failed to allocate space for vector", INS_ENOMEM, 0);
  }

  vector->size = n;
  vector->stride = stride;
  vector->data = block->data + offset;
  vector->block = block;
  vector->owner = 0;

  return vector;
}

INS_VECTOR_TYPE *
INS_VECTOR_FUNC(alloc_from_vector)(INS_VECTOR_TYPE * other,
                                   const size_t offset,
                                   const size_t n,
                                   const size_t stride) {
  INS_VECTOR_TYPE *vector;

  // Check to make sure the the given `stride` is a positive integer
  if (stride == 0) {
    INS_ERROR_VAL("stride must be a positive integer", INS_EINVAL, 0);
  }

  // Check to make sure that the `other` has enough enough elements for the
  // vector. We have `v[i] = other[offset + i * stride]`, for `i = 0, 1, n-1`,
  // therefore the index of the last vector element `offset + (n-1) * stride`
  // must less than `other->size`.
  if (other->size <= offset + (n > 0 ? n - 1 : 0) * stride) {
    INS_ERROR_VAL("vector would extend pass the end of the input vector",
                  INS_EINVAL, 0);
  }

  vector = (INS_VECTOR_TYPE *) malloc(sizeof(INS_VECTOR_TYPE));

  if (vector == 0) {
    INS_ERROR_VAL("failed to allocate space for vector", INS_ENOMEM, 0);
  }

  vector->size = n;
  vector->stride = stride * other->stride;
  vector->data = other->data + offset * other->stride;
  vector->block = other->block;
  vector->owner = 0;

  return vector;
}

void INS_VECTOR_FUNC(free)(INS_VECTOR_TYPE * vector) {
  if (vector == 0) {
    return;
  }

  if (vector->owner) {
    INS_BLOCK_FUNC(free)(vector->block);
  }

  free(vector);
}

void INS_VECTOR_FUNC(set_zero)(INS_VECTOR_TYPE * v) {
  INS_NUMERIC_TYPE * const data = v->data;
  const size_t size = v->size;
  const size_t stride = v->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    data[i * stride] = INS_NUMERIC_ZERO;
  }
}

void INS_VECTOR_FUNC(set_all)(INS_VECTOR_TYPE * v, double x) {
  INS_NUMERIC_TYPE * const data = v->data;
  const size_t size = v->size;
  const size_t stride = v->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    data[i * stride] = x;
  }
}

void INS_VECTOR_FUNC(set_basis)(INS_VECTOR_TYPE * v, size_t i) {
  INS_NUMERIC_TYPE * const data = v->data;
  const size_t size = v->size;
  const size_t stride = v->stride;

  int j;

  for (j = 0; j < size; ++j) {
    data[j * stride] = INS_NUMERIC_ZERO;
  }

  data[i * stride] = INS_NUMERIC_ONE;
}
