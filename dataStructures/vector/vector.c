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

int shift_right(struct vector *vec) {
  if (vec == NULL) {
    return -EINVAL;
  }
  if (vec->size == 0) {
    return 0;
  }
  void *last = malloc(vec->datatype_size);
  if (last == NULL) {
    return -ENOMEM;
  }
  void *src, *dest;
  memcpy(last, (char *)vec->memory + ((vec->size - 1) * vec->datatype_size),
         vec->datatype_size);

  dest = (char *)vec->memory + vec->datatype_size;
  src = (char *)vec->memory;

  memmove(dest, src, (vec->size - 1) * vec->datatype_size);
  memcpy((char *)vec->memory, last, vec->datatype_size);
  free(last);
  return 0;
}
int delete_element_by_index(struct vector *vec, int index) {
  if (vec == NULL) {
    return -EINVAL;
  }
  if (index < 0 || index >= vec->size) {
    return -ERANGE;
  }
  void *dest = (char *)vec->memory + (index * vec->datatype_size);
  void *src = (char *)dest + vec->datatype_size;

  vec->size--;
  memmove(dest, src, (vec->size - index) * vec->datatype_size);

  return 0;
}
/*
ALGORITHMS
*/

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
int vector_improved_search(struct vector *vec, void *to_find, int *index) {
  if (!vec || !to_find || !index) {
    return -EINVAL;
  }
  if (vec->compare_func == NULL) {
    return -EINVAL;
  }
  int position = -1;
  for (size_t i = 0; i < vec->size; i++) {
    if (vec->compare_func((char *)vec->memory + (i * vec->datatype_size),
                          to_find)) {
      position = i;
    }
  }
  if (position == 0) {
    *index = position;
    return 0;
  }
  if (position == -1) {
    return -ENOENT;
  }
  void *to_move1 = (char *)vec->memory + (vec->datatype_size * position);
  void *to_move2 = (char *)vec->memory + (vec->datatype_size * (position - 1));

  void *tmp = malloc(vec->datatype_size);
  if (tmp == NULL) {
    return -ENOMEM;
  }
  memcpy(tmp, to_move1, vec->datatype_size);

  memmove(to_move1, to_move2, vec->datatype_size);
  memmove(to_move2, tmp, vec->datatype_size);

  position = position - 1;

  *index = position;

  free(tmp);

  return 0;
}

static void recursive_print(struct vector *vec, void *el, int index) {
  if (index >= vec->size) {
    return;
  }
  vec->print(el);
  recursive_print(vec, el + vec->datatype_size, index + 1);
}

int vector_print_rec(struct vector *vec) {
  if (vec == NULL || vec->print == NULL) {
    return -EINVAL;
  }
  if (vec->size == 0) {
    return 0;
  }
  recursive_print(vec, vec->memory, 0);
  return 0;
}

int vector_print(struct vector *vec) {
  if (vec == NULL || vec->print == NULL) {
    return -EINVAL;
  }
  if (vec->size == 0) {
    return 0;
  }
  for (size_t i = 0; i < vec->size; i++) {
    vec->print((char *)vec->memory + (i * vec->datatype_size));
  }
  return 0;
}

int shift_left(struct vector *vec) {
  if (vec == NULL) {
    return -EINVAL;
  }
  if (vec->size == 0) {
    return 0;
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
    return vector_push_front(vec, to_copy);
  }
  if (position == vec->size) {
    return vector_push(vec, to_copy);
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
  (*vec)->datatype_size = datatype_size;
  (*vec)->size = 0;
  (*vec)->free_vec = NULL;
  (*vec)->copy = NULL;
  (*vec)->memory = malloc(datatype_size * capacity);
  if ((*vec)->memory == NULL) {
    free(*vec);
    *vec = NULL;
    return -ENOMEM;
  }
  return 0;
}
