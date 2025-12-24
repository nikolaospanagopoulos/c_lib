#include "vector.h"
#include <stdio.h>
#include <string.h>

size_t get_size(struct vector *vec) { return vec->size; }

static enum VECTOR_ERRORS vector_expand(struct vector *vec) {
  vec->capacity *= 2;
  void *tmp = realloc(vec->memory, vec->capacity * vec->datatype_size);
  if (tmp == NULL) {
    return ALLOC_ERROR;
  }
  vec->memory = tmp;
  return OK;
}
enum VECTOR_ERRORS vector_insert(struct vector *vec, int position,
                                 void *to_copy) {

  if (vec == NULL || to_copy == NULL) {
    return NULL_PTR;
  }
  if (vec->capacity == vec->size) {
    printf("capacity: %lu, size: %lu\n", vec->capacity, vec->size);
    vector_expand(vec);
  }
  if (position >= vec->size) {
    return OUT_OF_BOUNDS_ERROR;
  }

  if (position == 0) {
    return vec->push_front(vec, to_copy);
  }
  if (position == vec->size - 1) {
    return vec->push(vec, to_copy);
  }
  vec->size++;
  for (int i = vec->size - 1; i > position; i--) {
    memcpy((char *)vec->memory + (vec->datatype_size * i),
           (char *)vec->memory + (vec->datatype_size * (i - 1)),
           vec->datatype_size);
  }
  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory + (vec->datatype_size * position), to_copy);
  } else {
    memcpy((char *)vec->memory + (vec->datatype_size * position), to_copy,
           vec->datatype_size);
  }

  return OK;
}
enum VECTOR_ERRORS vector_push_front(struct vector *vec, void *to_copy) {
  if (vec->capacity == vec->size) {
    printf("capacity: %lu, size: %lu\n", vec->capacity, vec->size);
    vector_expand(vec);
  }
  vec->size++;
  for (int i = vec->size - 1; i > 0; i--) {
    memcpy((char *)vec->memory + (vec->datatype_size * i),
           (char *)vec->memory + (vec->datatype_size * (i - 1)),
           vec->datatype_size);
  }

  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory, to_copy);
  } else {
    memcpy((char *)vec->memory, to_copy, vec->datatype_size);
  }
  return OK;
}

enum VECTOR_ERRORS vector_push(struct vector *vec, void *to_copy) {
  if (vec->capacity == vec->size) {
    printf("capacity: %lu, size: %lu\n", vec->capacity, vec->size);
    vector_expand(vec);
  }
  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory + (vec->size * vec->datatype_size), to_copy);
  } else {
    memcpy((char *)vec->memory + (vec->size * vec->datatype_size), to_copy,
           vec->datatype_size);
  }
  vec->size++;
  return OK;
}

enum VECTOR_ERRORS vector_init(struct vector **vec, size_t capacity,
                               size_t datatype_size) {
  *vec = malloc(sizeof(struct vector));
  if (*vec == NULL) {
    return ALLOC_ERROR;
  }
  (*vec)->capacity = capacity;
  (*vec)->push = vector_push;
  (*vec)->datatype_size = datatype_size;
  (*vec)->size = 0;
  (*vec)->push_front = vector_push_front;
  (*vec)->free_vec = NULL;
  (*vec)->copy = NULL;
  (*vec)->get_size = get_size;
  (*vec)->memory = malloc(datatype_size * capacity);
  if ((*vec)->memory == NULL) {
    return ALLOC_ERROR;
  }
  return OK;
}
