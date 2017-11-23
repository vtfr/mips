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
	DECLAR("A1",          ":1", "B", IN, "(4 downto 0)", "");
	DECLAR("A2",          ":1", "B", IN, "(4 downto 0)", "");
	DECLAR("A3",          ":1", "B", IN, "(4 downto 0)", "");
	DECLAR("Data",        ":1", "B", IN, "(31 downto 0)", "");

	// Saídas
	DECLAR("RD1", ":1", "B", OUT,   "(31 downto 0)", "");
	DECLAR("RD2", ":1", "B", OUT,   "(31 downto 0)", "");

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

	int registradores[32];
	for (int d = 0; d < sizeof valoresDeTeste; d++)
	for (int a = 0; a < 31; a++)
	for (int we = 0; we <= 1; we++) {
		int rd1, rd2;

		RegFile(registradores, a, a, a, we, valoresDeTeste[d], 1, 0, &rd1, &rd2);

		AFFECT(inttostr(current), "Clk", "0");
		AFFECT(inttostr(current), "A1", IntToStr(a));
		AFFECT(inttostr(current), "A2", IntToStr(a));
		AFFECT(inttostr(current), "A3", IntToStr(a));
		AFFECT(inttostr(current), "Data", IntToStr(valoresDeTeste[d]));
		AFFECT(inttostr(current), "WriteEnable", IntToStr(we))
		current += ATRASO;

		AFFECT(inttostr(current), "Clk", "1");
		current += ATRASO;

		AFFECT(inttostr(current), "RD1", IntToStr(rd1));
		AFFECT(inttostr(current), "RD2", IntToStr(rd2));
		current += ATRASO;
	}

	SAV_GENPAT();

	return 0;
}
