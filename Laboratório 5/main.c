#include <stdio.h>
#include <stdlib.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void dump (void *p, int n) {
  unsigned char *p1 = p;
  while (n--) {
    printf("%p - %02x\n", p1, *p1);
    p1++;
  }
}

int main() {
	
	struct X2 {
		long l;
		char c;
	}x2;
	x2.l = 0x0102030405060708;
	x2.c = 0xc1;
	printf("tamanho de x2 = %d\nx2: \n",sizeof(x2));
	dump(&x2,sizeof(x2));
	
	return 0;
}
