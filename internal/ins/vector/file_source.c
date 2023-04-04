int INS_VECTOR_FUNC(fread)(INS_VECTOR_TYPE *v, FILE *stream) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  const size_t elem_size = sizeof(INS_NUMERIC_TYPE);
  size_t num_elems;

  if (stride == 1) {
    num_elems = fread(v->data, elem_size, size, stream);
    if (num_elems != size) {
      INS_ERROR("fread failed", INS_EFAILED);
    }
  } else {
    size_t i;

    // TODO(linh): Call `fread` inside a loop? What if the size of the
    // vector is big, say, order of million elements?
    for (i = 0; i < size; ++i) {
      num_elems = fread(v->data + i * stride, elem_size, 1, stream);
      if (num_elems != 1) {
        INS_ERROR("fread failed", INS_EFAILED);
      }
    }
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(fwrite)(const INS_VECTOR_TYPE *v, FILE *stream) {
  const size_t size = v->size;
  const size_t stride = v->stride;

  const size_t elem_size = sizeof(INS_NUMERIC_TYPE);
  size_t num_elems;

  if (stride == 1) {
    num_elems = fwrite(v->data, elem_size, size, stream);
    if (num_elems != size) {
      INS_ERROR("fwrite failed", INS_EFAILED);
    }
  } else {
    size_t i;

    // TODO(linh): Call `fwrite` inside a loop? What if the size of the
    // vector is big, say, order of million elements?
    for (i = 0; i < size; ++i) {
      num_elems = fwrite(v->data + i * stride, elem_size, 1, stream);
      if (num_elems != 1) {
        INS_ERROR("fwrite failed", INS_EFAILED);
      }
    }
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(fprintf)(const INS_VECTOR_TYPE *v,
                             FILE *stream,
                             const char *format) {
  const size_t size = v->size;
  const size_t stride = v->stride;
  const INS_NUMERIC_TYPE *data = v->data;

  size_t i;

  for (i = 0; i < size; ++i) {
    if (fprintf(stream, format, data[i * stride]) < 0) {
      INS_ERROR("fprintf failed", INS_EFAILED);
    }

    if (putc('\n', stream) == EOF) {
      INS_ERROR("putc failed", INS_EFAILED);
    }
  }

  return INS_SUCCESS;
}

int INS_VECTOR_FUNC(fscanf)(INS_VECTOR_TYPE *v, FILE *stream) {
  const size_t size = v->size;
  const size_t stride = v->stride;
  INS_NUMERIC_TYPE * const data = v->data;

  size_t i;
  INS_NUMERIC_TYPE tmp;

  for (i = 0; i < size; ++i) {
    if (fscanf(stream, INS_NUMERIC_INPUT_FORMAT, &tmp) != 1) {
      INS_ERROR("fscanf failed", INS_EFAILED);
    }
    data[i * stride] = tmp;
  }

  return INS_SUCCESS;
}
