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
