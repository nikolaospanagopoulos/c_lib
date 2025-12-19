#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum VECTOR_ERRORS {
  OK = 0,
  ALLOC_ERROR,
  OUT_OF_BOUNDS_ERROR,
  VECTOR_EMPTY,
  VECTOR_NULL_PTR,
  NOT_FOUND,
  FUNCTION_MISSING
};
struct vector {
  size_t capacity;
  void *memory;
  size_t datatype_size;
  size_t size;
  enum VECTOR_ERRORS (*push)(struct vector *vec, void *to_copy);
  size_t (*get_size)(struct vector *vec);
  enum VECTOR_ERRORS (*copy)(void *dest, void *src);
  void (*free_vec)(struct vector *vec);
};
static enum VECTOR_ERRORS vector_expand(struct vector *vec);
enum VECTOR_ERRORS vector_push(struct vector *vec, void *to_copy);
enum VECTOR_ERRORS vector_init(struct vector **vec, size_t capacity,
                               size_t datatype_size);
