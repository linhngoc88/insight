int INS_VECTOR_FUNC(add)(INS_VECTOR_TYPE * x, const INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * x_stride] += y->data[i * y_stride];
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(sub)(INS_VECTOR_TYPE * x, const INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * x_stride] -= y->data[i * y_stride];
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(mul)(INS_VECTOR_TYPE * x, const INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * x_stride] *= y->data[i * y_stride];
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(div)(INS_VECTOR_TYPE * x, const INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * x_stride] /= y->data[i * y_stride];
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(scale)(INS_VECTOR_TYPE * x, INS_NUMERIC_TYPE alpha) {
  const size_t n = x->size;
  const size_t stride = x->stride;

#if defined(INS_USE_NUMERIC_TYPE_DOUBLE)

  cblas_dscal(n, alpha, x->data, stride);

#elif defined(INS_USE_NUMERIC_TYPE_FLOAT)

  cblas_sscal(n, alpha, x->data, stride);

#else

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * stride] *= alpha;
  }

#endif

  return INS_SUCCESS;
}

int
INS_VECTOR_FUNC(add_constant)(INS_VECTOR_TYPE * x, INS_NUMERIC_TYPE alpha) {
  const size_t size = x->size;
  const size_t stride = x->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * stride] += alpha;
  }

  return INS_SUCCESS;
}

INS_NUMERIC_TYPE
INS_VECTOR_FUNC(sum)(const INS_VECTOR_TYPE * x) {
  const size_t size = x->size;
  const size_t stride = x->stride;

  size_t i;
  INS_NUMERIC_TYPE sum = INS_NUMERIC_ZERO;

  for (i = 0; i < size; ++i) {
    sum += x->data[i * stride];
  }

  return sum;
}

int
INS_VECTOR_FUNC(axpy)(INS_NUMERIC_TYPE alpha,
                      const INS_VECTOR_TYPE * x,
                      INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

#if defined(INS_USE_NUMERIC_TYPE_DOUBLE)

  cblas_daxpy(size, alpha, x->data, x_stride, y->data, y_stride);

#elif defined(INS_USE_NUMERIC_TYPE_FLOAT)

  cblas_saxpy(size, alpha, x->data, x_stride, y->data, y_stride);

#else

  size_t i ;

  for (i = 0; i < size; ++i) {
    y->data[i * y_stride] += alpha * x->data[i * x_stride];
  }

#endif

  return INS_SUCCESS;
}
