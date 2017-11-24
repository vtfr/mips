#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Função que simula o comportamento do registrador
 */
void RegFile(int registradores[32], int a1, int a2, int a3, int writeEnable,
	int data, int clk, int aclk, int* rd1, int* rd2) {

	// Retorna os valores desejados caso o registrador em questão não seja o
	// registrador zero
	*rd1 = a1 ? registradores[a1 & 0x1F] : 0x0;
	*rd2 = a2 ? registradores[a2 & 0x1F] : 0x0;

	// Caso seja clock de subida escreve no registrador
	if (clk == 1 && clk != aclk) {
		if (writeEnable) {
			registradores[a3 & 0x1F] = data;
		}
	}
}

int main(void) {
	DEF_GENPAT("regfile");

	// Entrada
	DECLAR("Clk",         ":1", "B", IN,   "", "");
	DECLAR("WriteEnable", ":1", "B", IN, "", "");
	DECLAR("A1",          ":1", "X", IN, "(4 downto 0)", "");
	DECLAR("A2",          ":1", "X", IN, "(4 downto 0)", "");
	DECLAR("A3",          ":1", "X", IN, "(4 downto 0)", "");
	DECLAR("Data",        ":1", "X", IN, "(31 downto 0)", "");

	// Saídas
	DECLAR("RD1", ":1", "X", OUT,   "(31 downto 0)", "");
	DECLAR("RD2", ":1", "X", OUT,   "(31 downto 0)", "");

	// Misc
	DECLAR("vdd", ":1", "B", IN, "", "");
	DECLAR("vss", ":1", "B", IN, "", "");
	AFFECT("0", "vdd", "1");
	AFFECT("0", "vss", "0");

	/*
	 * Escreve valores diferentes na memória e depois checa se foram escritos
	 * corretamente
	 */
	const int valoresDeTeste[] = {
		0x00000000, 0xFFFFFFFF, 0xAABBCCDD,
		0x01010101, 0xCCCCCCCC, 0x77777777,
	 	0x88888888, 0x10101010 };
	 	
	#define SIZE(x) (sizeof(x)/sizeof(x[0]))
	
	/*
	 * Escreve valores na memória
	 */
	for (int i = 0; i < 31; i++) {
		SAFFECT(current, "Clk", 0);
		SAFFECT(current, "A1", 0);
		SAFFECT(current, "A2", 0);
		SAFFECT(current, "A3", i);
		SAFFECT(current, "Data", valoresDeTeste[i % SIZE(valoresDeTeste)]);
		SAFFECT(current, "WriteEnable", 1);
		current += ATRASO;
		
		SAFFECT(current, "Clk", 1);
		current += ATRASO;
	}

	SAFFECT(current, "WriteEnable", 0);
	SAFFECT(current, "Clk", 0);

	/*
	 * Verifica RD1 e RD2
	 */
	for (int i = 0; i < 31; i++) {
		SAFFECT(current, "A1", i);
		SAFFECT(current, "A2", i);
		SAFFECT(current, "RD1", valoresDeTeste[i % SIZE(valoresDeTeste)]);
		SAFFECT(current, "RD2", valoresDeTeste[i % SIZE(valoresDeTeste)]);
		current += ATRASO;
	}

	SAV_GENPAT();

	return 0;
}
