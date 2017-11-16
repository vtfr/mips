#ifndef CONTROL_STATE_H
#define CONTROL_STATE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
	MemToReg    unsigned :1;

	// Define se é é instrução ou data
	IOrD unsigned :1;

	// Define o que será escrito no PC
	// 0 => Resultado da Alu
	// 1 => Jump Address
	// 2 => ShiftLeft2(Instruction [25 - 0]) + PC [31 - 28]
	// 3 => Não usado
	PCSource unsigned :2;

	// Define se será inscrito um valor entre os bits [15 - 0] ou [20 - 16]
	// da instrução no registrador
	RegDest unsigned :1;

	// Informações para a Alu
	AluOp   unsigned :1;
	AluSrcA unsigned :2;
	AluSrcB unsigned :2;
	AluCtrl unsigned :3;

	// Flags responsáveis por habilitar escrita/leitura
	PCWrite     unsigned :1;
	PCWriteCond unsigned :1;
	IRWrite     unsigned :1;
	MemWrite    unsigned :1;
	MemRead     unsigned :1;
	PCWrite     unsigned :1;

	// Estado
	State ControlState;
} Control;

/*
 * Define qual será o próximo estado do controle
 */
Control ProximoEstadoControle(Control atual) {
	Control proximo = atual;

	switch (atual.State) {
	case CONTROL_FETCH:
		proximo.State   = CONTROL_DECODE;
		proximo.IorD    = 0;
		proximo.ALUSrcA = 0;
		proximo.ALUSrcB = 1;
		proximo.ALUOp   = 0;
		proximo.PCSrc   = 0;
		proximo.IRWrite = 1;
		proximo.PCWrite = 1;
		break;

	case CONTROL_DECODE:
		proximo.ALUSrcA = 0;
		proximo.ALUSrcB = 2;
		proximo.ALUOp   = 0;

		switch (atual.Opcode) {
		case 35: proximo.State = CONTROL_MEMADDR;     break;
		case 43: proximo.State = CONTROL_MEMADDR;     break;
		case 0:  proximo.State = CONTROL_EXECUTE;     break;
		case 4:  proximo.State = CONTROL_BRANCH;      break;
		case 9:  proximo.State = CONTROL_ADDIEXECUTE; break;
		case 2:  proximo.State = CONTROL_JUMP;        break;
		}
		break;

	case CONTROL_MEMADDR:
		proximo.ALUSrcA = 1;
		proximo.ALUSrcB = 2;
		proximo.ALUOp   = 0;

		switch (atual.Opcode) {
		case 35: proximo.State = CONTROL_MEMREAD;  break;
		case 43: proximo.State = CONTROL_MEMWRITE; break;
		}
		break;

	case CONTROL_MEMREAD:
		proximo.State = CONTROL_MEMWRITEBACK;
		proximo.IorD = 1;
		break;

	case CONTROL_MEMWRITE:
		proximo.State = CONTROL_FETCH;
		proximo.IorD = 1;
		proximo.MemWrite = 1;
		break;

	case CONTROL_EXECUTE:
		proximo.State = CONTROL_ALUWRITEBACK;
		proximo.ALUSrcA = 1;
		proximo.ALUSrcB = 0;
		proximo.ALUOp = 2;

		switch (atual.Funct) {
		case 32: proximo.AluCtrl = 2; break;
		case 34: proximo.AluCtrl = 6; break;
		case 36: proximo.AluCtrl = 0; break;
		case 37: proximo.AluCtrl = 1; break;
		case 42: proximo.AluCtrl = 7; break;
		}
		break;

	case CONTROL_ALUWRITEBACK:
		proximo.State = FETCH;
		proximo.RegDest = 1;
		proximo.MemToReg = 0;
		proximo.RegWrite = 1;
		break;

	case CONTROL_BRANCH:
		proximo.State = CONTROL_FETCH;
		proximo.ALUSrcA = 1;
		proximo.AluSrcB = 0
		proximo.ALUOp = 1;
		proximo.PCSrc = 1;
		proximo.Branch = 1;
		break;

	case CONTROL_ADDIEXECUTE:
		proximo.State = CONTROL_ADDIWRITEBACK;
		proximo.ALUSrcA = 1;
		proximo.ALUSrcB = 2;
		proximo.ALUOp = 0;
		break;

	case CONTROL_ADDIWRITEBACK:
		proximo.State = CONTROL_FETCH;
		proximo.RegDest = 0;
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




#endif // CONTROL_STATE_H
