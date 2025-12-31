#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
  size_t capacity;
  void *memory;
  size_t datatype_size;
  size_t size;
  int (*push)(struct vector *vec, void *to_copy);
  int (*get_size)(struct vector *vec, size_t *size);
  int (*copy)(void *dest, void *src);
  void (*free_vec)(struct vector *vec);
  void (*print)(struct vector *vec);
  int (*push_front)(struct vector *vec, void *to_copy);
  bool (*compare_func)(void *element1, void *element2);
};
static int vector_expand(struct vector *vec);
int vector_push_front(struct vector *vec, void *to_copy);
int vector_push(struct vector *vec, void *to_copy);
int vector_init(struct vector **vec, size_t capacity, size_t datatype_size);
int vector_insert(struct vector *vec, size_t position, void *to_copy);

int vector_find(struct vector *vec, void *to_find, int *index);
int vector_get(struct vector *vec, size_t index, void **result);
int get_size(struct vector *vec, size_t *size);
int shift_left(struct vector *vec);
int shift_right(struct vector *vec);
