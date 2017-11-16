#include "genpat.h"

#include <util.h>

static int current = 0;

// Reseta o registrador
void ResetaRegistrador(void) {
	LABEL("Reset")
	AFFECT(inttostr(current), "Clk", "0")
	AFFECT(inttostr(current), "Q", "*");
	current += ATRASO;

	AFFECT(inttostr(current), "Clk", "1")
	AFFECT(inttostr(current), "Reset", "1");
	current += ATRASO;
}

int main(void) {
	DEF_GENPAT("reg");

	// Entrada
	DECLAR("Clk",    ":1", "B", IN,   "", "");
	DECLAR("Enable", ":1", "B", IN,   "", "");
	DECLAR("Reset",  ":1", "B", IN,   "", "");
	DECLAR("Q",      ":1", "B", OUT,  "(31 downto 0)", "");
	DECLAR("D",      ":1", "B", IN,   "(31 downto 0)", "");

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
		0x01010101, 0xCCCCCCCC, 0x77777777,
	 	0x88888888, 0x10101010 };


	/*
	 * Primeiro teste: é possível mudar o valor do registrador sem a flag
	 * enable? Deve falhar em todos os testes mantendo o valor 0xAABBCCDD
	 * definido inicialmente
	 */
	LABEL("Sem Enable")

	AFFECT(inttostr(current), "Clk", "0");
	current += ATRASO;

	AFFECT(inttostr(current), "Clk", "1");
	AFFECT(inttostr(current), "Enable", "1");
	AFFECT(inttostr(current), "D", IntToStr(0xAABBCCDD));
	current += ATRASO;

	AFFECT(inttostr(current), "Clk", "0");
	AFFECT(inttostr(current), "Enable", "0");
	current += ATRASO;

	AFFECT(inttostr(current), "Q", IntToStr(0xAABBCCDD));

	for (int d = 0; d < sizeof valoresDeTeste; d++) {
		AFFECT(inttostr(current), "Clk", "0");
		AFFECT(inttostr(current), "D", valoresDeTeste[d]);
		current += ATRASO;

		AFFECT(inttostr(current), "Clk", "1");
		current += ATRASO;
	}

	ResetaRegistrador();

	/*
	 * Segundo teste: é possível mudar o valor do registrador?
	 */
	LABEL("Com Enable")

	AFFECT(inttostr(current), "Enable", "1");
 	for (int d = 0; d < sizeof valoresDeTeste; d++) {
 		AFFECT(inttostr(current), "Clk", "0");
 		AFFECT(inttostr(current), "Q", "*");

 		current += ATRASO;
 		AFFECT(inttostr(current), "Clk", "1");
 		AFFECT(inttostr(current), "D", IntToStr(valoresDeTeste[d]));

 		current += ATRASO;
		AFFECT(inttostr(current), "Clk", "0");
		AFFECT(inttostr(current), "Q", IntToStr(valoresDeTeste[d]));
 	}

	SAV_GENPAT();

	return 0;
}
