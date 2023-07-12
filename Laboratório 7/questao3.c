#include <stdio.h>

int main (void) {
  for (int n = 1; n < 11; n++)
    printf("%d -> %d\n", n, n*n);
  return 0;
}