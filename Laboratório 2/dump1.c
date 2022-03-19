#include <stdio.h>

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main (void) {
  printf("int\n");    
  int i = 10000;
  dump(&i, sizeof(i));
  printf("long\n");
  long l = 10000;
  dump(&l, sizeof(l));
  printf("short\n");
  short s = 10000;
  dump(&s, sizeof(s));
  char c = 'a';
  char d = 97;
  printf("char\n");
  dump(&c, sizeof(c));
  dump(&d, sizeof(d));
  return 0;
}