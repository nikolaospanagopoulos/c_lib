#include "vector.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

/*
TODO:
1. create a struct example
2. start implementing algorithms
3. do error handling in examples
4. check different datatypes are handled correctly
5. check without copy fn
*/

int get_size(struct vector *vec, size_t *size) {
  if (vec == NULL || size == NULL) {
    return -EINVAL;
  }
  *size = vec->size;
  return 0;
}

int shift_left(struct vector *vec) {
  if (vec == NULL) {
    return -EINVAL;
  }
  void *first = malloc(vec->datatype_size);
  if (first == NULL) {
    return -ENOMEM;
  }
  memcpy(first, vec->memory, vec->datatype_size);
  memmove((char *)vec->memory, (char *)vec->memory + vec->datatype_size,
          (vec->size - 1) * vec->datatype_size);

  memcpy((char *)vec->memory + (vec->datatype_size * (vec->size - 1)), first,
         vec->datatype_size);

  free(first);

  return 0;
}

static int vector_expand(struct vector *vec) {
  size_t new_capacity;
  void *new_allocated_mem;
  if (!vec) {
    return -EINVAL;
  }
  if (vec->capacity == 0) {
    new_capacity = 1;
  } else {
    new_capacity = vec->capacity * 2;
  }
  new_allocated_mem = realloc(vec->memory, new_capacity * vec->datatype_size);
  if (new_allocated_mem == NULL) {
    return -ENOMEM;
  }
  vec->memory = new_allocated_mem;
  vec->capacity = new_capacity;
  return 0;
}
int vector_insert(struct vector *vec, size_t position, void *to_copy) {

  if (vec == NULL || to_copy == NULL) {
    return -EINVAL;
  }
  if (position > vec->size) {
    return -ERANGE;
  }
  void *src, *dest;
  int ret;
  if (vec->capacity == vec->size) {
    ret = vector_expand(vec);
    if (ret) {
      return ret;
    }
  }

  if (position == 0) {
    return vec->push_front(vec, to_copy);
  }
  if (position == vec->size) {
    return vec->push(vec, to_copy);
  }
  src = (char *)vec->memory + (position * vec->datatype_size);
  dest = (char *)src + vec->datatype_size;
  memmove(dest, src, (vec->size - position) * vec->datatype_size);
  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory + (vec->datatype_size * position), to_copy);
  } else {
    memcpy((char *)vec->memory + (vec->datatype_size * position), to_copy,
           vec->datatype_size);
  }
  vec->size++;

  return 0;
}
int vector_get(struct vector *vec, size_t index, void **result) {
  if (vec == NULL || result == NULL) {
    return -EINVAL;
  }
  if (index < 0 || index >= vec->size) {
    return -ERANGE;
  }
  memcpy(*result, (char *)vec->memory + (index * vec->datatype_size),
         vec->datatype_size);

  return 0;
}
int vector_find(struct vector *vec, void *to_find, int *index) {
  if (!vec || !to_find || !index) {
    return -EINVAL;
  }
  if (vec->compare_func == NULL) {
    return -EINVAL;
  }
  for (size_t i = 0; i < vec->size; i++) {
    if (vec->compare_func((char *)vec->memory + (i * vec->datatype_size),
                          to_find)) {
      *index = i;
      return 0;
    }
  }
  *index = -1;
  return -ENOENT;
}
int vector_push_front(struct vector *vec, void *to_copy) {
  if (vec == NULL) {
    return -EINVAL;
  }
  int ret;
  int new_size;
  if (vec->capacity == vec->size) {
    ret = vector_expand(vec);
    if (ret) {
      return ret;
    }
  }
  memmove((char *)vec->memory + vec->datatype_size, vec->memory,
          vec->size * vec->datatype_size);

  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory, to_copy);
  } else {
    memcpy((char *)vec->memory, to_copy, vec->datatype_size);
  }
  vec->size++;
  return 0;
}

int vector_push(struct vector *vec, void *to_copy) {
  if (!vec || !to_copy) {
    return -EINVAL;
  }
  int ret;
  if (vec->capacity == vec->size) {
    ret = vector_expand(vec);
    if (ret) {
      return ret;
    }
  }

  if (vec->copy != NULL) {
    vec->copy((char *)vec->memory + (vec->size * vec->datatype_size), to_copy);
  } else {
    memcpy((char *)vec->memory + (vec->size * vec->datatype_size), to_copy,
           vec->datatype_size);
  }
  vec->size++;
  return 0;
}

int vector_init(struct vector **vec, size_t capacity, size_t datatype_size) {
  *vec = malloc(sizeof(struct vector));
  if (*vec == NULL) {
    return -ENOMEM;
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
    free(*vec);
    *vec = NULL;
    return -ENOMEM;
  }
  return 0;
}
