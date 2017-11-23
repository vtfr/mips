#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Tipos de operação da ULA
 */
typedef enum {
	ALU_ADD = 0x00,
	ALU_SUB = 0x01,
	ALU_AND = 0x02,
	ALU_OR  = 0x06,
} ALUOp;

/*
 * Resultado da ULA
 */
typedef struct {
	int s;
	int zero;
} ALUResult;

/*
 * Unidade Lógica Aritmétroca
 */
int ALU(int a, int b, ALUOp ctrl) {
	ALUResult result;

	switch (ctrl) {
	case ALU_ADD: result.s = a + b;
	case ALU_SUB: result.s = a - b;
	case ALU_AND: result.s = a & b;
	case ALU_OR:  result.s = a | b;
	}

	result.zero = result.s == 0;
	return result;
}


int main(void) {
	DEF_GENPAT("alu");

	// Entrada
	DECLAR("A",     ":1", "B", IN, "(31 downto 0)", "");
	DECLAR("B",     ":1", "B", IN, "(31 downto 0)", "");
	DECLAR("ALUOp", ":1", "B", IN, "(3 downto 0)", "");

	// Saída
	DECLAR("S",    ":1", "B", OUT, "(31 downto 0)", "");
	DECLAR("Zero", ":1", "B", OUT, "", "");

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
		0x00000000, 0x000FFFFF, 0xAABBCCDD,
		0x01010101, 0xCCCCCCCC, 0x77777777,
	 	0x88888888, 0x10101010 };

	const ALUOp ops[] = {
		ALU_ADD, ALU_SUB, ALU_AND, ALU_OR
	};

	for (int a = 0; a < sizeof valoresDeTeste; a++)
	for (int b = 0; b < sizeof valoresDeTeste; b++)
	for (int op = 0; op < sizeof opsDeTeste; op++) {
		ALUResult result = ALU(valoresDeTeste[a], valoresDeTeste[b], ops[op]);

		AFFECT(current, "Ctrl", IntToStr(valoresDeTeste[c]));
		AFFECT(current, "A",    IntToStr(valoresDeTeste[a]));
		AFFECT(current, "B",    IntToStr(valoresDeTeste[b]));
		AFFECT(current, "S",    IntToStr(result.s));
		AFFECT(current, "Zero", IntToStr(result.zero));
		current += ATRASO;
	}

	SAV_GENPAT();
	return 0;
}
