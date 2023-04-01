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

INS_NUMERIC_TYPE
INS_VECTOR_FUNC(max)(const INS_VECTOR_TYPE *v) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE max = v->data[0 * stride];
  INS_NUMERIC_TYPE cur;

  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur > max) {
      max = cur;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      return cur;
    }

#endif

  }

  return max;
}

void
INS_VECTOR_FUNC(minmax)(const INS_VECTOR_TYPE * v,
                        INS_NUMERIC_TYPE * min_out,
                        INS_NUMERIC_TYPE * max_out) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE min = v->data[0 * stride];
  INS_NUMERIC_TYPE max = min;

  INS_NUMERIC_TYPE cur;
  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur < min) {
      min = cur;
    }

    if (cur > max) {
      max = cur;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      min = cur;
      max = cur;
      break;
    }

#endif

  }

  *min_out = min;
  *max_out = max;
}

size_t
INS_VECTOR_FUNC(min_index)(const INS_VECTOR_TYPE * v) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE min = v->data[0 * stride];
  INS_NUMERIC_TYPE cur;

  size_t imin = 0;
  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur < min) {
      min = cur;
      imin = i;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      return i;
    }

#endif

  }

  return imin;
}

size_t
INS_VECTOR_FUNC(max_index)(const INS_VECTOR_TYPE * v) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE max = v->data[0 * stride];
  INS_NUMERIC_TYPE cur;

  size_t imax = 0;
  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur > max) {
      max = cur;
      imax = i;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      return i;
    }

#endif

  }

  return imax;
}

void
INS_VECTOR_FUNC(minmax_index)(const INS_VECTOR_TYPE * v,
                              size_t * imin_out,
                              size_t * imax_out) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  INS_NUMERIC_TYPE min = v->data[0 * stride];
  INS_NUMERIC_TYPE max = min;
  INS_NUMERIC_TYPE cur;

  size_t imin = 0;
  size_t imax = 0;
  size_t i;

  for (i = 0; i < size; ++i) {
    cur = v->data[i * stride];

    if (cur < min) {
      min = cur;
      imin = i;
    }

    if (cur > max) {
      max = cur;
      imax = i;
    }

#ifdef INS_FLOATING_POINT

    if (isnan(cur)) {
      imin = i;
      imax = i;
      break;
    }

#endif

  }

  *imin_out = imin;
  *imax_out = imax;
}
