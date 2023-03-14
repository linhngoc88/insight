// Template for ins_block_[qualifier] types.

#define INS_BLOCK_TYPE INS_TYPE(ins_block)
#define INS_BLOCK_FUNC(name) INS_FUNC(ins_block, name)

// Allocates space for a block and return a pointer to the (empty) block struct.
// If allocation failed, call the error handler, and return 0 as the result.
static INS_BLOCK_TYPE * allocate_empty_block();

INS_BLOCK_TYPE * INS_BLOCK_FUNC(alloc)(const size_t count) {
  // Allocate memory for block struct.
  INS_BLOCK_TYPE * block = allocate_empty_block();
  if (block == 0) { return 0; }

  // Allocate memory for the block elements.
  block->data = (INS_NUMERIC_TYPE *) malloc(count * sizeof(INS_NUMERIC_TYPE));

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
  INS_BLOCK_TYPE * block = allocate_empty_block();
  if (block == 0) { return 0; }

  // Allocate memory for the block elements and initialize elements to 0
  block->data = (INS_NUMERIC_TYPE *) calloc(count, sizeof(INS_NUMERIC_TYPE));

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
  const size_t size = sizeof(INS_NUMERIC_TYPE);
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
  const size_t size = sizeof(INS_NUMERIC_TYPE);
  const size_t nitems_written = fwrite(block->data, size, nitems, stream);

  // If the number of items written to the stream is not equal to the number
  // of items in the block, call the error handler and return the error code
  // `INS_EFAILED`.
  if (nitems_written != nitems) {
    INS_ERROR("fwrite failed", INS_EFAILED);
  }

  return 0;
}

static INS_BLOCK_TYPE * allocate_empty_block() {
  // Allocate memory for block struct.
  INS_BLOCK_TYPE * block;
  block = (INS_BLOCK_TYPE *) malloc(sizeof(INS_BLOCK_TYPE));

  // If allocation failed, call the error handler, and return 0 as the result.
  if (block == 0) {
    INS_ERROR_VAL("failed to allocate space for block struct", INS_ENOMEM, 0);
  }

  return block;
}

#undef INS_BLOCK_TYPE
#undef INS_BLOCK_FUNC
