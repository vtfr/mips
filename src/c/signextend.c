#include "genpat.h"

#include <util.h>

static int current = 0;

// Extende um valor de 16 bits para 32 bits preservando o sinal
int SignExtend(int value) {
	value &= 0x0000FFFF;
	
	// Caso o bit número 15 esteja ativo, então o valor é um inteiro negativo
	// de 16 bits
	if (value & 0x8000) {
		return 0xFFFF0000 | value;
	}

	return value;

}

int main(void) {
	DEF_GENPAT("signextend");

	// Entrada
	DECLAR("A",    ":1", "X", IN,  "(15 downto 0)", "");

	// Saída
	DECLAR("S",    ":1", "X", OUT, "(31 downto 0)", "");

	// Misc
	DECLAR("vdd", ":1", "B", IN, "", "");
	DECLAR("vss", ":1", "B", IN, "", "");
	AFFECT("0", "vdd", "1");
	AFFECT("0", "vss", "0");

	// Esse macro transforma um número negativo de 32 bits em um de 16 bits
	#define NEG(x) (x & 0x0000FFFF)

	const int valoresDeTeste[] = {
		NEG(-10), NEG(-20), NEG(-30),
		10, 20, 30 };

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))
	
	for (int a = 0; a < SIZE(valoresDeTeste); a++) {
		const int valor = valoresDeTeste[a];
			
		SAFFECT(current, "A", valor);
		SAFFECT(current, "S", SignExtend(valor));
 		current += ATRASO;
 	}

	SAV_GENPAT();

	return 0;
}
