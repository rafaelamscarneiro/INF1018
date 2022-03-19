/* Rafaela Maria Souza Carneiro 2011483 */
/* Maria Beatriz de Lima e Silva Sobreira 2010953 */

#include <stdio.h>
#include "converte.h"

unsigned int calcula_tamanho(char c) {		/* funcao auxiliar */
  /* verifica se o caractere tem 1, 2, 3 ou 4 bytes */
  if (!(c >> 7))  			/* se o bit mais significativo eh 0, caractere tem 1 byte */
    return 1;
  else {
    if ((c >> 4) & 1) return 4; 	/* se o terceiro bit mais significativo eh 1, caractere tem 3 bytes */
    if ((c >> 5) & 1) return 3; 	/* se o quarto bit mais significativo eh 1, caractere tem 4 bytes */
    return 2;     			/* se nao entrou nas condicoes acima, o caractere tem 2 bytes */
  }
}

void troca_bits(unsigned char *a, unsigned char *b, short n) /* funcao auxiliar */
{
  char aux = *a << n; 
  *b = *b & 0x3f;
  *b = *b | aux;
  *a = *a & 0x3f;
  *a = *a >> (8-n);   
}

int utf_varint(FILE *arq_entrada, FILE* arq_saida) {
  unsigned char c;
  if (!arq_entrada) 
  {
    fputs("Erro na leitura", stderr);
    return -1;
  }
  if (!arq_saida){
  	fputs("Erro na gravacao", stderr);
  	return -1;
  }
  int result = 1;
  while (result) {
    result = fread(&c, 1, 1, arq_entrada);
    if (calcula_tamanho(c) == 1) {
      /* transformando pra varint*/
      if ((c >> 7) & 1){ 	/* se o primeiro bit for 1 */ 
        unsigned char i[2]; 
        i[0]= c;
        i[1]=1; 		/*so vai sobrar ele e varios zeros, acho q da pra deixar assi*/
        fwrite(&i, 2, 1, arq_saida);
      }
      else{
        unsigned char i = c 	/*nao havera mais bytes a seguir entao nao acrescentamos nd*/; 
        fwrite(&i, 1, 1, arq_saida);
      }
    }
    else if (calcula_tamanho(c) == 2) 
    {
      unsigned char d; 
      result = fread(&d, 1, 1, arq_entrada);
      troca_bits(&c, &d, 6);			/* pegando o code point */
      if  ((c>>7)&1 || (c>>6)&1){ 		/* se um dos 2 primeiros bits é 1 */
        unsigned char i[3];
        i[0]= d  | 0x80  			/*primeiro bit vira 1*/ ;
        i[1]= ((c<<1) | (d>>7)) | 0x80 ;	/*primeiro bit vira 1*/
        i[2]=c>>6;
        fwrite(&i,3,1,arq_saida);
      }
      else{
        unsigned char i[2]; 			/*mesma logica caso 1 byte comece com 1*/
        i[0]= d | 0x80;
        i[1]=(c<<1) | (d>>7) ;
        fwrite(&i, 2, 1, arq_saida);
      }
    }
    else if (calcula_tamanho(c) == 3) 
    { 
      unsigned char d[2];
      result = fread(d, 1, 2, arq_entrada);
      troca_bits(&d[0], &d[1], 6);
      troca_bits(&c, &d[0], 4);
      // o byte mais significativo nunca fara parte do code point
      if  ((d[0]>>7)&1 || (d[0]>>6)&1){
        unsigned char i[3];
        i[0]= d[1]  | 0x80  				/*primeiro bit vira 1*/ ;
        i[1]= ((d[0]<<1) | (d[1]>>7)) | 0x80 ;	/*primeiro bit vira 1*/
        i[2]=d[0]>>6;
        fwrite(&i,3,1,arq_saida);
      }
      else{

        unsigned char i[2]; /*mesma logica caso 1 byte comece com 1*/
        i[0]= d[1] | 0x80;
        i[1]=(d[0]<<1) | (d[1]>>7) ;
        fwrite(&i, 2, 1, arq_saida);
      }
    }
    else if (calcula_tamanho(c) == 4) 
    { 
      unsigned char d[3];
      result = fread(d, 1, 3, arq_entrada);
      troca_bits(&d[1], &d[2], 6);  
      troca_bits(&d[0], &d[1], 4);
      c = c << 2; 			/* pegando os 3 bits menos significativos do 4 byte */
      c = c & 0x3f; 			/* apaga os 2 bits mais significativos do 4 byte */
      d[0] = d[0] | c;  		/* 3 byte decodificado */     
      if  ((d[0]>>7)&1 || (d[0]>>6)&1 || (d[0]>>5)&1){
        unsigned char i[4];
        i[0]= d[2]  | 0x80  		/*primeiro bit vira 1*/ ;
        i[1]= ((d[0]<<1) | (d[1]>>7)) | 0x80;
        i[2]=((d[1]<<2 | d[2]>>6)) | 0x80;
        i[3]=d[0]>>5;
        fwrite(i, 4, 1, arq_saida);
      }
      else{		/* os 3 primeiros bits do primeiro byte sao 0 -> codificacao como varint vai ter 3 bytes */
        unsigned char i[3]; 		
        i[0]= d[2] | 0x80;			/* primeiro byte = ultimo byte do code point com o primeiro bit sendo 1 */
        i[1]=((d[1]<<1) | (d[2]>>7)) | 0x80;	/* segundo byte = segundo byte do codepoint  */
        i[2]=(d[0]<<2) | (d[1]>>6);
        fwrite(i, 3, 1, arq_saida);
      }
    }
  }
  fclose(arq_entrada);
  fclose(arq_saida);
  return 0;
}

