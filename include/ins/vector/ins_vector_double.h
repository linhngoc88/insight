#ifndef INS_VECTOR_DOUBLE_H_
#define INS_VECTOR_DOUBLE_H_

#include <stdlib.h>
#include <ins/ins_errno.h>
#include <ins/block/ins_block_double.h>

struct ins_vector_struct {
  // Number of elements in the vector.
  size_t size;

  // The step-size from one element to the next in physical memory. `stride`
  // should be `1` if elements of the vector are sitting right next to each
  // other without any gaps in between, and it should be greater than `1`
  // otherwise. The following is an example of a vector that has 3 elements
  // and 2 as its stride:
  //
  //   +------+------+------+------+------+
  //   | 1.0  | xxxx | 2.0  | xxxx | 3.0  |
  //   +------+------+------+------+------+
  //   ^             ^
  //   | stride = 2  | it takes two doubles to get from the first element to
  //   +-------------+ the second element.
  size_t stride;

  // The location of the first element of the vector in memory.
  double * data;

  // The location of the memory block in which the vector elemenst are located
  // (if any). If the vector owns this block then the `owner` field is set to
  // one and the block will be deallocated when the vector is freed. If the
  // vector points to a block owned by another object then the `owner` field
  // is set to zero and the underlying block will not be deallcoated with the
  // vector.
  ins_block * block;
  int owner;
};

typedef struct ins_vector_struct ins_vector;

/* Allocation
 --------------------------------------------------------------------------*/

// Creates a vector of length `n` and returns a pointer to the newly created
// vector struct. A new block is allocated for the elements of the vector, and
// stored in the `block` component of the vector struct. The block is "owned"
// by the vector and will be deallocated when the vector is freed.
// Zero-size requests are valid and return a non-null result.
ins_vector * ins_vector_alloc(const size_t n);

// Allocates memory for a vector of lenght `n` and initializes all the elements
// of the vector to zero.
ins_vector * ins_vector_calloc(const size_t n);

// Allocates memory for a vector of length `n` and returns a pointer to the
// newly created block struct. The vector shares its elements with the given
// block `b` starting at the given `offset`, and the distance between two
// consecutive elements are given by `stride`. In other words,
//
//   `v[i] = b[offset + i * stride] for i = 0, 1, ... n-1`.
//
// The block `b` will not be deallocated when the vector is freed.
ins_vector *
ins_vector_alloc_from_block(ins_block * b,
                            const size_t offset,
                            const size_t n,
                            const size_t stride);

// Allocates memory for a vector of length `n` and returns a pointer to the
// newly created block struct. The vector shares its elements with another
// vector `v` starting at the given `offset`, and the distance between two
// consecutive elements are given by `stride`. In other words,
//
//   `v'[i] = v[offset + i * stride] for i = 0, 1, ... n-1`.
//
// The underlying block owned by the input vector `v` will not be deallocated
// when the output vector is freed.
ins_vector *
ins_vector_alloc_from_vector(ins_vector * v,
                             const size_t offset,
                             const size_t n,
                             const size_t stride);

// Frees a previously allocated vector `v`. If the vector was created using
// `ins_vector_alloc` or `ins_vector_calloc` then the underlying block will
// also be deallocated. If the vector has been created from another object
// the the memory is still owned by that object and will not be deallocated.
void ins_vector_free(ins_vector * v);

// A function-like macro that returns the element of the vector at the
// specified index.
// TODO(linh): how about make it as an inline function instead?
#define ins_vector_get(v, i) ((v)->data[(i) * (v)->stride])

// A function-like macro that sets the element of the vector at the
// specified index to some new value.
// TODO(linh): how about make it as an inline function instead?
#define ins_vector_set(v, i, x) (v)->data[(i) * (v)->stride] = (x);

/* Initializing vector elements
 --------------------------------------------------------------------------*/

// Set all elements of the vector `v` to zero.
void ins_vector_set_zero(ins_vector * v);

// Set all elements of the vector `v` to the value `x`.
void ins_vector_set_all(ins_vector * v, double x);

// Set all elements of the vector `v` to zero except for the i-th element
// which is set to one. No bounds checking are performed, therefore clients
// have to make sure that the given index `i` is within bounds, i.e.,
// `0 <= i < v->size`.
void ins_vector_set_basis(ins_vector * v, size_t i);

/* Vector operations
 ---------------------------------------------------------------------------*/

// Adds the elements of the vector `y` to the elements of the vector `x`.
// The result `x_i <- x_i + y_i` is stored in `x` and `y` remains unchanged.
// The two vectors must have the same length.
int ins_vector_add(ins_vector * x, const ins_vector * y);

