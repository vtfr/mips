#ifndef UTIL_H
#define UTIL_H

#include "genpat.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Facilita a realização do affect usando a mesma funcionalidade do AFFECT
 * original, porém mais rápido (sem alocação de memória na heap), mais seguro
 * (sem leaking de memória) e funciona para todos os casos. Valores binários
 * devem ser definidos com "B" e valores hexadecimais com "X" no DECLAR, para
 * que essa função funcione.
 */
#define SAFFECT(pos, name, value) \
	do { GNP_LINE=__LINE__; affect(pos, name, value); } while (0)

void affect(int pos, char* name, int value) {
	char bufferPos[32], bufferValor[64];
	sprintf(bufferPos, "%i", pos);
	sprintf(bufferValor, "0x%X", value);
	d_AFFECT(bufferPos, name, bufferValor);
}


/*
 * Lê o atraso dos argumentos enviados ao compilador ou usa um valor padrão
 */
#ifndef ATRASO
#  define ATRASO 1
#endif

#ifndef PROPAG
#  define PROPAG 0
#endif


#endif // UTIL_H
