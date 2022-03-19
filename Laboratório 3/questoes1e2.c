#include <stdio.h>

int odd_ones(unsigned int x){	/*questao 2*/
	int n;
	for (; x; x = x >> 1)
		if (x & 1)
			n++;
	if (n & 1)
		return 1;
	return 0;
}

int main() {
	/*questao 1*/
	unsigned int x = 0x87654321;
	unsigned int y, z;
	
	/* o byte menos significativo de x e os outros bits em 0 */
	y = (0x000000ff & x);
	
	/* o byte mais significativo com todos os bits em '1' 
    e os outros bytes com o mesmo valor dos bytes de x */
    z = (0xff000000 | x);
	
	printf("%08x %08x\n", y, z);
	
	/*questao 2*/
	printf("%x tem numero %s de bits\n",0x01010101,odd_ones(0x01010101) ? "impar":"par");
    printf("%x tem numero %s de bits\n",0x01030101,odd_ones(0x01030101) ? "impar":"par");
	
	return 0;
}
	
	
	