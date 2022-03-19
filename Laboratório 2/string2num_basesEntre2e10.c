#include <ctype.h>
#include <stdio.h>

int string2num (char *s, int base) {
  int a = 0;
  for (; *s; s++)
    a = a*base + (*s - '0');
  return a;
}

int main (void) {
  printf("%d\n", string2num("777", 8));
  printf("%d\n", string2num("777", 10));
  return 0;
}