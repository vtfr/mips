#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Aloca memória que só será liberada no final da execução
 * TODO: manter um vetor com os ponteiros alocados para desalocá-los no final
 * do programa seria o correto, porém como a duração desse programa é ínfima
 * não faz diferença
 */
char* IntToStr(int v) {
	char* buffer = (char *)malloc(32);
	sprintf(buffer, "%i", v);

	return buffer;
}

/*
 * Lê o atraso dos argumentos enviados ao compilador ou usa um valor padrão
 */
#ifndef ATRASO
#  define ATRASO 1
#endif


#endif // UTIL_H
