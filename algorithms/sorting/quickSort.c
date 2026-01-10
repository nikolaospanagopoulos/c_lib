#include <stdio.h>

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(int *arr, int low, int high) {
  int pivotPoint = arr[high];

  int i = low - 1;

  for (int j = low; j <= high - 1; j++) {
    if (arr[j] < pivotPoint) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

void quickSort(int *arr, int low, int high) {
  if (low >= high) {
    return;
  }
  int pivotPoint = partition(arr, low, high);

  quickSort(arr, low, pivotPoint - 1);
  quickSort(arr, pivotPoint + 1, high);
}
void printArr(int *arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
int main() {

  int arr[] = {5, 7, 8, 89, 4, 1256, 2, 3, 4, 12};
  printArr(arr, 10);
  quickSort(arr, 0, 9);
  printArr(arr, 10);

  return 0;
}
