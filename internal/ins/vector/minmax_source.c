INS_NUMERIC_TYPE
INS_VECTOR_FUNC(min)(const INS_VECTOR_TYPE *v) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE min = v->data[0 * stride];
  INS_NUMERIC_TYPE cur;

  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur < min) {
      min = cur;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      return cur;
    }

#endif

  }

  return min;
}
