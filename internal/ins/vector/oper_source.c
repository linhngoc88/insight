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

int INS_VECTOR_FUNC(scale)(INS_VECTOR_TYPE * x, INS_BASE alpha) {
  const size_t n = x->size;
  const size_t stride = x->stride;

#if defined(INS_BASE_DOUBLE)

  cblas_dscal(n, alpha, x->data, stride);

#elif defined(INS_BASE_FLOAT)

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
INS_VECTOR_FUNC(add_constant)(INS_VECTOR_TYPE * x, INS_BASE alpha) {
  const size_t size = x->size;
  const size_t stride = x->stride;

  size_t i;

  for (i = 0; i < size; ++i) {
    x->data[i * stride] += alpha;
  }

  return INS_SUCCESS;
}

INS_BASE
INS_VECTOR_FUNC(sum)(const INS_VECTOR_TYPE * x) {
  const size_t size = x->size;
  const size_t stride = x->stride;

  size_t i;
  INS_BASE sum = INS_NUMERIC_ZERO;

  for (i = 0; i < size; ++i) {
    sum += x->data[i * stride];
  }

  return sum;
}

int
INS_VECTOR_FUNC(axpy)(INS_BASE alpha,
                      const INS_VECTOR_TYPE * x,
                      INS_VECTOR_TYPE * y) {
  const size_t size = x->size;

  if (y->size != size) {
    INS_ERROR("vectors must have same length", INS_EBADLEN);
  }

  const size_t x_stride = x->stride;
  const size_t y_stride = y->stride;

#if defined(INS_BASE_DOUBLE)

  cblas_daxpy(size, alpha, x->data, x_stride, y->data, y_stride);

#elif defined(INS_BASE_FLOAT)

  cblas_saxpy(size, alpha, x->data, x_stride, y->data, y_stride);

#else

  size_t i ;

  for (i = 0; i < size; ++i) {
    y->data[i * y_stride] += alpha * x->data[i * x_stride];
  }

#endif

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(swap)(INS_VECTOR_TYPE * v, INS_VECTOR_TYPE * w) {
  const size_t size = v->size;

  if (w->size != size) {
    INS_ERROR("vectors must have same length", INS_EINVAL);
  }

  const size_t v_stride = v->stride;
  const size_t w_stride = w->stride;

  INS_BASE * const v_data = v->data;
  INS_BASE * const w_data = w->data;

#if defined(INS_BASE_DOUBLE)

  cblas_dswap(size, v_data, v_stride, w_data, w_stride);

#elif defined(INS_BASE_FLOAT)

  cblas_sswap(size, v_data, v_stride, w_data, w_stride);

#else

  size_t i;
  INS_BASE tmp;

  for (i = 0; i < size; ++i) {
    tmp = v_data[i * v_stride];
    v_data[i * v_stride] = w_data[i * w_stride];
    w_data[i * w_stride] = tmp;
  }

#endif

  return INS_SUCCESS;
}

int
INS_VECTOR_FUNC(copy)(INS_VECTOR_TYPE *dst, const INS_VECTOR_TYPE *src) {
  const size_t size = src->size;

  if (dst->size != size) {
    INS_ERROR("vectors must have same length", INS_EINVAL);
  }

  const size_t src_stride = src->stride;
  const size_t dst_stride = dst->stride;

  const INS_BASE * src_data = src->data;
  INS_BASE * const dst_data = dst->data;

#if defined(INS_BASE_DOUBLE)

  cblas_dcopy(size, src_data, src_stride, dst_data, dst_stride);

#elif defined(INS_BASE_FLOAT)

  cblas_scopy(size, src_data, src_stride, dst_data, dst_stride);

#else

  size_t i;

  for (i = 0; i < size; ++i) {
    dst_data[i * dst_stride] = src_data[i * src_stride];
  }

#endif

  return INS_SUCCESS;
}

INS_BASE
INS_VECTOR_FUNC(dot)(const INS_VECTOR_TYPE *v, const INS_VECTOR_TYPE *w) {
  const size_t size = v->size;

  if (w->size != size) {
    INS_ERROR("vectors must have same length", INS_EINVAL);
  }

  const size_t v_stride = v->stride;
  const size_t w_stride = w->stride;

  const INS_BASE * v_data = v->data;
  const INS_BASE * w_data = w->data;

#if defined(INS_BASE_DOUBLE)

  return cblas_ddot(size, v_data, v_stride, w_data, w_stride);

#elif defined(INS_BASE_FLOAT)

  return cblas_sdot(size, v_data, v_stride, w_data, w_stride);

#else

  size_t i;
  INS_BASE ret = INS_NUMERIC_ZERO;

  for (i = 0; i < size; ++i) {
    ret += v_data[i * v_stride] * w_data[i * w_stride];
  }

  return ret;

#endif
}

INS_BASE
INS_VECTOR_FUNC(nrm2)(const INS_VECTOR_TYPE *v) {
  const size_t size = v->size;
  const size_t stride = v->stride;
  const INS_BASE * data = v->data;

#if defined(INS_BASE_DOUBLE)

  return cblas_dnrm2(size, data, stride);

#elif defined(INS_BASE_FLOAT)

  return cblas_snrm2(size, data, stride);

#else
#error Unsupport operation
#endif
}