int varint_utf(FILE *arq_entrada, FILE *arq_saida) {
	if (!arq_entrada) {
		fputs("Erro de leitura", stderr);
		return -1;
	}
	if (!arq_saida) {
		fputs("Erro na gravacao", stderr);
		return -1;
	}
	int result = 1, i = 0;
	unsigned char n[4] = {0, 0, 0, 0};
	while (result) {
		result = fread(&n[0], 1, 1, arq_entrada);
		while ((n[i] >> 7) & 1) {
			i++;
			fread(&n[i], 1, 1, arq_entrada);
		}
		i++;
		if (i == 1) {
			unsigned int x = n[0];
			if (!(x >> 7)){
				unsigned char c = x;
				fwrite(&c, 1, 1, arq_saida);
			}
			else {
				unsigned char c[2];		/* se bit mais significativo == 1, em utf8 ele vai ter 2 bytes */
				c[0] = 0xc0 | x >> 6; 		/* pegando os dois bits mais significativos */	
				c[1] = x & 0xbf;		/* o proprio byte, mas com os dois bits mais sigficativos sendo 10 */
				fwrite(&c, 2, 1, arq_saida);
			}
		}
		else if (i == 2) {
			unsigned int x;
			x = (n[0] & 0x7f) | (n[1] << 7);			/* apaga o primeiro bit do n[0] e dps coloca no lugar dele o ultimo bit do n[1] */ 
										/* esse eh o segundo byte do numero */
			x = x | (n[1] >> 1) << 8;				/* tira o ultimo bit do n[1] e coloca ele no primeiro byte do numero */
			if (x >> 11 == 0){ 					/* vendo se apos o 11o bit (dir->esq) so tem 0s (porque ai em utf8 ele vai ter so 2 bytes) */
				unsigned  char c[2];
				c[0] = (0x1c & x >> 6) | (x >> 6 & 0x03);	/* pegando os 3 ultimos bits do primeiro byte e os 2 primeiros bits do segundo byte */ 
				c[0] = (c[0] | 0xc0) & 0xdf; 			/* os 3 primeiros bits sendo 110 */
				c[1] = (x & 0xbf) | 0xb0;			/* o segundo byte, mas com os 2 primeiros bits sendo 10 */
				fwrite(&c, 2, 1, arq_saida);
			}
			else {
				unsigned char c[3];
				c[0] = ((x >> 12) | 0xe0) & 0xef;	/* 4 primeiros bits sendo 1110 e o resto igual a n >> 12 */ 
				c[1] = (x >> 6 & 0xbf) | 0x80; 	/* 2 primeiros bits sendo 10 e o resto igual n >> 6 */
				c[2] = (x & 0xbf) | 0x80; 		/* 2 primeiros bits sendo 10 e o resto igual n */
				fwrite(&c, 3, 1, arq_saida);
			}
		}
		else if (i == 3) {
			unsigned int x;
			x = n[0] & ((n[1] << 7) | 0x7f);
			x = x | (((n[1] >> 1) | (n[2] << 6)) << 8);
			x = x | ((n[2] >> 2) << 16);
			unsigned char c[4];
			c[0] = (x >> 18) | 0xf0;				
			c[1] = ((x >> 12) & 0xbf) | 0x80;
			c[2] = ((x >> 6) & 0xbf) | 0x80;
			c[3] = (x & 0xbf) | 0x80;
			fwrite(c, 4, 1, arq_saida);
		}
		else {				
			unsigned int x;
			x = n[0] & ((n[1] << 7) | 0x7f);
			x = x | ((((n[1] >> 1) & 0x3f) | (n[2] << 6)) << 8);
			x = x | ((((n[2] >> 2) & 0x1f) | (n[3] << 5)) << 16); 
			x = x | ((n[3] >> 3) << 24);
			unsigned char c[4];
			c[0] = (x >> 18) | 0xf0;				
			c[1] = ((x >> 12) & 0xbf) | 0x80;
			c[2] = ((x >> 6) & 0xbf) | 0x80;
			c[3] = (x & 0xbf) | 0x80;
			fwrite(&c, 4, 1, arq_saida);	
		}
		i = 0;
	}
	fclose(arq_entrada);
	fclose(arq_saida);
	return 0;
}
