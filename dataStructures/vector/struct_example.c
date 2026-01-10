#include "vector.h"
#include <stdio.h>

struct person {
  char *name;
  int age;
};
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
int copy_fn_str(void *dest, void *src) {
  struct person **p = dest;
  const struct person *const s = src;
  *p = malloc(sizeof(struct person));
  (*p)->age = s->age;
  (*p)->name = malloc(strlen(s->name) + 1);
  strncpy((*p)->name, s->name, strlen(s->name));
  (*p)->name[strlen(s->name)] = '\0';
  return 0;
}

void print_vector_elements_struct(struct vector *vec) {

  for (size_t i = 0; i < vec->size; i++) {
    struct person *current =
        *(struct person **)(vec->memory + i * vec->datatype_size);
    printf("name: %s, age: %d\n", current->name, current->age);
  }
}
int main() {
  struct vector *vec;
  vector_init(&vec, 10, sizeof(struct person *));

  vec->copy = copy_fn_str;
  vec->free_vec = vector_free_struct;
  struct person p = {.name = "nikos", .age = 30};

  vec->push(vec, &p);
  p.name = "kostas";

  print_vector_elements_struct(vec);
  vec->free_vec(vec);

  return 0;
}
