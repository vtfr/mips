#include <util.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static unsigned current = 0;

/*
 * Estados do controle
 */
typedef enum {
	CONTROL_FETCH          = 0x00,
	CONTROL_DECODE         = 0x01,
	CONTROL_MEMADDR        = 0x02,
	CONTROL_MEMREAD        = 0x03,
	CONTROL_MEMWRITEBACK   = 0x04,
	CONTROL_MEMWRITE       = 0x05,
	CONTROL_EXECUTE        = 0x06,
	CONTROL_ALUWRITEBACK   = 0x07,
	CONTROL_BRANCH	       = 0x08,
	CONTROL_ADDIEXECUTE    = 0x09,
	CONTROL_ADDIWRITEBACK  = 0x0A,
	CONTROL_JUMP           = 0x0B,
} ControlState;


/*
 * Estado da CPU para um determinado momento do tempo
 */
typedef struct {
	// 0 => Escreve nos registradores o resultado da Alu
	// 1 => Escreve nos registradores o valor do Memory Data Register
	unsigned MemToReg :1;

	// Define se será inscrito um valor entre os bits [15 - 0] ou [20 - 16]
	// da instrução no registrador
	unsigned RegDst :1;
	
	// Define se é é instrução ou data
	unsigned IOrD :1;

	// Define o que será escrito no PC
	// 0 => Resultado da Alu
	// 1 => Jump Address
	// 2 => ShiftLeft2(Instruction [25 - 0]) + PC [31 - 28]
	// 3 => Não usado
	unsigned PCSrc :2;

	// Informações para a Alu
	unsigned AluOp   :2;
	unsigned AluSrcA :1;
	unsigned AluSrcB :2;
	unsigned AluCtrl :3;

	// Flags responsáveis por habilitar escrita/leitura
	unsigned IRWrite     :1;
	unsigned MemWrite    :1;
	unsigned RegWrite    :1;
	unsigned PCWrite     :1;
	unsigned Branch      :1;

	// Estado
	ControlState State;
} Control;

/*
 * Define qual será o próximo estado do controle
 */
