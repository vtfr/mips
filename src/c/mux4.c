#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Função que simula o comportamento do Mux4
 */
int Mux4(int ctrl, int a, int b, int c, int d) {
	switch (ctrl) {
	default:
	case 0: return a;
	case 1: return b;
	case 2: return c;
	case 3: return d;
	}
}

int main(void) {
	DEF_GENPAT("mux4");

	// Entrada
	DECLAR("A",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("B",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("C",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("D",    ":1", "X", IN,  "(31 downto 0)", "");
	DECLAR("Ctrl", ":1", "X", IN,  "(1 downto 0)", "");

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
		0x00000000, 0xFFFFFFFF, 0xAABBCCDD,
		0x01010101, 0xCCCCCCCC };

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))

	for (int a = 0; a < SIZE(valoresDeTeste); a++)
	for (int b = 0; b < SIZE(valoresDeTeste); b++)
	for (int c = 0; c < SIZE(valoresDeTeste); c++)
 	for (int d = 0; d < SIZE(valoresDeTeste); d++)
 	for (int ctrl = 0; ctrl < 4; ctrl++) {
		// Avalia o resultado do mux
		const int s = Mux4(ctrl,
			valoresDeTeste[a],
			valoresDeTeste[b],
			valoresDeTeste[c],
			valoresDeTeste[d]);

		SAFFECT(current, "Ctrl", ctrl);

		SAFFECT(current, "A", valoresDeTeste[a]);
		SAFFECT(current, "B", valoresDeTeste[b]);
		SAFFECT(current, "C", valoresDeTeste[c]);
		SAFFECT(current, "D", valoresDeTeste[d]);
		current += PROPAG;

		SAFFECT(current, "S", s);
 		current += ATRASO;
 	}

	SAV_GENPAT();

	return 0;
}
