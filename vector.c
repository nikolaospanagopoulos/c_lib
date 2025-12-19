#include "vector.h"
#include <stdio.h>
#include <string.h>

struct person {
  char *name;
  int age;
};
size_t get_size(struct vector *vec) { return vec->size; }

static enum VECTOR_ERRORS vector_expand(struct vector *vec) {
  vec->capacity *= 2;
  vec->memory = realloc(vec->memory, vec->capacity);
  printf("expanding...\n");
  if (vec->memory == NULL) {
    return ALLOC_ERROR;
  }
  return OK;
}
enum VECTOR_ERRORS copy_fn_str(void *dest, void *src) {
  struct person **p = dest;
  const struct person *const s = src;
  *p = malloc(sizeof(struct person));
  (*p)->age = s->age;
  (*p)->name = malloc(strlen(s->name) + 1);
  strncpy((*p)->name, s->name, strlen(s->name));
  (*p)->name[strlen(s->name)] = '\0';
  return OK;
}

void vector_free_struct(struct vector *vec) {
  for (int i = 0; i < vec->size; i++) {
    struct person *to_free =
        *(struct person **)((char *)vec->memory + (i * vec->datatype_size));
    free(to_free->name);
    free(to_free);
  }
  free(vec->memory);
  free(vec);
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
  (*vec)->free_vec = NULL;
  (*vec)->copy = NULL;
  (*vec)->get_size = get_size;
  (*vec)->memory = malloc(datatype_size * capacity);
  if ((*vec)->memory == NULL) {
    return ALLOC_ERROR;
  }
  return OK;
}

void print_vector_elements(struct vector *vec) {
  for (size_t i = 0; i < vec->size; i++) {
    printf("%c\n", *(char *)(vec->memory + i * vec->datatype_size));
  }
}
void print_vector_elements_struct(struct vector *vec) {

  for (size_t i = 0; i < vec->size; i++) {
    struct person *current =
        *(struct person **)(vec->memory + i * vec->datatype_size);
    printf("name: %s, age: %d\n", current->name, current->age);
  }
}
void print_vector_elements_int(struct vector *vec) {

  for (size_t i = 0; i < vec->size; i++) {
    printf("%d\n", *(int *)(vec->memory + i * vec->datatype_size));
  }
}

/*
int main() {

  struct vector *example_vec = NULL;
  vector_init(&example_vec, 10, sizeof(char));

  char x = 'x';
  char y = 'y';

  example_vec->push(example_vec, &x);
  example_vec->push(example_vec, &y);
  example_vec->push(example_vec, &x);
  example_vec->push(example_vec, &y);
  x = 't';

  print_vector_elements(example_vec);

  free(example_vec->memory);
  free(example_vec);

  struct vector *num_vec = NULL;
  vector_init(&num_vec, 10, sizeof(int));

  for (int i = 1; i < 10; i++) {
    vector_push(num_vec, &i);
  }
  print_vector_elements_int(num_vec);
  free(num_vec->memory);
  free(num_vec);

  struct vector *vec;
  vector_init(&vec, 10, sizeof(struct person));

  vec->copy = copy_fn_str;
  vec->free_vec = vector_free_struct;
  struct person p = {.name = "nikos", .age = 30};

  vec->push(vec, &p);
  p.name = "kostas";

  print_vector_elements_struct(vec);
  vec->free_vec(vec);

  return 0;
}
*/