Control NextControlState(Control atual, int opcode, int func) {
	Control proximo = atual;

	switch (atual.State) {
	default:
	case CONTROL_FETCH:
		proximo.State   = CONTROL_DECODE;
		proximo.IOrD    = 0;
		proximo.AluSrcA = 0;
		proximo.AluSrcB = 1;
		proximo.AluOp   = 0;
		proximo.PCSrc   = 0;
		proximo.IRWrite = 1;
		proximo.PCWrite = 1;
		break;

	case CONTROL_DECODE:
		proximo.AluSrcA = 0;
		proximo.AluSrcB = 3;
		proximo.AluOp   = 0;

		switch (opcode) {
		case 35: proximo.State = CONTROL_MEMADDR;     break;
		case 43: proximo.State = CONTROL_MEMADDR;     break;
		case 0:  proximo.State = CONTROL_EXECUTE;     break;
		case 4:  proximo.State = CONTROL_BRANCH;      break;
		case 9:  proximo.State = CONTROL_ADDIEXECUTE; break;
		case 2:  proximo.State = CONTROL_JUMP;        break;
		}
		break;

	case CONTROL_MEMADDR:
		proximo.AluSrcA = 1;
		proximo.AluSrcB = 2;
		proximo.AluOp   = 0;

		switch (opcode) {
		default:
		case 35: proximo.State = CONTROL_MEMREAD;  break;
		case 43: proximo.State = CONTROL_MEMWRITE; break;
		}
		break;

	case CONTROL_MEMREAD:
		proximo.State = CONTROL_MEMWRITEBACK;
		proximo.IOrD = 1;
		break;
	
	case CONTROL_MEMWRITEBACK:
		proximo.State = CONTROL_FETCH;

		proximo.RegDst = 0;
		proximo.MemToReg = 1;
		proximo.RegWrite = 1;
		break;
		
	case CONTROL_MEMWRITE:
		proximo.State = CONTROL_FETCH;
		proximo.IOrD = 1;
		proximo.MemWrite = 1;
		break;

	case CONTROL_EXECUTE:
		proximo.State = CONTROL_ALUWRITEBACK;
		proximo.AluSrcA = 1;
		proximo.AluSrcB = 0;
		proximo.AluOp = 2;

		switch (func) {
		case 35: proximo.AluCtrl = 2; break;
		case 34: proximo.AluCtrl = 6; break;
		case 36: proximo.AluCtrl = 0; break;
		case 37: proximo.AluCtrl = 1; break;
		case 42: proximo.AluCtrl = 7; break;
		default: proximo.AluCtrl = 0; break;
		}
		break;

	case CONTROL_ALUWRITEBACK:
		proximo.State = CONTROL_FETCH;
		proximo.RegDst = 1;
		proximo.MemToReg = 0;
		proximo.RegWrite = 1;
		break;

	case CONTROL_BRANCH:
		proximo.State = CONTROL_FETCH;
		proximo.AluSrcA = 1;
		proximo.AluSrcB = 0;
		proximo.AluOp = 1;
		proximo.PCSrc = 1;
		proximo.Branch = 1;
		break;

	case CONTROL_ADDIEXECUTE:
		proximo.State = CONTROL_ADDIWRITEBACK;
		proximo.AluSrcA = 1;
		proximo.AluSrcB = 2;
		proximo.AluOp = 0;
		break;

	case CONTROL_ADDIWRITEBACK:
		proximo.State = CONTROL_FETCH;
		proximo.RegDst = 0;
		proximo.MemToReg = 0;
		break;

	case CONTROL_JUMP:
		proximo.State = CONTROL_FETCH;
		proximo.PCSrc = 2;
		proximo.PCWrite = 1;
		break;
	}

	return proximo;
}

/*
 * Envia os resultados do controle para o pat
 */
void AffectControl(Control controle) {
	SAFFECT(current, "MemToReg",    controle.MemToReg);
	SAFFECT(current, "IOrD",        controle.IOrD);
	SAFFECT(current, "PCSrc",       controle.PCSrc);
	SAFFECT(current, "RegDst",      controle.RegDst);
	SAFFECT(current, "AluOp",       controle.AluOp);
	SAFFECT(current, "AluSrcA",     controle.AluSrcA);
	SAFFECT(current, "AluSrcB",     controle.AluSrcB);
	SAFFECT(current, "ALUCtrl",     controle.AluCtrl);
	SAFFECT(current, "PCWrite",     controle.PCWrite);
	SAFFECT(current, "IRWrite",     controle.IRWrite);
	SAFFECT(current, "RegWrite",    controle.RegWrite);
	SAFFECT(current, "MemWrite",    controle.MemWrite);
	SAFFECT(current, "Branch",      controle.Branch);
}

Control ResetControl() {
	Control controle = { 0 };
	controle.State = CONTROL_FETCH;
	controle.AluSrcB = 1;
	controle.IRWrite = 1;
	controle.PCWrite = 1;

	SAFFECT(current, "Clk", 0);
	SAFFECT(current, "Reset", 1);
	AffectControl(controle);
	LABEL("Reset");
	
	current += ATRASO;
	SAFFECT(current, "Reset", 0);

	return controle;
}

