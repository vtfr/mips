#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Função que simula o comportamento do Mux2
 */
int Mux2(int ctrl, int a, int b) {
	return ((ctrl == 0) ? a : b);
}

int main(void) {
	DEF_GENPAT("mux2");

	// Entrada
	DECLAR("A",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("B",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("Ctrl", ":1", "B", IN,  "", "");

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
		0x0FFFFFFF, 0xCCCCCCCC, 0xFEFEFEFE,
		0x10101010, 0xAAAAAAAA };

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))
	
	for (int ctrl = 0; ctrl < 2; ctrl++)
	for (int a = 0; a < SIZE(valoresDeTeste); a++)
	for (int b = 0; b < SIZE(valoresDeTeste); b++) {
		// Avalia o resultado do mux
		const int s = Mux2(ctrl, valoresDeTeste[a], valoresDeTeste[b]);

		SAFFECT(current, "Ctrl", ctrl);
		SAFFECT(current, "A", valoresDeTeste[a]);
		SAFFECT(current, "B", valoresDeTeste[b]);
		current += PROPAG;

		SAFFECT(current, "S", s);
 		current += ATRASO;
 	}

	SAV_GENPAT();

	return 0;
}
