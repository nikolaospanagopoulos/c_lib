#include "vector.h"
#include <stdio.h>

void vector_free(struct vector *vec) {
  for (int i = 0; i < vec->size; i++) {
    char *to_free = *(char **)(vec->memory + (i * vec->datatype_size));
    free(to_free);
  }
  free(vec->memory);
  free(vec);
}
enum VECTOR_ERRORS copy_fn(void *dest, void *src) {
  char **d = dest;
  *d = malloc(strlen(src) + 1);
  strncpy(*d, (char *)src, strlen((char *)src));
  (*d)[strlen(src)] = '\0';
  return OK;
}
void print_vector_elements_char(struct vector *vec) {

  for (size_t i = 0; i < vec->size; i++) {
    printf("%lu:%s\n", i, *(char **)(vec->memory + (i * vec->datatype_size)));
  }
}

int main() {

  struct vector *string_vec;
  vector_init(&string_vec, 5, sizeof(char *));
  string_vec->free_vec = vector_free;
  char *name = "nikos";
  string_vec->copy = copy_fn;
  string_vec->print = print_vector_elements_char;
  string_vec->push(string_vec, name);
  string_vec->push(string_vec, "2e2e2e21e2e2e2e2e2");
  string_vec->push_front(string_vec, "2e2e2e21e2e2e2e2e2");
  string_vec->push_front(string_vec, "nikos");
  string_vec->push_front(string_vec, "jos");
  name = "kostas";
  vector_insert(string_vec, 0, "first");
  vector_insert(string_vec, 2, "second");
  vector_insert(string_vec, 5, "kostas");
  string_vec->print(string_vec);
  printf("size: %lu\n", string_vec->get_size(string_vec));

  string_vec->free_vec(string_vec);

  return 0;
}
