// Template for ins_block_[qualifier] types.

// Allocates space for a block and return a pointer to the (empty) block struct.
// If allocation failed, call the error handler, and return 0 as the result.
static INS_BLOCK_TYPE * INS_BLOCK_FUNC(allocate_empty)();

INS_BLOCK_TYPE * INS_BLOCK_FUNC(alloc)(const size_t count) {
  // Allocate memory for block struct.
  INS_BLOCK_TYPE * block = INS_BLOCK_FUNC(allocate_empty)();
  if (block == 0) { return 0; }

  // Allocate memory for the block elements.
  block->data = (INS_BASE *) malloc(count * sizeof(INS_BASE));

  // If block data allocation failed, free the allocated block, call the error
  // handler, and return 0 as the result.
  if (block->data == 0 && count > 0) {
    free(block);
    INS_ERROR_VAL("failed to allocate space for block data", INS_ENOMEM, 0);
  }

  block->size = count;
  return block;
}

INS_BLOCK_TYPE * INS_BLOCK_FUNC(calloc)(const size_t count) {
  // Allocate memory for block struct.
  INS_BLOCK_TYPE * block = INS_BLOCK_FUNC(allocate_empty)();
  if (block == 0) { return 0; }

  // Allocate memory for the block elements and initialize elements to 0
  block->data = (INS_BASE *) calloc(count, sizeof(INS_BASE));

  // If block data allocation failed, free the allocated block, call the error
  // handler, and return 0 as the result.
  if (block->data == 0 && count > 0) {
    free(block);
    INS_ERROR_VAL("failed to allocate space for block data", INS_ENOMEM, 0);
  }

  block->size = count;
  return block;
}

void INS_BLOCK_FUNC(free)(INS_BLOCK_TYPE * block) {
  if (block == 0) { return; }
  free(block->data);
  free(block);
}

int INS_BLOCK_FUNC(fread)(INS_BLOCK_TYPE * block, FILE * stream) {
  const size_t nitems = block->size;
  const size_t size = sizeof(INS_BASE);
  const size_t nitems_read = fread(block->data, size, nitems, stream);

  // If the number of items read from the stream is not equal to the number
  // of items preallocated in the block, call the error handler and return
  // the error code `INS_EFAILED`.
  if (nitems_read != nitems) {
    INS_ERROR("fread failed", INS_EFAILED);
  }

  return 0;
}

int INS_BLOCK_FUNC(fwrite)(const INS_BLOCK_TYPE * block, FILE * stream) {
  const size_t nitems = block->size;
  const size_t size = sizeof(INS_BASE);
  const size_t nitems_written = fwrite(block->data, size, nitems, stream);

  // If the number of items written to the stream is not equal to the number
  // of items in the block, call the error handler and return the error code
  // `INS_EFAILED`.
  if (nitems_written != nitems) {
    INS_ERROR("fwrite failed", INS_EFAILED);
  }

  return 0;
}

int INS_BLOCK_FUNC(fprintf)(const INS_BLOCK_TYPE * block, FILE * stream,
                            const char * format) {
  const size_t size = block->size;
  const INS_BASE * data = block->data;

  size_t i;

  for (i = 0; i < size; ++i) {
    // Writes the next element of the block to the stream. If fprintf fails,
    // call the error handler and return the status code `INS_EFAILED`.
    if (fprintf(stream, format, data[i]) < 0) {
      INS_ERROR("fprintf failed", INS_EFAILED);
    }

    // Writes the next newline character to the stream. If putc fails, call
    // the error handler and return the status code `INS_EFAILED`.
    if (putc('\n', stream) == EOF) {
      INS_ERROR("putc failed", INS_EFAILED);
    }
  }

  return INS_SUCCESS;
}

int INS_BLOCK_FUNC(fscanf)(INS_BLOCK_TYPE * block, FILE * stream) {
  const size_t size = block->size;

  size_t i;
  INS_BASE tmp;

  for (i = 0; i < size; ++i) {
    if (fscanf(stream, INS_INPUT_FORMAT, &tmp) != 1) {
      INS_ERROR("fscanf failed", INS_EFAILED);
    }
    block->data[i] = tmp;
  }

  return INS_SUCCESS;
}

static INS_BLOCK_TYPE * INS_BLOCK_FUNC(allocate_empty)() {
  // Allocate memory for block struct.
  INS_BLOCK_TYPE * block;
  block = (INS_BLOCK_TYPE *) malloc(sizeof(INS_BLOCK_TYPE));

  // If allocation failed, call the error handler, and return 0 as the result.
  if (block == 0) {
    INS_ERROR_VAL("failed to allocate space for block struct", INS_ENOMEM, 0);
  }

  return block;
}