// Printa o nome do estado atual do controle
void PrintControlState(ControlState atual) {
	#define P(x) do { LABEL(x); printf("Estado: %s\n", x); } while(0);

	switch (atual) {
	case CONTROL_FETCH:         P("FETCH");         break;
	case CONTROL_DECODE:        P("DECODE");        break;
	case CONTROL_MEMADDR:       P("MEMADDR");       break;
	case CONTROL_MEMREAD:       P("MEMREAD");       break;
	case CONTROL_MEMWRITE:      P("MEMWRITE");      break;
	case CONTROL_EXECUTE:       P("EXECUTE");       break;
	case CONTROL_ALUWRITEBACK:  P("ALUWRITEBACK");  break;
	case CONTROL_BRANCH:        P("BRANCH");        break;
	case CONTROL_ADDIEXECUTE:   P("ADDIEXECUTE");   break;
	case CONTROL_ADDIWRITEBACK: P("ADDIWRITEBACK"); break;
	case CONTROL_JUMP:          P("JUMP");          break;
	}
	
	#undef P
}

int main() {
	DEF_GENPAT("control");

	// Entrada
	DECLAR("Clk",       ":1", "B", IN,  "", "");
	DECLAR("Reset",     ":1", "B", IN,  "", "");

	DECLAR("OPCode",     ":1", "X", IN,  "(6 downto 0)", "");
	DECLAR("Funct",      ":1", "X", IN,  "(5 downto 0)", "");
	
	DECLAR("MemToReg",  ":1", "B", OUT, "", "");
	DECLAR("RegDst",    ":1", "B", OUT, "", "");
	DECLAR("IOrD",      ":1", "B", OUT, "", "");
	DECLAR("PCSrc",     ":1", "X", OUT, "(1 downto 0)", "");
	DECLAR("AluOP",     ":1", "X", OUT, "(1 downto 0)", "");
	DECLAR("AluSrcA",   ":1", "B", OUT, "", "");
	DECLAR("AluSrcB",   ":1", "X", OUT, "(1 downto 0)", "");
	DECLAR("ALUCtrl",   ":1", "X", OUT, "(2 downto 0)", "");

	DECLAR("IRWrite",   ":1", "B", OUT, "", "");
	DECLAR("MemWrite",  ":1", "B", OUT, "", "");
	DECLAR("PCWrite",   ":1", "B", OUT, "", "");
	DECLAR("RegWrite",  ":1", "B", OUT, "", "");
	DECLAR("Branch",    ":1", "B", OUT, "", "");

	// Misc
	DECLAR("vdd", ":1", "B", IN, "", "");
	DECLAR("vss", ":1", "B", IN, "", "");
	SAFFECT(0, "vdd", 1);
	SAFFECT(0, "vss", 0);

	SAFFECT(0, "Clk", 0);
	SAFFECT(0, "Reset", 0);
	SAFFECT(0, "OPCode", 0);
	SAFFECT(0, "Funct", 0);

	Control control = ResetControl();

	/*
	 * Conjunto de instruções para testar a Unidade de Controle
	 */
	const struct {
		unsigned OPCode;
		unsigned Funct;
	} programa[] = {
		{ 0, 36 }, // Adição              (OP 0, Funct 36)
		{ 0, 37 }, // Subtração           (OP 0, Funct 37)
		{ 2, 0  }, // Pulo incondicional  (OP 2)
		{ 35, 0 }, // Leitura da memória  (OP 35)
		{ 43, 0 }, // Escrita na memória  (OP 43)
	};

	#define SIZE(x) (sizeof(x)/sizeof(x[0]))

	// Inicia a máquina de estados e a mantem rodando até as instruções
	// acabarem
	int instruction, funct, i = 0;
	while (1) {
		if (control.State == CONTROL_FETCH) {
			instruction = programa[i].OPCode;
			funct       = programa[i].Funct;
			i++;
			
			if (i > SIZE(programa))
				break;	
		}
		
		
		SAFFECT(current, "Clk", 0);
		SAFFECT(current, "OPCode", instruction);
		SAFFECT(current, "Funct", funct);
		current += ATRASO;
	
		SAFFECT(current, "Clk", 1);
		ControlState state = control.State;
		
		control = NextControlState(control, instruction, funct);
		AffectControl(control);
		PrintControlState(state);
		current += ATRASO;
	}
	
	SAV_GENPAT();
	return 0;
}
