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
    printf("%s\n", *(char **)(vec->memory + i * vec->datatype_size));
  }
}

int main() {

  struct vector *string_vec;
  vector_init(&string_vec, 5, sizeof(char *));
  string_vec->free_vec = vector_free;
  char *name = "nikos";
  string_vec->copy = copy_fn;
  string_vec->push(string_vec, name);
  string_vec->push(string_vec, "2e2e2e21e2e2e2e2e2");
  name = "kostas";
  print_vector_elements_char(string_vec);

  string_vec->free_vec(string_vec);

  return 0;
}
