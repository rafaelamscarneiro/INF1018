#include <stdio.h>

int odd_ones(unsigned int x){
	int n;
	for (; x; x = x >> 1)
		if (x & 1)
			n++;
	if (n & 1)
		return 1;
	return 0;
}

int main() {
	printf("%x tem numero %s de bits\n",0x01010101,odd_ones(0x01010101) ? "impar":"par");
    printf("%x tem numero %s de bits\n",0x01030101,odd_ones(0x01030101) ? "impar":"par");
    return 0;
}