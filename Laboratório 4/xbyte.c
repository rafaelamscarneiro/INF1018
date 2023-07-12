#include <stdio.h>
#include <math.h>

typedef unsigned packed_t;

/* extrai byte indicado e retorna valor inteiro correspondente (32 bits) com sinal */
int xbyte (packed_t word, int bytenum) {
	int i;
	i = (word >> bytenum * 8) & 0x000000ff;
	if (i >> 7 & 1)
	    i = i - pow(2, 8);
	return i;
}