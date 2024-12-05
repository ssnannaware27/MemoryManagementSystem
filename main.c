#include <stdio.h>
#include <stdlib.h>
#include <main.h>
int main() {
  int *arr = (int *)new_malloc(sizeof(int) * 8);
  int i = 0;
  for(i = 0; i < 8; i++) {
    arr[i] = i;
  }
  for(i = 0; i < 8; i++) {
    printf("%d", arr[i]);
  }
  new_free(arr);
  return 0;
}
