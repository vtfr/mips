#include "genpat.h"

#include <util.h>

static int current = 0;

int main(void) {
	DEF_GENPAT("shift2");

	// Entrada
	DECLAR("A",    ":1", "X", IN,  "(31 downto 0)", "");

	// Saída
	DECLAR("S",    ":1", "X", OUT, "(31 downto 0)", "");

	// Misc
	DECLAR("vdd", ":1", "B", IN, "", "");
	DECLAR("vss", ":1", "B", IN, "", "");
	AFFECT("0", "vdd", "1");
	AFFECT("0", "vss", "0");

	/*
	 * Inicia os testes. Itera os valores de teste para cada variável e verifica
	 * se a saída está como esperado
	 */
	const int valoresDeTeste[] = {
		0x00000001, 0x00000002, 0x00000400,
		0x0FFFFFFF, 0xCCCCCCCC, 0xFEFEFEFE,
		0x10101010, 0xAAAAAAAA, 0x77777777 };

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))
	
	for (int a = 0; a < SIZE(valoresDeTeste); a++) {
		const int valor = valoresDeTeste[a];
			
		SAFFECT(current, "A", valor);
		SAFFECT(current, "S", valor << 2);
 		current += ATRASO;
 	}

	SAV_GENPAT();

	return 0;
}
