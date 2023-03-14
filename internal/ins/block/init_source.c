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
