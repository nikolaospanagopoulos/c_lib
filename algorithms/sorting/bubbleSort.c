#include <stdbool.h>
#include <stdio.h>

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void bubbleSort(int *arr, int n) {
  int i, j;
  bool swapped;
  for (int i = 0; i < n - 1; i++) {
    swapped = false;
    for (j = 0; j < n - 1 - i; j++) {
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
        swapped = true;
      }
    }
    if (swapped == false) {
      break;
    }
  }
}

void printArr(int *arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int main() {

  int arr[] = {10, 5000, 18, 1, 500, 12, 3};
  printArr(arr, 7);
  bubbleSort(arr, 7);
  printArr(arr, 7);

  return 0;
}
