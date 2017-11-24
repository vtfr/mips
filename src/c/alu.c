#include "genpat.h"

#include <util.h>

static int current = 0;

/*
 * Tipos de operação da ULA
 */
typedef enum {
	ALU_ADD = 0,
	ALU_SUB = 1,
	ALU_AND = 2,
	ALU_OR  = 6,
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
ALUResult ALU(ALUOp ctrl, int a, int b) {
	ALUResult result;

	switch (ctrl) {
	case ALU_ADD: result.s = a + b; break;
	case ALU_SUB: result.s = a - b; break;
	case ALU_AND: result.s = a & b; break;
	case ALU_OR:  result.s = a | b; break;
	}

	result.zero = result.s == 0;
	return result;
}


int main(void) {
	DEF_GENPAT("alu");

	// Entrada
	DECLAR("A",    ":1", "X", IN, "(31 downto 0)", "");
	DECLAR("B",    ":1", "X", IN, "(31 downto 0)", "");
	DECLAR("Ctrl", ":1", "X", IN, "(2 downto 0)", "");

	// Saída
	DECLAR("S",    ":1", "X", OUT, "(31 downto 0)", "");
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
		ALU_AND, ALU_OR, ALU_ADD, ALU_SUB
	};

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))

	for (int op = 0; op < SIZE(ops); op++)
	for (int a = 0; a < SIZE(valoresDeTeste); a++)
	for (int b = 0; b < SIZE(valoresDeTeste); b++) {
		ALUResult result = ALU(ops[op], valoresDeTeste[a], valoresDeTeste[b]);
		
		SAFFECT(current, "Ctrl", (unsigned)ops[op]);
		SAFFECT(current, "A",    valoresDeTeste[a]);
		SAFFECT(current, "B",    valoresDeTeste[b]);
		current += PROPAG;

		SAFFECT(current, "S",    result.s);
		SAFFECT(current, "Zero", result.zero);
		current += ATRASO;
	}

	SAV_GENPAT();
	return 0;
}
