#include "genpat.h"

#include <util.h>

static int current = 0;

int main(void) {
	DEF_GENPAT("mux2");

	// Entrada
	DECLAR("A",    ":1", "B", IN,  "(31 downto 0)", "");
	DECLAR("B",    ":1", "B", IN,  "(31 downto 0)", "");
	DECLAR("Ctrl", ":1", "B", IN,  "", "");

	DECLAR("S",    ":1", "B", OUT, "(31 downto 0)", "");

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

	for (int a = 0; a < sizeof valoresDeTeste; a++)
	for (int b = 0; b < sizeof valoresDeTeste; b++)
 	for (int ctrl = 0; ctrl < 4; ctrl++) {
		// Avalia o resultado do mux
		const int s = c == 0 ? valoresDeTeste[a] : valoresDeTeste[b];

		AFFECT(IntToStr(current), "Ctrl", IntToStr(ctrl));

		AFFECT(IntToStr(current), "A", IntToStr(valoresDeTeste[a]));
		AFFECT(IntToStr(current), "B", IntToStr(valoresDeTeste[b]));
		AFFECT(IntToStr(current), "S", "*");
		current += ATRASO;

		AFFECT(IntToStr(current), "S", IntToStr(s));
 		current += ATRASO;
 	}

	SAV_GENPAT();

	return 0;
}
