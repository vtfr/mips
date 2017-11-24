#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Função que simula o comportamento do registrador
 */
int RegEnable(int* estado, int data, int enable, int reset, int clk, int aclk) {
	// Caso seja clock de subida, escreve no registrador
	if (clk == 1 && clk != aclk) {
		if (reset) {
			*estado = 0;
		} else if (enable) {
			*estado = data;
		}
	}

	return *estado;
}

int main(void) {
	DEF_GENPAT("reg_enable");

	// Entrada
	DECLAR("Clk",         ":1", "B", IN,  "", "");
	DECLAR("Reset",       ":1", "B", IN, "", "");
	DECLAR("WriteEnable", ":1", "B", IN, "", "");
	DECLAR("Data",        ":1", "X", IN, "(31 downto 0)", "");

	// Saídas
	DECLAR("S", ":1", "X", OUT,   "(31 downto 0)", "");

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


	SAFFECT(current, "Clk", 0);
	SAFFECT(current, "Reset", 0);
	SAFFECT(current, "WriteEnable", 0);
	SAFFECT(current, "Data", 0);
	current += ATRASO;
	
	LABEL("Reset");
	SAFFECT(current, "Clk", 1);
	SAFFECT(current, "Reset", 1);
	current += ATRASO;
	
	SAFFECT(current, "Reset", 0);
	
	#define SIZE(x) (sizeof(x)/sizeof(x[0]))
	
	int registrador = 0;
	for (int d = 0; d < SIZE(valoresDeTeste); d++)
	for (int e = 0; e <= 1; e++)
	for (int c = 0; c <= 1; c++) {
		int s = RegEnable(&registrador, valoresDeTeste[d], e, 0, c, !c);

		SAFFECT(current, "Clk", c);
		SAFFECT(current, "WriteEnable", e);
		SAFFECT(current, "Data", valoresDeTeste[d]);
		current += PROPAG;
		
		SAFFECT(current, "S", s);
		current += ATRASO;
	}

	SAV_GENPAT();

	return 0;
}