// Subtracts the elements of the vector `y` from the elements of the vector
// `x`. The result `x_i <- x_i - y_i` is stored in `x` and `y` remains
// unchanged. The two vectors must have the same length.
int ins_vector_sub(ins_vector * x, const ins_vector * y);


// Multiplies the elements of the vector `y` by the elements of the vector
// `x`. The result `x_i <- x_i * y_i` is stored in `x` and `y` remains
// unchanged. The two vectors must have the same length.
int ins_vector_mul(ins_vector * x, const ins_vector * y);

// Divides the elements of the vector `x` by the elements of the vector `y`.
// The result `x_i <- x_i / y_i` is stored in `x` and `y` remains unchanged.
// The two vectors must have the same length.
int ins_vector_div(ins_vector * x, const ins_vector * y);

// Multiplies the elements of the vector `x` by a constant factor `alpha`.
// The result `x_i <- alpha * x_i` is stored in `x`.
int ins_vector_scale(ins_vector * x, double alpha);

// Adds the constant value `alpha` to the elements of the vector `x`. The
// result `x_i <- x_i + alpha` is stored in x.
int ins_vector_add_constant(ins_vector * x, double alpha);

// Returns the sum of the emements of the vector `x`.
double ins_vector_sum(const ins_vector * x);

// Performs the operation `y <- alpha * x + y`. The vectors `x` and `y` must
// have the same length.
int ins_vector_axpy(double alpha, const ins_vector * x, ins_vector * y);

// Exchanges the elements of the vectors `v` and `w` by copying. The two
// vectors must have the same length. The function returns `INS_SUCCESS`
// for success and `INS_EINVAL` if two vectors have different lengths.
int ins_vector_swap(ins_vector *v, ins_vector *w);

// Copies the elements of the vector `src` into the vector `dest`. The two
// vectors must have the same length. The return value is `INS_SUCCESS`
// for success and `INS_EINVAL` if two vectors have different lengths.
int ins_vector_copy(ins_vector *dest, const ins_vector *src);

// Computes the dot product of the two vectors `v` and `w`. The two vectors
// must have the same length.
double ins_vector_dot(const ins_vector *v, const ins_vector *w);

// Computes and returns the Euclidean of the vector `v`.
double ins_vector_nrm2(const ins_vector *);

/* Maximum and mininum elements
   -----------------------------------------------------------------------*/

// Returns the minimum value in the vector `v`.
double ins_vector_min(const ins_vector *v);

// Returns the maximum value in the vector `v`.
double ins_vector_max(const ins_vector *v);

// Returns the minimum and the maximum values in the vector `v`, storing
// them in `min_out` and `max_out`, respectively.
void ins_vector_minmax(const ins_vector *v, double *min_out, double *max_out);

// Returns the index of the minimum value in the vector `v`. When there
// are several equal minimum elements then the lowest index is returned.
size_t ins_vector_min_index(const ins_vector *v);

// Returns the index of the maximum value in the vector `v`. When there
// are several equal maximum elements then the lowest index is returned.
size_t ins_vector_max_index(const ins_vector *v);

// Returns the indices of the minimum and the maximum values in the vector
// `v`, storing them in `imin_out` and `imax_out`, respectively.
// When there are several equal minimum or maximum elements then the lowest
// indices are returned.
void ins_vector_minmax_index(const ins_vector * v,
                             size_t * imin_out,
                             size_t * imax_out);

/* Reading and writing vectors
   -----------------------------------------------------------------------*/

// Reads into the vector `v` from the open stream `stream` in binary format.
// The vector `v` must be preallocated with the correct length since the
// function uses the size of `v` to determine how many bytes to read.
// The return value is `INS_SUCCESS` for success and `INS_EFAILED` if there
// was a problem reading from the file.
int ins_vector_fread(ins_vector *v, FILE *stream);

// Writes the elements of the vector `v` to the stream `stream` in binary
// format. The return value is `INS_SUCCESS` for success and `INS_EFAILED`
// if there was a problem writing to the file.
int ins_vector_fwrite(const ins_vector *v, FILE *stream);

// Writes the elements of the vector `v` line-by-line to the open stream
// `stream` using the format specifier `format`, which should be one of
// `%g`, `%e`, or `%f` formats for floating point numbers and `%d` for
// integers. The function returns `INS_SUCCESS` for success and `INS_EFAILED`
// if there was a problem writing to the file.
int ins_vector_fprintf(const ins_vector *v, FILE *stream, const char *format);

// Reads formatted data from the stream `stream` into the vector `v`. The
// vector `v` must be preallocated with the correct length since the function
// uses the size of `v` to determine how many bytes to read. The function
// returns `INS_SUCCESS` for success and `INS_EFAILED` if there was a problem
// reading from the file.
int ins_vector_fscanf(ins_vector *v, FILE *stream);

#endif  // INS_VECTOR_DOUBLE_H_
